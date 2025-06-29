#pragma once

#include "../../Shader/Shader.hpp"
#include "../../Texture/Texture.hpp"
#include "../../Collision/Collision.hpp"
#include "../../Data/Data.hpp"
#include "../../Memory/VBO/VBO.hpp"
#include "../../Memory/VAO/VAO.hpp"

#include <GLFW/glfw3.h>

class Button{
  public:
    Button(const std::string &texPath,glm::vec3 &origin,glm::vec3 &size);
    ~Button()=default;
    
    void render(GLFWwindow *window,Shader &shader,glm::vec3 &cursorPos);

  private:
    Rectangle buttonRect;
    
    unsigned int texture;
    unsigned int vbo;
    unsigned int vao;

    bool isHovered = false;
    bool isActive = false;
};
