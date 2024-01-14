// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture.hpp"
#include "skybox.hpp"

using namespace std;
using namespace glm;

Skybox::Skybox() {}

Skybox::Skybox(vector<string> faces, float size)
{
    skyboxTexture = loadSkybox(faces);
    generateVertices(size);
    generateIndices();
}

void Skybox::generateVertices(float size){

    vertices.push_back(vec3(-size, size, size));
    vertices.push_back(vec3(size, size, size));
    vertices.push_back(vec3(-size, -size, size));
    vertices.push_back(vec3(size, -size, size));
    vertices.push_back(vec3(size, size, -size));
    vertices.push_back(vec3(size, -size, -size));
    vertices.push_back(vec3(-size, size, -size));
    vertices.push_back(vec3(-size, -size, -size));
}

void Skybox::generateIndices(){

    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(2);
    indices.push_back(1);
    indices.push_back(3);
    indices.push_back(2);

    indices.push_back(1);
    indices.push_back(4);
    indices.push_back(3);
    indices.push_back(4);
    indices.push_back(5);
    indices.push_back(3);

    indices.push_back(4);
    indices.push_back(6);
    indices.push_back(5);
    indices.push_back(6);
    indices.push_back(7);
    indices.push_back(5);

    indices.push_back(6);
    indices.push_back(0);
    indices.push_back(7);
    indices.push_back(0);
    indices.push_back(2);
    indices.push_back(7);

    indices.push_back(2);
    indices.push_back(3);
    indices.push_back(7);
    indices.push_back(3);
    indices.push_back(5);
    indices.push_back(7);

    indices.push_back(0);
    indices.push_back(1);
    indices.push_back(6);
    indices.push_back(1);
    indices.push_back(4);
    indices.push_back(6);

}

void Skybox::generateBuffers(std::vector<glm::vec3> vertices, std::vector<unsigned short> indices){
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &elementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

}

void Skybox::disableArrays(){
    glDisableVertexAttribArray(0);
}

void Skybox::deleteBuffers(){
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &elementbuffer);
}

void Skybox::draw(GLuint programID){
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
        0,        
        3,        
        GL_FLOAT, 
        GL_FALSE, 
        0,        
        (void *)0 
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

    glDrawElements(
            GL_TRIANGLES,        
            indices.size(), 
            GL_UNSIGNED_SHORT,   
            (void *)0            
        );

    
    glActiveTexture(GL_TEXTURE0);

    glUniform1i(glGetUniformLocation(programID, "skyboxTexture"), 0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
}

void Skybox::sendMatrixToShader(GLuint programID, glm::mat4 Model, glm::mat4 View, glm::mat4 Projection){
    glUniformMatrix4fv(glGetUniformLocation(programID, "ModelMatrix"), 1, GL_FALSE, &Model[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(programID, "ViewMatrix"), 1, GL_FALSE, &View[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(programID, "ProjectionMatrix"), 1, GL_FALSE, &Projection[0][0]);
}