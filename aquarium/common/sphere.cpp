#include "sphere.hpp"
#include "texture.hpp"
#include "shader.hpp"
#include <glm/gtc/matrix_transform.hpp>

Sphere::Sphere(){}

Sphere::Sphere(float radius, int azimut, int elevation, glm::vec3 centre, GLuint waterProgramID){
    centre_sphere = centre;
    radius_sphere = radius;
    createSphereData(azimut, elevation);
    sendValueToShader(waterProgramID);
    CreateVerticesNormals();


}

void Sphere::createSphereData(int numSegments, int numStacks) {

    vertices.clear();
    indices.clear();

    for (int i = 0; i <= numStacks; ++i) {
        float stackAngle = M_PI / 2.0f - static_cast<float>(i) / numStacks * M_PI;
        float xy = radius_sphere * std::cos(stackAngle);
        float z = radius_sphere * std::sin(stackAngle);

        for (int j = 0; j <= numSegments; ++j) {
            float sectorAngle = 2.0f * M_PI * static_cast<float>(j) / numSegments;

            float x = xy * std::cos(sectorAngle);
            float y = xy * std::sin(sectorAngle);

            glm::vec3 vertex = glm::vec3(x, y, z) + centre_sphere;

            vertices.push_back(vertex);
        }
    }

    for (int i = 0; i < numStacks; ++i) {
        for (int j = 0; j < numSegments; ++j) {
            int first = i * (numSegments + 1) + j;
            int second = first + numSegments + 1;

            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(first + 1);

            indices.push_back(second);
            indices.push_back(second + 1);
            indices.push_back(first + 1);
        }
    }
}

void Sphere::CreateVerticesNormals(){
    vertices_normals.clear();
    vertices_normals.resize(vertices.size());

     for (size_t i = 0; i < indices.size(); i += 3) {
        glm::vec3 v0 = vertices[indices[i]];
        glm::vec3 v1 = vertices[indices[i + 1]];
        glm::vec3 v2 = vertices[indices[i + 2]];

        // Compute the normal using the cross product of the edges
        glm::vec3 faceNormal = glm::cross(v1 - v0, v2 - v0);

        // Accumulate the normal for each vertex of the face
        vertices_normals[indices[i]] += faceNormal;
        vertices_normals[indices[i + 1]] += faceNormal;
        vertices_normals[indices[i + 2]] += faceNormal;
    }

    // Normalize the normals
    for (size_t i = 0; i < vertices_normals.size(); ++i) {
        vertices_normals[i] = glm::normalize(vertices_normals[i]);
    }
}

void Sphere::generateSphereBuffers(std::vector<glm::vec3> vertices, std::vector<unsigned short> indices, std::vector<glm::vec3> normals){
    glGenBuffers(1, &vertexbuffersphere);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffersphere);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &elementbuffersphere);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffersphere);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &normalbuffersphere);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffersphere);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

}

void Sphere::drawSphere(GLuint programID, bool clip, glm::vec4 clipPlane){
    if(clip){
        glEnable(GL_CLIP_DISTANCE0);
        glUniform4f(glGetUniformLocation(programID, "clippingPlane"), clipPlane.x, clipPlane.y, clipPlane.z, clipPlane.w);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffersphere);
        glVertexAttribPointer(
            0,        
            3,        
            GL_FLOAT, 
            GL_FALSE, 
            0,        
            (void *)0 
        );

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffersphere);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, normalbuffersphere);
        glVertexAttribPointer(
            1,        
            3,        
            GL_FLOAT, 
            GL_FALSE, 
            0,        
            (void *)0 
        );

        glDrawElements(
            GL_TRIANGLES,        
            indices.size(), 
            GL_UNSIGNED_SHORT,   
            (void *)0            
        );
    }
    else{
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffersphere);
        glVertexAttribPointer(
            0,        
            3,        
            GL_FLOAT, 
            GL_FALSE, 
            0,        
            (void *)0 
        );

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffersphere);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, normalbuffersphere);
        glVertexAttribPointer(
            1,        
            3,        
            GL_FLOAT, 
            GL_FALSE, 
            0,        
            (void *)0 
        );

        glDrawElements(
            GL_TRIANGLES,        
            indices.size(), 
            GL_UNSIGNED_SHORT,   
            (void *)0            
        );
    }
}

void Sphere::sendValueToShader(GLuint waterProgramID){
    glUniform1f(glGetUniformLocation(waterProgramID, "rayonSphere"), radius_sphere);
    glUniform3fv(glGetUniformLocation(waterProgramID, "centreSphere"), 1, &centre_sphere[0]);
}

void Sphere::disableArrays(){
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void Sphere::deleteBuffers(){
    glDeleteBuffers(1, &vertexbuffersphere);
    glDeleteBuffers(1, &elementbuffersphere);
    glDeleteBuffers(1, &normalbuffersphere);
}

void Sphere::sendMatrixToShader(GLuint programID, glm::mat4 Model, glm::mat4 View, glm::mat4 Projection){
    glUniformMatrix4fv(glGetUniformLocation(programID, "ModelMatrix"), 1, GL_FALSE, &Model[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(programID, "ViewMatrix"), 1, GL_FALSE, &View[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(programID, "ProjectionMatrix"), 1, GL_FALSE, &Projection[0][0]);
}

void Sphere::sendInfosToShader(GLuint programID, glm::vec3 camPos, glm::vec3 lightPos, glm::vec3 lightColor){
    glUniform3fv(glGetUniformLocation(programID, "cameraPos"), 1, &camPos[0]);
    glUniform3fv(glGetUniformLocation(programID, "lightPos"), 1, &lightPos[0]);
    glUniform3fv(glGetUniformLocation(programID, "lightColor"), 1, &lightColor[0]);
}
