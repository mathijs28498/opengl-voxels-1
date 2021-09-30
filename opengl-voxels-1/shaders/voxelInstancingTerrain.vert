#version 330 core
//precision highp int;

layout (location = 0) in vec4 vVertPos;
layout (location = 1) in vec3 vColor;
layout (location = 2) in unsigned int vEnabledFaces;

out vec3 gColor;
out float gSizeMult;
out unsigned int gEnabledFaces;

void main() {
	gl_Position =  vec4(vVertPos.xyz, 1.0);
//	float r = vColorInt / 255;
//	float g = (255 - vColorInt) / 255;
	
//	gColor = vec3(r, g, 0);
	gColor = vColor;
	gSizeMult = vVertPos.w;
	gEnabledFaces = vEnabledFaces;
}