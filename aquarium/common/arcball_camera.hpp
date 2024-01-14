#ifndef ARCBALL_HPP
#define ARCBALL_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <GL/glew.h>
#include <glm/ext.hpp>

class ArcballCamera {
    glm::mat4 center_translation, translation;
    glm::quat rotation;
    glm::mat4 camera, inv_camera;

public:

    ArcballCamera(const glm::vec3 &eye, const glm::vec3 &center, const glm::vec3 &up);
    void rotate(glm::vec2 prev_mouse, glm::vec2 cur_mouse);    
    void pan(glm::vec2 mouse_delta);
    void zoom(const float zoom_amount);
    const glm::mat4 &transform() const;
    const glm::mat4 &inv_transform() const;
    glm::vec3 eye() const;
    glm::vec3 dir() const;
    glm::vec3 up() const;

private:
    void update_camera();
};



#endif