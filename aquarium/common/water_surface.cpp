#include "water_surface.hpp"
#include "texture.hpp"


WaterSurface::WaterSurface(){
    w = Water() ;
    generateVertices();
    generateIndices();
    generateNormals();
    generateUvs();
    //uvs = compute_uv(vertices);
}

void WaterSurface::generateVertices(){
    // glm::vec3 front_right = glm::vec3(0.5, 0.31, 0.5);
    // glm::vec3 front_left = glm::vec3(-0.5, 0.31, 0.5);
    // glm::vec3 back_right = glm::vec3(0.5, 0.31, -0.5);
    // glm::vec3 back_left = glm::vec3(-0.5, 0.31, -0.5);
    // vertices = {front_left, front_right, back_right, back_left};
    vertices.resize(w.vertices.size()/2);
    size_t taille = vertices.size() ;
    for(int i = 0 ; i<taille ; i++){
        //std::cout<<w.vertices[taille+i][0]<< " ; " <<w.vertices[taille+i][1]<<" ; "<<w.vertices[taille+i][2]<<std::endl ;
        vertices[i] = glm::vec3(w.vertices[taille + i][0],0.31, w.vertices[taille+i][2]);
        //std::cout<<vertices[i][0]<< " ; " <<vertices[i][1]<<" ; "<<vertices[i][2]<<std::endl ;
        //std::cout<<"uv : "<<uvs[i][0]<<" ; "<<uvs[i][1]<<std::endl ;
    }
}

void WaterSurface::generateIndices(){
    // indices = {0, 1, 3,
    //            1, 2, 3};
    size_t count = w.vertices.size()/2 ;
    for (int i = 0; i < w.resolution; i++)
    {
        for (int j = 0; j < w.resolution; j++)
        {
            this->indices.push_back(i * (w.resolution + 1) + j);
            this->indices.push_back((i + 1) * (w.resolution + 1) + j);
            this->indices.push_back(i * (w.resolution + 1) + j + 1);

            this->indices.push_back((i + 1) * (w.resolution + 1) + j);
            this->indices.push_back((i + 1) * (w.resolution + 1) + j + 1);
            this->indices.push_back(i * (w.resolution + 1) + j + 1);
        }
    }
}

void WaterSurface::generateNormals(){
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

void WaterSurface::generateUvs(){
    //uvs = compute_uv(vertices);
    uvs.resize(w.vertices.size()/2);
    size_t taille = uvs.size() ;
    for(int i = 0 ; i<taille ; i++){
        //std::cout<<w.vertices[taille+i][0]<< " ; " <<w.vertices[taille+i][1]<<" ; "<<w.vertices[taille+i][2]<<std::endl ;
        uvs[i] = glm::vec2(abs(w.width/2 + vertices[i][0]/w.width),abs(w.depth/2 + vertices[i][2]/w.depth));
        //std::cout<<"uv : "<<uvs[i][0]<<" ; "<<uvs[i][1]<<std::endl ;
    }
}

void WaterSurface::generateBuffers(std::vector<glm::vec3> vertices, std::vector<unsigned short> indices, std::vector<glm::vec3> normals, std::vector<glm::vec2> uvs){
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &elementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &normalsbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalsbuffer);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
}

void WaterSurface::draw(GLuint programID, GLuint frameBufferTexture){
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

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, normalsbuffer);
    glVertexAttribPointer(
        1,        
        3,        
        GL_FLOAT, 
        GL_FALSE, 
        0,        
        (void *)0 
    );

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glVertexAttribPointer(
        2, 
        2, 
        GL_FLOAT, 
        GL_FALSE, 
        0, 
        (void *)0);

    glDrawElements(
        GL_TRIANGLES,        
        indices.size(), 
        GL_UNSIGNED_SHORT,   
        (void *)0            
    );

    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, frameBufferTexture);
    glUniform1i(glGetUniformLocation(programID, "textureSurface"), 1);
}

void WaterSurface::disableArrays(){
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}

void WaterSurface::deleteBuffers(){
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &elementbuffer);
    glDeleteBuffers(1, &normalsbuffer);
}

void WaterSurface::sendMatrixToShader(GLuint programID, glm::mat4 Model, glm::mat4 View, glm::mat4 Projection){
    glUniformMatrix4fv(glGetUniformLocation(programID, "ModelMatrix"), 1, GL_FALSE, &Model[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(programID, "ViewMatrix"), 1, GL_FALSE, &View[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(programID, "ProjectionMatrix"), 1, GL_FALSE, &Projection[0][0]);
}

void WaterSurface::sendInfosToShader(GLuint programID, glm::vec3 camPos, glm::vec3 lightPos, glm::vec3 lightColor){
    glUniform3fv(glGetUniformLocation(programID, "cameraPos"), 1, &camPos[0]);
    glUniform3fv(glGetUniformLocation(programID, "lightPos"), 1, &lightPos[0]);
    glUniform3fv(glGetUniformLocation(programID, "lightColor"), 1, &lightColor[0]);
}
