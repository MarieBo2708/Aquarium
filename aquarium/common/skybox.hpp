#ifndef SKYBOX_H
#define SKYBOX_H

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <math.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <glm/glm.hpp>

class Skybox
{
public:
    
    std::vector<glm::vec3> vertices;
    std::vector<unsigned short> indices;
    GLuint skyboxTexture, vertexbuffer, elementbuffer;

    Skybox();

    Skybox(std::vector<std::string> faces, float size);

    void generateVertices(float size);

    void generateIndices();

    void generateBuffers(std::vector<glm::vec3> vertices, std::vector<unsigned short> indices);

    void disableArrays();

    void deleteBuffers();

    void draw(GLuint programID);

    void sendMatrixToShader(GLuint programID, glm::mat4 Model, glm::mat4 View, glm::mat4 Projection);


};

#endif