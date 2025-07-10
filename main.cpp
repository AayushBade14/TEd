#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "./Include/Shader/Shader.hpp"
#include "./Include/Texture/Texture.hpp"
#include "./Include/Memory/VBO/VBO.hpp"
#include "./Include/Memory/VAO/VAO.hpp"
#include "./Include/Data/Data.hpp"
#include "./Include/Collision/Collision.hpp"
#include "./Include/UI/Button/Button.hpp"

#include "./Include/Camera/Camera2D.hpp"
#include "./Include/TextureAtlas/TextureAtlas.hpp"
#include "./Include/TileManager/TileManager.hpp"
#include <iostream>
#include <utility>

#define WIDTH 1920
#define HEIGHT 1044
#define TITLE "TEd"

glm::vec3 screenToWorld(float xpos,float ypos,const glm::mat4& view,const glm::mat4& projection){
  [[maybe_unusable]]glm::vec4 screenPos = glm::vec4(xpos,ypos,0.0f,1.0f);

  float ndcX = (2.0f * xpos)/WIDTH - 1.0f;
  float ndcY = 1.0f - (2.0f * ypos)/HEIGHT;
  glm::vec4 clipSpacePos = glm::vec4(ndcX,ndcY,0.0f,1.0f);

  glm::mat4 invVP = glm::inverse(projection * view);
  glm::vec4 worldPos = invVP * clipSpacePos;
  worldPos /= worldPos.w;

  return glm::vec3(worldPos);
}

void framebuffer_size_callback(GLFWwindow *window,int width,int height){
  glViewport(0,0,width,height);
}

bool is_atlas = true;
bool qHeldLastFrame = false;
float uv_x = 0.0f;
float uv_y = 0.0f;

void processInput(GLFWwindow *window){
  if(glfwGetKey(window,GLFW_KEY_ESCAPE)==GLFW_PRESS)
    glfwSetWindowShouldClose(window,true);
  
  bool Theld = false;
  if(glfwGetKey(window,GLFW_KEY_T)==GLFW_PRESS){
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    Theld = true;
  }
  if(!Theld && glfwGetKey(window,GLFW_KEY_T)==GLFW_RELEASE){
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    Theld = false;
  }
  

  if(glfwGetKey(window,GLFW_KEY_Q)==GLFW_PRESS){
    is_atlas = true;
  }
  if(glfwGetKey(window,GLFW_KEY_E)==GLFW_PRESS){
    is_atlas = false;
  }

}

std::vector<std::pair<int,int>> animUV;

int main(void){

  if(glfwInit()<0){
    std::cerr<<"ERROR: GLFW_INIT!"<<std::endl;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,6);
  glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(WIDTH,
                                        HEIGHT,
                                        TITLE,
                                        NULL,
                                        NULL);

  if(!window){
    glfwTerminate();
    std::cerr<<"ERROR: Creating window!"<<std::endl;
  }

  glfwMakeContextCurrent(window);

  if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
    glfwDestroyWindow(window);
    glfwTerminate();

    std::cerr<<"ERROR: GLAD_INIT!"<<std::endl;
  }
  
  glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);
  
  float dt = 0.0f;
  float lf = 0.0f;
  double xpos = 0.0f;
  double ypos = 0.0f;
  TextureAtlas atlas(window,1600.0f,832.0f,13,25);
  TileManager tm(window,"./Assets/Tileset.png",100,100,64.0f,64.0f,13.0f,25.0f);
  while(!glfwWindowShouldClose(window)){
    float cf = (float)glfwGetTime();
    dt = cf - lf;
    lf = cf;
    
    processInput(window);
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glfwGetCursorPos(window,&xpos,&ypos);
    glm::vec3 cPos = screenToWorld(xpos,ypos,atlas.getViewMatrix(),atlas.getProjectionMatrix());
    glm::vec3 gPos = screenToWorld(xpos,ypos,tm.getViewMatrix(),tm.getProjectionMatrix());

    if(is_atlas)
      atlas.renderTextureAtlas(dt,cPos,uv_x,uv_y);
    else{
      animUV = atlas.getAnimSelections();
      tm.renderTiles(dt,uv_x,uv_y,gPos,animUV);
    }
    //std::cout<<"UVX: "<<uv_x<<" UVY: "<<uv_y<<std::endl; 
    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
