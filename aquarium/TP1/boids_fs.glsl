#version 330 core

out vec4 FragColor;

in vec3 faceNormals;
in vec3 fragPos;
in vec2 UVS;

uniform sampler2D texture_boid_1;
uniform sampler2D texture_boid_2;
uniform sampler2D texture_boid_3;
uniform int mode;

void main(){
    if(mode==1){
        vec3 col = texture(texture_boid_1, UVS).rgb;
        FragColor = vec4(col, 1.);
    }
    else if(mode==2){
        vec3 col = texture(texture_boid_2, UVS).rgb;
        FragColor = vec4(col, 1.);
    }
    else if(mode==3){
        vec3 col = texture(texture_boid_3, UVS).rgb;
        FragColor = vec4(col, 1.); 
    }
    
    
    
        

}