#version 330 core
layout(points) in;
layout(triangle_strip, max_vertices = 24) out;

//flat in lowp vec3 gColor[];
//flat in int gEnabledFaces[];
//flat out lowp vec3 fColor;

uniform float voxSize = 0.25;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void AddQuad(vec4 center, vec4 dy, vec4 dx) {
    gl_Position = center + (dx - dy);
    EmitVertex();
    gl_Position = center + (-dx - dy);
    EmitVertex();
    gl_Position = center + (dx + dy);
    EmitVertex();
    gl_Position = center + (-dx + dy);
    EmitVertex();
    EndPrimitive();
}
bool IsCulled(vec4 normal) {
    return normal.z > 0;
}

// TODO: Make own addQuad function
void main() {
    vec4 position = gl_in[0].gl_Position;
    vec4 center = vec4(position.xyz * voxSize * 2, 1);
    mat4 mvp = proj * view * model;
    
    // FRONT
    gl_Position = mvp * (center + vec4(-voxSize, -voxSize, voxSize, 0));
    EmitVertex();
    gl_Position =  mvp * (center + vec4(-voxSize, voxSize, voxSize, 0));
    EmitVertex();
    gl_Position = mvp * (center + vec4(voxSize, -voxSize, voxSize, 0));
    EmitVertex();
    gl_Position =  mvp * (center + vec4(voxSize, voxSize, voxSize, 0));
    EmitVertex();
    EndPrimitive();

    // BACK
    gl_Position = mvp * (center + vec4(-voxSize, -voxSize, -voxSize, 0));
    EmitVertex();
    gl_Position = mvp * (center + vec4(voxSize, -voxSize, -voxSize, 0));
    EmitVertex();
    gl_Position =  mvp * (center + vec4(-voxSize, voxSize, -voxSize, 0));
    EmitVertex();
    gl_Position =  mvp * (center + vec4(voxSize, voxSize, -voxSize, 0));
    EmitVertex();
    EndPrimitive();

    // LEFT
    gl_Position = mvp * (center + vec4(-voxSize, voxSize, voxSize, 0));
    EmitVertex();
    gl_Position =  mvp * (center + vec4(-voxSize, -voxSize, voxSize, 0));
    EmitVertex();
    gl_Position =  mvp * (center + vec4(-voxSize, voxSize, -voxSize, 0));
    EmitVertex();
    gl_Position = mvp * (center + vec4(-voxSize, -voxSize, -voxSize, 0));
    EmitVertex();
    EndPrimitive();

    // RIGHT
    gl_Position = mvp * (center + vec4(voxSize, voxSize, voxSize, 0));
    EmitVertex();
    gl_Position =  mvp * (center + vec4(voxSize, voxSize, -voxSize, 0));
    EmitVertex();
    gl_Position =  mvp * (center + vec4(voxSize, -voxSize, voxSize, 0));
    EmitVertex();
    gl_Position = mvp * (center + vec4(voxSize, -voxSize, -voxSize, 0));
    EmitVertex();
    EndPrimitive();

    // TOP
    gl_Position = mvp * (center + vec4(voxSize, voxSize, -voxSize, 0));
    EmitVertex();
    gl_Position =  mvp * (center + vec4(voxSize, voxSize, voxSize, 0));
    EmitVertex();
    gl_Position = mvp * (center + vec4(-voxSize, voxSize, -voxSize, 0));
    EmitVertex();
    gl_Position =  mvp * (center + vec4(-voxSize, voxSize, voxSize, 0));
    EmitVertex();
    EndPrimitive();

    // BACK
    gl_Position = mvp * (center + vec4(voxSize, -voxSize, -voxSize, 0));
    EmitVertex();
    gl_Position = mvp * (center + vec4(-voxSize, -voxSize, -voxSize, 0));
    EmitVertex();
    gl_Position =  mvp * (center + vec4(voxSize, -voxSize, voxSize, 0));
    EmitVertex();
    gl_Position =  mvp * (center + vec4(-voxSize, -voxSize, voxSize, 0));
    EmitVertex();
    EndPrimitive();
}

//#version 330 core
//layout (points) in;
//layout (triangle_strip, max_vertices = 5) out;
//
//uniform mat4 model;
//uniform mat4 view;
//uniform mat4 proj;
//
//void buildHouse(vec4 position, mat4 mvp) {
//    gl_Position = mvp * (position + vec4(-0.2, -0.2, 0.0, 0.0));    // 1:bottom-left
//    EmitVertex();   
//    gl_Position = mvp * (position + vec4(-0.2,  0.2, 0.0, 0.0));    // 3:top-left
//    EmitVertex();
//    gl_Position = mvp * (position + vec4( 0.2, -0.2, 0.0, 0.0));    // 2:bottom-right
//    EmitVertex();
//    gl_Position = mvp * (position + vec4( 0.0,  0.4, 0.0, 0.0));    // 5:top
//    EmitVertex();
//    gl_Position = mvp * (position + vec4( 0.2,  0.2, 0.0, 0.0));    // 4:top-right
//    EmitVertex();
//    EndPrimitive();
//}
//
//void main() { 
//    mat4 mvp = model;
//    vec4 position = gl_in[0].gl_Position;
//    buildHouse(position, mvp);
//}    