#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in float inTexIndex;

out vec2 texCoord;
out float texIndex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	gl_Position = projection * view * model * vec4(aPos, 1.0f);

	texCoord = aTexCoord;
	texIndex = inTexIndex;
}