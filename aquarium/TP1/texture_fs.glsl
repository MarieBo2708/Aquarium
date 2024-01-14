#version 330 core

out vec4 FragColor;

in vec2 UVS;
in vec3 fragPos;
in vec3 faceNormals;

uniform sampler2D textureFace;
uniform vec3 lightColor;
uniform vec3 lightPos; 
uniform vec3 cameraPos;

void main(){
        
       vec3 faceColor = texture(textureFace, UVS).rgb;
       vec3 normalesFace = normalize(faceNormals);
       vec3 lightDir = normalize(lightPos - fragPos);
       vec3 viewDir = normalize(cameraPos - fragPos);
       vec3 reflectDir = reflect(-lightDir, normalesFace);

       float ambientStrength = 1.;
       vec3 ambient = ambientStrength * lightColor;
       vec3 ambientColor = ambient * faceColor;
       float diff = max(dot(normalesFace, lightDir), 0.0);
       vec3 diffuse = diff * lightColor;
       vec3 diffuseColor = diffuse * faceColor;
       float specularStrength = 1.;
       float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128.);
       vec3 specular = specularStrength * spec * lightColor; 
       vec3 specularColor = specular * faceColor;
       FragColor = vec4(specularColor + ambientColor + diffuseColor, 1.);
}
