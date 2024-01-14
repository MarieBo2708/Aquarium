// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow *window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

using namespace glm;

#include <common/texture.hpp>
#include <common/sphere.hpp>
#include <common/arcball_camera.hpp>
#include <common/shader.hpp>
#include <common/Water.hpp>
#include <common/transparent_faces.hpp>
#include <common/texture_faces.hpp>
#include <common/water_surface.hpp>
#include <common/Water.hpp>
#include <common/boids.hpp>
#include <common/skybox.hpp>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_pos_callback(GLFWwindow *window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;

glm::vec2 transform_mouse(glm::vec2 in)
{
    return glm::vec2(in.x * 2.f / SCR_WIDTH - 1.f, 1.f - 2.f * in.y / SCR_HEIGHT);
}

// camera
glm::vec3 eye(0, 0, 2);
glm::vec3 center(0);
glm::vec3 up(0, 1, 0);
ArcballCamera camera(eye, center, up);
glm::vec2 prev_mouse(-2.f);
bool left_mouse_button_pressed = false;
bool right_mouse_button_pressed = false;
glm::mat4 proj_inv;

// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

// Matrix
glm::mat4 Model;
glm::mat4 View;
glm::mat4 Projection;

glm::vec3 centre_sphere = glm::vec3(0., 1., 0.);
glm::vec3 lightColor = glm::vec3(1., 1., 1.);
glm::vec3 lightPos = glm::vec3(0., 3., 0.);
glm::vec4 clipPlane = glm::vec4(0., 1., 0., 2.);

glm::vec3 departMouvement = centre_sphere;

bool atteintObjectif = true;
glm::vec3 positionDepart = glm::vec3(0.f, 0.f, 0.f);
glm::vec3 positionObjectif = glm::vec3(0, 0, 0);
float dist;
float angleAutourY = 0.f;
float theta = 0.f;
glm::vec3 deplacement = glm::vec3(1, 0, 0);
int cptTab = 0;
std::vector<float> derniersAngles(10);
bool stab = false;
int valeur;

glm::vec3 deplacementBis = glm::vec3(0, 0, 0);
glm::vec3 deplacementTer = glm::vec3(0, 0, 0);
glm::vec3 position;

int mode;

float angleAutourYboids = 0.f;
float angleAutourYboidsBis = 0.f;
float angleEnCoursAutourY = 0.f;

float angleBoid = 0.f;
float angleBoidBis = 0.f;

transparentFaces transFaces;
GLuint transFacesProgramID;
Sphere sphere;
GLuint sphereProgramID;
Water water;
GLuint waterProgramID;
WaterSurface surface;
GLuint surfaceProgramID;
Boids boid1;
std::string textureBoid1("ecailles1.jpg");
Boids boid2;
std::string textureBoid2("ecailles2.jpg");
Boids boid3;
std::string textureBoid3("ecailles3.jpg");
GLuint boidProgramID;

textureFaces textFaces;
std::string texturePath("carreaux.jpg");
GLuint textFacesProgramID;

int valDeplacement = 0;

Skybox skybox;
std::vector<std::string> skyboxFaces = {"rainbow_lf.png", "rainbow_rt.png",
                                        "rainbow_up.png", "rainbow_dn.png",
                                        "rainbow_ft.png", "rainbow_bk.png"};
GLuint skyboxProgramID;

int main(void)
{
    // Initialise GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow(1024, 768, "TP1 - GLFW", NULL, NULL);
    if (window == NULL)
    {
        fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, mouse_pos_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement
    //  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, 1024 / 2, 768 / 2);

    // Dark blue background
    glClearColor(1.f, 1.f, 1.f, 1.f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Cull triangles which normal is not towards the camera
    glDisable(GL_CULL_FACE);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    transFacesProgramID = LoadShaders("transparent_vs.glsl", "transparent_fs.glsl");
    sphereProgramID = LoadShaders("sphere_vs.glsl", "sphere_fs.glsl");
    waterProgramID = LoadShaders("water_vs.glsl", "water_fs.glsl");
    textFacesProgramID = LoadShaders("texture_vs.glsl", "texture_fs.glsl");
    //surfaceProgramID = LoadShaders("surface_vs.glsl", "surface_fs.glsl");
    boidProgramID = LoadShaders("boids_vs.glsl", "boids_fs.glsl");
    skyboxProgramID = LoadShaders("skybox_vs.glsl", "skybox_fs.glsl");

    GLuint texture = loadTexture2DFromFilePath(texturePath);

    // For speed computation
    double lastTime = glfwGetTime();
    int nbFrames = 0;

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_BLEND);

    glDepthFunc(GL_LESS);

    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    sphere = Sphere(0.1, 50, 50, centre_sphere, waterProgramID);
    sphere.generateSphereBuffers(sphere.vertices, sphere.indices, sphere.vertices_normals);

    water = Water();
    water.createSurface() ;
    water.generateWaterBuffers(water.vertices, water.indices, water.normals);
    water.generateWaterBuffersSurface(water.surfaceVertices, water.surfaceIndices, water.surfaceNormals, water.surfaceUVs);

    transFaces = transparentFaces();
    transFaces.generateBuffers(transFaces.vertices, transFaces.indices, transFaces.normals);

    textFaces = textureFaces();
    textFaces.generateBuffers(textFaces.vertices, textFaces.indices, textFaces.normals, textFaces.uvs);

    // surface = WaterSurface();
    // surface.generateBuffers(surface.vertices, surface.indices, surface.normals, surface.uvs);

    boid1 = Boids("fish.obj", textureBoid1.c_str());
    boid1.ModelBoids = glm::mat4(1.);
    boid1.ModelBoids = glm::scale(boid1.ModelBoids, glm::vec3(0.1, 0.1, 0.1));
    boid1.generateBuffers(boid1.vertices, boid1.indices, boid1.normals, boid1.uvs);

    boid2 = Boids("fish.obj", textureBoid2.c_str());
    boid2.ModelBoids = glm::mat4(1.);
    boid2.ModelBoids = glm::scale(boid2.ModelBoids, glm::vec3(0.1, 0.1, 0.1));
    boid2.ModelBoids = glm::translate(boid2.ModelBoids, glm::vec3(-4, -4, -4));
    boid2.generateBuffers(boid2.vertices, boid2.indices, boid2.normals, boid2.uvs);

    boid3 = Boids("fish.obj", textureBoid3.c_str());
    boid3.ModelBoids = glm::mat4(1.);
    boid3.ModelBoids = glm::scale(boid2.ModelBoids, glm::vec3(0.1, 0.1, 0.1));
    boid3.ModelBoids = glm::translate(boid2.ModelBoids, glm::vec3(4, 4, 4));
    boid3.generateBuffers(boid3.vertices, boid3.indices, boid3.normals, boid3.uvs);

    skybox = Skybox(skyboxFaces, 8);
    skybox.generateBuffers(skybox.vertices, skybox.indices);

    unsigned int reflexionframebuffer;
    glGenFramebuffers(1, &reflexionframebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, reflexionframebuffer);

    unsigned int reflectiontexture;
    glGenTextures(1, &reflectiontexture);
    glBindTexture(GL_TEXTURE_2D, reflectiontexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 900, 700, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, reflectiontexture, 0);

    unsigned int reflexionrbo;
    glGenRenderbuffers(1, &reflexionrbo);
    glBindRenderbuffer(GL_RENDERBUFFER, reflexionrbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 900, 700); // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, reflexionrbo); // now actually attach it
    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now

    unsigned int refractionframebuffer;
    glGenFramebuffers(1, &refractionframebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, refractionframebuffer);

    unsigned int refractiontexture;
    glGenTextures(1, &refractiontexture);
    glBindTexture(GL_TEXTURE_2D, refractiontexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 900, 700, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, refractiontexture, 0);

    unsigned int refractionrbo;
    glGenRenderbuffers(1, &refractionrbo);
    glBindRenderbuffer(GL_RENDERBUFFER, refractionrbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 900, 700); // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, refractionrbo); // now actually attach it
    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now


    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    bool sphereWasIntersectingWater = false;

   

    do
    {
        // Measure speed
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);
        if (glm::length(centre_sphere - glm::vec3(boid1.ModelBoids[3][0], boid1.ModelBoids[3][1], boid1.ModelBoids[3][2])) < 0.5)
        {
            boid1.peur(sphere, deplacement, theta, angleAutourY, boid2, boid3);
            for(int i = 0 ; i<10 ; i++){
                derniersAngles[i] = 0 ;
            }
        }
        else if (glm::length(centre_sphere - glm::vec3(boid1.ModelBoids[3][0], boid1.ModelBoids[3][1], boid1.ModelBoids[3][2])) >= 0.5)
        {
            boid1.deplacementChef(sphere, atteintObjectif, positionDepart, positionObjectif, dist, deltaTime, angleAutourY, theta, deplacement, cptTab, derniersAngles, stab, valeur);
        }
        boid1.ModelBoids = glm::rotate(boid1.ModelBoids, -glm::radians(theta), glm::vec3(0.0f, 1.0f, 0.0f));
        boid1.ModelBoids = glm::rotate(boid1.ModelBoids, glm::radians(angleAutourY), glm::vec3(0.0f, 1.0f, 0.0f));
        boid1.ModelBoids = glm::translate(boid1.ModelBoids, glm::vec3(deplacement[0], deplacement[1], deplacement[2]));
        boid1.ModelBoids = glm::rotate(boid1.ModelBoids, -glm::radians(angleAutourY), glm::vec3(0.0f, 1.0f, 0.0f));

        if (glm::length(centre_sphere - glm::vec3(boid2.ModelBoids[3][0], boid2.ModelBoids[3][1], boid2.ModelBoids[3][2])) < 0.5)
        {
            boid2.peur(sphere, deplacementBis, angleBoid, angleAutourYboids, boid1, boid3);
        }
        else if (glm::length(centre_sphere - glm::vec3(boid2.ModelBoids[3][0], boid2.ModelBoids[3][1], boid2.ModelBoids[3][2])) >= 0.5)
        {
            boid2.deplacementAutre(boid3, deplacementTer, deplacement, glm::vec3(boid1.ModelBoids[3][0], boid1.ModelBoids[3][1], boid1.ModelBoids[3][2]), angleAutourY, theta, deplacementBis, angleAutourYboids, angleBoid);
        }
        boid2.ModelBoids = glm::rotate(boid2.ModelBoids, -glm::radians(angleBoid), glm::vec3(0.0f, 1.0f, 0.0f));
        boid2.ModelBoids = glm::rotate(boid2.ModelBoids, glm::radians(angleAutourYboids), glm::vec3(0.0f, 1.0f, 0.0f));
        boid2.ModelBoids = glm::translate(boid2.ModelBoids, deplacementBis);
        boid2.ModelBoids = glm::rotate(boid2.ModelBoids, -glm::radians(angleAutourYboids), glm::vec3(0.0f, 1.0f, 0.0f));

        if (glm::length(centre_sphere - glm::vec3(boid3.ModelBoids[3][0], boid3.ModelBoids[3][1], boid3.ModelBoids[3][2])) < 0.5)
        {
            boid3.peur(sphere, deplacementTer, angleBoidBis, angleAutourYboidsBis, boid2, boid3);
        }
        else if (glm::length(centre_sphere - glm::vec3(boid3.ModelBoids[3][0], boid3.ModelBoids[3][1], boid3.ModelBoids[3][2])) >= 0.5)
        {
            boid3.deplacementAutre(boid2, deplacementBis, deplacement, glm::vec3(boid1.ModelBoids[3][0], boid1.ModelBoids[3][1], boid1.ModelBoids[3][2]), angleAutourY, theta, deplacementTer, angleAutourYboidsBis, angleBoidBis);
        }

        boid3.ModelBoids = glm::rotate(boid3.ModelBoids, -glm::radians(angleBoidBis), glm::vec3(0.0f, 1.0f, 0.0f));
        boid3.ModelBoids = glm::rotate(boid3.ModelBoids, glm::radians(angleAutourYboidsBis), glm::vec3(0.0f, 1.0f, 0.0f));
        boid3.ModelBoids = glm::translate(boid3.ModelBoids, deplacementTer);
        boid3.ModelBoids = glm::rotate(boid3.ModelBoids, -glm::radians(angleAutourYboidsBis), glm::vec3(0.0f, 1.0f, 0.0f));

        // Model = glm::mat4(1.0);
        // Projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        // View = glm::lookAt(camera.eye(), camera.dir(), camera.up());
        // proj_inv = glm::inverse(Projection);

        glBindFramebuffer(GL_FRAMEBUFFER, reflexionframebuffer);
        glEnable(GL_DEPTH_TEST);
        glClearColor(1.f, 1.f, 1.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        // glEnable(GL_BLEND);

        eye = glm::vec3(0., -0.1, 0.);

        Model = glm::mat4(1.0);
        Projection = glm::perspective(glm::radians(90.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        View = glm::lookAt(eye, glm::vec3(0., 2., 0.), camera.up());
        proj_inv = glm::inverse(Projection); 
     
        glUseProgram(skyboxProgramID);
        skybox.sendMatrixToShader(skyboxProgramID, Model, View, Projection);
        skybox.draw(skyboxProgramID);
        
        glUseProgram(textFacesProgramID);
        textFaces.sendMatrixToShader(textFacesProgramID, Model, View, Projection);
        textFaces.drawFaces(textFacesProgramID, texture, false, clipPlane);
        textFaces.sendInfosToShader(textFacesProgramID, camera.eye(), lightPos, lightColor);

        glUseProgram(boidProgramID);
        mode = 1;
        glUniform1i(glGetUniformLocation(boidProgramID, "mode"), mode);

        glUseProgram(sphereProgramID);
        sphere.sendMatrixToShader(sphereProgramID, Model, View, Projection);
        sphere.drawSphere(sphereProgramID, false, clipPlane);
        sphere.sendInfosToShader(sphereProgramID, -camera.eye(), lightPos, lightColor);

        glUseProgram(transFacesProgramID);
        transFaces.sendMatrixToShader(transFacesProgramID, Model, View, Projection);
        transFaces.drawFaces(transFacesProgramID, false, clipPlane);
        transFaces.sendInfosToShader(transFacesProgramID, camera.eye(), lightPos, lightColor);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f); 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //END REFLECTION FRAME BUFFER

        //REFRACTION FRAME BUFFER
        glBindFramebuffer(GL_FRAMEBUFFER, refractionframebuffer);
        glEnable(GL_DEPTH_TEST);
        glClearColor(1.f, 1.f, 1.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        // glEnable(GL_BLEND); 

        eye = glm::vec3(0., 0.3, 0.);

        Model = glm::mat4(1.0);
        Projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        View = glm::lookAt(eye, glm::vec3(0., -2., 0.), camera.up());
        proj_inv = glm::inverse(Projection); 

        glUseProgram(textFacesProgramID);
        textFaces.sendMatrixToShader(textFacesProgramID, Model, View, Projection);
        textFaces.drawFaces(textFacesProgramID, texture, false, clipPlane);
        textFaces.sendInfosToShader(textFacesProgramID, camera.eye(), lightPos, lightColor);

        glUseProgram(boidProgramID);
        mode = 1;
        glUniform1i(glGetUniformLocation(boidProgramID, "mode"), mode);
        if (mode == 1)
        {
            boid1.sendMatrixToShader(boidProgramID, boid1.ModelBoids, View, Projection);
            boid1.draw(boidProgramID, "texture_boid_1", 3, 0, 1, 2);
        }

        mode = 2;
        glUniform1i(glGetUniformLocation(boidProgramID, "mode"), mode);
        if (mode == 2)
        {
            boid2.sendMatrixToShader(boidProgramID, boid2.ModelBoids, View, Projection);
            boid2.draw(boidProgramID, "texture_boid_2", 4, 3, 4, 5);
        }

        mode = 3;
        glUniform1i(glGetUniformLocation(boidProgramID, "mode"), mode);
        if (mode == 3)
        {
            boid3.sendMatrixToShader(boidProgramID, boid3.ModelBoids, View, Projection);
            boid3.draw(boidProgramID, "texture_boid_3", 5, 6, 7, 8);
        }

        glUseProgram(sphereProgramID);
        sphere.sendMatrixToShader(sphereProgramID, Model, View, Projection);
        sphere.drawSphere(sphereProgramID, false, clipPlane);
        sphere.sendInfosToShader(sphereProgramID, camera.eye(), lightPos, lightColor);

        glUseProgram(waterProgramID);
        water.sendMatrixToShader(waterProgramID, Model, View, Projection);
        water.draw(false, sphere, departMouvement, valDeplacement, sphereWasIntersectingWater, waterProgramID, currentFrame, false, clipPlane, reflectiontexture, refractiontexture, camera.eye(), lightPos, lightColor);

        glUseProgram(transFacesProgramID);
        transFaces.sendMatrixToShader(transFacesProgramID, Model, View, Projection);
        transFaces.drawFaces(transFacesProgramID, false, clipPlane);
        transFaces.sendInfosToShader(transFacesProgramID, camera.eye(), lightPos, lightColor);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f); 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //END REFRACTION FRAME BUFFER

        camera.eye() = eye;

        Model = glm::mat4(1.0);
        Projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        View = glm::lookAt(camera.eye(), camera.dir(), camera.up());
        proj_inv = glm::inverse(Projection);

        glUseProgram(skyboxProgramID);
        skybox.sendMatrixToShader(skyboxProgramID, Model, View, Projection);
        skybox.draw(skyboxProgramID);

        glUseProgram(textFacesProgramID);
        textFaces.sendMatrixToShader(textFacesProgramID, Model, View, Projection);
        textFaces.drawFaces(textFacesProgramID, texture, false, clipPlane);
        textFaces.sendInfosToShader(textFacesProgramID, camera.eye(), lightPos, lightColor);

        glUseProgram(boidProgramID);
        mode = 1;
        glUniform1i(glGetUniformLocation(boidProgramID, "mode"), mode);
        if (mode == 1)
        {
            boid1.sendMatrixToShader(boidProgramID, boid1.ModelBoids, View, Projection);
            boid1.draw(boidProgramID, "texture_boid_1", 3, 0, 1, 2);
        }

        mode = 2;
        glUniform1i(glGetUniformLocation(boidProgramID, "mode"), mode);
        if (mode == 2)
        {
            boid2.sendMatrixToShader(boidProgramID, boid2.ModelBoids, View, Projection);
            boid2.draw(boidProgramID, "texture_boid_2", 4, 3, 4, 5);
        }

        mode = 3;
        glUniform1i(glGetUniformLocation(boidProgramID, "mode"), mode);
        if (mode == 3)
        {
            boid3.sendMatrixToShader(boidProgramID, boid3.ModelBoids, View, Projection);
            boid3.draw(boidProgramID, "texture_boid_3", 5, 6, 7, 8);
        }
        

        glUseProgram(sphereProgramID);
        sphere.sendMatrixToShader(sphereProgramID, Model, View, Projection);
        sphere.drawSphere(sphereProgramID, false, clipPlane);
        sphere.sendInfosToShader(sphereProgramID, camera.eye(), lightPos, lightColor);

        glUseProgram(waterProgramID);
        water.sendMatrixToShader(waterProgramID, Model, View, Projection);
        water.draw(true, sphere, departMouvement, valDeplacement, sphereWasIntersectingWater, waterProgramID, currentFrame, false, clipPlane, reflectiontexture, refractiontexture, camera.eye(), lightPos, lightColor);

        glUseProgram(transFacesProgramID);
        transFaces.sendMatrixToShader(transFacesProgramID, Model, View, Projection);
        transFaces.drawFaces(transFacesProgramID, false, clipPlane);
        transFaces.sendInfosToShader(transFacesProgramID, camera.eye(), lightPos, lightColor);

        transFaces.disableArrays();
        sphere.disableArrays();
        water.disableArrays();
        textFaces.disableArrays();
        boid1.disableArrays();
        boid2.disableArrays();
        boid3.disableArrays();

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);

    glDeleteVertexArrays(1, &VertexArrayID);

    glDeleteProgram(sphereProgramID);
    glDeleteProgram(transFacesProgramID);
    glDeleteProgram(waterProgramID);
    glDeleteProgram(textFacesProgramID);
    glDeleteProgram(boidProgramID);

    transFaces.deleteBuffers();
    sphere.deleteBuffers();
    water.deleteBuffers();
    textFaces.deleteBuffers();
    boid1.deleteBuffers();
    boid2.deleteBuffers();
    boid3.deleteBuffers();

    glDeleteFramebuffers(1, &reflexionframebuffer);
    glDeleteRenderbuffers(1, &reflexionrbo);
    glDeleteTextures(1, &reflectiontexture);

    glDeleteFramebuffers(1, &refractionframebuffer);
    glDeleteRenderbuffers(1, &refractionrbo);
    glDeleteTextures(1, &refractiontexture);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}

void processInput(GLFWwindow *window)
{

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        if (valDeplacement != 0)
        {
            departMouvement = centre_sphere;
        }

        valDeplacement = 0;
        glBindBuffer(GL_ARRAY_BUFFER, sphere.vertexbuffersphere);
        if (centre_sphere.y <= 1)
            centre_sphere = glm::vec3(centre_sphere.x, centre_sphere.y + 0.01, centre_sphere.z);
        sphere.centre_sphere = centre_sphere;
        sphere.createSphereData(50, 50);
        glBufferData(GL_ARRAY_BUFFER, sphere.vertices.size() * sizeof(glm::vec3), &sphere.vertices[0], GL_STATIC_DRAW);
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        if (valDeplacement != 5)
        {
            departMouvement = centre_sphere;
        }
        valDeplacement = 5;
        glBindBuffer(GL_ARRAY_BUFFER, sphere.vertexbuffersphere);
        if (centre_sphere.y > water.center.y - water.height / 2 + sphere.radius_sphere + 0.01)
            centre_sphere = glm::vec3(centre_sphere.x, centre_sphere.y - 0.01, centre_sphere.z);
        sphere.centre_sphere = centre_sphere;
        sphere.createSphereData(50, 50);
        glBufferData(GL_ARRAY_BUFFER, sphere.vertices.size() * sizeof(glm::vec3), &sphere.vertices[0], GL_STATIC_DRAW);
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        if (valDeplacement != 1)
        {
            departMouvement = centre_sphere;
        }
        valDeplacement = 1;
        glBindBuffer(GL_ARRAY_BUFFER, sphere.vertexbuffersphere);
        if (centre_sphere.x < water.center.x + water.width / 2 - sphere.radius_sphere - 0.01)
            centre_sphere = glm::vec3(centre_sphere.x + 0.01, centre_sphere.y, centre_sphere.z);
        sphere.centre_sphere = centre_sphere;
        sphere.createSphereData(50, 50);
        glBufferData(GL_ARRAY_BUFFER, sphere.vertices.size() * sizeof(glm::vec3), &sphere.vertices[0], GL_STATIC_DRAW);
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        if (valDeplacement != 2)
        {
            departMouvement = centre_sphere;
        }
        valDeplacement = 2;
        glBindBuffer(GL_ARRAY_BUFFER, sphere.vertexbuffersphere);
        if (centre_sphere.x > water.center.x - water.width / 2 + sphere.radius_sphere + 0.01)
            centre_sphere = glm::vec3(centre_sphere.x - 0.01, centre_sphere.y, centre_sphere.z);
        sphere.centre_sphere = centre_sphere;
        sphere.createSphereData(50, 50);
        glBufferData(GL_ARRAY_BUFFER, sphere.vertices.size() * sizeof(glm::vec3), &sphere.vertices[0], GL_STATIC_DRAW);
    }

    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
    {
        if (valDeplacement != 3)
        {
            departMouvement = centre_sphere;
        }
        valDeplacement = 3;
        glBindBuffer(GL_ARRAY_BUFFER, sphere.vertexbuffersphere);
        if (centre_sphere.z > water.center.z - water.depth / 2 + sphere.radius_sphere + 0.01)
            centre_sphere = glm::vec3(centre_sphere.x, centre_sphere.y, centre_sphere.z - 0.01);
        sphere.centre_sphere = centre_sphere;
        sphere.createSphereData(50, 50);
        glBufferData(GL_ARRAY_BUFFER, sphere.vertices.size() * sizeof(glm::vec3), &sphere.vertices[0], GL_STATIC_DRAW);
    }

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        if (valDeplacement != 4)
        {
            departMouvement = centre_sphere;
        }
        valDeplacement = 4;
        glBindBuffer(GL_ARRAY_BUFFER, sphere.vertexbuffersphere);
        if (centre_sphere.z < water.center.z + water.depth / 2 - sphere.radius_sphere - 0.01)
            centre_sphere = glm::vec3(centre_sphere.x, centre_sphere.y, centre_sphere.z + 0.01);
        sphere.centre_sphere = centre_sphere;
        sphere.createSphereData(50, 50);
        glBufferData(GL_ARRAY_BUFFER, sphere.vertices.size() * sizeof(glm::vec3), &sphere.vertices[0], GL_STATIC_DRAW);
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (action == GLFW_PRESS)
        {
            left_mouse_button_pressed = true;
        }
        else if (action == GLFW_RELEASE)
        {
            left_mouse_button_pressed = false;
            prev_mouse = glm::vec2(-2.f); // Réinitialiser la position précédente
        }
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        if (action == GLFW_PRESS)
        {
            right_mouse_button_pressed = true;
        }
        else if (action == GLFW_RELEASE)
        {
            right_mouse_button_pressed = false;
            prev_mouse = glm::vec2(-2.f); // Réinitialiser la position précédente
        }
    }
}
// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_pos_callback(GLFWwindow *window, double xpos, double ypos)
{
    const glm::vec2 cur_mouse = transform_mouse(glm::vec2(static_cast<float>(xpos), static_cast<float>(ypos)));
    if (prev_mouse != glm::vec2(-2.f))
    {
        if (left_mouse_button_pressed)
        {
            camera.rotate(prev_mouse, cur_mouse);
        }
        else if (right_mouse_button_pressed)
        {
            glm::vec2 dxy = cur_mouse - prev_mouse;
            glm::vec4 dxy4 = proj_inv * glm::vec4(dxy.x, dxy.y, 0, 1);
            camera.pan(glm::vec2(dxy4.x, dxy4.y));
        }
    }
    prev_mouse = cur_mouse;
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.zoom(static_cast<float>(yoffset) * 0.1f);
}
