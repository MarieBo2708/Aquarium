#version 330 core

layout (location = 0) in vec3 boid1_vertices;
layout (location = 2) in vec3 boid1_normals;
layout (location = 1) in vec2 boid1_uvs;

layout (location = 3) in vec3 boid2_vertices;
layout (location = 5) in vec3 boid2_normals;
layout (location = 4) in vec2 boid2_uvs;

layout (location = 6) in vec3 boid3_vertices;
layout (location = 8) in vec3 boid3_normals;
layout (location = 7) in vec2 boid3_uvs;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

out vec3 faceNormals;
out vec3 fragPos;
out vec2 UVS;

uniform int mode;

void main(){
    if(mode == 1){
        fragPos = vec3(ModelMatrix * vec4(boid1_vertices, 1.0));
        UVS = boid1_uvs;
        faceNormals = boid1_normals;
        gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(boid1_vertices, 1.0);
    }
    else if(mode == 2){
        fragPos = vec3(ModelMatrix * vec4(boid2_vertices, 1.0));
        UVS = boid2_uvs;
        faceNormals = boid2_normals;
        gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(boid2_vertices, 1.0);
    }
    else if(mode==3){
        fragPos = vec3(ModelMatrix * vec4(boid3_vertices, 1.0));
        UVS = boid3_uvs;
        faceNormals = boid3_normals;
        gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(boid3_vertices, 1.0);
    }
    

}
