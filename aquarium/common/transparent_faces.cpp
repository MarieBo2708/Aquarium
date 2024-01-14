#include "transparent_faces.hpp"

transparentFaces::transparentFaces(){
    generateVertices();
    
    generateIndices();
    
    generateNormals();
}


void transparentFaces::generateVertices(){
    vertices = {glm::vec3(-0.5, -0.5, 0.5),
                glm::vec3(-0.5, 0.5, 0.5),
                glm::vec3(-0.5, 0.5, -0.5),
                glm::vec3(-0.5, -0.5, -0.5),
                glm::vec3(0.5, 0.5, 0.5),
                glm::vec3(0.5, -0.5, 0.5),
                };
}

void transparentFaces::generateIndices(){
    indices = {0, 1, 2,
                0, 2, 3,
                0, 1, 4,
                0, 4, 5,
                };
    }

void transparentFaces::generateNormals(){
    normals.clear();
    normals.resize(vertices.size());
    for (size_t i = 0; i < indices.size(); i += 3) {
        glm::vec3 v0 = vertices[indices[i]];
        glm::vec3 v1 = vertices[indices[i + 1]];
        glm::vec3 v2 = vertices[indices[i + 2]];

        // Compute the normal using the cross product of the edges
        glm::vec3 faceNormal = glm::cross(v1 - v0, v2 - v0);

        // Accumulate the normal for each vertex of the face
        normals[indices[i]] += faceNormal;
        normals[indices[i + 1]] += faceNormal;
        normals[indices[i + 2]] += faceNormal;
    }

    // Normalize the normals
    for (size_t i = 0; i < normals.size(); ++i) {
        normals[i] = glm::normalize(normals[i]);
    }

}

void transparentFaces::generateBuffers(std::vector<glm::vec3> vertices, std::vector<unsigned short> indices, std::vector<glm::vec3> normals){
    glGenBuffers(1, &vertexbuffertransparant);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffertransparant);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &elementbuffertransparent);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffertransparent);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &normalsbuffertransparent);
    glBindBuffer(GL_ARRAY_BUFFER, normalsbuffertransparent);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

}

void transparentFaces::drawFaces(GLuint programID, bool clip, glm::vec4 clipPlane){
    if(clip){
        glEnable(GL_CLIP_DISTANCE0);
        glUniform4f(glGetUniformLocation(programID, "clippingPlane"), clipPlane.x, clipPlane.y, clipPlane.z, clipPlane.w);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffertransparant);
        glVertexAttribPointer(
            0,        // attribute
            3,        // size
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0,        // stride
            (void *)0 // array buffer offset
        );

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, normalsbuffertransparent);
        glVertexAttribPointer(
            1,        // attribute
            3,        // size
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0,        // stride
            (void *)0 // array buffer offset
        );

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffertransparent);
        
        glDepthFunc(GL_LESS);
        glDrawElements(
            GL_TRIANGLES,      // mode
            indices.size(),    // count
            GL_UNSIGNED_SHORT, // type
            (void *)0          // element array buffer offset
        );
    }
    else{
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffertransparant);
        glVertexAttribPointer(
            0,        // attribute
            3,        // size
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0,        // stride
            (void *)0 // array buffer offset
        );

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, normalsbuffertransparent);
        glVertexAttribPointer(
            1,        // attribute
            3,        // size
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0,        // stride
            (void *)0 // array buffer offset
        );

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffertransparent);
        
        glDepthFunc(GL_LESS);
        glDrawElements(
            GL_TRIANGLES,      // mode
            indices.size(),    // count
            GL_UNSIGNED_SHORT, // type
            (void *)0          // element array buffer offset
        );
    }
}

void transparentFaces::disableArrays(){
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void transparentFaces::deleteBuffers(){
    glDeleteBuffers(1, &vertexbuffertransparant);
    glDeleteBuffers(1, &elementbuffertransparent);
    glDeleteBuffers(1, &normalsbuffertransparent);
}

void transparentFaces::sendMatrixToShader(GLuint programID, glm::mat4 Model, glm::mat4 View, glm::mat4 Projection){
    glUniformMatrix4fv(glGetUniformLocation(programID, "ModelMatrix"), 1, GL_FALSE, &Model[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(programID, "ViewMatrix"), 1, GL_FALSE, &View[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(programID, "ProjectionMatrix"), 1, GL_FALSE, &Projection[0][0]);
}

void transparentFaces::sendInfosToShader(GLuint programID, glm::vec3 camPos, glm::vec3 lightPos, glm::vec3 lightColor){
    glUniform3fv(glGetUniformLocation(programID, "cameraPos"), 1, &camPos[0]);
    glUniform3fv(glGetUniformLocation(programID, "lightPos"), 1, &lightPos[0]);
    glUniform3fv(glGetUniformLocation(programID, "lightColor"), 1, &lightColor[0]);
}
