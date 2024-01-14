#version 330 core

out vec4 FragColor;

in vec3 faceNormals;
in vec3 fragPos;
in vec3 skyboxTextCoord;

uniform samplerCube skyboxTexture;

void main(){
    
    FragColor = texture(skyboxTexture, skyboxTextCoord);
        

}