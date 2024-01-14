#ifndef BOIDS_H
#define BOIDS_H

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
#include <common/sphere.hpp>

class Boids{
    public:
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<unsigned short> indices;
        std::vector<glm::vec2> uvs;
        GLuint texture, uvbuffer, elementbuffer, vertexbuffer, normalsbuffer;
        glm::mat4 ModelBoids;
        float speed;
        glm::vec3 orientation;

        Boids();

        Boids(const char* path_obj, const char* path_texture);

        void generateBuffers(std::vector<glm::vec3> vertices, std::vector<unsigned short> indices, std::vector<glm::vec3> normals, std::vector<glm::vec2> uvs);
        void draw(GLuint programID, std::string textID, int textNb, int loc1, int loc2, int loc3);
        void disableArrays();
        void deleteBuffers();
        void sendMatrixToShader(GLuint programID, glm::mat4 Model, glm::mat4 View, glm::mat4 Projection);

        void deplacementChef(Sphere s, bool &atteintObjectif, glm::vec3 &positionDepart, glm::vec3 &positionObjectif, float &distance, float &deltaTime, float &angleAutourY, float &angle, glm::vec3 &deplacement, int &cptTab, std::vector<float> &derniersAngles, bool stab, int &valeur);

        void deplacementAutre(Boids other, glm::vec3 &deplacementOther, glm::vec3 &deplacementChef, glm::vec3 positionChef, float &angleChefTotal, float &angleChefEnCours, glm::vec3 &deplacementBoid, float &angleBoidTotal, float &angleBoidEnCours);

        void peur(Sphere s, glm::vec3 &deplacement, float &angle, float &angleAutourY, Boids b1, Boids b2);
};

#endif