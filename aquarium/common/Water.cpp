#include "Water.hpp"
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>

Water::Water()
{
    center = glm::vec3(-0.004, -0.1, -0.004);
    height = 0.79;
    width = 0.99;
    depth = 0.99;
    resolution = 50;
    float amplitude = 0;
    float propagation = 0.0f;
    glm::vec3 intersection = glm::vec3(1000, 1000, 1000);
    intersections.resize(4);
    firstIntersections.resize(4);
    donneesOndes.resize(4);
    nbIntersections = -1;
    createAquarium();
    generateNormals();
    generateSurfaceNormals();
}

void Water::resize(float h, float w, float d)
{
    height = h;
    width = w;
    depth = d;
}

void Water::createAquarium()
{
    float _minX = center[0] - width / 2;
    float _maxX = center[0] + width / 2;
    float _minY = center[1] - height / 2;
    float _maxY = center[1] + height / 2;
    float _minZ = center[2] - depth / 2;
    float _maxZ = center[2] + depth / 2;
    /*     5_____7
          /|    /|
         / |   / |
        1__|__3__|6
        |  4  | /
        |     |/
        0_____2
    */

    glm::vec3 sommet0 = glm::vec3(_minX, _minY, _minZ);
    glm::vec3 sommet1 = glm::vec3(_minX, _maxY, _minZ);
    glm::vec3 sommet2 = glm::vec3(_maxX, _minY, _minZ);
    glm::vec3 sommet3 = glm::vec3(_maxX, _maxY, _minZ);
    glm::vec3 sommet4 = glm::vec3(_minX, _minY, _maxZ);
    glm::vec3 sommet5 = glm::vec3(_minX, _maxY, _maxZ);
    glm::vec3 sommet6 = glm::vec3(_maxX, _minY, _maxZ);
    glm::vec3 sommet7 = glm::vec3(_maxX, _maxY, _maxZ);
    unsigned short cpt = 0;

    for (int i = 0; i < resolution + 1; i++)
    {
        for (int j = 0; j < resolution + 1; j++)
        {
            this->vertices.push_back(sommet0 + glm::vec3(width * (float)i / resolution, 0, depth * (float)j / resolution));
        }
    }

    size_t count = this->vertices.size();
    for (int i = 0; i < resolution + 1; i++)
    {
        for (int j = 0; j < resolution + 1; j++)
        {
            this->vertices.push_back(sommet1 + glm::vec3(width * (float)i / resolution, 0, depth * (float)j / resolution));
            this->surfaceVertices.push_back(sommet1 + glm::vec3(width * (float)i / resolution, 0, depth * (float)j / resolution));
        }
    }

    // MUR DU FOND
    for (int i = 0; i < resolution; i++)
    {
        this->indices.push_back(resolution * (resolution + 1) + i);
        this->indices.push_back(resolution * (resolution + 1) + i + count);
        this->indices.push_back(resolution * (resolution + 1) + i + 1);

        this->indices.push_back(resolution * (resolution + 1) + i + count);
        this->indices.push_back(resolution * (resolution + 1) + i + count + 1);
        this->indices.push_back(resolution * (resolution + 1) + i + 1);
    }

    // MUR DU BAS
    for (int i = 0; i < resolution; i++)
    {
        for (int j = 0; j < resolution; j++)
        {
            this->indices.push_back(i * (resolution + 1) + j);
            this->indices.push_back((i + 1) * (resolution + 1) + j);
            this->indices.push_back(i * (resolution + 1) + j + 1);

            this->indices.push_back((i + 1) * (resolution + 1) + j);
            this->indices.push_back((i + 1) * (resolution + 1) + j + 1);
            this->indices.push_back(i * (resolution + 1) + j + 1);
        }
    }

    // MUR DE GAUCHE
    for (int i = resolution; i > 0; i--)
    {

        this->indices.push_back(i * (resolution + 1));
        this->indices.push_back(i * (resolution + 1) + count);
        this->indices.push_back((i - 1) * (resolution + 1));

        this->indices.push_back(i * (resolution + 1) + count);
        this->indices.push_back((i - 1) * (resolution + 1) + count);
        this->indices.push_back((i - 1) * (resolution + 1));
    }
    
    // MUR DE DEVANT
    for (int i = 0; i < resolution; i++)
    {
        this->indices.push_back(i);
        this->indices.push_back(i + count);
        this->indices.push_back(i + 1);

        this->indices.push_back(i + count);
        this->indices.push_back(i + count + 1);
        this->indices.push_back(i + 1);
    }

    // MUR DE DROITE
    for (int i = resolution; i > 0; i--)
    {
        this->indices.push_back((i + 1) * (resolution + 1) - 1);
        this->indices.push_back((i + 1) * (resolution + 1) - 1 + count);
        this->indices.push_back(i * (resolution + 1) - 1);

        this->indices.push_back((i + 1) * (resolution + 1) - 1 + count);
        this->indices.push_back(i * (resolution + 1) - 1 + count);
        this->indices.push_back(i * (resolution + 1) - 1);
    }
    
    // MUR DU HAUT
    for (int i = 0; i < resolution; i++)
    {
        for (int j = 0; j < resolution; j++)
        {
            this->indices.push_back(count + i * (resolution + 1) + j);
            this->indices.push_back(count + (i + 1) * (resolution + 1) + j);
            this->indices.push_back(count + i * (resolution + 1) + j + 1);

            this->indices.push_back(count + (i + 1) * (resolution + 1) + j);
            this->indices.push_back(count + (i + 1) * (resolution + 1) + j + 1);
            this->indices.push_back(count + i * (resolution + 1) + j + 1);

            // this->surfaceIndices.push_back(i * (resolution + 1) + j);
            // this->surfaceIndices.push_back((i + 1) * (resolution + 1) + j);
            // this->surfaceIndices.push_back(i * (resolution + 1) + j + 1);

            // this->surfaceIndices.push_back((i + 1) * (resolution + 1) + j);
            // this->surfaceIndices.push_back((i + 1) * (resolution + 1) + j + 1);
            // this->surfaceIndices.push_back(i * (resolution + 1) + j + 1);
        }
    }

    surfaceUVs.resize(vertices.size() / 2);
    size_t taille = surfaceUVs.size();
    for (int i = 0; i < taille; i++)
    {
        surfaceUVs[i] = glm::vec2(width / 2 + vertices[count + i][0] / width, depth / 2 + vertices[count + i][2] / depth);
    }
}

void Water::generateSurfaceNormals()
{
    this->surfaceNormals.clear();
    this->surfaceNormals.resize(this->surfaceVertices.size());

    for (size_t i = 0; i < this->surfaceIndices.size(); i += 3)
    {
        glm::vec3 v0 = this->surfaceVertices[this->surfaceIndices[i]];
        glm::vec3 v1 = this->surfaceVertices[this->surfaceIndices[i + 1]];
        glm::vec3 v2 = this->surfaceVertices[this->surfaceIndices[i + 2]];

        // Compute the normal using the cross product of the edges
        glm::vec3 faceNormal = glm::cross(v1 - v0, v2 - v0);

        // Accumulate the normal for each vertex of the face
        this->surfaceNormals[this->surfaceIndices[i]] += faceNormal;
        this->surfaceNormals[this->surfaceIndices[i + 1]] += faceNormal;
        this->surfaceNormals[this->surfaceIndices[i + 2]] += faceNormal;
    }

    for (size_t i = 0; i < this->surfaceNormals.size(); ++i)
    {
        this->surfaceNormals[i] = glm::normalize(this->surfaceNormals[i]);
    }
}

void Water::generateNormals()
{
    this->normals.clear();
    this->normals.resize(this->vertices.size());
    int count = this->vertices.size() / 2;

    for (size_t i = 0; i < this->indices.size(); i += 3)
    {
        glm::vec3 v0 = this->vertices[this->indices[i]];
        glm::vec3 v1 = this->vertices[this->indices[i + 1]];
        glm::vec3 v2 = this->vertices[this->indices[i + 2]];

        // Compute the normal using the cross product of the edges
        glm::vec3 faceNormal = glm::cross(v1 - v0, v2 - v0);

        // Accumulate the normal for each vertex of the face
        this->normals[this->indices[i]] += faceNormal;
        this->normals[this->indices[i + 1]] += faceNormal;
        this->normals[this->indices[i + 2]] += faceNormal;
    }

    // Normalize the normals
    for (size_t i = 0; i < this->normals.size(); ++i)
    {
        if (i <= count)
        {
            this->normals[i] = glm::normalize(this->normals[i]);
        }
        else
        {
            this->normals[i].y *= (-1);
            this->normals[i] = glm::normalize(this->normals[i]);
        }
    }
}

unsigned short Water::getShorterDistance(Sphere sphere)
{
    int count = this->vertices.size() / 2;
    float distance_min = FLT_MAX;
    unsigned short indice;
    for (unsigned int i = 0; i < this->indices.size(); i++)
    {
        if (this->indices[i] > count)
        {
            float distance = glm::length(sphere.centre_sphere - this->vertices[this->indices[i]]);
            if (distance < distance_min)
            {
                distance_min = distance;
                indice = this->indices[i];
            }
        }
    }
    return indice;
}

bool Water::intersectBetweenWaterAndSphere(Sphere sphere)
{
    // std::cout<<sphere.centre_sphere.x<<", "<<sphere.centre_sphere.y<<", "<<sphere.centre_sphere.z<<std::endl;
    // std::cout<<sphere.radius_sphere<<std::endl;
    unsigned short closer_indice = getShorterDistance(sphere);
    // std::cout<<closer_indice<<std::endl;
    glm::vec3 closer_vertex = this->vertices[closer_indice];
    float distance_sphere_water_y = abs(sphere.centre_sphere.y - closer_vertex.y);
    float distance_sphere_water_x = sphere.centre_sphere.x - closer_vertex.x;
    float distance_sphere_water_z = sphere.centre_sphere.z - closer_vertex.z;
    if ((distance_sphere_water_y <= sphere.radius_sphere) //&&  sphere.centre_sphere.y >= closer_vertex.y)
                                                          //|| (distance_sphere_water_y >= sphere.radius_sphere &&  sphere.centre_sphere.y <= closer_vertex.y))
    )
    {
        // std::cout << "INTERSECTION!" << std::endl;
        return true;
    }
    else
    {
        // std::cout << "NO INTERSECTION!" << std::endl;
        return false;
    }
}

void Water::generateWaterBuffers(std::vector<glm::vec3> vertices, std::vector<unsigned short> indices, std::vector<glm::vec3> normals)
{

    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &elementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &normalsbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, normalsbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
}

void Water::generateWaterBuffersSurface(std::vector<glm::vec3> vertices, std::vector<unsigned short> indices, std::vector<glm::vec3> normals, std::vector<glm::vec2> uvs)
{

    glGenBuffers(1, &surfacevertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, surfacevertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &surfaceelementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, surfaceelementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

    // glGenBuffers(1, &surfacenormalsbuffer);
    // glBindBuffer(GL_ARRAY_BUFFER, surfacenormalsbuffer);
    // glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

    // glGenBuffers(1, &surfaceuvbuffer);
    // glBindBuffer(GL_ARRAY_BUFFER, surfaceuvbuffer);
    // glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
}

void Water::draw(bool texture, Sphere s, glm::vec3 &departMouvement, int &valDeplacement, bool &sphereWasIntersectingWater, GLuint programID, float currentFrame, bool clip, glm::vec4 clipPlane, GLuint reflectionTexture, GLuint refractionTexture, glm::vec3 camPos, glm::vec3 lightPos, glm::vec3 lightColor)
{
    if (clip)
    {
        glEnable(GL_CLIP_DISTANCE0);
        glUniform4f(glGetUniformLocation(programID, "clippingPlane"), clipPlane.x, clipPlane.y, clipPlane.z, clipPlane.w);

        simulateOnde(s, sphereWasIntersectingWater, departMouvement, valDeplacement);

        sendValueToShader(programID, currentFrame, camPos, lightPos, lightColor);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, this->vertexbuffer);
        glVertexAttribPointer(
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void *)0);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, this->normalsbuffer);
        glVertexAttribPointer(
            1,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void *)0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementbuffer);

        glDrawElements(
            GL_TRIANGLES,
            indices.size(),
            GL_UNSIGNED_SHORT,
            (void *)0);
    }
    else
    {
        simulateOnde(s, sphereWasIntersectingWater, departMouvement, valDeplacement);

        sendValueToShader(programID, currentFrame, camPos, lightPos, lightColor);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, this->vertexbuffer);
        glVertexAttribPointer(
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void *)0);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, this->normalsbuffer);
        glVertexAttribPointer(
            1,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void *)0);

        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, this->surfacenormalsbuffer);
        glVertexAttribPointer(
            2,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void *)0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->elementbuffer);

        glDrawElements(
            GL_TRIANGLES,
            indices.size(),
            GL_UNSIGNED_SHORT,
            (void *)0);

        if (texture == true)
        {
            glActiveTexture(GL_TEXTURE0 + 1);
            glBindTexture(GL_TEXTURE_2D, reflectionTexture);
            glUniform1i(glGetUniformLocation(programID, "reflectionTexture"), 1);

            glActiveTexture(GL_TEXTURE0 + 2);
            glBindTexture(GL_TEXTURE_2D, refractionTexture);
            glUniform1i(glGetUniformLocation(programID, "refractionTexture"), 2);
        }
    }
}

void Water::sendValueToShader(GLuint programID, float currentFrame, glm::vec3 camPos, glm::vec3 lightPos, glm::vec3 lightColor)
{
    glUniform3fv(glGetUniformLocation(programID, "intersection"), 1, &intersections[0][0]);
    glUniform3fv(glGetUniformLocation(programID, "intersection1"), 1, &intersections[1][0]);
    glUniform3fv(glGetUniformLocation(programID, "intersection2"), 1, &intersections[2][0]);
    glUniform3fv(glGetUniformLocation(programID, "intersection3"), 1, &intersections[3][0]);

    glUniform3fv(glGetUniformLocation(programID, "firstIntersection"), 1, &firstIntersections[0][0]);
    glUniform3fv(glGetUniformLocation(programID, "firstIntersection1"), 1, &firstIntersections[1][0]);
    glUniform3fv(glGetUniformLocation(programID, "firstIntersection2"), 1, &firstIntersections[2][0]);
    glUniform3fv(glGetUniformLocation(programID, "firstIntersection3"), 1, &firstIntersections[3][0]);

    glUniform1f(glGetUniformLocation(programID, "time"), currentFrame);
    amplitude1 = donneesOndes[0][0];
    propagation1 = donneesOndes[0][1];
    amplitude2 = donneesOndes[1][0];
    propagation2 = donneesOndes[1][1];
    amplitude3 = donneesOndes[2][0];
    propagation3 = donneesOndes[2][1];
    amplitude4 = donneesOndes[3][0];
    propagation4 = donneesOndes[3][1];
    glUniform1f(glGetUniformLocation(programID, "amplitude"), amplitude1);
    glUniform1f(glGetUniformLocation(programID, "propagation"), propagation1);
    glUniform1f(glGetUniformLocation(programID, "amplitude1"), amplitude2);
    glUniform1f(glGetUniformLocation(programID, "propagation1"), propagation2);
    glUniform1f(glGetUniformLocation(programID, "amplitude2"), amplitude3);
    glUniform1f(glGetUniformLocation(programID, "propagation2"), propagation3);
    glUniform1f(glGetUniformLocation(programID, "amplitude3"), amplitude4);
    glUniform1f(glGetUniformLocation(programID, "propagation3"), propagation4);

    glUniform1i(glGetUniformLocation(programID, "direction"), direction);

    glUniform3fv(glGetUniformLocation(programID, "cameraPos"), 1, &camPos[0]);
    glUniform3fv(glGetUniformLocation(programID, "lightPos"), 1, &lightPos[0]);
    glUniform3fv(glGetUniformLocation(programID, "lightColor"), 1, &lightColor[0]);
}

void Water::simulateOnde(Sphere s, bool &sphereWasIntersectingWater, glm::vec3 &departMouvement, int &valDeplacement)
{
    int ancienneDirection = direction;
    bool inter = intersectBetweenWaterAndSphere(s);
    if (inter == true)
    {
        direction = valDeplacement;
        glm::vec3 lastIntersection = intersection;
        intersection = vertices[getShorterDistance(s)];
        if (ancienneDirection > 0 && ancienneDirection < 5 && direction > 0 && direction < 5 && ancienneDirection != direction)
        {
            nbIntersections = (nbIntersections + 1) % 4;
            firstIntersections[nbIntersections] = intersection;
            intersections[nbIntersections] = intersection;
            donneesOndes[nbIntersections][0] = glm::length(departMouvement - intersection);
            donneesOndes[nbIntersections][1] = 0;
        }
        else if (amplitude == 0)
        {
            if (sphereWasIntersectingWater == false)
            {
                nbIntersections = (nbIntersections + 1) % 4;
            }
            amplitude = abs(departMouvement.y - intersection.y) / 5;
            propagation = 0;
            firstIntersections[nbIntersections] = intersection;
            intersections[nbIntersections] = intersection;
            donneesOndes[nbIntersections] = glm::vec2(amplitude, propagation);
        }
        else if (donneesOndes[nbIntersections][0] >= 0 && sphereWasIntersectingWater == true && lastIntersection != intersection)
        {
            if (donneesOndes[nbIntersections][0] > 0.01)
            {
                donneesOndes[nbIntersections][0] /= 1.001;
                donneesOndes[nbIntersections][1] += 0.005;
            }
            intersections[nbIntersections] = intersection;
        }
        else if (donneesOndes[nbIntersections][0] != 0 && sphereWasIntersectingWater == true && lastIntersection == intersection)
        {
            if (donneesOndes[nbIntersections][0] > 0.01)
            {
                donneesOndes[nbIntersections][0] /= 1.001;
                donneesOndes[nbIntersections][1] += 0.005;
            }
            else if (donneesOndes[nbIntersections][0] > 0)
            {
                donneesOndes[nbIntersections][0] -= 0.00001;
                donneesOndes[nbIntersections][1] += 0.005;
            }
            // intersections[nbIntersections] = intersection;
            // firstIntersections[nbIntersections] = intersection;
        }

        else if (amplitude != 0 && sphereWasIntersectingWater == false)
        {
            nbIntersections = (nbIntersections + 1) % 4;
            intersections[nbIntersections] = intersection;
            firstIntersections[nbIntersections] = intersection;
            amplitude = abs(departMouvement.y - intersection.y) / 5;
            propagation = 0;
            donneesOndes[nbIntersections] = glm::vec2(amplitude, propagation);
        }
        for (int i = 0; i < 4; i++)
        {
            if (donneesOndes[i][0] > 0.01)
            {
                donneesOndes[i][0] /= 1.001;
                donneesOndes[i][1] += 0.005;
            }
            else if (donneesOndes[i][0] > 0)
            {
                donneesOndes[i][0] -= 0.0001;
                donneesOndes[i][1] += 0.005;
            }
            else if (donneesOndes[i][0] <= 0)
            {
                donneesOndes[i][0] = 0;
            }
        }

        sphereWasIntersectingWater = true;
    }
    else
    {
        if (sphereWasIntersectingWater == true)
        {
            nbIntersections = (nbIntersections + 1) % 4;
            donneesOndes[nbIntersections][0] = abs(departMouvement.y - intersection.y) / 5;
            donneesOndes[nbIntersections][1] = 0;
            firstIntersections[nbIntersections] = intersection;
            intersections[nbIntersections] = intersection;
        }
        for (int i = 0; i < 4; i++)
        {
            if (donneesOndes[i][0] > 0.01)
            {
                donneesOndes[i][0] /= 1.001;
                donneesOndes[i][1] += 0.005;
            }
            else if (donneesOndes[i][0] > 0)
            {
                donneesOndes[i][0] -= 0.00001;
                donneesOndes[i][1] += 0.005;
            }
            else if (donneesOndes[i][0] < 0)
            {
                donneesOndes[i][0] = 0;
                // donneesOndes[i][1] += 0.005;
            }
        }

        sphereWasIntersectingWater = false;
    }
}

void Water::disableArrays()
{
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(4);
}

void Water::deleteBuffers()
{
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &elementbuffer);
    glDeleteBuffers(1, &normalsbuffer);
    glDeleteBuffers(1, &surfacevertexbuffer);
    glDeleteBuffers(1, &surfaceelementbuffer);
    glDeleteBuffers(1, &surfacenormalsbuffer);
    glDeleteBuffers(1, &surfaceuvbuffer);
}

void Water::sendMatrixToShader(GLuint programID, glm::mat4 Model, glm::mat4 View, glm::mat4 Projection)
{
    glUniformMatrix4fv(glGetUniformLocation(programID, "ModelMatrix"), 1, GL_FALSE, &Model[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(programID, "ViewMatrix"), 1, GL_FALSE, &View[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(programID, "ProjectionMatrix"), 1, GL_FALSE, &Projection[0][0]);
}

void Water::createSurface()
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            surfaceVertices.push_back(glm::vec3(width * (float)i / 10, 1, depth * (float)j / 10));
        }
    }
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            this->surfaceIndices.push_back(i * 11 + j);
            this->surfaceIndices.push_back((i + 1) * 11 + j);
            this->surfaceIndices.push_back(i * 11 + j + 1);
        }
    }
}

void Water::drawSurface()
{
    glEnableVertexAttribArray(3);
    glBindBuffer(GL_ARRAY_BUFFER, this->surfacevertexbuffer);
    glVertexAttribPointer(
        3,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        (void *)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->surfaceelementbuffer);

    glDrawElements(
        GL_TRIANGLES,
        surfaceIndices.size(),
        GL_UNSIGNED_SHORT,
        (void *)0);
}
