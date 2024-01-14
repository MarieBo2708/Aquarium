#version 330 core

out vec4 FragColor;

in vec3 normals;
in vec3 fragPos;

uniform vec3 lightColor;
uniform vec3 lightPos; 
uniform vec3 cameraPos;

void main(){
        
       vec3 objectColor = vec3(1., 0., 0.);
       float ambientStrength = 0.2;
       vec3 ambient = ambientStrength * lightColor;
       vec3 ambientColor = ambient * objectColor;

       vec3 normale_sphere = normalize(normals);
       vec3 lightDir = normalize(lightPos - fragPos);
       float diff = max(dot(normale_sphere, lightDir), 0.0);
       vec3 diffuse = diff * lightColor;
       vec3 diffuseColor = diffuse * objectColor;

       float specularStrength = 0.5;
       vec3 viewDir = normalize(cameraPos - fragPos);
       vec3 reflectDir = reflect(-lightDir, normale_sphere);
       float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
       vec3 specular = specularStrength * spec * lightColor; 
       vec3 specularColor = specular * objectColor;

       FragColor = vec4(specularColor + diffuseColor + ambientColor, 1.);
        

}

