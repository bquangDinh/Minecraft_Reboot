#version 330 core
uniform sampler2DArray ourTexture;

out vec4 FragColor;

in vec2 texCoord;
in float texIndex;

void main() {
	if (texIndex == -1.0){
		discard;
	}

	FragColor = texture(ourTexture, vec3(texCoord, texIndex));
} 