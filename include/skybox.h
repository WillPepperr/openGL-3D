#ifndef SKYBOX_H
#define SKYBOX_H

#include "glad/glad.h"
#include <string>

class SkyBox {
public:
	SkyBox();

	void initiateBuffer();
	void loadImages();
	void draw();
	
	std::string facesCubemap[6] = 
	{
		"textures/skyboxes/blueNebula/BlueSkybox_right1.png",
		"textures/skyboxes/blueNebula/BlueSkybox_left2.png",
		"textures/skyboxes/blueNebula/BlueSkybox_top3.png",
		"textures/skyboxes/blueNebula/BlueSkybox_bottom4.png",
		"textures/skyboxes/blueNebula/BlueSkybox_front5.png",
		"textures/skyboxes/blueNebula/BlueSkybox_back6.png"
	};

private:

	float skyboxvertices[24] = 
	{
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f 
	};

	unsigned int skyboxIndices[36] =
	{
		// Right
		1, 2, 6,
		6, 5, 1,
		// Left
		0, 4, 7,
		7, 3, 0,
		// Top
		4, 5, 6,
		6, 7, 4, 
		// Bottom
		0, 3, 2,
		2, 1, 0,
		// Back
		0, 1, 5, 
		5, 4, 0, 
		//Front
		3, 7, 6,
		6, 2, 3
	};

	unsigned int VAO, VBO, EBO;
	unsigned int cubemapTexture;
};

#endif
