#version 330 core

out vec4 FragColor;

in vec3 faceNormals;
in vec3 fragPos;
in vec4 projectedTexCoords;
in vec2 UVS;

uniform vec3 lightColor;
uniform vec3 lightPos; 
uniform vec3 cameraPos;
uniform sampler2D textureSurface;

void main(){
    vec2 textCoords = (projectedTexCoords.xy / projectedTexCoords.w) * 0.5 + 0.5;
    FragColor = texture(textureSurface, vec2(-UVS.x, UVS.y));
        

}