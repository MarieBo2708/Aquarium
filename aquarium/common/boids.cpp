#include "boids.hpp"
#include "texture.hpp"
#include "objloader.hpp"

Boids::Boids() {}

Boids::Boids(const char *path_obj, const char *path_texture)
{
    loadAssImp(path_obj, indices, vertices, uvs, normals);
    texture = loadTexture2DFromFilePath(path_texture);
}

glm::vec3 rdPos()
{
    float x = ((float)rand() / RAND_MAX) * 0.8 - 0.4;
    float y = ((float)rand() / RAND_MAX) * 0.4 - 0.2;
    float z = ((float)rand() / RAND_MAX) * 0.8 - 0.4;
    return glm::vec3(x, y, z);
}

bool stability(std::vector<float> tab)
{
    size_t taille = tab.size();
    float val1 = tab[0];
    float val2 = tab[1];
    if (val1 == val2)
    {
        return false;
    }
    for (size_t i = 2; i < taille; i++)
    {
        if (tab[i] != val1 && i % 2 == 0)
        {
            return false;
        }
        if (tab[i] != val2 && i % 2 == 1)
        {
            return false;
        }
    }
    return true;
}

void modifTab(std::vector<float> &tab)
{
    size_t taille = tab.size();
    for (size_t i = 1; i < taille; i++)
    {
        tab[i - 1] = tab[i];
    }
    tab[taille - 1] = 0;
}

void Boids::deplacementChef(Sphere s, bool &atteintObjectif, glm::vec3 &positionDepart, glm::vec3 &positionObjectif, float &distance, float &deltaTime, float &angleAutourY, float &angle, glm::vec3 &deplacement, int &cptTab, std::vector<float> &derniersAngles, bool stab, int &valeur)
{
    positionDepart = glm::vec3(ModelBoids[3][0], ModelBoids[3][1], ModelBoids[3][2]);
    speed = 0.005 ;
    if (atteintObjectif == true)
    {
        
        positionObjectif = rdPos();
        atteintObjectif = false;

        cptTab = 0;
        valeur = 0;
        distance = glm::length(positionObjectif - positionDepart);
         //distance * deltaTime;
    }

    if (atteintObjectif == false)
    {
        // if (glm::length(s.centre_sphere - glm::vec3(ModelBoids[3][0], ModelBoids[3][1], ModelBoids[3][2])) > 0.5)
        // {
        glm::vec3 deplacement1 = glm::normalize(glm::vec3(cos(glm::radians(angleAutourY + 1)), positionObjectif[1] - ModelBoids[3][1], sin(glm::radians(angleAutourY + 1)))) * speed;
        glm::vec3 nextPos1 = glm::vec3(ModelBoids[3][0], 0, ModelBoids[3][2]) + deplacement1;

        glm::vec3 deplacement2 = glm::normalize(glm::vec3(cos(glm::radians(angleAutourY - 1)), positionObjectif[1] - ModelBoids[3][1], sin(glm::radians(angleAutourY - 1)))) * speed;
        glm::vec3 nextPos2 = glm::vec3(ModelBoids[3][0], 0, ModelBoids[3][2]) + deplacement2;
        // if (glm::length(s.centre_sphere - glm::vec3(ModelBoids[3][0], ModelBoids[3][1], ModelBoids[3][2])) < 0.5){
        //     std::cout<<"ICI : "<<glm::length(s.centre_sphere - glm::vec3(ModelBoids[3][0], ModelBoids[3][1], ModelBoids[3][2]))<<std::endl ;
        //     //angle = 0 ;
        //     deplacement = glm::normalize(-glm::vec3(s.centre_sphere - glm::vec3(ModelBoids[3][0], ModelBoids[3][1], ModelBoids[3][2]))) * 0.1f;
        // }
        if (glm::length(positionObjectif - nextPos1) <= glm::length(positionObjectif - nextPos2))
        {
            angle = 1;
            deplacement = deplacement1;
        }
        else if (glm::length(positionObjectif - nextPos1) >= glm::length(positionObjectif - nextPos2))
        {
            angle = -1;
            deplacement = deplacement2;
        }
        if (glm::length(s.centre_sphere - glm::vec3(ModelBoids[3][0], ModelBoids[3][1], ModelBoids[3][2])) > 0.5)
        {
            if (cptTab < 10)
            {
                derniersAngles[cptTab] = angle;
                cptTab++;
            }

            if (cptTab == 10)
            {
                stab = stability(derniersAngles);
                if (stab == false)
                {
                    modifTab(derniersAngles);
                    derniersAngles[9] = angle;
                }
                else
                {
                    if (((valeur + 40) / 80) % 2 == 0)
                    {
                        angle = -0.1;
                        deplacement = glm::normalize(glm::vec3(cos(glm::radians(angleAutourY - 0.1)), (positionObjectif[1] - ModelBoids[3][1]) / length(positionObjectif - glm::vec3(ModelBoids[3][0], ModelBoids[3][1], ModelBoids[3][2])), sin(glm::radians(angleAutourY - 0.1)))) * speed;
                    }
                    else if (((valeur + 40) / 80) % 2 == 1)
                    {
                        angle = 0.1;
                        deplacement = glm::normalize(glm::vec3(cos(glm::radians(angleAutourY + 0.1)), (positionObjectif[1] - ModelBoids[3][1]) / length(positionObjectif - glm::vec3(ModelBoids[3][0], ModelBoids[3][1], ModelBoids[3][2])), sin(glm::radians(angleAutourY + 0.1)))) * speed;
                    }
                    valeur++;
                }
            }
       }
        angleAutourY += angle;
        int val = (int)angleAutourY;
        float reste = angleAutourY - val;
        angleAutourY = val % 360 + reste;

        if (glm::distance(positionDepart, positionObjectif) < 0.2)
        {
            atteintObjectif = true;
        }
    }

    if((ModelBoids[3][0] + deplacement[0])>0.45 || (ModelBoids[3][0] + deplacement[0])<-0.45){
        deplacement[0] = 0 ;
    }
    if((ModelBoids[3][1] + deplacement[1])>0.45 || (ModelBoids[3][1] + deplacement[1])<-0.25){
        deplacement[1] = 0 ;
    }
    if((ModelBoids[3][2] + deplacement[2])>0.45 || (ModelBoids[3][2] + deplacement[2])<-0.45){
        deplacement[2] = 0 ;
    }
}

void Boids::peur(Sphere s, glm::vec3 &deplacement, float &angle, float &angleAutourY, Boids b1, Boids b2)
{
    glm::vec3 positionB1 = glm::vec3(b1.ModelBoids[3][0], b1.ModelBoids[3][1], b1.ModelBoids[3][2]);
    glm::vec3 positionB2 = glm::vec3(b2.ModelBoids[3][0], b2.ModelBoids[3][1], b2.ModelBoids[3][2]);
    glm::vec3 lastDeplacement = deplacement;
    glm::vec3 positionBoid = glm::vec3(ModelBoids[3][0], ModelBoids[3][1], ModelBoids[3][2]);
    float distance = glm::length(s.centre_sphere - positionBoid);
    float deplacementX = glm::normalize(positionBoid[0] - s.centre_sphere[0]) * speed;
    bool danger = false ;
    if (positionBoid[0] + deplacementX < -0.25 || positionBoid[0] + deplacementX > 0.25)
    {
        deplacementX = 0.0;
        
    }
    float deplacementY = glm::normalize(positionBoid[1] - s.centre_sphere[1]) * speed;
    if (positionBoid[1] + deplacementY < -0.25 || positionBoid[1] + deplacementY > 0.05)
    {
        deplacementY = 0.0;
        
    }
    float deplacementZ = glm::normalize(positionBoid[2] - s.centre_sphere[2]) * speed;
    if (positionBoid[2] + deplacementZ < -0.15 || positionBoid[2] + deplacementZ > 0.25)
    {
        deplacementZ = 0.0;
    }
    deplacement = glm::vec3(deplacementX, deplacementY, deplacementZ);
    glm::vec3 positionObjectif = positionBoid + deplacement ;
    glm::vec3 deplacement1 = glm::normalize(glm::vec3(cos(glm::radians(angleAutourY + 1)), positionObjectif[1] - ModelBoids[3][1], sin(glm::radians(angleAutourY + 1)))) * speed ;
    glm::vec3 nextPos1 = glm::vec3(ModelBoids[3][0], 0, ModelBoids[3][2]) + deplacement1;

    glm::vec3 deplacement2 = glm::normalize(glm::vec3(cos(glm::radians(angleAutourY - 1)), positionObjectif[1] - ModelBoids[3][1], sin(glm::radians(angleAutourY - 1)))) * speed ;
    glm::vec3 nextPos2 = glm::vec3(ModelBoids[3][0], 0, ModelBoids[3][2]) + deplacement2;
    if (glm::length(positionObjectif - nextPos1) <= glm::length(positionObjectif - nextPos2))
    {
        angle = 1;
        deplacement = deplacement1;
    }
    else if (glm::length(positionObjectif - nextPos1) >= glm::length(positionObjectif - nextPos2))
    {
        angle = -1;
        deplacement = deplacement2;
    }
    angleAutourY += angle;
    if(speed < 0.1) speed += 0.001f ;
    if((ModelBoids[3][0] + deplacement[0])>0.25 || (ModelBoids[3][0] + deplacement[0])<-0.25){
        //deplacement[0] = 0 ;

        danger = true ;
    }
    if((ModelBoids[3][1] + deplacement[1])<-0.25){
        deplacement[1] += 0.001 ;
        danger = true ;
    }
    if((ModelBoids[3][1] + deplacement[1])>0.05){
        deplacement[1] -= 0.001 ;
        danger = true ;
    }
    if((ModelBoids[3][2] + deplacement[2])>0.25 || (ModelBoids[3][2] + deplacement[2])<-0.25){
        //deplacement[2] = 0 ;
        danger = true ;
    }
    if(danger = true && speed > 0) speed -= 0.0002f ;
    if((ModelBoids[3][0] + deplacement[0])>0.45 || (ModelBoids[3][0] + deplacement[0])<-0.45){
        deplacement[0] = 0 ;

        //danger = true ;
    }
    if((ModelBoids[3][1] + deplacement[1])>0.25 || (ModelBoids[3][1] + deplacement[1])<-0.45){
        deplacement[1] = 0 ;
        //danger = true ;
    }
    if((ModelBoids[3][2] + deplacement[2])>0.45 || (ModelBoids[3][2] + deplacement[2])<-0.45){
        deplacement[2] = 0 ;
        //danger = true ;
    }
    float l1 = glm::length(positionBoid + deplacement - positionB1);
    float l2 = glm::length(positionBoid + deplacement - positionB2);
    if(l1 < 0.1 || l2 < 0.01) deplacement = glm::vec3(0,0,0);
}

float ecartAngle(float &angle1, float &angle2)
{
    float val;
    if (angle1 >= 0 && angle2 >= 0 && angle1 >= angle2)
    {
        val = abs(angle1 - angle2);
        if (val < 180)
        {
            return val;
        }
        else
        {
            return 360 - val;
        }
    }
    if (angle1 >= 0 && angle2 >= 0 && angle1 <= angle2)
    {
        val = abs(angle1 - angle2);
        if (val < 180)
        {
            return -val;
        }
        else
        {
            return -(360 - val);
        }
    }
    if (angle1 <= 0 && angle2 <= 0 && angle1 <= angle2)
    {
        val = abs(angle1 - angle2);
        if (val < 180)
        {
            return val;
        }
        else
        {
            return 360 - val;
        }
    }
    if (angle1 <= 0 && angle2 <= 0 && angle1 >= angle2)
    {
        val = abs(angle1 - angle2);
        if (val < 180)
        {
            return -val;
        }
        else
        {
            return -(360 - val);
        }
    }
    if (angle1 <= 0 && angle2 > 0)
    {
        float angle1bis = 360 + angle1;
        if (angle1bis >= angle2)
        {
            val = abs(angle1bis - angle2);
            if (val < 180)
            {
                return val;
            }
            else
            {
                return 360 - val;
            }
        }
        if (angle1bis <= angle2)
        {
            val = abs(angle1bis - angle2);
            if (val < 180)
            {
                return -val;
            }
            else
            {
                return -(360 - val);
            }
        }
    }
    if (angle1 >= 0 && angle2 < 0)
    {
        float angle2bis = 360 + angle2;
        val = abs(angle1 - angle2bis);
        if (angle1 >= angle2bis)
        {
            val = abs(angle1 - angle2bis);
            if (val < 180)
            {
                return val;
            }
            else
            {
                return 360 - val;
            }
        }
        if (angle1 <= angle2bis)
        {
            val = abs(angle1 - angle2bis);
            if (val < 180)
            {
                return -val;
            }
            else
            {
                return -(360 - val);
            }
        }
    }
}

void Boids::deplacementAutre(Boids other, glm::vec3 &deplacementOther, glm::vec3 &deplacementChef, glm::vec3 positionChef, float &angleChefTotal, float &angleChefEnCours, glm::vec3 &deplacementBoid, float &angleBoidTotal, float &angleBoidEnCours)
{
    glm::vec3 positionEnCours = glm::vec3(ModelBoids[3][0], ModelBoids[3][1], ModelBoids[3][2]);
    glm::vec3 positionOther = glm::vec3(other.ModelBoids[3][0], other.ModelBoids[3][1], other.ModelBoids[3][2]);
    float ecart = glm::length(positionChef - positionEnCours);
    float ecartOther = glm::length(positionOther - positionEnCours);
    deplacementBoid = glm::vec3(0, 0, 0);
    if (ecartOther < 0.2)
    {
        if (positionOther[0] > positionEnCours[0])
        {
            deplacementBoid[0] += deplacementOther[0] - 0.01;
        }
        else if (positionOther[0] < positionEnCours[0])
        {
            deplacementOther[0] += deplacementOther[0] + 0.01;
        }
        if (positionOther[1] > positionEnCours[1])
        {
            deplacementBoid[1] += deplacementOther[1] - 0.01;
        }
        else if (positionOther[1] < positionEnCours[1])
        {
            deplacementBoid[1] += deplacementOther[1] + 0.01;
        }
        if (positionOther[2] > positionEnCours[2])
        {
            deplacementBoid[2] += deplacementOther[2] - 0.01;
        }
        else if (positionOther[2] < positionEnCours[2])
        {
            deplacementBoid[2] += deplacementOther[2] + 0.01;
        }
    }
    else if (ecartOther > 0.25 /* && ecart <= 0.2 */)
    {
        if (positionOther[0] > positionEnCours[0])
        {
            deplacementBoid[0] += deplacementOther[0] + 0.01;
        }
        else if (positionOther[0] < positionEnCours[0])
        {
            deplacementOther[0] += deplacementOther[0] - 0.01;
        }
        if (positionOther[1] > positionEnCours[1])
        {
            deplacementBoid[1] += deplacementOther[1] + 0.01;
        }
        else if (positionOther[1] < positionEnCours[1])
        {
            deplacementBoid[1] += deplacementOther[1] - 0.01;
        }
        if (positionOther[2] > positionEnCours[2])
        {
            deplacementBoid[2] += deplacementOther[2] + 0.01;
        }
        else if (positionOther[2] < positionEnCours[2])
        {
            deplacementBoid[2] += deplacementOther[2] - 0.01;
        }
    }
    if (ecart < 0.15)
    {
        if (positionChef[0] > positionEnCours[0])
        {
            deplacementBoid[0] += deplacementChef[0] - 0.01;
        }
        else if (positionChef[0] < positionEnCours[0])
        {
            deplacementBoid[0] += deplacementChef[0] + 0.01;
        }
        if (positionChef[1] > positionEnCours[1])
        {
            deplacementBoid[1] += deplacementChef[1] - 0.01;
        }
        else if (positionChef[1] < positionEnCours[1])
        {
            deplacementBoid[1] += deplacementChef[1] + 0.01;
        }
        if (positionChef[2] > positionEnCours[2])
        {
            deplacementBoid[2] += deplacementChef[2] - 0.01;
        }
        else if (positionChef[2] < positionEnCours[2])
        {
            deplacementBoid[2] += deplacementChef[2] + 0.01;
        }
    }
    else if (ecart > 0.2)
    {
        //std::cout<<"ecartAutre : "<<ecartOther<<" ; ecart chef : "<<ecart<<std::endl ;

        if (positionChef[0] > positionEnCours[0])
        {
            deplacementBoid[0] += deplacementChef[0] + 0.01;
        }
        else if (positionChef[0] < positionEnCours[0])
        {
            deplacementBoid[0] += deplacementChef[0] - 0.01;
        }
        if (positionChef[1] > positionEnCours[1])
        {
            deplacementBoid[1] += deplacementChef[1] + 0.01;
        }
        else if (positionChef[1] < positionEnCours[1])
        {
            deplacementBoid[1] += deplacementChef[1] - 0.01;
        }
        if (positionChef[2] > positionEnCours[2])
        {
            deplacementBoid[2] += deplacementChef[2] + 0.01;
        }
        else if (positionChef[2] < positionEnCours[2])
        {
            deplacementBoid[2] += deplacementChef[2] - 0.01;
        }
    }

    else{
        //std::cout<<"ecartAutre : "<<ecartOther<<" ; ecart chef : "<<ecart<<std::endl ;
        deplacementBoid = glm::vec3(abs(deplacementChef[0]), abs(deplacementChef[1]), abs(deplacementChef[2]));
    }

    if((ModelBoids[3][0] + deplacementBoid[0])>0.4 || (ModelBoids[3][0] + deplacementBoid[0])<-0.4){
        deplacementBoid[0] = 0 ;
    }
    if((ModelBoids[3][1] + deplacementBoid[1])>0.2 || (ModelBoids[3][1] + deplacementBoid[1])<-0.4){
        deplacementBoid[1] = 0 ;
    }
    if((ModelBoids[3][2] + deplacementBoid[2])>0.4 || (ModelBoids[3][2] + deplacementBoid[2])<-0.4){
        deplacementBoid[2] = 0 ;
    }
    int testAngle = 1;
    int testAngleTotal = angleBoidTotal + 1;
    angleBoidEnCours = 0;
    float ec = fmod(angleChefTotal - angleBoidTotal + 180, 360) - 180;
    if (ec > 180)
    {
        ec = 360 - ec;
    }
    if (ec < -180)
    {
        ec = 360 + ec;
    }
    if (ec < -20 && abs(angleChefEnCours) > 0.5)
    {
        angleBoidEnCours = -1;
        angleBoidTotal = angleBoidTotal - 1;
    }
    else if (ec > 20 && abs(angleChefEnCours) > 0.5)
    {
        angleBoidEnCours = 1;
        angleBoidTotal = angleBoidTotal + 1;
    }
    if (angleChefEnCours >= 0.05 && angleChefEnCours <= 0.15)
    {
        angleBoidEnCours = 0.1;
        angleBoidTotal += 0.1;
    }
    else if (angleChefEnCours >= -0.15 && angleChefEnCours <= -0.05)
    {
        angleBoidEnCours = -0.1;
        angleBoidTotal += -0.1;
    }
    // if(ec>-20 && ec<20 && abs(angleChefEnCours)>0.5){
    //     std::cout<<"ici"<<std::endl ;

    // }
    int val = (int)angleBoidTotal;
    float reste = angleBoidTotal - val;
    angleBoidTotal = val % 360 + reste;
}

void Boids::generateBuffers(std::vector<glm::vec3> vertices, std::vector<unsigned short> indices, std::vector<glm::vec3> normals, std::vector<glm::vec2> uvs)
{
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

void Boids::draw(GLuint programID, std::string textID, int textNb, int loc1, int loc2, int loc3)
{
    glEnableVertexAttribArray(loc1);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
        loc1,     // attribute
        3,        // size
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        0,        // stride
        (void *)0 // array buffer offset
    );

    glEnableVertexAttribArray(loc2);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glVertexAttribPointer(
        loc2,
        2,
        GL_FLOAT,
        GL_FALSE,
        0,
        (void *)0);

    // Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

    glEnableVertexAttribArray(loc3);
    glBindBuffer(GL_ARRAY_BUFFER, normalsbuffer);
    glVertexAttribPointer(
        loc3,     // attribute
        3,        // size
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        0,        // stride
        (void *)0 // array buffer offset
    );

    glDrawElements(
        GL_TRIANGLES,      // mode
        indices.size(),    // count
        GL_UNSIGNED_SHORT, // type
        (void *)0          // element array buffer offset
    );

    glActiveTexture(GL_TEXTURE0 + textNb);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(programID, textID.c_str()), textNb);
}

void Boids::disableArrays()
{
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}

void Boids::deleteBuffers()
{
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &elementbuffer);
    glDeleteBuffers(1, &normalsbuffer);
}

void Boids::sendMatrixToShader(GLuint programID, glm::mat4 Model, glm::mat4 View, glm::mat4 Projection)
{
    glUniformMatrix4fv(glGetUniformLocation(programID, "ModelMatrix"), 1, GL_FALSE, &Model[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(programID, "ViewMatrix"), 1, GL_FALSE, &View[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(programID, "ProjectionMatrix"), 1, GL_FALSE, &Projection[0][0]);
}