#include "ground.h"

Ground::Ground(float size)
	: size(size), VAO(0), VBO(0) {}

Ground::~Ground() {
	cleanupRenderData();
}

void Ground::initRenderData() {
	float half = size / 2.0f;

	float vertices[] = {
		// positions                // tex coords
		-half, 0.0f, -half,         0.0f, 0.0f,
		 half, 0.0f, -half,         size, 0.0f,
		 half, 0.0f,  half,         size, size,

		 half, 0.0f,  half,         size, size,
		-half, 0.0f,  half,         0.0f, size,
		-half, 0.0f, -half,         0.0f, 0.0f
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); // Position
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))); // TexCoords
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void Ground::draw(Shader& shader) {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position);
	model = glm::scale(model, glm::vec3(1.0f));       
	model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0, 1, 0));
	shader.setMat4("model", model);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void Ground::cleanupRenderData() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}