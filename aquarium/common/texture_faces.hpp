#ifndef TEXT_H
#define TEXT_H

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <iostream>
#include <math.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <glm/glm.hpp>

class textureFaces{
    public:
    std::vector<glm::vec3> vertices;
    std::vector<unsigned short> indices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;
    GLuint vertexbuffertexture;
    GLuint elementbuffertexture;
    GLuint normalsbuffertexture;
    GLuint uvbuffertexture;

    textureFaces();
    void generateVertices();
    void generateIndices();
    void generateNormals();
    void generateUvs();
    void generateBuffers(std::vector<glm::vec3> vertices, std::vector<unsigned short> indices, std::vector<glm::vec3> normals, std::vector<glm::vec2> uvs);
    void drawFaces(GLuint programID, GLuint texture, bool clip, glm::vec4 clipPlane);
    void disableArrays();
    void deleteBuffers();
    void sendMatrixToShader(GLuint programID, glm::mat4 Model, glm::mat4 View, glm::mat4 Projection);
    void sendInfosToShader(GLuint programID, glm::vec3 camPos, glm::vec3 lightPos, glm::vec3 lightColor);

};

#endif