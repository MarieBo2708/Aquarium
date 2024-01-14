#version 330 core

out vec4 FragColor;

in vec3 faceNormals;
in vec3 fragPos;

uniform vec3 lightColor;
uniform vec3 lightPos; 
uniform vec3 cameraPos;

void main(){
       vec3 faceColor = vec3(0.2, 0.2, 0.2) ;
       vec3 normalesFace = normalize(faceNormals);
       vec3 lightDir = normalize(lightPos - fragPos);
       vec3 viewDir = normalize(cameraPos - fragPos);
       vec3 reflectDir = reflect(-lightDir, normalesFace);

       
       float ambientStrength = 0.5;
       vec3 ambient = ambientStrength * lightColor;
       vec3 ambientColor = ambient * faceColor;
       float diff = max(dot(normalesFace, lightDir), 0.0);
       vec3 diffuse = diff * lightColor;
       vec3 diffuseColor = diffuse * faceColor;
       float specularStrength = 0.5;
       float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.);
       vec3 specular = specularStrength * spec * lightColor; 
       vec3 specularColor = specular * faceColor;

       FragColor = vec4(specularColor + diffuseColor + ambientColor, 0.2); 
        

}
