#include "RenderingManager.h"

#include <algorithm>
#include <random>

#include "CamerasManager.h"
#include "EditorUIManager.h"
#include "GameObject.h"
#include "Math.h"
#include "MeshPrimitivesPool.h"
#include "MeshRenderer.h"
#include "PostProcessMaterial.h"
#include "PostProcessRenderer.h"
#include "SceneManager.h"
#include "Time.h"
#include "WindowManager.h"

namespace GamesGoEngine
{
	RenderingManager* RenderingManager::instance = nullptr;

	RenderingManager::RenderingManager()
	{
		lightsManager = new LightsManager();

		//debug & tools materials
		normalDebugMaterial = new Material("res/shaders/debugNormals.vert.glsl", "res/shaders/debugNormals.frag.glsl", "res/shaders/debugNormals.geom.glsl");		
		objectSelectionMaterial = new Material("res/shaders/unlit.vert.glsl", "res/shaders/objectSelection.frag.glsl");

		//render pipeline materials
		depthMapMaterial = new Material("res/shaders/RenderPipeline/depthMap.vert.glsl", "res/shaders/RenderPipeline/depthMap.frag.glsl");
		omniDepthMapMaterial = new Material("res/shaders/RenderPipeline/omniDepthMap.vert.glsl", "res/shaders/RenderPipeline/omniDepthMap.frag.glsl", "res/shaders/RenderPipeline/omniDepthMap.geom.glsl");

		// post process related materials
		bloomDownsampleMaterial = std::make_shared<PostProcessMaterial>("res/shaders/PostProcess/bloomDownsample.frag.glsl");
		bloomUpsampleMaterial = std::make_shared<PostProcessMaterial>("res/shaders/PostProcess/bloomUpsample.frag.glsl");
		hdrToneMappingGammaCorrectionMaterial = std::make_shared<PostProcessMaterial>("res/shaders/PostProcess/hdrToneMappingGammaCorrection.frag.glsl");
		editorOutlineMaterial = std::make_shared<PostProcessMaterial>("res/shaders/PostProcess/editorOutline.frag.glsl");
		deferredShadingMaterial = std::make_shared<PostProcessMaterial>("res/shaders/PostProcess/deferredShadingSimple.frag.glsl");
		deferredAmbientLightShadowedAdditiveMaterial = std::make_shared<PostProcessMaterial>("res/shaders/PostProcess/deferredShadingAddAmbientLightShadow.frag.glsl");
		deferredDirectionalLightShadowedAdditiveMaterial = std::make_shared<PostProcessMaterial>("res/shaders/PostProcess/deferredShadingAddDirectionalLightShadow.frag.glsl");
		deferredPointLightShadowedAdditiveMaterial = std::make_shared<PostProcessMaterial>("res/shaders/PostProcess/deferredShadingAddPointLightShadow.frag.glsl");
		deferredSpotLightShadowedAdditiveMaterial = std::make_shared<PostProcessMaterial>("res/shaders/PostProcess/deferredShadingAddSpotLightShadow.frag.glsl");
		textureMergerMaterial = std::make_shared<PostProcessMaterial>("res/shaders/PostProcess/textureMerger.frag.glsl");
		ssaoMaterial = std::make_shared<PostProcessMaterial>("res/shaders/PostProcess/ssao.frag.glsl");
		ssaoBlurMaterial = std::make_shared<PostProcessMaterial>("res/shaders/PostProcess/ssaoBlur.frag.glsl");
		fxaaMaterial = std::make_shared<PostProcessMaterial>("res/shaders/PostProcess/AntiAliasing/FXAA.frag.glsl");;
	}

	RenderingManager::~RenderingManager()
	{
		delete lightsManager;
		delete normalDebugMaterial;
		delete objectSelectionMaterial;
		delete depthMapMaterial;
		delete omniDepthMapMaterial;
	}

	RenderingManager* RenderingManager::GetInstance()
	{
		if (instance == nullptr)
		{
			instance = new RenderingManager();
		}
		return instance;
	}

	void RenderingManager::Init()
	{
		glEnable(GL_DEPTH_TEST);

		glEnable(GL_STENCIL_TEST);
		//glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		const int buffersWidth = GetInstance()->currentBuffersWidth;
		const int buffersHeight = GetInstance()->currentBuffersHeight;

		GetInstance()->ConfigureFramebuffers(buffersWidth, buffersHeight, true);
		GetInstance()->ConfigureUniformBufferObjects();
		GetInstance()->CreateDebugMaterials();
		GetInstance()->InitGammaCorrection();
		GetInstance()->InitSSAOData();
	}

	void RenderingManager::InitGammaCorrection()
	{
		// make sure that the default gamma value & exposure is set in the material
		SetGamma(GetGamma());
		SetExposure(GetExposure());
	}

	void RenderingManager::InitSSAOData()
	{
		// generate sample kernel
		std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // generates random floats between 0.0 and 1.0
		std::default_random_engine generator;
		for (unsigned int i = 0; i < ssaoKernelSize; ++i)
		{
			glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));
			sample = glm::normalize(sample);
			sample *= randomFloats(generator);
			float scale = float(i) / 64.0f;

			// scale samples s.t. they're more aligned to center of kernel
			float minDist = 0.1f;
			float maxDist = 1.0f;
			scale = minDist + scale * scale * (maxDist - minDist);
			sample *= scale;
			ssaoKernel.push_back(sample);
		}

		// generate noise texture
		std::vector<glm::vec3> ssaoNoise;
		for (unsigned int i = 0; i < 16; i++)
		{
			glm::vec3 noise(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, 0.0f); // rotate around z-axis (in tangent space)
			ssaoNoise.push_back(noise);
		}

		ssaoNoiseTexture;
		glGenTextures(1, &ssaoNoiseTexture);
		glBindTexture(GL_TEXTURE_2D, ssaoNoiseTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	void RenderingManager::ConfigureFramebuffers(GLint screenWidth, GLint screenHeight, bool shouldGenerateFramebuffer)
	{
		ConfigureFramebuffer(screenWidth, screenHeight, framebuffer1, textureColorBuffer1, depthStencilBuffer1, stencilView1, shouldGenerateFramebuffer);
		ConfigureFramebuffer(screenWidth, screenHeight, framebuffer2, textureColorBuffer2, depthStencilBuffer2, stencilView2, shouldGenerateFramebuffer);
		ConfigureFramebuffer(screenWidth, screenHeight, shadowFBO1, shadowColorBuffer1, shadowDepthStencilBuffer1, shadowStencilView1, shouldGenerateFramebuffer);
		ConfigureFramebuffer(screenWidth, screenHeight, shadowFBO2, shadowColorBuffer2, shadowDepthStencilBuffer2, shadowStencilView2, shouldGenerateFramebuffer);
		ConfigureShadowMapFramebuffer(shadowWidth, shadowHeight, directionalDepthMapFBO, directionalDepthMap, shouldGenerateFramebuffer);
		ConfigureShadowMapFramebuffer(shadowWidth, shadowHeight, spotLightDepthMapFBO, spotLightDepthMap, shouldGenerateFramebuffer);
		ConfigureShadowCubeMapFramebuffer(shadowWidth, shadowHeight, omniDepthMapFBO, omniDepthMap, shouldGenerateFramebuffer);
		ConfigureGBuffer(screenWidth, screenHeight, shouldGenerateFramebuffer);
		ConfigureSSAOBuffers(screenWidth, screenHeight, shouldGenerateFramebuffer);
		ConfigureBloomBuffer(screenWidth, screenHeight, shouldGenerateFramebuffer);
		ConfigureObjectSelectionBuffer(screenWidth, screenHeight, shouldGenerateFramebuffer);
	}

	void RenderingManager::ConfigureFramebuffer(GLint screenWidth, GLint screenHeight,
		unsigned int& framebuffer, unsigned int& textureColorBuffer,
		unsigned int& depthStencilBuffer, unsigned int& stencilView,
		bool shouldGenerateFramebuffer)
	{
		if (shouldGenerateFramebuffer)
		{
			glGenFramebuffers(1, &framebuffer);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

		// create a color attachment texture
		glGenTextures(1, &textureColorBuffer);
		glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screenWidth, screenHeight, 0, GL_RGBA, GL_FLOAT, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);

		// create a depth & stencil attachment texture
		glGenTextures(1, &depthStencilBuffer);
		glBindTexture(GL_TEXTURE_2D, depthStencilBuffer);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, screenWidth, screenHeight, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, screenWidth, screenHeight);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthStencilBuffer, 0);

		glGenTextures(1, &stencilView);
		glTextureView(stencilView, GL_TEXTURE_2D, depthStencilBuffer, GL_DEPTH24_STENCIL8, 0, 1, 0, 1);

		// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void RenderingManager::ConfigureShadowMapFramebuffer(GLint shadowMapWidth, GLint shadowMapHeight,
		unsigned& framebuffer, unsigned& shadowMap, bool shouldGenerateFramebuffer)
	{
		if (shouldGenerateFramebuffer)
		{
			glGenFramebuffers(1, &framebuffer);
		}

		glGenTextures(1, &shadowMap);
		glBindTexture(GL_TEXTURE_2D, shadowMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
			shadowMapWidth, shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		// ensure that the objects outside of map are not in shadow
		constexpr float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);
		// no color data needed for depth so disable draw & read buffers in the framebuffer
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void RenderingManager::ConfigureShadowCubeMapFramebuffer(GLint shadowMapWidth, GLint shadowMapHeight,
		unsigned& framebuffer, unsigned& shadowCubeMap, bool shouldGenerateFramebuffer)
	{
		if (shouldGenerateFramebuffer)
		{
			glGenFramebuffers(1, &framebuffer);
		}

		glGenTextures(1, &shadowCubeMap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, shadowCubeMap);
		for (unsigned int i = 0; i < 6; ++i)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, shadowMapWidth, shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowCubeMap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void RenderingManager::ConfigureGBuffer(GLint screenWidth, GLint screenHeight, bool shouldGenerateFramebuffer)
	{
		if (shouldGenerateFramebuffer)
		{
			glGenFramebuffers(1, &gBuffer);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
		// position color buffer
		glGenTextures(1, &gPosition);
		glBindTexture(GL_TEXTURE_2D, gPosition);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screenWidth, screenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
		// normal color buffer
		glGenTextures(1, &gNormal);
		glBindTexture(GL_TEXTURE_2D, gNormal);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screenWidth, screenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
		// color color buffer
		glGenTextures(1, &gAlbedo);
		glBindTexture(GL_TEXTURE_2D, gAlbedo);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screenWidth, screenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedo, 0);
		// metallic color buffer
		glGenTextures(1, &gMetallic);
		glBindTexture(GL_TEXTURE_2D, gMetallic);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenWidth, screenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, gMetallic, 0);
		// roughness color buffer
		glGenTextures(1, &gRoughness);
		glBindTexture(GL_TEXTURE_2D, gRoughness);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenWidth, screenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, gRoughness, 0);
		// ambient occlusion color buffer
		glGenTextures(1, &gAmbientOcclusion);
		glBindTexture(GL_TEXTURE_2D, gAmbientOcclusion);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenWidth, screenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT5, GL_TEXTURE_2D, gAmbientOcclusion, 0);
		// emissive color buffer
		glGenTextures(1, &gEmissive);
		glBindTexture(GL_TEXTURE_2D, gEmissive);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, screenWidth, screenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT6, GL_TEXTURE_2D, gEmissive, 0);
		// tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
		unsigned int attachments[7] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5, GL_COLOR_ATTACHMENT6 };
		glDrawBuffers(7, attachments);

		// create a depth & stencil attachment texture
		glGenTextures(1, &gDepth);
		glBindTexture(GL_TEXTURE_2D, gDepth);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, screenWidth, screenHeight, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, screenWidth, screenHeight);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, gDepth, 0);

		glGenTextures(1, &gStencil);
		glTextureView(gStencil, GL_TEXTURE_2D, gDepth, GL_DEPTH24_STENCIL8, 0, 1, 0, 1);

		// finally check if framebuffer is complete
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Framebuffer not complete!" << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void RenderingManager::ConfigureSSAOBuffers(GLint screenWidth, GLint screenHeight, bool shouldGenerateFramebuffer)
	{
		if (shouldGenerateFramebuffer)
		{
			glGenFramebuffers(1, &ssaoFBO);
			glGenFramebuffers(1, &ssaoBlurFBO);
		}

		// SSAO color buffer
		glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
		glGenTextures(1, &ssaoColorBuffer);
		glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenWidth, screenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer, 0);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "SSAO Framebuffer not complete!" << std::endl;

		// SSAO blur stage
		glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
		glGenTextures(1, &ssaoColorBufferBlur);
		glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, screenWidth, screenHeight, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBufferBlur, 0);
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "SSAO Blur Framebuffer not complete!" << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void RenderingManager::ConfigureBloomBuffer(GLint screenWidth, GLint screenHeight, bool shouldGenerateFramebuffer)
	{
		if (shouldGenerateFramebuffer)
		{
			glGenFramebuffers(1, &bloomFBO);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, bloomFBO);

		glm::vec2 mipSize((float)screenWidth, (float)screenHeight);
		glm::ivec2 mipIntSize((int)screenWidth, (int)screenHeight);
		// Safety check
		if (screenWidth > (unsigned int)INT_MAX || screenHeight > (unsigned int)INT_MAX)
		{
			std::cerr << "Window size conversion overflow - cannot build bloom FBO!\n";
			return;
		}

		// TODO: This should be done only if bloom mip chain is empty or bloomMipChainLength changed
		// bloom textures clean up (just to be sure we don't have some trash data)
		for (size_t i = 0; i < bloomMipChain.size(); i++)
		{
			glDeleteTextures(1, &bloomMipChain[i].texture);
		}
		bloomMipChain.clear();

		// bloom textures setup
		for (unsigned int i = 0; i < bloomMipChainLength; i++)
		{
			BloomMip mip;

			mipSize *= 0.5f;
			mipIntSize /= 2;
			mip.size = mipSize;
			mip.intSize = mipIntSize;

			glGenTextures(1, &mip.texture);
			glBindTexture(GL_TEXTURE_2D, mip.texture);
			// we are downscaling an HDR color buffer, so we need a float texture format
			glTexImage2D(GL_TEXTURE_2D, 0, GL_R11F_G11F_B10F,
				(int)mipSize.x, (int)mipSize.y,
				0, GL_RGB, GL_FLOAT, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

			bloomMipChain.emplace_back(mip);
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bloomMipChain[0].texture, 0);

		// setup attachments
		unsigned int attachments[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, attachments);

		// check completion status
		int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			printf("bloom buffer FBO error, status: 0x\%x\n", status);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void RenderingManager::ConfigureObjectSelectionBuffer(GLint screenWidth, GLint screenHeight, bool shouldGenerateFramebuffer)
	{
		if (shouldGenerateFramebuffer)
		{
			glGenFramebuffers(1, &objectSelectionFBO);
		}

		// create a color attachment texture
		glBindFramebuffer(GL_FRAMEBUFFER, objectSelectionFBO);
		glGenTextures(1, &objectSelectionColorBuffer);
		glBindTexture(GL_TEXTURE_2D, objectSelectionColorBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R32I, screenWidth, screenHeight, 0, GL_RED_INTEGER, GL_INT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, objectSelectionColorBuffer, 0);

		// create a depth & stencil attachment texture
		glGenTextures(1, &objectSelectionDepthStencilBuffer);
		glBindTexture(GL_TEXTURE_2D, objectSelectionDepthStencilBuffer);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, screenWidth, screenHeight);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, objectSelectionDepthStencilBuffer, 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Object selection not complete!" << std::endl;

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void RenderingManager::ResizeBuffers(GLint buffersWidth, GLint buffersHeight)
	{
		GetInstance()->ResizeBuffersInternal(buffersWidth, buffersHeight);
	}

	void RenderingManager::ResizeBuffersInternal(GLint buffersWidth, GLint buffersHeight)
	{
		currentBuffersWidth = buffersWidth;
		currentBuffersHeight = buffersHeight;

		// not the most lightweight approach since we need to recreate textures but it is required due to how we write data from stencil buffer into texture
		glDeleteTextures(1, &textureColorBuffer1);
		glDeleteTextures(1, &textureColorBuffer2);
		glDeleteTextures(1, &shadowColorBuffer1);
		glDeleteTextures(1, &shadowColorBuffer2);

		glDeleteTextures(1, &depthStencilBuffer1);
		glDeleteTextures(1, &depthStencilBuffer2);
		glDeleteTextures(1, &shadowDepthStencilBuffer1);
		glDeleteTextures(1, &shadowDepthStencilBuffer2);

		glDeleteTextures(1, &stencilView1);
		glDeleteTextures(1, &stencilView2);
		glDeleteTextures(1, &shadowStencilView1);
		glDeleteTextures(1, &shadowStencilView2);

		glDeleteTextures(1, &directionalDepthMap);
		glDeleteTextures(1, &spotLightDepthMap);
		glDeleteTextures(1, &omniDepthMap);

		glDeleteTextures(1, &gPosition);
		glDeleteTextures(1, &gNormal);
		glDeleteTextures(1, &gAlbedo);
		glDeleteTextures(1, &gMetallic);
		glDeleteTextures(1, &gRoughness);
		glDeleteTextures(1, &gAmbientOcclusion);
		glDeleteTextures(1, &gEmissive);
		glDeleteTextures(1, &gDepth);
		glDeleteTextures(1, &gStencil);

		glDeleteTextures(1, &ssaoColorBuffer);
		glDeleteTextures(1, &ssaoColorBufferBlur);

		for (size_t i = 0; i < bloomMipChain.size(); i++)
		{
			glDeleteTextures(1, &bloomMipChain[i].texture);
		}
		bloomMipChain.clear();

		glDeleteTextures(1, &objectSelectionColorBuffer);
		glDeleteTextures(1, &objectSelectionDepthStencilBuffer);

		GetInstance()->ConfigureFramebuffers(currentBuffersWidth, currentBuffersHeight, false);
	}

	void RenderingManager::ConfigureUniformBufferObjects()
	{
		glGenBuffers(1, &uboMatrices);
		glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
		glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4));

		glGenBuffers(1, &uboCameraData);
		glBindBuffer(GL_UNIFORM_BUFFER, uboCameraData);
		glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::vec3), NULL, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		glBindBufferRange(GL_UNIFORM_BUFFER, 1, uboCameraData, 0, 2 * sizeof(glm::vec3));

		glGenBuffers(1, &uboTimeData);
		glBindBuffer(GL_UNIFORM_BUFFER, uboTimeData);
		glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(float), NULL, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		glBindBufferRange(GL_UNIFORM_BUFFER, 2, uboTimeData, 0, 2 * sizeof(float));
	}

	void RenderingManager::CreateDebugMaterials()
	{

	}

	void RenderingManager::Update()
	{
		// update uniform buffer objects
		GetInstance()->UpdateUniformBufferObjects();

		// TODO: more optimal sorting, it could sort on camera view change, not on every draw frame
		SortTransparentMeshRenderers();

		const int viewportWidth = GetInstance()->currentBuffersWidth;
		const int viewportHeight = GetInstance()->currentBuffersHeight;

		// Rendering the scene
		// set viewport
		glViewport(0, 0, viewportWidth, viewportHeight);

		// clear both buffers to ensure no unwanted data and set default color at the start of frame rendering
		glBindFramebuffer(GL_FRAMEBUFFER, GetInstance()->framebuffer1);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glStencilMask(~0);
		glClearStencil(0);

		glBindFramebuffer(GL_FRAMEBUFFER, GetInstance()->framebuffer2);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glStencilMask(~0);
		glClearStencil(0);

		// enable depth testing & stencil testing
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);

		GetInstance()->UpdateObjectSelectionBuffer();

		GetInstance()->DrawDeferredShadedObjects(viewportWidth, viewportHeight);

		glBindFramebuffer(GL_FRAMEBUFFER, GetInstance()->framebuffer2);

		glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)
		glEnable(GL_STENCIL_TEST);

		GetInstance()->DrawForwardShadedObjects(viewportWidth, viewportHeight);

		GetInstance()->DrawSkybox();
		GetInstance()->DrawDebug();

		// pass the framebuffer2 (the one we worked on so far) to framebuffer1 so both have same stencil and depth data (used by some screen effects)
		glBindFramebuffer(GL_READ_FRAMEBUFFER, GetInstance()->framebuffer2);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, GetInstance()->framebuffer1);

		glBlitFramebuffer(0, 0, viewportWidth, viewportHeight, 0, 0, viewportWidth, viewportHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
		glBlitFramebuffer(0, 0, viewportWidth, viewportHeight, 0, 0, viewportWidth, viewportHeight, GL_STENCIL_BUFFER_BIT, GL_NEAREST);
		glBlitFramebuffer(0, 0, viewportWidth, viewportHeight, 0, 0, viewportWidth, viewportHeight, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

		GetInstance()->DrawScreenEffects(viewportWidth, viewportHeight);

		// Draw black on screen as color texture information is going to be passed to an apprioprate UI panel
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		const int windowWidth = WindowManager::GetCurrentWidth();
		const int windowHeight = WindowManager::GetCurrentHeight();
		glViewport(0, 0, windowWidth, windowHeight);		
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void RenderingManager::UpdateUniformBufferObjects()
	{
		glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(CamerasManager::GetActiveCameraProjectionMatrix()));
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(CamerasManager::GetActiveCameraViewMatrix()));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glBindBuffer(GL_UNIFORM_BUFFER, uboCameraData);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec3), glm::value_ptr(CamerasManager::GetActiveCameraPosition()));
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec3), sizeof(glm::vec3), glm::value_ptr(CamerasManager::GetActiveCameraDirection()));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glBindBuffer(GL_UNIFORM_BUFFER, uboTimeData);
		const auto time = static_cast<float>(Time::GetTime());
		const auto deltaTime = static_cast<float>(Time::GetDeltaTime());
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(float), &time);
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(float), sizeof(float), &deltaTime);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void RenderingManager::UpdateObjectSelectionBuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, objectSelectionFBO);

		glClearColor(-1.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// Write to framebuffer object id
		for (size_t i = 0; i < meshRenderers.size(); i++)
		{
			MeshRenderer* meshRenderer = meshRenderers[i];
			if (meshRenderer != nullptr && Component::IsValid(meshRenderer))
			{
				objectSelectionMaterial->SetInt("objectIdNumber", meshRenderer->GetOwner()->GetObjectId());
				meshRenderer->Draw(objectSelectionMaterial);
			}
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void RenderingManager::UpdateGBuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glStencilMask(~0);
		glClearStencil(0);
		glClear(GL_STENCIL_BUFFER_BIT);
		DrawRenderersOfLightModel(meshRenderers, LightModelType::LitDeferred);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void RenderingManager::UpdateSSAOBuffers(int width, int height)
	{
		if (ssaoEnabled)
		{
			// generate SSAO texture
			glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
			glClear(GL_COLOR_BUFFER_BIT);
			for (unsigned int i = 0; i < ssaoKernelSize; ++i)
			{
				ssaoMaterial->SetVector3(("samples[" + std::to_string(i) + "]").c_str(), ssaoKernel[i]);
			}
			ssaoMaterial->SetTexture("gPosition", 0, gPosition);
			ssaoMaterial->SetTexture("gNormal", 1, gNormal);
			ssaoMaterial->SetTexture("texNoise", 2, ssaoNoiseTexture);
			// tile noise texture over screen based on screen dimensions divided by noise size, noise size is constant 4x4
			ssaoMaterial->SetVector2("noiseScale", glm::vec2((float)width / 4.0f, (float)height / 4.0f));
			ssaoMaterial->Draw(glm::mat4());
			MeshPrimitivesPool::GetQuadPrimitive()->DrawSubMesh(0);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			// blur SSAO texture to remove noise
			glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
			glClear(GL_COLOR_BUFFER_BIT);
			ssaoBlurMaterial->SetTexture("ssaoInput", 0, ssaoColorBuffer);
			ssaoBlurMaterial->Draw(glm::mat4());
			MeshPrimitivesPool::GetQuadPrimitive()->DrawSubMesh(0);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}

	void RenderingManager::UpdateDirectionalShadowMap(Light* directionalLight, glm::mat4& lightSpaceMatrix)
	{
		if (!Component::IsValid(directionalLight))
		{
			return;
		}

		float near_plane = 1.0f, far_plane = 30.0f;
		glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, 10.0f, -10.0f, near_plane, far_plane);

		Transform* lightTransform = directionalLight->GetOwner()->GetTransform();
		glm::vec3 lightPosition = lightTransform->GetPosition() - 10.0f * lightTransform->GetForward();
		glm::vec3 lightLookAtTarget = lightTransform->GetPosition() + 10.0f * lightTransform->GetForward();
		glm::mat4 lightView = glm::lookAt(lightPosition, lightLookAtTarget, glm::vec3(0.0f, 1.0f, 0.0f));
		lightSpaceMatrix = lightProjection * lightView;
		depthMapMaterial->SetMat4("lightSpaceMatrix", lightSpaceMatrix);

		glViewport(0, 0, shadowWidth, shadowHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, directionalDepthMapFBO);

		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glCullFace(GL_FRONT);
		DrawShadowCastingRenderers(meshRenderers, depthMapMaterial);
		glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// TODO: remove and implement more elegant solution, this is not universal solution, just one for specific example scene
		for (size_t i = 0; i < meshRenderers.size(); i++)
		{
			std::shared_ptr<Material> outMaterial;
			if (meshRenderers[i]->TryGetMaterial(outMaterial, 0))
			{
				outMaterial->SetMat4("directionalLightSpaceMatrix", lightSpaceMatrix);
				outMaterial->SetTexture("directionalLightShadowMap", 0, directionalDepthMap);
			}
		}
	}

	void RenderingManager::UpdateSpotLightShadowMap(Light* spotLight, glm::mat4& lightSpaceMatrix)
	{
		if (!Component::IsValid(spotLight))
		{
			return;
		}

		float near_plane = 0.1f, far_plane = 30.0f;
		// TODO: calculate fov and aspect properly to adjust to the value of spot light
		glm::mat4 lightProjection = glm::perspective(40.0f, 1.0f, near_plane, far_plane);

		Transform* lightTransform = spotLight->GetOwner()->GetTransform();
		glm::vec3 lightPosition = lightTransform->GetPosition();
		glm::vec3 lightLookAtTarget = lightPosition + 10.0f * lightTransform->GetForward();
		glm::mat4 lightView = glm::lookAt(lightPosition, lightLookAtTarget, glm::vec3(0.0f, 1.0f, 0.0f));
		lightSpaceMatrix = lightProjection * lightView;
		depthMapMaterial->SetMat4("lightSpaceMatrix", lightSpaceMatrix);

		glViewport(0, 0, shadowWidth, shadowHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, spotLightDepthMapFBO);

		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glCullFace(GL_FRONT);
		DrawShadowCastingRenderers(meshRenderers, depthMapMaterial);
		glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// TODO: remove and implement more elegant solution, this is not universal solution, just one for specific example scene
		for (size_t i = 0; i < meshRenderers.size(); i++)
		{
			std::shared_ptr<Material> outMaterial;
			if (meshRenderers[i]->TryGetMaterial(outMaterial, 0))
			{
				outMaterial->SetMat4("spotLightSpaceMatrix", lightSpaceMatrix);
				outMaterial->SetTexture("spotLightShadowMap", 1, spotLightDepthMap);
			}
		}
	}

	void RenderingManager::UpdateOmnidirectionalShadowMap(Light* pointLight)
	{
		if (!Component::IsValid(pointLight))
		{
			return;
		}

		float aspect = (float)shadowWidth / (float)shadowHeight;
		float near = 0.1f;
		float far = 25.0f;
		glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, near, far);

		glm::vec3 lightPos = pointLight->GetOwner()->GetTransform()->GetPosition();

		std::vector<glm::mat4> shadowTransforms;
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));

		glViewport(0, 0, shadowWidth, shadowHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, omniDepthMapFBO);

		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glCullFace(GL_FRONT);
		for (unsigned int i = 0; i < 6; ++i)
		{
			std::stringstream stream;
			std::string matrixName;
			stream << "shadowMatrices[" << std::to_string(i) << "]";
			stream >> matrixName;
			omniDepthMapMaterial->SetMat4(matrixName.c_str(), shadowTransforms[i]);
		}
		omniDepthMapMaterial->SetFloat("far_plane", far);
		omniDepthMapMaterial->SetVector3("lightPos", lightPos);
		DrawShadowCastingRenderers(meshRenderers, omniDepthMapMaterial);
		//glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// TODO: remove and implement more elegant solution, this is not universal solution, just one for specific example scene
		for (size_t i = 0; i < meshRenderers.size(); i++)
		{
			std::shared_ptr<Material> outMaterial;
			if (meshRenderers[i]->TryGetMaterial(outMaterial, 0))
			{
				outMaterial->SetCubeTexture("pointLightShadowMap", 2, omniDepthMap);
				outMaterial->SetFloat("far_plane", far);
				// Legacy feature for the normal/parallax mapping shaders
				outMaterial->SetVector3("lightPos", lightPos);
			}
		}
	}

	// TODO: since it's not optimal to do this every frame, it should be cached, values can be updated on "SetIsCastinShadow(...)" call
	bool RenderingManager::AreThereAnyShadowCasters() const
	{
		for (size_t i = 0; i < meshRenderers.size(); i++)
		{
			if (meshRenderers[i]->IsCastingShadow())
			{
				return true;
			}
		}
		return false;
	}

	void RenderingManager::DrawDeferredShadedObjects(int width, int height)
	{
		if (AreThereAnyDeferredRendereredMeshes())
		{
			UpdateGBuffer();

			// disable depth and buffer for deferred shading rendering since it's quad rendering over whole screen
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_STENCIL_TEST);

			UpdateSSAOBuffers(width, height);

			std::vector<Light*> lights = GetLightsManager()->GetAllLights();
			size_t lastReachedLightIndex = 0;
			for (size_t i = 0; i < lights.size(); i++)
			{
				const unsigned int currentFramebuffer = i % 2 == 0 ? framebuffer2 : framebuffer1;
				glBindFramebuffer(GL_FRAMEBUFFER, currentFramebuffer);
				glClear(GL_COLOR_BUFFER_BIT);

				std::shared_ptr<PostProcessMaterial> deferredLightMaterial = nullptr;
				Light* currentLight = nullptr;
				bool isCurrentLightAmbient = false;

				// set light data
				if (AmbientLight* ambientLight = dynamic_cast<AmbientLight*>(lights[i]))
				{
					isCurrentLightAmbient = true;
					deferredLightMaterial = deferredAmbientLightShadowedAdditiveMaterial;
					currentLight = ambientLight;
				}
				else if (DirectionalLight* directionalLight = dynamic_cast<DirectionalLight*>(lights[i]))
				{
					deferredLightMaterial = deferredDirectionalLightShadowedAdditiveMaterial;
					currentLight = directionalLight;

					// update shadow map
					glm::mat4 lightSpaceMatrix;
					UpdateDirectionalShadowMap(directionalLight, lightSpaceMatrix);
					deferredLightMaterial->SetTexture("directionalLightShadowMap", 1, directionalDepthMap);
					deferredLightMaterial->SetMat4("directionalLightSpaceMatrix", lightSpaceMatrix);
				}
				else if (PointLight* pointLight = dynamic_cast<PointLight*>(lights[i]))
				{
					deferredLightMaterial = deferredPointLightShadowedAdditiveMaterial;
					currentLight = pointLight;

					// update shadow map
					UpdateOmnidirectionalShadowMap(pointLight);
					deferredLightMaterial->SetCubeTexture("pointLightShadowMap", 1, omniDepthMap);
					deferredLightMaterial->SetFloat("far_plane", 25.0f);
				}
				else if (SpotLight* spotLight = dynamic_cast<SpotLight*>(lights[i]))
				{
					deferredLightMaterial = deferredSpotLightShadowedAdditiveMaterial;
					currentLight = spotLight;

					// update shadow map
					glm::mat4 lightSpaceMatrix;
					UpdateSpotLightShadowMap(spotLight, lightSpaceMatrix);
					deferredLightMaterial->SetTexture("spotLightShadowMap", 1, spotLightDepthMap);
					deferredLightMaterial->SetMat4("spotLightSpaceMatrix", lightSpaceMatrix);
				}
				else
				{
					std::cout << "This type of light is not supported yet!" << std::endl;
					break;
				}

				if (deferredLightMaterial != nullptr && currentLight != nullptr)
				{
					glBindFramebuffer(GL_FRAMEBUFFER, currentFramebuffer);
					glDisable(GL_DEPTH_TEST);
					glDisable(GL_STENCIL_TEST);
					glViewport(0, 0, width, height);
					glClear(GL_COLOR_BUFFER_BIT);

					// update light data
					const GLuint shaderProgram = deferredLightMaterial->GetShaderProgram();
					currentLight->SetLightInShader(shaderProgram, false);

					// update general texture data
					if (isCurrentLightAmbient)
					{
						deferredLightMaterial->SetTexture("screenTexture", 0, i % 2 == 0 ? textureColorBuffer1 : textureColorBuffer2);
						deferredLightMaterial->SetTexture("gAlbedo", 1, gAlbedo);
						deferredLightMaterial->SetTexture("gAmbientOcclusion", 2, gAmbientOcclusion);
						deferredLightMaterial->SetTexture("gEmissive", 3, gEmissive);
						deferredLightMaterial->SetTexture("ssao", 4, ssaoColorBufferBlur);
						// use ambient occlusion texture only if ssao is disabled
						deferredLightMaterial->SetFloat("useAmbientOcclusionTexture", ssaoEnabled ? 0.0f : 1.0f);
					}
					else
					{
						deferredLightMaterial->SetTexture("screenTexture", 0, i % 2 == 0 ? textureColorBuffer1 : textureColorBuffer2);
						deferredLightMaterial->SetTexture("gPosition", 2, gPosition);
						deferredLightMaterial->SetTexture("gNormal", 3, gNormal);
						deferredLightMaterial->SetTexture("gAlbedo", 4, gAlbedo);
						deferredLightMaterial->SetTexture("gMetallic", 5, gMetallic);
						deferredLightMaterial->SetTexture("gRoughness", 6, gRoughness);
						deferredLightMaterial->SetTexture("gEmissive", 7, gEmissive);
						deferredLightMaterial->SetTexture("gAmbientOcclusion", 8, gAmbientOcclusion);
					}
					deferredLightMaterial->Draw(glm::mat4());
					MeshPrimitivesPool::GetQuadPrimitive()->DrawSubMesh(0);
				}

				lastReachedLightIndex++;
			}

			// blitting if necessary so framebuffer2 has the most recent data
			if ((lastReachedLightIndex - 1) % 2 == 1)
			{
				// after rendering the last shadow pass, blit data to the framebuffer that's used later for the rest of rendering process
				glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer1);
				glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer2);

				// blit only color
				glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
			}

			// make sure both main framebuffers have correct depth & stencil data from the gBuffer
			glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer1);

			glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_STENCIL_BUFFER_BIT, GL_NEAREST);
			glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

			glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer2);

			glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_STENCIL_BUFFER_BIT, GL_NEAREST);
			glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer2);
		}
	}

	void RenderingManager::DrawForwardShadedObjects(int width, int height)
	{
		if (AreThereAnyForwardRendereredMeshes())
		{
			std::vector<Light*> lights = GetLightsManager()->GetAllLights();
			const bool areThereAnyShadowCasters = AreThereAnyShadowCasters();
			if (!lights.empty() && areThereAnyShadowCasters)
			{
				for (size_t i = 0; i < lights.size(); i++)
				{
					glEnable(GL_DEPTH_TEST);
					glEnable(GL_STENCIL_TEST);

					DirectionalLight* directionalLight = dynamic_cast<DirectionalLight*>(lights[i]);
					if (directionalLight != nullptr)
					{
						glm::mat4 lightSpaceMatrix;
						UpdateDirectionalShadowMap(directionalLight, lightSpaceMatrix);
					}
					PointLight* pointLight = dynamic_cast<PointLight*>(lights[i]);
					if (pointLight != nullptr)
					{
						UpdateOmnidirectionalShadowMap(pointLight);
					}
					SpotLight* spotLight = dynamic_cast<SpotLight*>(lights[i]);
					if (spotLight != nullptr)
					{
						glm::mat4 lightSpaceMatrix;
						UpdateSpotLightShadowMap(spotLight, lightSpaceMatrix);
					}

					// reset viewport
					glViewport(0, 0, width, height);
					const unsigned int currentFramebuffer = i % 2 == 0 ? framebuffer2 : framebuffer1;
					glBindFramebuffer(GL_FRAMEBUFFER, currentFramebuffer);
					// clear color and depth buffer for next rendering steps so that new pass can be rendered on top of the previous one
					// i > 1 since we have 2 separate framebuffers, no need to clean buffer is nothing is written yet
					if (i > 1)
					{
						glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

						// before drawing new meshes, make sure that gBuffer data is copied if needed
						if (AreThereAnyDeferredRendereredMeshes())
						{
							glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
							glBindFramebuffer(GL_DRAW_FRAMEBUFFER, currentFramebuffer);

							glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_STENCIL_BUFFER_BIT, GL_NEAREST);
							glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);

							glBindFramebuffer(GL_FRAMEBUFFER, currentFramebuffer);
						}
					}

					ApplyLightForRenderers(lights[i], opaqueMeshRenderers);
					ApplyLightForRenderers(lights[i], transparentMeshRenderers);

					DrawRenderersExceptLightModel(opaqueMeshRenderers, LightModelType::LitDeferred);
					DrawRenderersExceptLightModel(transparentMeshRenderers, LightModelType::LitDeferred);

					if (i > 0)
					{
						glDisable(GL_DEPTH_TEST);
						glDisable(GL_STENCIL_TEST);

						glBindFramebuffer(GL_FRAMEBUFFER, i % 2 == 0 ? shadowFBO2 : shadowFBO1);
						glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

						// merge last texture and sum of previous textures
						// set latest texture
						textureMergerMaterial->SetTexture("screenTexture1", 0, i % 2 == 0 ? textureColorBuffer2 : textureColorBuffer1);
						// set sum of textures
						if (i == 1) // for i equal 1 we use the first texture to sum first and second texture
						{
							textureMergerMaterial->SetTexture("screenTexture2", 1, textureColorBuffer2);
						}
						else // otherwise we use sum
						{
							textureMergerMaterial->SetTexture("screenTexture2", 1, i % 2 == 0 ? shadowColorBuffer1 : shadowColorBuffer2);
						}

						textureMergerMaterial->Draw(glm::mat4());
						MeshPrimitivesPool::GetQuadPrimitive()->DrawSubMesh(0);
					}

					// clear lights so the next light pass won't be affected by previous one
					ClearLightsForRenderers(opaqueMeshRenderers);
					ClearLightsForRenderers(transparentMeshRenderers);
				}

				if (lights.size() > 1)
				{
					// after rendering the last shadow pass, blit data to the framebuffer that's used later for the rest of rendering process
					glBindFramebuffer(GL_READ_FRAMEBUFFER, lights.size() % 2 == 0 ? shadowFBO1 : shadowFBO2);
					glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer2);

					// blit only color
					glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

					glEnable(GL_DEPTH_TEST);
					glEnable(GL_STENCIL_TEST);
				}
				glBindFramebuffer(GL_FRAMEBUFFER, framebuffer2);
			}
			else
			{
				glBindFramebuffer(GL_FRAMEBUFFER, framebuffer2);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				ApplyAllLightDataForForwardRenderers();
				DrawRenderersExceptLightModel(opaqueMeshRenderers, LightModelType::LitDeferred);
				DrawRenderersExceptLightModel(transparentMeshRenderers, LightModelType::LitDeferred);
			}
		}
	}

	void RenderingManager::DrawSkybox()
	{
		if (skybox != nullptr)
		{
			skybox->Draw();
		}
	}

	void RenderingManager::DrawDebug()
	{
		if (IsNormalsDebugEnabled() && (normalDebugMaterial != nullptr))
		{
			DrawRenderers(meshRenderers, normalDebugMaterial);
		}
	}

	void RenderingManager::DrawScreenEffects(int width, int height)
	{
		// disable depth and stencil test so we don't overwrite it for screen effects
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_STENCIL_TEST);

		// sort post process materials if it's the first frame to make sure their container is properly initialized
		if (firstRenderedFrame)
		{
			firstRenderedFrame = false;
			SortPostProcessMaterials();
		}

		// Post process effects
		const size_t postProcessEffects = usedPostProcessMaterials.size();
		if (IsPostProcessingEnabled())
		{
			// post processing via ping pong rendering
			for (size_t i = 0; i < postProcessEffects; i++)
			{
				glBindFramebuffer(GL_FRAMEBUFFER, i % 2 == 0 ? framebuffer2 : framebuffer1);
				glClear(GL_COLOR_BUFFER_BIT);

				const std::shared_ptr<Material> ppMaterial = usedPostProcessMaterials[i];
				ppMaterial->SetTexture("screenTexture", 0, i % 2 == 0 ? textureColorBuffer1 : textureColorBuffer2);
				ppMaterial->SetTexture("depthStencilTexture", 1, i % 2 == 0 ? depthStencilBuffer1 : depthStencilBuffer2);
				ppMaterial->SetTexture("stencilView", 2, i % 2 == 0 ? stencilView1 : stencilView2);
				ppMaterial->Draw(glm::mat4());
				MeshPrimitivesPool::GetQuadPrimitive()->DrawSubMesh(0);
			}
		}
		int lastPostProcessMaterialIndex = postProcessEffects;

		// Bloom
		if (IsBloomEnabled())
		{
			// Bind bloom buffer
			glBindFramebuffer(GL_FRAMEBUFFER, bloomFBO);

			// DOWNSAMPLE
			glm::vec2 screenResolution = glm::vec2((float)width, (float)height);
			bloomDownsampleMaterial->SetVector2("screenResolution", screenResolution);
			bloomDownsampleMaterial->SetTexture("screenTexture", 0, lastPostProcessMaterialIndex % 2 == 0 ? textureColorBuffer1 : textureColorBuffer2);

			// Progressively downsample through the mip chain
			for (int i = 0; i < bloomMipChain.size(); i++)
			{
				const BloomMip& mip = bloomMipChain[i];
				glViewport(0, 0, mip.size.x, mip.size.y);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mip.texture, 0);

				// Render screen-filled quad of resolution of current mip
				bloomDownsampleMaterial->Draw(glm::mat4());
				MeshPrimitivesPool::GetQuadPrimitive()->DrawSubMesh(0);

				// Set current mip resolution as srcResolution for next iteration
				bloomDownsampleMaterial->SetVector2("screenResolution", mip.size);
				bloomDownsampleMaterial->SetTexture("screenTexture", 0, mip.texture);
			}

			// UPSAMPLE
			const float bloomFilterRadius = 0.005f;
			bloomUpsampleMaterial->SetFloat("filterRadius", bloomFilterRadius);
			bloomUpsampleMaterial->SetFloat("screenAspectRatio", (float)width / (float)height);

			// Enable additive blending
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE);
			glBlendEquation(GL_FUNC_ADD);

			for (int i = bloomMipChain.size() - 1; i > 0; i--)
			{
				const BloomMip& mip = bloomMipChain[i];
				const BloomMip& nextMip = bloomMipChain[i - 1];

				// Bind viewport and texture from where to read
				bloomUpsampleMaterial->SetTexture("screenTexture", 0, mip.texture);

				// Set framebuffer render target (we write to this texture)
				glViewport(0, 0, nextMip.size.x, nextMip.size.y);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, nextMip.texture, 0);

				// Render screen-filled quad of resolution of current mip
				bloomUpsampleMaterial->Draw(glm::mat4());
				MeshPrimitivesPool::GetQuadPrimitive()->DrawSubMesh(0);
			}

			// Disable additive blending
			glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA); // Restore if this was default
			glDisable(GL_BLEND);

			// Reset viewport
			glViewport(0, 0, width, height);

			// Unbind bloom buffer
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		// HDR tone mapping and gamma correction
		if (IsHDRToneMappingAndGammaEnabled())
		{
			glBindFramebuffer(GL_FRAMEBUFFER, lastPostProcessMaterialIndex % 2 == 0 ? framebuffer2 : framebuffer1);
			glClear(GL_COLOR_BUFFER_BIT);
			hdrToneMappingGammaCorrectionMaterial->SetTexture("screenTexture", 0, lastPostProcessMaterialIndex % 2 == 0 ? textureColorBuffer1 : textureColorBuffer2);
			hdrToneMappingGammaCorrectionMaterial->SetTexture("depthStencilTexture", 1, lastPostProcessMaterialIndex % 2 == 0 ? depthStencilBuffer1 : depthStencilBuffer2);
			hdrToneMappingGammaCorrectionMaterial->SetTexture("stencilView", 2, lastPostProcessMaterialIndex % 2 == 0 ? stencilView1 : stencilView2);
			hdrToneMappingGammaCorrectionMaterial->SetTexture("bloomBlur", 3, bloomMipChain[0].texture);
			hdrToneMappingGammaCorrectionMaterial->SetFloat("bloomEnabled", IsBloomEnabled() ? 1.0f : 0.0f);
			hdrToneMappingGammaCorrectionMaterial->SetFloat("bloomStrength", bloomStrength);
			hdrToneMappingGammaCorrectionMaterial->Draw(glm::mat4());
			MeshPrimitivesPool::GetQuadPrimitive()->DrawSubMesh(0);
			lastPostProcessMaterialIndex++;
		}
		
		glBindFramebuffer(GL_FRAMEBUFFER, lastPostProcessMaterialIndex % 2 == 0 ? framebuffer2 : framebuffer1);
		glClear(GL_COLOR_BUFFER_BIT);
		editorOutlineMaterial->SetTexture("screenTexture", 0, lastPostProcessMaterialIndex % 2 == 0 ? textureColorBuffer1 : textureColorBuffer2);
		editorOutlineMaterial->SetTexture("depthStencilTexture", 1, lastPostProcessMaterialIndex % 2 == 0 ? depthStencilBuffer1 : depthStencilBuffer2);
		editorOutlineMaterial->SetTexture("stencilView", 2, lastPostProcessMaterialIndex % 2 == 0 ? stencilView1 : stencilView2);
		editorOutlineMaterial->Draw(glm::mat4());
		MeshPrimitivesPool::GetQuadPrimitive()->DrawSubMesh(0);
		lastPostProcessMaterialIndex++;

		// lastly apply anti aliasing post process
		if (aaEnabled)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, lastPostProcessMaterialIndex % 2 == 0 ? framebuffer2 : framebuffer1);
			glClear(GL_COLOR_BUFFER_BIT);
			fxaaMaterial->SetTexture("screenTexture", 0, lastPostProcessMaterialIndex % 2 == 0 ? textureColorBuffer1 : textureColorBuffer2);
			fxaaMaterial->SetFloat("contrastThreshold", fxaaContrastThreshold);
			fxaaMaterial->SetFloat("relativeThreshold", fxaaRelativeThreshold);
			fxaaMaterial->SetFloat("subpixelBlending", fxaaSubpixelBlending);
			fxaaMaterial->SetInt("edgeSearchStepsNumber", fxaaEdgeSearchStepsNumber);
			fxaaMaterial->Draw(glm::mat4());
			MeshPrimitivesPool::GetQuadPrimitive()->DrawSubMesh(0);
			lastPostProcessMaterialIndex++;
		}

		finalColorBuffer = lastPostProcessMaterialIndex % 2 == 0 ? textureColorBuffer1 : textureColorBuffer2;
	}

	void RenderingManager::DrawRenderers(const std::vector<MeshRenderer*>& renderers)
	{
		for (size_t i = 0; i < renderers.size(); i++)
		{
			MeshRenderer* meshRenderer = renderers[i];
			if (meshRenderer != nullptr)
			{
				meshRenderer->Draw();
			}
		}
	}

	void RenderingManager::DrawRenderersOfLightModel(const std::vector<MeshRenderer*>& renderers, LightModelType lightModel)
	{
		for (size_t i = 0; i < renderers.size(); i++)
		{
			MeshRenderer* meshRenderer = renderers[i];
			if (meshRenderer != nullptr && meshRenderer->GetLightModelType() == lightModel)
			{
				meshRenderer->Draw();
			}
		}
	}

	void RenderingManager::DrawRenderersExceptLightModel(const std::vector<MeshRenderer*>& renderers,
		LightModelType lightModel)
	{
		for (size_t i = 0; i < renderers.size(); i++)
		{
			MeshRenderer* meshRenderer = renderers[i];
			if (meshRenderer != nullptr && meshRenderer->GetLightModelType() != lightModel)
			{
				meshRenderer->Draw();
			}
		}
	}

	void RenderingManager::DrawRenderers(const std::vector<MeshRenderer*>& renderers, Material* material)
	{
		for (size_t i = 0; i < renderers.size(); i++)
		{
			MeshRenderer* meshRenderer = renderers[i];
			if (meshRenderer != nullptr)
			{
				meshRenderer->Draw(material);
			}
		}
	}

	void RenderingManager::DrawShadowCastingRenderers(const std::vector<MeshRenderer*>& renderers, Material* material)
	{
		for (size_t i = 0; i < renderers.size(); i++)
		{
			MeshRenderer* meshRenderer = renderers[i];
			if ((meshRenderer != nullptr) && (meshRenderer->IsCastingShadow()))
			{
				meshRenderer->Draw(material);
			}
		}
	}

	void RenderingManager::DrawPostProcessEffects()
	{

	}

	void RenderingManager::ApplyLightForRenderers(Light* light, const std::vector<MeshRenderer*>& renderers)
	{
		for (size_t i = 0; i < renderers.size(); i++)
		{
			for (size_t j = 0; j < renderers[i]->materialList.size(); j++)
			{
				light->SetLightInShader(renderers[i]->materialList[j]->GetShaderProgram(), true, true, 0, true, 1);
			}
		}
	}

	void RenderingManager::ApplyAllLightDataForForwardRenderers()
	{
		for (size_t i = 0; i < meshRenderers.size(); i++)
		{
			if (meshRenderers[i]->GetLightModelType() == LightModelType::LitForward)
			{
				for (size_t j = 0; j < meshRenderers[i]->materialList.size(); j++)
				{
					const GLuint shaderProgram = meshRenderers[i]->materialList[j]->GetShaderProgram();
					glUseProgram(shaderProgram);
					GetLightsManager()->SetLightsInShader(shaderProgram);
				}
			}
		}
	}

	void RenderingManager::ClearLightsForRenderers(const std::vector<MeshRenderer*>& renderers)
	{
		for (size_t i = 0; i < renderers.size(); i++)
		{
			for (size_t j = 0; j < renderers[i]->materialList.size(); j++)
			{
				const GLuint shaderProgram = renderers[i]->materialList[j]->GetShaderProgram();
				glUseProgram(shaderProgram);
				glUniform1f(glGetUniformLocation(shaderProgram, "ambientLightActive"), 0.0f);
				glUniform1i(glGetUniformLocation(shaderProgram, "dirLightsNumber"), 0);
				glUniform1i(glGetUniformLocation(shaderProgram, "pointLightsNumber"), 0);

				// TODO: think how to make it more generic since some shaders have different texture indices and this could overwrite their textures
				// always make sure that textures are set, even when they're not used; textures must be bound and complete			
				renderers[i]->materialList[j]->SetTexture("directionalLightShadowMap", 0, GetInstance()->directionalDepthMap);
				renderers[i]->materialList[j]->SetTexture("spotLightShadowMap", 1, GetInstance()->spotLightDepthMap);
				renderers[i]->materialList[j]->SetCubeTexture("pointLightShadowMap", 2, GetInstance()->omniDepthMap);
			}
		}
	}

	void RenderingManager::SetSkybox(SkyboxRenderer* skybox)
	{
		if (GetInstance()->skybox != nullptr)
		{
			delete GetInstance()->skybox;
		}
		GetInstance()->skybox = skybox;
	}

	void RenderingManager::AddMeshRenderer(MeshRenderer* meshRenderer)
	{
		GetInstance()->meshRenderers.push_back(meshRenderer);
		SortMeshRenderers();
	}

	void RenderingManager::RemoveMeshRenderer(MeshRenderer* meshRenderer)
	{
		for (size_t i = 0; i < GetInstance()->meshRenderers.size(); i++)
		{
			if (GetInstance()->meshRenderers[i] == meshRenderer)
			{
				GetInstance()->meshRenderers.erase(GetInstance()->meshRenderers.begin() + i);
				break;
			}
		}
		SortMeshRenderers();
	}

	void RenderingManager::AddPostProcessMaterial(const std::shared_ptr<PostProcessMaterial>& ppMaterial)
	{
		GetInstance()->postProcessMaterials.push_back(ppMaterial);
	}

	void RenderingManager::RemovePostProcessMaterial(const std::shared_ptr<PostProcessMaterial>& ppMaterial)
	{
		for (size_t i = 0; i < GetInstance()->postProcessMaterials.size(); i++)
		{
			if (ppMaterial == GetInstance()->postProcessMaterials[i])
			{
				GetInstance()->postProcessMaterials.erase(GetInstance()->postProcessMaterials.begin() + i);
				break;
			}
		}
	}

	void RenderingManager::SortMeshRenderers()
	{
		GetInstance()->opaqueMeshRenderers.clear();
		GetInstance()->transparentMeshRenderers.clear();

		for (size_t i = 0; i < GetInstance()->meshRenderers.size(); i++)
		{
			MeshRenderer* meshRenderer = GetInstance()->meshRenderers[i];
			if (meshRenderer->GetRenderQueuePosition() < static_cast<int>(RenderQueuePosition::Transparent))
			{
				GetInstance()->opaqueMeshRenderers.push_back(meshRenderer);
			}
			else
			{
				GetInstance()->transparentMeshRenderers.push_back(meshRenderer);
			}
		}

		SortOpaqueMeshRenderers();
		SortTransparentMeshRenderers();
	}

	void RenderingManager::SortPostProcessMaterials()
	{
		std::sort(GetInstance()->postProcessMaterials.begin(), GetInstance()->postProcessMaterials.end(), ComparePostProcessMaterialsPositions);
		GetInstance()->usedPostProcessMaterials.clear();
		for (size_t i = 0; i < GetInstance()->postProcessMaterials.size(); i++)
		{
			if (!Math::IsNearlyZero(GetInstance()->postProcessMaterials[i]->GetBlendWeight()))
			{
				GetInstance()->usedPostProcessMaterials.push_back(GetInstance()->postProcessMaterials[i]);
			}
		}
	}

	void RenderingManager::SortOpaqueMeshRenderers()
	{
		std::sort(GetInstance()->opaqueMeshRenderers.begin(), GetInstance()->opaqueMeshRenderers.end(), CompareRenderersPositions);
	}

	void RenderingManager::SortTransparentMeshRenderers()
	{
		// For now we assume 2 things:
		// 1) The order in render queue doesn't matter if it's after transparent
		// 2) For now we use simple & naive sort based on distance from the camera
		std::sort(GetInstance()->transparentMeshRenderers.begin(), GetInstance()->transparentMeshRenderers.end(), CompareTransparentRenderersPositions);
	}

	LightsManager* RenderingManager::GetLightsManager()
	{
		return GetInstance()->lightsManager;
	}

	void RenderingManager::EnablePostProcessing(bool enable)
	{
		GetInstance()->postProcessingEnabled = enable;
	}

	bool RenderingManager::IsPostProcessingEnabled()
	{
		return (GetInstance()->postProcessingEnabled);
	}

	void RenderingManager::EnableBloom(bool enable)
	{
		GetInstance()->bloomEnabled = enable;
	}

	bool RenderingManager::IsBloomEnabled()
	{
		return (GetInstance()->bloomEnabled);
	}

	void RenderingManager::EnableHDRToneMappingAndGamma(bool enable)
	{
		GetInstance()->hdrTonemappingAndGamma = enable;
	}

	bool RenderingManager::IsHDRToneMappingAndGammaEnabled()
	{
		return (GetInstance()->hdrTonemappingAndGamma);
	}

	void RenderingManager::SetWireframeOnly(bool wireframeOnly)
	{
		glPolygonMode(GL_FRONT_AND_BACK, wireframeOnly ? GL_LINE : GL_FILL);
		EnablePostProcessing(!wireframeOnly);
		GetInstance()->renderWireframeOnly = wireframeOnly;
	}

	bool RenderingManager::IsWireframeOnly()
	{
		return GetInstance()->renderWireframeOnly;
	}

	void RenderingManager::EnableNormalsDebug(bool enable)
	{
		GetInstance()->normalsDebugEnabled = enable;
	}

	bool RenderingManager::IsNormalsDebugEnabled()
	{
		return GetInstance()->normalsDebugEnabled;
	}

	void RenderingManager::SetGamma(float gammaVal)
	{
		GetInstance()->SetGammaInternal(gammaVal);
	}

	float RenderingManager::GetGamma()
	{
		return GetInstance()->gamma;
	}

	void RenderingManager::SetGammaInternal(float gammaVal)
	{
		gamma = gammaVal;
		if (hdrToneMappingGammaCorrectionMaterial != nullptr)
		{
			hdrToneMappingGammaCorrectionMaterial->SetFloat("gamma", gamma);
		}
	}

	void RenderingManager::SetExposure(float exposureVal)
	{
		GetInstance()->SetExposureInternal(exposureVal);
	}

	float RenderingManager::GetExposure()
	{
		return GetInstance()->exposure;
	}

	void RenderingManager::SetExposureInternal(float exposureVal)
	{
		exposure = exposureVal;
		if (hdrToneMappingGammaCorrectionMaterial != nullptr)
		{
			hdrToneMappingGammaCorrectionMaterial->SetFloat("exposure", exposure);
		}
	}

	void RenderingManager::SetBloomStrength(float amount)
	{
		GetInstance()->bloomStrength = amount;
	}

	float RenderingManager::GetBloomStrength()
	{
		return GetInstance()->bloomStrength;
	}

	void RenderingManager::SetBloomMipChainLength(unsigned int length)
	{
		GetInstance()->bloomMipChainLength = length;
		GetInstance()->ConfigureBloomBuffer(GetInstance()->currentBuffersWidth, GetInstance()->currentBuffersHeight, false);
	}

	unsigned int RenderingManager::GetBloomMipChainLength()
	{
		return GetInstance()->bloomMipChainLength;
	}

	void RenderingManager::SetShadowMapResolution(unsigned shadowMapRes)
	{
		GetInstance()->SetShadowMapResolutionInternal(shadowMapRes);
	}

	unsigned int RenderingManager::GetShadowMapResolution()
	{
		// shadow maps are currently square textures
		return GetInstance()->shadowWidth;
	}

	void RenderingManager::SetShadowMapResolutionInternal(unsigned shadowMapRes)
	{
		glDeleteTextures(1, &directionalDepthMap);
		glDeleteTextures(1, &spotLightDepthMap);
		glDeleteTextures(1, &omniDepthMap);

		shadowWidth = shadowMapRes;
		shadowHeight = shadowMapRes;

		ConfigureShadowMapFramebuffer(shadowWidth, shadowHeight, directionalDepthMapFBO, directionalDepthMap, false);
		ConfigureShadowMapFramebuffer(shadowWidth, shadowHeight, spotLightDepthMapFBO, spotLightDepthMap, false);
		ConfigureShadowCubeMapFramebuffer(shadowWidth, shadowHeight, omniDepthMapFBO, omniDepthMap, false);
	}

	void RenderingManager::EnableSSAO(bool enable)
	{
		GetInstance()->EnableSSAOInternal(enable);
	}

	bool RenderingManager::IsSSAOEnabled()
	{
		return GetInstance()->ssaoEnabled;
	}

	void RenderingManager::EnableSSAOInternal(bool enable)
	{
		ssaoEnabled = enable;
		if (!ssaoEnabled)
		{
			// clear ssaoBlurFBO color so that there's no ambient occlusion data in the ssao texture
			// do this only on disable since on enable ssaoBlurFBO is updated every frame anyway
			glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
			glClear(GL_COLOR_BUFFER_BIT);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
	}

	void RenderingManager::EnableAntiAliasing(bool enable)
	{
		GetInstance()->aaEnabled = enable;
	}

	bool RenderingManager::IsAntiAliasingEnabled()
	{
		return GetInstance()->aaEnabled;
	}

	void RenderingManager::SetAntiAliasingAlgorithm(AntiAliasingAlgorithm algorithm)
	{
		GetInstance()->aaAlgorithm = algorithm;
	}

	AntiAliasingAlgorithm RenderingManager::GetAntiAliasingAlgorithm()
	{
		return GetInstance()->aaAlgorithm;
	}

	float RenderingManager::GetFXAAContrastThreshold()
	{
		return GetInstance()->fxaaContrastThreshold;
	}

	void RenderingManager::SetFXAAContrastThreshold(float contrastThreshold)
	{
		GetInstance()->fxaaContrastThreshold = contrastThreshold;
	}

	float RenderingManager::GetFXAARelativeThreshold()
	{
		return GetInstance()->fxaaRelativeThreshold;
	}

	void RenderingManager::SetFXAARelativeThreshold(float relativeThreshold)
	{
		GetInstance()->fxaaRelativeThreshold = relativeThreshold;
	}

	float RenderingManager::GetFXAASubpixelBlending()
	{
		return GetInstance()->fxaaSubpixelBlending;
	}

	void RenderingManager::SetFXAASubpixelBlending(float subpixelBlending)
	{
		GetInstance()->fxaaSubpixelBlending = subpixelBlending;
	}

	int RenderingManager::GetFXAAEdgeSearchStepsNumber()
	{
		return GetInstance()->fxaaEdgeSearchStepsNumber;
	}

	void RenderingManager::SetFXAAEdgeSearchStepsNumber(int edgeSearchStepsNumber)
	{
		GetInstance()->fxaaEdgeSearchStepsNumber = edgeSearchStepsNumber;
	}

	int RenderingManager::GetObjectIdAt(int x, int y)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, GetInstance()->objectSelectionFBO);
		glReadBuffer(GL_COLOR_ATTACHMENT0);
		int objectId = 0;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &objectId);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return objectId;
	}

	unsigned RenderingManager::GetFinalColorBuffer()
	{
		return GetInstance()->finalColorBuffer;
	}

	float RenderingManager::GetBufferAspectRatio()
	{
		const float width = static_cast<float>(GetInstance()->currentBuffersWidth);
		const float height = static_cast<float>(GetInstance()->currentBuffersHeight);
		return width / height;
	}

	bool RenderingManager::CompareRenderersPositions(MeshRenderer* mr1, MeshRenderer* mr2)
	{
		return (mr1->GetRenderQueuePosition() < mr2->GetRenderQueuePosition());
	}

	bool RenderingManager::CompareTransparentRenderersPositions(MeshRenderer* mr1, MeshRenderer* mr2)
	{
		if (mr1->GetOwner() != nullptr && mr2->GetOwner() != nullptr)
		{
			const float length1 = glm::length(mr1->GetOwner()->GetTransform()->GetPosition() - CamerasManager::GetActiveCameraPosition());
			const float length2 = glm::length(mr2->GetOwner()->GetTransform()->GetPosition() - CamerasManager::GetActiveCameraPosition());
			return (length1 > length2);
		}
		else
		{
			return CompareRenderersPositions(mr1, mr2);
		}
	}

	bool RenderingManager::ComparePostProcessMaterialsPositions(const std::shared_ptr<PostProcessMaterial>& ppm1,
		const std::shared_ptr<PostProcessMaterial>& ppm2)
	{
		return (ppm1->GetPostProcessOrder() < ppm2->GetPostProcessOrder());
	}

	bool RenderingManager::AreThereAnyDeferredRendereredMeshes() const
	{
		return GetRenderersNumberOfType(LightModelType::LitDeferred) > 0;
	}

	bool RenderingManager::AreThereAnyForwardRendereredMeshes() const
	{
		// By default unlit mesh renderers are rendered with forward rendering approach
		return (GetRenderersNumberOfType(LightModelType::LitForward) > 0 || GetRenderersNumberOfType(LightModelType::Unlit) > 0);
	}

	// TODO: optimize this, this value could be cached since even if this property changes, it won't change often
	size_t RenderingManager::GetRenderersNumberOfType(LightModelType lightModel) const
	{
		size_t count = 0;
		for (size_t i = 0; i < meshRenderers.size(); i++)
		{
			if (meshRenderers[i]->GetLightModelType() == lightModel)
			{
				count++;
			}
		}
		return count;
	}
}