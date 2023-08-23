#include "SceneExample_Instancing.h"

#include "MeshImported.h"
#include "MeshRenderer.h"
#include "TextureLoader.h"
#include "Time.h"

SceneExample_Instancing::SceneExample_Instancing()
{
	// Planet initialization
	GameObject* planetObject = new GameObject();

	MeshRenderer* planetMeshRenderer = new MeshRenderer();
	std::shared_ptr<MeshImported> planetMesh = std::make_shared<MeshImported>((GLchar*)"res/planet/planet.obj");
	planetMeshRenderer->SetMesh(planetMesh);

	std::shared_ptr<Material> planetMaterial = std::make_shared<Material>("res/shaders/unlitTextured.vert.glsl", "res/shaders/unlitTextured.frag.glsl");
	planetMaterial->SetTextureByPath("mainTexture", 0, (GLchar*)"res/planet/mars.png", true);
	planetMeshRenderer->SetMaterial(planetMaterial, 0);

	planetObject->AddComponent(planetMeshRenderer);

	std::string planetName = "planet";
	planetObject->SetName(planetName);

	scene->AddGameObject(planetObject);

	// Asteroid field initialization
	//1) Calculate the positions
	unsigned int asteroidsNumber = 500000;
	glm::mat4* modelMatrices;
	modelMatrices = new glm::mat4[asteroidsNumber];
	srand(Time::GetTime()); // initialize random seed	
	float radius = 200.0;
	float offset = 75.0f;
	for (unsigned int i = 0; i < asteroidsNumber; i++)
	{
		glm::mat4 model = glm::mat4(1.0f);
		// 1. translation: displace along circle with 'radius' in range [-offset, offset]
		float angle = (float)i / (float)asteroidsNumber * 360.0f;
		float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float x = sin(angle) * radius + displacement;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float y = displacement * 0.4f; // keep height of field smaller compared to width of x and z
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z = cos(angle) * radius + displacement;
		model = glm::translate(model, glm::vec3(x, y, z));

		// 2. scale: scale between 0.05 and 0.25f
		float scale = (rand() % 20) / 100.0f + 0.05;
		model = glm::scale(model, glm::vec3(scale));

		// 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
		float rotAngle = (rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		// 4. now add to list of matrices
		modelMatrices[i] = model;
	}

	// 2) Create asteroids and add them to scene
	std::shared_ptr<MeshImported> asteroidMesh = std::make_shared<MeshImported>((GLchar*)"res/rock/rock.obj");
	GLuint asteroidTexture = TextureLoader::LoadTexture((GLchar*)"res/rock/rock.png", false);

	//InitAsteroidsUninstanced(asteroidsNumber, asteroidMesh, asteroidTexture, modelMatrices);
	InitAsteroidsInstanced(asteroidsNumber, asteroidMesh, asteroidTexture, modelMatrices);

	// Set editor spectator
	GameObject* editorSpectatorObject = AddEditorSpectator(glm::vec3(0.0f, 60.0f, 300.0f), glm::vec3(0.0f, 180.0f, 0.0f));

	delete modelMatrices;
}

void SceneExample_Instancing::InitAsteroidsUninstanced(int asteroidsNumber, std::shared_ptr<MeshImported> asteroidMesh, GLuint asteroidTexture, glm::mat4* modelMatrices)
{
	// for uninstanced meshes 2000 is a safe limit before the FPS starts dropping to jittery levels
	int upperLimit = std::min(2000, asteroidsNumber);
	for (int i = 0; i < upperLimit; i++)
	{
		GameObject* asteroidObject = new GameObject();

		MeshRenderer* asteroidMeshRenderer = new MeshRenderer();
		asteroidMeshRenderer->SetMesh(asteroidMesh);

		std::shared_ptr<Material> asteroidMaterial = std::make_shared<Material>("res/shaders/unlitTextured.vert.glsl", "res/shaders/unlitTextured.frag.glsl");
		asteroidMaterial->SetTexture("mainTexture", 0, asteroidTexture);
		asteroidMeshRenderer->SetMaterial(asteroidMaterial, 0);

		asteroidObject->AddComponent(asteroidMeshRenderer);

		std::string asteroidName = "asteroid_";
		asteroidName.append(std::to_string(i));
		asteroidObject->SetName(asteroidName);

		asteroidObject->GetTransform()->SetTransformMatrix(modelMatrices[i]);

		scene->AddGameObject(asteroidObject);
	}
}

void SceneExample_Instancing::InitAsteroidsInstanced(int asteroidsNumber, std::shared_ptr<MeshImported> asteroidMesh,
	GLuint asteroidTexture, glm::mat4* modelMatrices)
{
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, asteroidsNumber * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

	for (int i = 0; i < asteroidMesh->GetSubMeshesCount(); i++)
	{
		unsigned int VAO = asteroidMesh->GetSubMesh(i)->GetVAO();
		glBindVertexArray(VAO);
		// vertex attributes
		std::size_t vec4Size = sizeof(glm::vec4);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

		// make attribute per instance instead of per vertex
		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	asteroidMesh->SetInstancing(true, asteroidsNumber);

	std::shared_ptr<Material> asteroidMaterial = std::make_shared<Material>("res/shaders/unlitTexturedInstanced.vert.glsl", "res/shaders/unlitTextured.frag.glsl");

	GameObject* asteroidField = new GameObject();

	MeshRenderer* asteroidFieldMeshRenderer = new MeshRenderer();
	asteroidFieldMeshRenderer->SetMesh(asteroidMesh);

	asteroidMaterial->SetTexture("mainTexture", 0, asteroidTexture);
	asteroidFieldMeshRenderer->SetMaterial(asteroidMaterial, 0);

	asteroidField->AddComponent(asteroidFieldMeshRenderer);

	std::string asteroidFieldName = "asteroid_field";
	asteroidField->SetName(asteroidFieldName);

	scene->AddGameObject(asteroidField);
}