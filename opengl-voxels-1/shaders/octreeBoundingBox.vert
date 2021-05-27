#version 330 core
layout (location = 0) in vec3 vVertPos;
layout (location = 1) in float vSize;
//layout (location = 2) in vec3 vSize;

out float gSize;

void main() {
	gl_Position =  vec4(vVertPos, 1.0);
	gSize = vSize;
}