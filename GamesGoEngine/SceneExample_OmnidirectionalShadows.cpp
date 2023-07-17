#include "SceneExample_OmnidirectionalShadows.h"

#include "MeshImported.h"
#include "MeshPrimitiveCube.h"
#include "MeshPrimitiveQuad.h"
#include "MeshPrimitiveSphere.h"
#include "MeshRenderer.h"

SceneExample_OmnidirectionalShadows::SceneExample_OmnidirectionalShadows()
{
	std::shared_ptr<MeshPrimitiveCube> cubeMesh = std::make_shared<MeshPrimitiveCube>();
	std::shared_ptr<MeshPrimitiveSphere> sphereMesh = std::make_shared<MeshPrimitiveSphere>();
	std::shared_ptr<MeshPrimitiveQuad> quadMesh = std::make_shared<MeshPrimitiveQuad>();

	std::shared_ptr<Material> brickwallMaterial = std::make_shared<Material>("res/shaders/litOmniShadowNormalMapping.vert.glsl", "res/shaders/litOmniShadowNormalMapping.frag.glsl");
	brickwallMaterial->SetTextureByPath((GLchar*)"diffuseTexture", 0, (GLchar*)"res/textures/brickwall/brickwall.jpg");
	brickwallMaterial->SetTextureByPath((GLchar*)"normalTexture", 2, (GLchar*)"res/textures/brickwall/brickwall_normal.jpg", false, false);
	brickwallMaterial->SetLightModel(LightModelType::LitForward);

	std::shared_ptr<Material> woodMaterial = std::make_shared<Material>("res/shaders/litOmniShadowSimple.vert.glsl", "res/shaders/litOmniShadowSimple.frag.glsl");
	woodMaterial->SetTextureByPath((GLchar*)"diffuseTexture", 0, (GLchar*)"res/textures/wood.png");
	woodMaterial->SetLightModel(LightModelType::LitForward);
	
	std::shared_ptr<Material> brickParallaxMaterial = std::make_shared<Material>("res/shaders/litOmniShadowParallaxMapping.vert.glsl", "res/shaders/litOmniShadowParallaxMapping.frag.glsl");
	brickParallaxMaterial->SetTextureByPath((GLchar*)"diffuseTexture", 0, (GLchar*)"res/textures/bricksParallax/bricks2.jpg");
	brickParallaxMaterial->SetTextureByPath((GLchar*)"normalTexture", 2, (GLchar*)"res/textures/bricksParallax/bricks2_normal.jpg", false, false);
	brickParallaxMaterial->SetTextureByPath((GLchar*)"parallaxTexture", 3, (GLchar*)"res/textures/bricksParallax/bricks2_disp.jpg", false, false);
	brickParallaxMaterial->SetLightModel(LightModelType::LitForward);
	brickParallaxMaterial->SetFloat("height_scale", 0.1f);

	std::shared_ptr<Material> woodParallaxMaterial = std::make_shared<Material>("res/shaders/litOmniShadowParallaxMapping.vert.glsl", "res/shaders/litOmniShadowParallaxMapping.frag.glsl");
	woodParallaxMaterial->SetTextureByPath((GLchar*)"diffuseTexture", 0, (GLchar*)"res/textures/woodParallax/wood.png");
	woodParallaxMaterial->SetTextureByPath((GLchar*)"normalTexture", 2, (GLchar*)"res/textures/woodParallax/toy_box_normal.png", false, false);
	woodParallaxMaterial->SetTextureByPath((GLchar*)"parallaxTexture", 3, (GLchar*)"res/textures/woodParallax/toy_box_disp.png", false, false);
	woodParallaxMaterial->SetLightModel(LightModelType::LitForward);
	woodParallaxMaterial->SetFloat("height_scale", 0.1f);

	// Add walls, floor and ceiling
	std::vector<glm::vec3> boundsPositions;
	boundsPositions.push_back(glm::vec3(0.0f, -2.0f, 0.0f));
	boundsPositions.push_back(glm::vec3(0.0f, 6.0f, 0.0f));
	boundsPositions.push_back(glm::vec3(-7.0f, 2.0f, 0.0f));
	boundsPositions.push_back(glm::vec3(7.0f, 2.0f, 0.0f));
	boundsPositions.push_back(glm::vec3(0.0f, 2.0f, -5.5f));
	boundsPositions.push_back(glm::vec3(0.0f, 2.0f, 7.0f));

	std::vector<glm::vec3> boundsRotations;
	boundsRotations.push_back(glm::vec3(-90.0f, 0.0f, 0.0f));
	boundsRotations.push_back(glm::vec3(90.0f, 0.0f, 0.0f));
	boundsRotations.push_back(glm::vec3(0.0f, 90.0f, 0.0f));
	boundsRotations.push_back(glm::vec3(0.0f, -90.0f, 0.0f));
	boundsRotations.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	boundsRotations.push_back(glm::vec3(0.0f, 180.0f, 0.0f));

	for (size_t i = 0; i < boundsPositions.size(); i++)
	{
		GameObject* boundObject = new GameObject();
		MeshRenderer* boundMeshRenderer = new MeshRenderer();
		boundMeshRenderer->SetMesh(quadMesh);

		if (i == 4)
		{
			boundMeshRenderer->SetMaterial(brickParallaxMaterial);
		}
		else if (i == 3)
		{
			boundMeshRenderer->SetMaterial(woodParallaxMaterial);
		}
		else
		{
			boundMeshRenderer->SetMaterial(brickwallMaterial);
		}

		boundObject->AddComponent(boundMeshRenderer);
		std::string name = "bound_";
		name.append(std::to_string(i));
		boundObject->SetName(name);
		boundObject->GetTransform()->SetPosition(boundsPositions[i]);
		boundObject->GetTransform()->SetRotationEulerDegrees(boundsRotations[i]);
		if (i <= 1) // floor & ceiling
		{
			boundObject->GetTransform()->SetScale(glm::vec3(7.0f));
		}
		else //rest of the walls
		{
			boundObject->GetTransform()->SetScale(glm::vec3(7.0f, 4.0f, 7.0f));
		}
		
		scene->AddGameObject(boundObject);
	}

	// Add cubes & sphere
	std::vector<glm::vec3> cubePositions;
	cubePositions.push_back(glm::vec3(2.0f, 3.0f, -3.5f));
	cubePositions.push_back(glm::vec3(-3.0f, 1.0f, -2.0f));
	cubePositions.push_back(glm::vec3(-0.3f, -0.7f, -3.5f));

	std::vector<glm::vec3> cubeRotations;
	cubeRotations.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	cubeRotations.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	cubeRotations.push_back(glm::vec3(45.0f, 45.0f, 0.0f));

	for (size_t i = 0; i < cubePositions.size(); i++)
	{
		GameObject* cubeObject = new GameObject();
		MeshRenderer* cubeMeshRenderer = new MeshRenderer();
		std::string name;
		if (i == 0)
		{
			name = "sphere_";
			cubeMeshRenderer->SetMesh(sphereMesh);
		}
		else
		{
			name = "cube_";
			cubeMeshRenderer->SetMesh(cubeMesh);
		}
		cubeMeshRenderer->SetMaterial(woodMaterial);
		cubeObject->AddComponent(cubeMeshRenderer);
		name.append(std::to_string(i));
		cubeObject->SetName(name);
		cubeObject->GetTransform()->SetPosition(cubePositions[i]);
		cubeObject->GetTransform()->SetRotationEulerDegrees(cubeRotations[i]);
		scene->AddGameObject(cubeObject);
	}

	// Add complex mesh
	GameObject* nanoSuitObject = new GameObject();
	nanoSuitObject->GetTransform()->SetPosition(glm::vec3(0.0f, -2.0f, 0.0f));
	nanoSuitObject->GetTransform()->SetRotationEulerDegrees(glm::vec3(0.0f, 135.0f, 0.0f));
	nanoSuitObject->GetTransform()->SetScale(glm::vec3(0.2f));
	MeshRenderer* nanoSuitMeshRenderer = new MeshRenderer();
	std::shared_ptr<MeshImported> nanoSuitMesh = std::make_shared<MeshImported>((GLchar*)"res/nanosuit/nanosuit.obj");
	nanoSuitMeshRenderer->SetMesh(nanoSuitMesh);

	const int NANOSUIT_MATERIALS_NUMBER = 7;
	std::shared_ptr<Material> nanoSuitMaterials[NANOSUIT_MATERIALS_NUMBER];

	std::vector<GLchar*> nanoSuitTexturePaths;
	nanoSuitTexturePaths.push_back((GLchar*)"res/nanosuit/glass_dif.png");
	nanoSuitTexturePaths.push_back((GLchar*)"res/nanosuit/leg_dif.png");
	nanoSuitTexturePaths.push_back((GLchar*)"res/nanosuit/hand_dif.png");
	nanoSuitTexturePaths.push_back((GLchar*)"res/nanosuit/glass_dif.png"); // this one is small 'lights' on the helmet under visor, no specific texture was given for those
	nanoSuitTexturePaths.push_back((GLchar*)"res/nanosuit/arm_dif.png");
	nanoSuitTexturePaths.push_back((GLchar*)"res/nanosuit/helmet_diff.png");
	nanoSuitTexturePaths.push_back((GLchar*)"res/nanosuit/body_dif.png");

	std::vector<GLchar*> nanoSuitNormalTexturePaths;
	nanoSuitNormalTexturePaths.push_back((GLchar*)"res/nanosuit/glass_ddn.png");
	nanoSuitNormalTexturePaths.push_back((GLchar*)"res/nanosuit/leg_showroom_ddn.png");
	nanoSuitNormalTexturePaths.push_back((GLchar*)"res/nanosuit/hand_showroom_ddn.png");
	nanoSuitNormalTexturePaths.push_back((GLchar*)"res/nanosuit/glass_ddn.png"); // this one is small 'lights' on the helmet under visor, no specific texture was given for those
	nanoSuitNormalTexturePaths.push_back((GLchar*)"res/nanosuit/arm_showroom_ddn.png");
	nanoSuitNormalTexturePaths.push_back((GLchar*)"res/nanosuit/helmet_showroom_ddn.png");
	nanoSuitNormalTexturePaths.push_back((GLchar*)"res/nanosuit/body_showroom_ddn.png");

	for (int i = 0; i < NANOSUIT_MATERIALS_NUMBER; i++)
	{
		nanoSuitMaterials[i] = std::make_shared<Material>("res/shaders/litOmniShadowNormalMapping.vert.glsl", "res/shaders/litOmniShadowNormalMapping.frag.glsl");
		nanoSuitMaterials[i]->SetTextureByPath((GLchar*)"diffuseTexture", 0, nanoSuitTexturePaths[i]);
		nanoSuitMaterials[i]->SetTextureByPath((GLchar*)"normalTexture", 2, nanoSuitNormalTexturePaths[i], false, false);
		nanoSuitMaterials[i]->SetLightModel(LightModelType::LitForward);
		nanoSuitMeshRenderer->SetMaterial(nanoSuitMaterials[i], i);
	}

	nanoSuitObject->AddComponent(nanoSuitMeshRenderer);

	std::string nanoSuitName = "nanosuit";
	nanoSuitObject->SetName(nanoSuitName);

	scene->AddGameObject(nanoSuitObject);

	// Add source of light
	GameObject* pointLightObject = new GameObject();
	pointLightObject->GetTransform()->SetPosition(glm::vec3(-1.9f, 0.0f, 1.9f));
	pointLightObject->GetTransform()->SetScale(glm::vec3(0.2f));
	// Add debug renderer
	std::shared_ptr<Material> lampMaterial = std::make_shared<Material>("res/shaders/unlit.vert.glsl", "res/shaders/unlit.frag.glsl");
	glm::vec3 lightColor = glm::vec3(10.0f, 8.0f, 6.0f);
	lampMaterial->SetVector3((GLchar*)"unlitColor", lightColor);
	MeshRenderer* lampMeshRenderer = new MeshRenderer();
	lampMeshRenderer->SetMesh(sphereMesh);
	lampMeshRenderer->SetMaterial(lampMaterial);
	lampMeshRenderer->SetIsCastingShadow(false);
	pointLightObject->AddComponent(lampMeshRenderer);
	// Add point light component
	PointLight* pointLight = new PointLight(0.02f * lightColor, 0.8f * lightColor, lightColor, 1.0f, 0.09f, 0.032f);
	pointLightObject->AddComponent(pointLight);
	// Name and add to scene
	pointLightObject->SetName("point_light");
	scene->AddGameObject(pointLightObject);

	AddEditorSpectator();
}