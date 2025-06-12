#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D grassTexture;

void main() {
	vec3 baseColor = vec3(0.1, 0.3, 0.1);
	vec3 texColor = texture(grassTexture, TexCoord).rgb;
	
	vec3 finalColor = mix(baseColor, texColor, 0.2);
	FragColor = vec4(finalColor, 1.0);
}