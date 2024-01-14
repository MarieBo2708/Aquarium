#include "texture_faces.hpp"
#include "texture.hpp"

textureFaces::textureFaces(){
    generateVertices();
    generateIndices();
    generateNormals();
    uvs = compute_uv(vertices);

}


void textureFaces::generateVertices(){
    vertices = {glm::vec3(-0.5, -0.5, -0.5),
                glm::vec3(-0.5, 0.5, -0.5),
                glm::vec3(0.5, 0.5, -0.5),
                glm::vec3(0.5, -0.5, -0.5),
                glm::vec3(0.5, 0.5, 0.5),
                glm::vec3(0.5, -0.5, 0.5),
                glm::vec3(-0.5,-0.5,-0.5),
                glm::vec3(-0.5,-0.5,0.5),
                glm::vec3(0.5,-0.5,0.5),
                glm::vec3(0.5,-0.5,-0.5)};
}

void textureFaces::generateIndices(){
    indices = {0, 1, 2,
                2, 3, 0,
                5, 3, 2,
                2, 4, 5,
                8, 7, 6,
                6, 9, 8};
}

void textureFaces::generateNormals(){
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

void textureFaces::generateBuffers(std::vector<glm::vec3> vertices, std::vector<unsigned short> indices, std::vector<glm::vec3> normals, std::vector<glm::vec2> uvs){
    glGenBuffers(1, &vertexbuffertexture);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffertexture);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &elementbuffertexture);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffertexture);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &uvbuffertexture);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffertexture);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

    glGenBuffers(1, &normalsbuffertexture);
    glBindBuffer(GL_ARRAY_BUFFER, normalsbuffertexture);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);


}

void textureFaces::generateUvs(){
    uvs = compute_uv(vertices);
}

void textureFaces::drawFaces(GLuint programID, GLuint texture, bool clip, glm::vec4 clipPlane){
    if(clip){
        glEnable(GL_CLIP_DISTANCE0);
        glUniform4f(glGetUniformLocation(programID, "clippingPlane"), clipPlane.x, clipPlane.y, clipPlane.z, clipPlane.w);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffertexture);
        glVertexAttribPointer(
            0,        // attribute
            3,        // size
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0,        // stride
            (void *)0 // array buffer offset
        );

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffertexture);
        glVertexAttribPointer(
            1, 
            2, 
            GL_FLOAT, 
            GL_FALSE, 
            0, 
            (void *)0);
        
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, normalsbuffertexture);
        glVertexAttribPointer(
            2,        // attribute
            3,        // size
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0,        // stride
            (void *)0 // array buffer offset
        );

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffertexture);
        glDrawElements(
            GL_TRIANGLES,      // mode
            indices.size(),    // count
            GL_UNSIGNED_SHORT, // type
            (void *)0          // element array buffer offset
        );

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(glGetUniformLocation(programID, "textureFace"), 0);
    }
    else{
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffertexture);
        glVertexAttribPointer(
            0,        // attribute
            3,        // size
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0,        // stride
            (void *)0 // array buffer offset
        );

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffertexture);
        glVertexAttribPointer(
            1, 
            2, 
            GL_FLOAT, 
            GL_FALSE, 
            0, 
            (void *)0);
        
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, normalsbuffertexture);
        glVertexAttribPointer(
            2,        // attribute
            3,        // size
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0,        // stride
            (void *)0 // array buffer offset
        );

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffertexture);
        glDrawElements(
            GL_TRIANGLES,      // mode
            indices.size(),    // count
            GL_UNSIGNED_SHORT, // type
            (void *)0          // element array buffer offset
        );

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(glGetUniformLocation(programID, "textureFace"), 0);
    }
    
}

void textureFaces::disableArrays(){
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}

void textureFaces::deleteBuffers(){
    glDeleteBuffers(1, &vertexbuffertexture);
    glDeleteBuffers(1, &elementbuffertexture);
    glDeleteBuffers(1, &normalsbuffertexture);
    glDeleteBuffers(1, &uvbuffertexture);

}

void textureFaces::sendMatrixToShader(GLuint programID, glm::mat4 Model, glm::mat4 View, glm::mat4 Projection){
    glUniformMatrix4fv(glGetUniformLocation(programID, "ModelMatrix"), 1, GL_FALSE, &Model[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(programID, "ViewMatrix"), 1, GL_FALSE, &View[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(programID, "ProjectionMatrix"), 1, GL_FALSE, &Projection[0][0]);
}

void textureFaces::sendInfosToShader(GLuint programID, glm::vec3 camPos, glm::vec3 lightPos, glm::vec3 lightColor){
    glUniform3fv(glGetUniformLocation(programID, "cameraPos"), 1, &camPos[0]);
    glUniform3fv(glGetUniformLocation(programID, "lightPos"), 1, &lightPos[0]);
    glUniform3fv(glGetUniformLocation(programID, "lightColor"), 1, &lightColor[0]);
}

