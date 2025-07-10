#include "./TileManager.hpp"

TileManager::TileManager(GLFWwindow *window,const std::string& texAtlasPath,int nRows,int nCols,float tW,float tH,float nxf,float nyf):
m_camera(window,100.0f),
m_atlasTexture(texAtlasPath),
m_gridTexture("./Assets/grid.png"),
m_vbo(verticesPlane,GL_STATIC_DRAW),
m_vao(),
m_tileShader("./Include/TileManager/Shaders/tileVert.glsl","./Include/TileManager/Shaders/tileFrag.glsl"),
m_ctileShader("./Include/TileManager/Shaders/tileVert.glsl","./Include/TileManager/Shaders/ctileFrag.glsl")
{
  m_window = window;
  m_nRows = nRows;
  m_nCols = nCols;
  m_tileWidth = tW;
  m_tileHeight = tH;
  this->nxf = nxf;
  this->nyf = nyf;
  
  m_atlasTexture.setSamplerValue(m_tileShader,"atlasTex",0);
  m_gridTexture.setSamplerValue(m_tileShader,"gridTex",1);

  m_vao.setAttribPointer(m_vbo,0,3,8,0);
  m_vao.setAttribPointer(m_vbo,1,2,8,6);

  m_grid.resize(m_nRows);
  for(auto& row : m_grid)
    row.resize(m_nCols);

  for(int y = 0;y < m_nRows;y++){
    for(int x = 0;x < m_nCols;x++){
      Tile tile;

      tile.r.origin = glm::vec3(
        x*m_tileWidth,
        (m_nRows-y-1)*m_tileHeight,
        0.0f
      );

      tile.r.size = glm::vec3(
        m_tileWidth,
        m_tileHeight,
        0.0f
      );

      tile.row = y;
      tile.col = x;

      m_grid[y][x] = tile;
    }
  }

  m_collision_grid.resize(m_nRows);
  for(auto& row : m_collision_grid)
    row.resize(m_nCols);

  for(int y = 0;y < m_nRows;y++){
    for(int x = 0;x < m_nCols;x++){
      CollisionTile ct;
      
      ct.r.origin = glm::vec3(
        x*m_tileWidth,
        (m_nRows-y-1)*m_tileHeight,
        0.3f
      );

      ct.r.size = glm::vec3(
        m_tileWidth,
        m_tileHeight,
        0.0f
      );
      
      ct.row = y;
      ct.col = x;

      m_collision_grid[y][x] = ct;

    }
  }
}

void TileManager::renderTiles(float dt,float uv_x,float uv_y,glm::vec3& cursorPos,std::vector<std::pair<int,int>>& animTiles){
  m_camera.update(dt);
  handleCollisions(uv_x,uv_y,cursorPos,animTiles);
  handleInputs(); 
  if(is_collision_layer){
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
  }
  else{
    glDisable(GL_BLEND);
  }
  
  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 view = m_camera.getViewMatrix();
  glm::mat4 projection = m_camera.getProjectionMatrix();
  
  m_atlasTexture.assignTextureUnit(0);
  m_gridTexture.assignTextureUnit(1);

  m_tileShader.use();
  for(int y = 0;y < m_nRows;y++){
    for(int x = 0;x < m_nCols;x++){
      
      if(!m_grid[y][x].is_animated){
        model = glm::mat4(1.0f);
        model = glm::translate(model,m_grid[y][x].r.origin);
        model = glm::scale(model,m_grid[y][x].r.size);
      
        m_tileShader.setValue("model",model);
        m_tileShader.setValue("view",view);
        m_tileShader.setValue("projection",projection);
        m_tileShader.setValue("is_clicked",m_grid[y][x].is_clicked);
        m_tileShader.setValue("uv_x",m_grid[y][x].uv_x);
        m_tileShader.setValue("uv_y",m_grid[y][x].uv_y);
        m_tileShader.setValue("nxf",nxf);
        m_tileShader.setValue("nyf",nyf);

        m_vao.bind();
        glDrawArrays(GL_TRIANGLES,0,6);
        m_vao.unbind();
      }
      else{
        static float animTime = 0.0f;
        static float animThresh = 0.01f;
        unsigned int ind = m_grid[y][x].animIndex;
        unsigned int maxind = m_grid[y][x].uv.size();
        animTime += dt;
        if(animTime >= animThresh){
          m_grid[y][x].animIndex += 1;
          animTime = 0.0f;
        }        
        model = glm::mat4(1.0f);
        model = glm::translate(model,m_grid[y][x].r.origin);
        model = glm::scale(model,m_grid[y][x].r.size);
      
        m_tileShader.setValue("model",model);
        m_tileShader.setValue("view",view);
        m_tileShader.setValue("projection",projection);
        m_tileShader.setValue("is_clicked",m_grid[y][x].is_clicked);
        m_tileShader.setValue("uv_x",m_grid[y][x].uv[(ind)%maxind].x);
        m_tileShader.setValue("uv_y",m_grid[y][x].uv[ind%maxind].y);
        m_tileShader.setValue("nxf",nxf);
        m_tileShader.setValue("nyf",nyf);

        m_vao.bind();
        glDrawArrays(GL_TRIANGLES,0,6);
        m_vao.unbind();

      }

    }
  }

  m_atlasTexture.unbind();
  m_gridTexture.unbind();

  if(is_collision_layer){
    m_ctileShader.use();
    for(int y = 0;y < m_nRows;y++){
      for(int x = 0;x < m_nCols;x++){
      
        model = glm::mat4(1.0f);
        model = glm::translate(model,m_collision_grid[y][x].r.origin);
        model = glm::scale(model,m_collision_grid[y][x].r.size);
      
        m_ctileShader.setValue("model",model);
        m_ctileShader.setValue("view",view);
        m_ctileShader.setValue("projection",projection);
        m_ctileShader.setValue("is_walkable",m_collision_grid[y][x].is_walkable);

        m_vao.bind();
        glDrawArrays(GL_TRIANGLES,0,6);
        m_vao.unbind();

      }
    }
  }
}

void TileManager::handleCollisions(float uv_x,float uv_y,glm::vec3& cursorPos,std::vector<std::pair<int,int>> &animTiles){
  
  if(!is_collision_layer){
    if(glfwGetMouseButton(m_window,GLFW_MOUSE_BUTTON_LEFT)==GLFW_PRESS){
      glm::vec3 clickPos = cursorPos;
      for(int y = 0;y < m_nRows;y++){
        for(int x = 0;x < m_nCols;x++){
          if(point_rect_collide(clickPos,m_grid[y][x].r)){
            if(m_grid[y][x].is_animated)
              m_grid[y][x].is_animated = false;
            m_grid[y][x].is_clicked = true;
            m_grid[y][x].uv_x = uv_x;
            m_grid[y][x].uv_y = uv_y;
          }
        }
      }
    }
    if(glfwGetMouseButton(m_window,GLFW_MOUSE_BUTTON_RIGHT)==GLFW_PRESS){
      glm::vec3 anim_cilckPos = cursorPos;
      for(int y = 0; y < m_nRows; y++){
        for(int x = 0; x < m_nCols; x++){
          if(point_rect_collide(anim_cilckPos,m_grid[y][x].r)){
            m_grid[y][x].is_animated = true;
            m_grid[y][x].is_clicked = true;
            for(unsigned int i = 0; i < animTiles.size(); i++ ){
              animUV tmp;
              tmp.y = 12 - animTiles[i].first;
              tmp.x = animTiles[i].second;
              std::cout<<"TMP: Y->"<<12 - tmp.y<<" X->"<<tmp.x<<std::endl;
              m_grid[y][x].uv.push_back(tmp);
            } 
          }
        }
      }
    }
  }
  else{
    if(glfwGetMouseButton(m_window,GLFW_MOUSE_BUTTON_LEFT)==GLFW_PRESS){
      glm::vec3 clickPosc = cursorPos;
      for(int y = 0;y < m_nRows;y++){
        for(int x = 0;x < m_nCols;x++){
          if(point_rect_collide(clickPosc,m_collision_grid[y][x].r)){
            m_collision_grid[y][x].is_walkable = true;
          }
        }
      }
    }
    if(glfwGetMouseButton(m_window,GLFW_MOUSE_BUTTON_RIGHT)==GLFW_PRESS){
      glm::vec3 clickPoscr = cursorPos;
      for(int y = 0;y < m_nRows;y++){
        for(int x = 0;x < m_nCols;x++){
          if(point_rect_collide(clickPoscr,m_collision_grid[y][x].r)){
            m_collision_grid[y][x].is_walkable = false;
          }
        }
      }
    }
  }

}

glm::mat4& TileManager::getViewMatrix(){
  return m_camera.getViewMatrix();
}

glm::mat4& TileManager::getProjectionMatrix(){
  return m_camera.getProjectionMatrix();
}

void TileManager::handleInputs(){
  if(glfwGetKey(m_window,GLFW_KEY_X)==GLFW_PRESS){
    is_collision_layer = true;
  }
  if(glfwGetKey(m_window,GLFW_KEY_Z)==GLFW_PRESS){
    is_collision_layer = false;
  }
}
