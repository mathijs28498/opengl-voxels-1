#version 330 core
in vec3 normal;
in vec2 texCoord;

out vec4 fragColor;

uniform vec3 color;
uniform float glfwTime;
uniform vec3 pos;

void main() {
//	fragColor = vec4(color, 1.0);
	fragColor = vec4(sin(glfwTime + sqrt(pos.x * pos.x+ pos.y * pos.y + pos.z * pos.z)) / 2 + 0.5, 
						cos(3.5 * glfwTime + sqrt(pos.x * pos.x + pos.y * pos.y + pos.z * pos.z)) / 2 + 0.5,
						1 - sin(2 * glfwTime + sqrt(pos.x * pos.x + pos.y * pos.y + pos.z * pos.z)) / 2 + 0.5, 1.0);
}