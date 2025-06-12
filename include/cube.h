#ifndef CUBE_H
#define CUBE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shaders.h"

class Cube {
public:
	
	Cube(glm::vec3 position, float width = 1.0f, float height = 1.0f, float depth = 1.0f,  float angle = 0.0f);

	void draw(Shader& shader);
	void setSize(float w, float h, float d);
	void setPosition(glm::vec3 pos);
	void setAngle(float angle);

	glm::vec3& getPosition();
	glm::vec3 getSize() { return glm::vec3(width, height, depth); }
	float getAngle() { return angle; }
	
	glm::mat4 getModelMatrix() const;  
	static void initRenderData();
	static void cleanupRenderData();

	glm::vec3 position;
	float width, height, depth;
	float angle;
	

private:

	static unsigned int VAO, VBO;
};
#endif
