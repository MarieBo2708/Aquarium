#ifndef TEXTURE_HPP
#define TEXTURE_HPP

// Load a .BMP file using our custom loader
GLuint loadBMP_custom(const char * imagepath);

//// Since GLFW 3, glfwLoadTexture2D() has been removed. You have to use another texture loading library, 
//// or do it yourself (just like loadBMP_custom and loadDDS)
//// Load a .TGA file using GLFW's own loader
//GLuint loadTGA_glfw(const char * imagepath);

// Load a .DDS file using GLFW's own loader
GLuint loadDDS(const char * imagepath);

std::vector<glm::vec2> compute_uv(std::vector<glm::vec3> vertices);

GLuint loadCubeTexture(std::vector<std::string> faces);

GLuint loadTextureFromFilePath(const std::string &path, const std::string directory);

GLuint loadTexture2DFromFilePath(const std::string &path);

void setDefaultTexture2DParameters(GLuint texture);

GLuint loadSkybox(std::vector<std::string> faces);

#endif