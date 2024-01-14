#version 330 core

layout (location = 0) in vec3 waterVertices;
layout (location = 1) in vec3 waterNormals;
layout (location = 2) in vec3 surfaceNormals;
layout (location = 3) in vec3 surfaceVertices;


out vec3 fragPos;
out vec2 UVS;
out vec3 modifiedPosition ;
out vec3 surface ;
out vec4 projectedTexCoords ;
out vec3 normals;
out vec3 surface_normals;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

uniform float time;
uniform float amplitude;
uniform float propagation;
uniform vec3 intersection;

uniform float amplitude1;
uniform float propagation1;
uniform vec3 intersection1;

uniform float amplitude2;
uniform float propagation2;
uniform vec3 intersection2;

uniform float amplitude3;
uniform float propagation3;
uniform vec3 intersection3;

uniform float rayonSphere;
uniform vec3 centreSphere;
uniform vec4 clippingPlane;

uniform vec3 firstIntersection;
uniform vec3 firstIntersection1;
uniform vec3 firstIntersection2;
uniform vec3 firstIntersection3;

uniform int direction ;

int random(vec2 st)
{
    return int(float(fract(sin(dot(st.xy, vec2(12.9898,78.233))) * 43758.5453123)) * 5);
}

float onde(vec3 pt, vec3 inter, vec3 firstInter, float propag, float amp){
    float waveHeight = 0 ;
    if(amp != 0){
        if(direction == 1){
                float distance = length(inter - firstInter);
                float distanceBis = length(pt - inter);
                if(pt.x >= firstInter.x  && pt.x <= inter.x  + 100 && abs(pt.x -0.1 - firstInter.x) <= distance - abs(pt.z - firstInter.z)  + 0.1 && distanceBis < 0.5 && abs(pt.z - inter.z)<0.3){
                    float frequency = 6.0 ;
                    float modulatedFrequency = frequency * (1.0 + 0.5 * sin(distance * time));
                    waveHeight = amp * ((sin(abs((length(pt - firstInter))) * propag * frequency - 3.1415926 / 2) + 1)) * 0.5;
                    if(distanceBis > distance - 0.2) waveHeight = 0.0 ;
                }
        }
        if(direction == 2){
                float distance = length(inter - firstInter);
                float distanceBis = length(pt - inter);
                if(pt.x <= firstInter.x && pt.x >= inter.x - 100 && abs( - pt.x + 0.1 + firstInter.x) <= distance - abs(pt.z - firstInter.z) + 0.1 && distanceBis < 0.5 && abs(pt.z - inter.z)<0.3){
                    float frequency = 6.0 ;
                    float modulatedFrequency = frequency * (1.0 + 0.5 * sin(distance * time));
                    waveHeight = amp * ((sin(abs((length(pt - firstInter))) * propag * frequency - 3.1415926 / 2) + 1)) * 0.5;
                    if(distanceBis > distance - 0.2) waveHeight = 0.0 ;
                }
        }
        if(direction == 3){
                float distance = length(inter - firstInter);
                float distanceBis = length(pt - inter);
                if(pt.z <= firstInter.z && pt.z >= inter.z - 100 && abs( - pt.z + 0.1 + firstInter.z) <= distance - abs(pt.x - firstInter.x) + 0.1 && distanceBis < 0.5 && abs(pt.x - inter.x)<0.3){
                    float frequency = 6.0 ;
                    float modulatedFrequency = frequency * (1.0 + 0.5 * sin(distance * time));
                    waveHeight = amp * ((sin(abs((length(pt - firstInter))) * propag * frequency - 3.1415926 / 2) + 1)) * 0.5;
                    if(distanceBis > distance - 0.2) waveHeight = 0.0 ;
                }
        }
        if(direction == 4){
                float distance = length(inter - firstInter);
                float distanceBis = length(pt - inter);
                if(pt.z >= firstInter.z && pt.z <= inter.z  + 100 && abs(pt.z -0.1 - firstInter.z) <= distance - abs(pt.x - firstInter.x)  + 0.1 && distanceBis < 0.5 && abs(pt.x - inter.x)<0.3){
                    float frequency = 6.0 ;
                    float modulatedFrequency = frequency * (1.0 + 0.5 * sin(distance * time));
                    waveHeight = amp * ((sin(abs((length(pt - firstInter))) * propag * frequency - 3.1415926 / 2) + 1)) * 0.5;
                    if(distanceBis > distance - 0.2) waveHeight = 0.0 ;
                }
        }
            float distance = length(firstInter - pt);
            if(distance < propag){
                float frequency = 10.0 ;
                waveHeight += amp * 5 * sin((distance*3.1415926*frequency)/propag)/(3.1415926 + distance*10);
                return waveHeight;
            }
       }
        
        
    return 0.0;
}


void main(){
    surface_normals = surfaceNormals;
    normals = mat3(transpose(inverse(ModelMatrix)))*waterNormals;
    float waveFrequency = 3 ;
    float waveAmplitude = 0.1 ;
    modifiedPosition = waterVertices;
    //surface = surfaceVertices ;
    vec4 worldPos = ModelMatrix * ProjectionMatrix * ViewMatrix * vec4(modifiedPosition, 1.);
    gl_ClipDistance[0] = dot(clippingPlane, worldPos);
    vec3 surf = surfaceVertices ;
    if (modifiedPosition.y > -0.4) {
            modifiedPosition.y += waveAmplitude * sin(waveFrequency * time + modifiedPosition.x + modifiedPosition.z)/20;
    }
    if(modifiedPosition.y > centreSphere.y - rayonSphere && modifiedPosition.y < centreSphere.y + rayonSphere){
            float a = sqrt(rayonSphere * rayonSphere - (centreSphere.y - modifiedPosition.y) * (centreSphere.y - modifiedPosition.y));
            vec2 centre = vec2(centreSphere.x, centreSphere.z);
            vec2 diff = vec2(modifiedPosition.x, modifiedPosition.z);
    
    }
    if (modifiedPosition.y > -0.4){
        
        float hauteur = (onde(modifiedPosition, intersection, firstIntersection, propagation, amplitude) + onde(modifiedPosition, intersection1, firstIntersection1, propagation1, amplitude1) + onde(modifiedPosition, intersection2, firstIntersection2, propagation2, amplitude2) + onde(modifiedPosition, intersection3, firstIntersection3, propagation3, amplitude3))/4; 
        modifiedPosition.y += 2.5*hauteur ;
        surface = modifiedPosition;
        UVS = vec2(surface.x + 0.495, surface.z + 0.495);
    }
    //projectedTexCoords = ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(surface, 1.0);
    if (modifiedPosition.y <= 0.8){
          fragPos = vec3(ModelMatrix * vec4(modifiedPosition, 1.0));
    }
    else fragPos = vec3(ModelMatrix * vec4(surf, 1.0));
    // }
    // if(modifiedPosition.y > -0.4){
         //fragPos = vec3(ModelMatrix * vec4(surface, 1.0));
    //}
    gl_Position = ProjectionMatrix * ViewMatrix * vec4(fragPos, 1.0);

}