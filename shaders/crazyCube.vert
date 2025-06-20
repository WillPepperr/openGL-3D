#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aNormal;

out vec3 ObjectPos;
out vec3 ObjectNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{

    ObjectPos = aPos; 
    ObjectNormal = aNormal; 
    vec4 worldPosition = model * vec4(aPos, 1.0);

    gl_Position = projection * view * worldPosition;
}
