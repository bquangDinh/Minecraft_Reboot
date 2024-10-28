#version 460 core

out vec4 FragColor;

void main() {
	vec4 color = vec4(1.0, 1.0, 1.0, 0.1);

	float gray = dot(color.rgb, vec3(0.299, 0.587, 0.114));

	FragColor = vec4(gray);

	FragColor.a *= 0.05;
}