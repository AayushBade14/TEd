#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

class Camera2D{
  public:
    Camera2D(GLFWwindow *window,float camSpeed);
    ~Camera2D()=default;

    glm::vec3& getCameraPosition();
    glm::mat4& getViewMatrix();
    glm::mat4& getProjectionMatrix();

    void update(float dt);
  private:
    GLFWwindow *m_window = nullptr;
    int m_screenWidth;
    int m_screenHeight;

    glm::vec3 m_cameraPosition;
    float m_cameraSpeed;
    
    glm::mat4 m_viewMatrix = glm::mat4(1.0f);
    glm::mat4 m_projectionMatrix = glm::mat4(1.0f);

    void handleInput(float dt);
};
