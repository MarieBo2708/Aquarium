#version 330 core

layout (location = 0) in vec3 sphereVertices;
layout (location = 1) in vec3 sphereNormals;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

out vec3 normals;
out vec3 fragPos;

uniform vec4 clippingPlane;


void main(){
    fragPos = vec3(ModelMatrix * vec4(sphereVertices, 1.0));
    vec4 worldPos = ModelMatrix * ProjectionMatrix * ViewMatrix * vec4(sphereVertices, 1.);
    gl_ClipDistance[0] = dot(clippingPlane, worldPos);
    normals = sphereNormals;
    gl_Position = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(sphereVertices, 1.0);

}


