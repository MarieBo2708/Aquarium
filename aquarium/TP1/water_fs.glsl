#version 330 core

out vec4 FragColor;
in vec2 UVS;
in vec3 modifiedPosition ;
in vec3 surface ;
in vec3 fragPos;
in vec3 normals;
in vec3 surface_normals;

uniform vec3 lightColor;
uniform vec3 lightPos; 
uniform vec3 cameraPos;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;

void main(){
    
    vec3 normale_water = normalize(normals);
    
    if(modifiedPosition.y < surface.y){
        vec3 objectColor = vec3(0., 0., 1.);
        float ambientStrength = 0.2;
        vec3 ambient = ambientStrength * lightColor;
        vec3 ambientColor = ambient * objectColor;

        vec3 lightDir = normalize(lightPos - fragPos);
        float diff = max(dot(normale_water, lightDir), 0.0);
        vec3 diffuse = diff * lightColor;
        vec3 diffuseColor = diffuse * objectColor;

        float specularStrength = 0.5;
        vec3 viewDir = normalize(cameraPos - fragPos);
        vec3 reflectDir = reflect(-lightDir, normale_water);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        vec3 specular = specularStrength * spec * lightColor; 
        vec3 specularColor = specular * objectColor;

        FragColor = vec4(specularColor + diffuseColor + ambientColor, 0.2);

    }
    else{

        vec3 reflectionColor = texture(reflectionTexture, vec2(-UVS.x, -UVS.y)).rgb;
        vec3 refractionColor = texture(refractionTexture, vec2(UVS.x, -UVS.y)).rgb;
        
        vec3 objectColor = mix(reflectionColor, refractionColor, 0.5).rgb;
        float ambientStrength = 0.2;
        vec3 ambient = ambientStrength * lightColor;
        vec3 ambientColor = ambient * objectColor;

        vec3 lightDir = normalize(lightPos - fragPos);
        float diff = max(dot(normale_water, lightDir), 0.0);
        vec3 diffuse = diff * lightColor;
        vec3 diffuseColor = diffuse * objectColor;

        float specularStrength = 0.5;
        vec3 viewDir = normalize(cameraPos - fragPos);
        vec3 reflectDir = reflect(-lightDir, normale_water);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        vec3 specular = specularStrength * spec * lightColor; 
        vec3 specularColor = specular * objectColor;

        FragColor = vec4(specularColor + diffuseColor + ambientColor, 1.);

    }

    //FragColor = vec4(0., 0., 1., 0.2);

}

