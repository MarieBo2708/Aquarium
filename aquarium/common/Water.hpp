#ifndef WATER_H
#define WATER_H

#include <vector>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "sphere.hpp"

class Water{
public:

    glm::vec3 center;
    float height;
    float width;
    float depth;
    float amplitude, propagation ;
    float time, amplitude1, propagation1, amplitude2, propagation2, amplitude3, propagation3, amplitude4, propagation4 ;
    glm::vec3 intersection;
    long int resolution;
    std::vector<glm::vec3> vertices;
    std::vector<unsigned short> indices;
    std::vector<glm::vec3> facesNormals;
    std::vector<glm::vec3> normals;
    
    GLuint vertexbuffer;
    GLuint elementbuffer;
    GLuint normalsbuffer;

    std::vector<glm::vec3> surfaceVertices ;
    std::vector<unsigned short> surfaceIndices;
    std::vector<glm::vec3> surfaceNormals ;
    std::vector<glm::vec2> surfaceUVs ;

    GLuint surfacevertexbuffer;
    GLuint surfaceelementbuffer;
    GLuint surfacenormalsbuffer;
    GLuint surfaceuvbuffer;

    int nbIntersections ;
    std::vector<glm::vec3> intersections ;
    std::vector<glm::vec2> donneesOndes ;
    int direction ;
    std::vector<glm::vec3> firstIntersections ;


    Water();
    void resize(float h, float w, float d);
    void createAquarium();
    void generateNormals();
    void generateSurfaceNormals();
    bool intersectBetweenWaterAndSphere(Sphere sphere);
    unsigned short getShorterDistance(Sphere sphere);
    void generateWaterBuffers(std::vector<glm::vec3> vertices, std::vector<unsigned short> indices, std::vector<glm::vec3> normals);
    void generateWaterBuffersSurface(std::vector<glm::vec3> vertices, std::vector<unsigned short> indices, std::vector<glm::vec3> normals, std::vector<glm::vec2> uvs);
    void draw(bool texture, Sphere s, glm::vec3 &departMouvement, int &valDeplacement, bool &sphereWasIntersectingWater, GLuint programID, float currentFrame, bool clip, glm::vec4 clipPlane, GLuint reflectionTexture, GLuint refractionTexture, glm::vec3 camPos, glm::vec3 lightPos, glm::vec3 lightColor);
    void sendValueToShader(GLuint programID, float currentFrame, glm::vec3 camPos, glm::vec3 lightPos, glm::vec3 lightColor);
    void simulateOnde(Sphere s, bool &sphereWasInterscetingWater, glm::vec3 &departMouvement, int &valDeplacement);
    void disableArrays();
    void deleteBuffers();
    void sendMatrixToShader(GLuint programID, glm::mat4 Model, glm::mat4 View, glm::mat4 Projection);
    void createSurface();
    void drawSurface();




    
            
};

#endif