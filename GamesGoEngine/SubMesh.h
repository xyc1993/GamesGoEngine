#pragma once

#include <string>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace GamesGoEngine
{
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
		glm::vec3 Tangent;
		glm::vec3 Bitangent;
	};

	class SubMesh
	{
	public:
		SubMesh(std::string name, std::vector<Vertex> vertices, std::vector<unsigned int> indices);

		void Draw();
		void DrawInstanced(int meshCount);
		// useful for debugging mesh with several sub meshes
		std::string GetName() const;
		unsigned int GetVAO() const;

	private:
		void SetupMesh();

		std::string name;
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		unsigned int VAO, VBO, EBO;
	};
}