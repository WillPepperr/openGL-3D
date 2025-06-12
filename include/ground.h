#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shaders.h"

class Ground {
public:
	Ground(float size = 200.0f);
	
	~Ground();

	void initRenderData();
	void draw(Shader& shader);
	void cleanupRenderData();

private:
	float size;
	glm::vec3 position = glm::vec3(0.0f);
	unsigned int VAO, VBO;
};

