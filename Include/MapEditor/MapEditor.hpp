#pragma once

#include <glad/glad.h>

#include "../Shader/Shader.hpp"
#include "../Texture/Texture.hpp"

#include "../Memory/VBO/VBO.hpp"
#include "../Memory/VAO/VAO.hpp"
#include "../Data/Data.hpp"

#include "../Collision/Collision.hpp"

#include "../Camera/Camera2D.hpp"

#include <iostream>

class MapEditor{
  public:
    MapEditor(GLFWwindow *window,int nRows,int nCols);
    ~MapEditor()=default;

    glm::mat4& getViewMatrix();
    glm::mat4& getProjectionMatrix();

    void renderMapEditor(float dt,glm::vec3& cursorPos);

  private:
    GLFWwindow *m_window = nullptr;

    VBO m_vbo;
    VAO m_vao;
  
    Camera2D m_camera;
    Texture m_gridTexture;
    Shader m_gridShader;

    int m_nRows;
    int m_nCols;
    
    std::vector<std::vector<Tile>> m_grid; 
};
