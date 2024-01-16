#version 330 core

out vec4 FragColor;
in vec2 UVS;
in vec3 modifiedPosition ;
in vec3 surface ;
in vec3 fragPos;
in vec3 normals;
in vec3 surface_normals;
in vec3 caustique ;
in vec4 clipSpace ;

uniform vec3 lightColor;
uniform vec3 lightPos; 
uniform vec3 cameraPos;
uniform float time;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;
uniform sampler2D distorsionTexture;

float pseudoRandom(vec3 pos, float time) {
    return fract(sin(dot(pos, vec3(12.9898, 78.233, 45.5432)) + time) * 43758.5453);
}

void main(){
    
    vec3 normale_water = normalize(normals);
    vec3 rayon = modifiedPosition - lightPos ;
    vec3 otherRayon ;
    vec3 newRay ;
    float angleIncidence ;
    float angleRefraction ;
    vec3 depart ;
    vec3 newPos = vec3(0,0,0);
    vec3 pt ;
    
    
    if (modifiedPosition.y == surface.y) {
        vec2 ndc = (clipSpace.xy/clipSpace.w)/2 + 0.5;
        vec2 refractTextCoord = vec2(UVS.x, -UVS.y);
        vec2 reflectTextCoord = vec2(-UVS.x, -UVS.y);

        vec3 reflectionColor = texture(reflectionTexture, reflectTextCoord).rgb;
        vec3 refractionColor = texture(refractionTexture, refractTextCoord).rgb;
        
        
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
    else if(modifiedPosition.y < surface.y){
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
        float transparency = 0.2 ;
        if(caustique.y <= -0.045  && caustique.x > -0.495 && caustique.z < 0.49 && modifiedPosition.y < -0.045){
            float tr = 0.05 + 0.35 * (fragPos.y + 0.495) ;
            if(caustique.y == -0.495){
                tr = 0.01;
            }
            if(tr > 0.2) tr = 0.2 ;
                FragColor = vec4((specularColor + diffuseColor + ambientColor), tr);
            }
            else FragColor = vec4(specularColor + diffuseColor + ambientColor, transparency);
        }
}

