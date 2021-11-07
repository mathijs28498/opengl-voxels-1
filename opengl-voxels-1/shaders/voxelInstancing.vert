#version 330 core
layout (location = 0) in vec3 vVertPos;
layout (location = 1) in vec4 vColor;
layout (location = 2) in float vSizeMult;

out vec4 gColor;
out float gSizeMult;


void main() {
	gl_Position =  vec4(vVertPos, 1.0);
	gColor = vColor;
	gSizeMult = vSizeMult;
}