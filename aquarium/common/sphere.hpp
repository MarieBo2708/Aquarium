#ifndef SPHERE_H
#define SPHERE_H

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

class Sphere{
    public:

        std::vector<glm::vec3> vertices;
        std::vector<unsigned short> indices;
        std::vector<std::vector<unsigned short>> triangles;
        std::vector<glm::vec3> vertices_normals;
        std::vector<glm::vec3> triangles_normals;
        glm::vec3 centre_sphere;
        float radius_sphere;
        GLuint vertexbuffersphere;
        GLuint elementbuffersphere;
        GLuint normalbuffersphere;


        Sphere();

        Sphere(float radius, int azimut, int elevation, glm::vec3 centre, GLuint programID);

        void createSphereData(int numSegments, int numStacks);

        void CreateVerticesNormals();

        void generateSphereBuffers(std::vector<glm::vec3> vertices, std::vector<unsigned short> indices, std::vector<glm::vec3> normals);

        void drawSphere(GLuint programID, bool clip, glm::vec4 clipPlane);

        void sendValueToShader(GLuint programID);

        void disableArrays();

        void deleteBuffers();

        void sendMatrixToShader(GLuint programID, glm::mat4 Model, glm::mat4 View, glm::mat4 Projection);

        void sendInfosToShader(GLuint programID, glm::vec3 camPos, glm::vec3 lightPos, glm::vec3 lightColor);




};

#endif