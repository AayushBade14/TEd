#pragma once

#include <glad/glad.h>

#include "../Shader/Shader.hpp"
#include "../Texture/Texture.hpp"

#include "../Memory/VBO/VBO.hpp"
#include "../Memory/VAO/VAO.hpp"
#include "../Data/Data.hpp"

#include "../Collision/Collision.hpp"

#include "../Camera/Camera2D.hpp"

class TextureAtlas{
  public:
    TextureAtlas(GLFWwindow *window,float atlasWidth,float atlasHeight,int nRows,int nCols);
    ~TextureAtlas()=default;
    
    glm::mat4& getViewMatrix();
    glm::mat4& getProjectionMatrix();

    void renderTextureAtlas(float dt,glm::vec3& cursorPos,float& uv_x,float& uv_y);

  private:
    GLFWwindow *m_window = nullptr;
    
    VBO m_vbo;
    VAO m_vao;
    
    Camera2D m_camera;
    Texture m_atlasTexture;
    Texture m_selectionTexture;
    Shader m_atlasShader;
    Shader m_selectionShader;

    float m_atlasWidth;
    float m_atlasHeight;
    int m_nRows;
    int m_nCols;

    float m_tileWidth;
    float m_tileHeight;

    std::vector<std::vector<Rectangle>> m_collisionTiles;

    int m_selectedRow = 0;
    int m_selectedCol = 0;

    void handleCollisions(glm::vec3& cursorPos);
};
