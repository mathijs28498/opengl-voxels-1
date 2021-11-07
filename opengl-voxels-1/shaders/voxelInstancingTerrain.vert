#version 330 core
struct Material {
  vec4 color;
};

layout (location = 0) in uint positionInt;
layout (location = 1) in uint materialAndEnabledInt;

out vec4 gColor;
out float gSizeMult;
out uint gEnabledFaces;

Material materials;

vec3 intToVec3(uint intArg) {
	float i0 = int( intArg		  & 0xFFu);
	float i1 = int((intArg >> 8u)  & 0xFFu);
	float i2 = int((intArg >> 16u) & 0xFFu);
	return vec3(i0, i1, i2);  
}

uint int32ToInt16(uint intArg) {
	return 0x00000000u | (intArg & 0xFFFFu);
}

vec4 intToVec4(uint intArg) {
	float i0 = int( intArg		  & 0xFFu);
	float i1 = int((intArg >> 8u)  & 0xFFu);
	float i2 = int((intArg >> 16u) & 0xFFu);
	float i3 = int((intArg >> 24u) & 0xFFu);
	return vec4(i0, i1, i2, i3);  
}

void main() {
	vec4 position = intToVec4(positionInt);

	gl_Position = vec4(position.xyz, 1.0);
	uint matID = int32ToInt16(materialAndEnabledInt);
	gColor = materials[matID].color;
	gSizeMult = position.w;
	gEnabledFaces = (materialAndEnabledInt >> 24u) & 0xFFu;
}