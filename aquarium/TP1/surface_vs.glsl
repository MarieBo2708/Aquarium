#version 330 core

layout (location = 0) in vec3 vertices;
layout (location = 1) in vec3 normals;
layout (location = 2) in vec2 uvs;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

out vec3 faceNormals;
out vec3 fragPos;
out vec4 projectedTexCoords;
out vec2 UVS;

void main(){
    fragPos = vec3(ModelMatrix * vec4(vertices, 1.0));
    UVS = uvs;
    faceNormals = normals;
    //projectedTexCoords = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(vertices, 1.0);
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(vertices, 1.0);

}