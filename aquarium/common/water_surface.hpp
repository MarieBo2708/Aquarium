#ifndef WATER_SURFACE_H
#define WATER_SURFACE_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <GL/glew.h>
#include "Water.hpp"


class WaterSurface{
    public:
    std::vector<glm::vec3> vertices;
    std::vector<unsigned short> indices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;
    GLuint vertexbuffer, elementbuffer, normalsbuffer, uvbuffer;
    Water w ;

    WaterSurface();
    void generateVertices();
    void generateIndices();
    void generateNormals();
    void generateUvs();
    void generateBuffers(std::vector<glm::vec3> vertices, std::vector<unsigned short> indices, std::vector<glm::vec3> normals, std::vector<glm::vec2> uvs);
    void draw(GLuint programID, GLuint frameBufferTexture);
    void disableArrays();
    void deleteBuffers();
    void sendMatrixToShader(GLuint programID, glm::mat4 Model, glm::mat4 View, glm::mat4 Projection);
    void sendInfosToShader(GLuint programID, glm::vec3 camPos, glm::vec3 lightPos, glm::vec3 lightColor);


};

#endif