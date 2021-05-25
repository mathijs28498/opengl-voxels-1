#version 330 core
layout(points) in;
layout(line_strip, max_vertices = 16) out;

in vec3 gColor[];

out vec3 fColor;

uniform float voxSize;
uniform float size;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform vec3 color;

void addQuad(mat4 mvp, vec4 center, vec4 v1, vec4 v2, vec4 v3, vec4 v4) {
    gl_Position = mvp * (center + v1);
    EmitVertex();
    gl_Position = mvp * (center + v2);
    EmitVertex();
    gl_Position = mvp * (center + v3);
    EmitVertex();
    gl_Position = mvp * (center + v4);
    EmitVertex();
    gl_Position = mvp * (center + v1);
    EmitVertex();
    EndPrimitive();
}

bool IsCulled(vec4 normal) {
    return normal.z > 0;
}

// TODO: Make own addQuad function
// TODO: Only draw visible faces with normal towards you (max_vertices could be 12)
void main() {
    vec4 position = gl_in[0].gl_Position;
    float realSize = voxSize / 2 * size;
    vec4 center = vec4(position.xyz * realSize * 2, 1);
    mat4 mvp = proj * view * model;
    fColor = color;
    
    vec4 luf = vec4(-realSize, realSize, realSize, 0);
    vec4 ruf = vec4(realSize, realSize, realSize, 0);
    vec4 ldf = vec4(-realSize, -realSize, realSize, 0);
    vec4 rdf = vec4(realSize, -realSize, realSize, 0);
    vec4 lub = vec4(-realSize, realSize, -realSize, 0);
    vec4 rub = vec4(realSize, realSize, -realSize, 0);
    vec4 ldb = vec4(-realSize, -realSize, -realSize, 0);
    vec4 rdb = vec4(realSize, -realSize, -realSize, 0);

    gl_Position = mvp * (center + luf);
    EmitVertex();
    gl_Position = mvp * (center + ruf);
    EmitVertex();
    gl_Position = mvp * (center + rub);
    EmitVertex();
    gl_Position = mvp * (center + lub);
    EmitVertex();
    gl_Position = mvp * (center + luf);
    EmitVertex();
    gl_Position = mvp * (center + ldf);
    EmitVertex();
    gl_Position = mvp * (center + rdf);
    EmitVertex();
    gl_Position = mvp * (center + rdb);
    EmitVertex();
    gl_Position = mvp * (center + ldb);
    EmitVertex();
    gl_Position = mvp * (center + ldf);
    EmitVertex();
    EndPrimitive();

    
    gl_Position = mvp * (center + ruf);
    EmitVertex();
    gl_Position = mvp * (center + rdf);
    EmitVertex();
    EndPrimitive();
    
    gl_Position = mvp * (center + rub);
    EmitVertex();
    gl_Position = mvp * (center + rdb);
    EmitVertex();
    EndPrimitive();
    
    gl_Position = mvp * (center + lub);
    EmitVertex();
    gl_Position = mvp * (center + ldb);
    EmitVertex();
    EndPrimitive();
}