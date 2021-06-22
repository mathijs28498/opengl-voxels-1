#version 330 core
layout (location = 0) in vec4 vVertPos;
layout (location = 1) in vec3 vColor;

out vec3 gColor;
out float gSizeMult;

void main() {
	gl_Position =  vec4(vVertPos.xyz, 1.0);
	gColor = vColor;
	gSizeMult = vVertPos.w;
}