#include "./TextureAtlas.hpp"

TextureAtlas::TextureAtlas(GLFWwindow *window,float atlasWidth,float atlasHeight,int nRows,int nCols) : m_camera(window,100.0f),
    m_vbo(verticesPlane,GL_STATIC_DRAW),
    m_vao(),
    m_atlasTexture("./Assets/Tileset.png"),
    m_selectionTexture("./Assets/selectionBox.png"),
    m_atlasShader("./Include/TextureAtlas/Shaders/atlasVert.glsl","./Include/TextureAtlas/Shaders/atlasFrag.glsl"),
    m_selectionShader("./Include/TextureAtlas/Shaders/atlasVert.glsl","./Include/TextureAtlas/Shaders/selectionFrag.glsl")
{
  
  this->m_window = window;
  this->m_atlasWidth = atlasWidth;
  this->m_atlasHeight = atlasHeight;
  this->m_nRows = nRows;
  this->m_nCols = nCols;

  m_tileWidth = m_atlasWidth/m_nCols;
  m_tileHeight = m_atlasHeight/m_nRows;

  m_atlasTexture.setSamplerValue(m_atlasShader,"atlas",0);
  m_selectionTexture.setSamplerValue(m_selectionShader,"selectionBox",0);
  
  m_vao.setAttribPointer(m_vbo,0,3,8,0);
  m_vao.setAttribPointer(m_vbo,1,2,8,6);

  m_collisionTiles.resize(m_nRows);
  for(auto& row : m_collisionTiles)
    row.resize(m_nCols);

  for(int y = 0;y < nRows;y++){
    for(int x = 0;x < nCols;x++){
      Rectangle r;
      r.origin = glm::vec3(
        x*m_tileWidth,
        (nRows-y-1)*m_tileHeight,
        0.0f
      );
      r.size = glm::vec3(
        m_tileWidth,
        m_tileHeight,
        0.0f
      );

      m_collisionTiles[y][x] = r;
    }
  }
}

void TextureAtlas::handleCollisions(glm::vec3& cursorPos){
  if(glfwGetMouseButton(m_window,GLFW_MOUSE_BUTTON_LEFT)==GLFW_PRESS){
    glm::vec3 clickPos = cursorPos;
    for(int y = 0; y < m_nRows;y++){
      for(int x = 0;x < m_nCols;x++){
        if(point_rect_collide(clickPos,m_collisionTiles[y][x])){
          m_selectedRow = y;
          m_selectedCol = x;
        }
      }
    }
  }
}

void TextureAtlas::renderTextureAtlas(float dt,glm::vec3& cursorPos){
  m_camera.update(dt);
  handleCollisions(cursorPos); 
  
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::scale(model,glm::vec3(1920.0f,1080.0f,0.0f));
  glm::mat4 view = m_camera.getViewMatrix();
  glm::mat4 projection = m_camera.getProjectionMatrix();

  m_atlasTexture.assignTextureUnit(0);
  m_atlasShader.use();
  m_atlasShader.setValue("model",model);
  m_atlasShader.setValue("view",view);
  m_atlasShader.setValue("projection",projection);
  m_vao.bind();
  glDrawArrays(GL_TRIANGLES,0,6);
  m_vao.unbind();
  m_atlasTexture.unbind();
  
  model = glm::mat4(1.0f);
  model = glm::translate(model,m_collisionTiles[m_selectedRow][m_selectedCol].origin);
  model = glm::scale(model,m_collisionTiles[m_selectedRow][m_selectedCol].size);
  m_selectionTexture.assignTextureUnit(0);
  m_selectionShader.use();
  m_selectionShader.setValue("model",model);
  m_selectionShader.setValue("view",view);
  m_selectionShader.setValue("projection",projection);
  m_vao.bind();
  glDrawArrays(GL_TRIANGLES,0,6);
  m_vao.unbind();
  m_selectionTexture.unbind();
}

glm::mat4& TextureAtlas::getViewMatrix(){
  return m_camera.getViewMatrix();
}

glm::mat4& TextureAtlas::getProjectionMatrix(){
  return m_camera.getProjectionMatrix();
}
