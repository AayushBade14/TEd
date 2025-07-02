#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../Shader/Shader.hpp"
#include "../Texture/Texture.hpp"

#include "../Memory/VBO/VBO.hpp"
#include "../Memory/VAO/VAO.hpp"
#include "../Data/Data.hpp"

#include "../Camera/Camera2D.hpp"
#include "../Collision/Collision.hpp"

struct Tile{
  Rectangle r;
  bool is_clicked = false;
  float uv_x;
  float uv_y;
  int row;
  int col;
};

class TileManager{
  public:
    TileManager(GLFWwindow *window,const std::string& texAtlasPath,int nRows,int nCols,float tW,float tH,float nxf,float nyf);
    ~TileManager()=default;
    
    glm::mat4& getViewMatrix();
    glm::mat4& getProjectionMatrix();

    void renderTiles(float dt,float uv_x,float uv_y,glm::vec3& cursorPos);


  private:
    GLFWwindow *m_window;
    Camera2D m_camera;
    
    std::vector<std::vector<Tile>> m_grid;

    VBO m_vbo;
    VAO m_vao;

    Texture m_atlasTexture;
    Texture m_gridTexture;
    Shader m_tileShader;

    int m_nRows;
    int m_nCols;

    float m_tileWidth;
    float m_tileHeight;
    float nxf;
    float nyf;
    
    void handleCollisions(float uv_x,float uv_y,glm::vec3& cursorPos);

};

