#version 330 core
layout (location = 0) in vec3 vVertPos;
layout (location = 1) in vec3 vColor;

out vec3 gColor;

void main() {
	gl_Position =  vec4(vVertPos, 1.0);
//	gColor = vec3((vVertPos.y - 50 + 30) / 30, 1 - (vVertPos.y - 50 + 20) / 30, (vVertPos.y - 50) / 30);
	if (vVertPos.y < 25) {
		gColor = vec3(0.149, 0.290, 0.890);
	} else if (vVertPos.y < 30) {
		gColor = vec3(0.133, 0.835, 0.968);
	} else if (vVertPos.y < 35) {
		gColor = vec3(0.886, 0.890, 0.149);
	} else if (vVertPos.y < 45) {
		gColor = vec3(0.176, 0.501, 0.309);
	} else if (vVertPos.y < 55) {
		gColor = vec3(0.760, 0.760, 0.760);
	} else if (vVertPos.y < 65) {
		gColor = vec3(0.462, 0.494, 0.498);
	} else {
		gColor = vec3(1, 1, 1);
	}
}