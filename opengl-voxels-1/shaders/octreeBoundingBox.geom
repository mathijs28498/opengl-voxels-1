#version 330 core
layout(points) in;
layout(line_strip, max_vertices = 16) out;

in vec3 gColor[];
in float gSize[];

out vec3 fColor;

uniform float voxSize;

uniform mat4 view;
uniform mat4 proj;
uniform vec3 color;

void addQuad(mat4 mvp, vec4 corner, vec4 v1, vec4 v2, vec4 v3, vec4 v4) {
    gl_Position = mvp * (corner + v1);
    EmitVertex();
    gl_Position = mvp * (corner + v2);
    EmitVertex();
    gl_Position = mvp * (corner + v3);
    EmitVertex();
    gl_Position = mvp * (corner + v4);
    EmitVertex();
    gl_Position = mvp * (corner + v1);
    EmitVertex();
    EndPrimitive();
}

void main() {
    vec4 position = gl_in[0].gl_Position;
    float realSize = voxSize * gSize[0];
    vec4 corner = vec4(position.xyz * voxSize, 1);
    mat4 mvp = proj * view;
    fColor = gColor[0];

    vec4 luf = vec4(0, realSize, realSize, 0);
    vec4 ruf = vec4(realSize, realSize, realSize, 0);
    vec4 ldf = vec4(0, 0, realSize, 0);
    vec4 rdf = vec4(realSize, 0, realSize, 0);
    vec4 lub = vec4(0, realSize, 0, 0);
    vec4 rub = vec4(realSize, realSize, 0, 0);
    vec4 ldb = vec4(0, 0, 0, 0);
    vec4 rdb = vec4(realSize, 0, 0, 0);

    gl_Position = mvp * (corner + luf);
    EmitVertex();
    gl_Position = mvp * (corner + ruf);
    EmitVertex();
    gl_Position = mvp * (corner + rub);
    EmitVertex();
    gl_Position = mvp * (corner + lub);
    EmitVertex();
    gl_Position = mvp * (corner + luf);
    EmitVertex();
    gl_Position = mvp * (corner + ldf);
    EmitVertex();
    gl_Position = mvp * (corner + rdf);
    EmitVertex();
    gl_Position = mvp * (corner + rdb);
    EmitVertex();
    gl_Position = mvp * (corner + ldb);
    EmitVertex();
    gl_Position = mvp * (corner + ldf);
    EmitVertex();
    EndPrimitive();

    
    gl_Position = mvp * (corner + ruf);
    EmitVertex();
    gl_Position = mvp * (corner + rdf);
    EmitVertex();
    EndPrimitive();
    
    gl_Position = mvp * (corner + rub);
    EmitVertex();
    gl_Position = mvp * (corner + rdb);
    EmitVertex();
    EndPrimitive();
    
    gl_Position = mvp * (corner + lub);
    EmitVertex();
    gl_Position = mvp * (corner + ldb);
    EmitVertex();
    EndPrimitive();
}