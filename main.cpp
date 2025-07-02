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

#include <iostream>

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

}

struct Tile{
  glm::vec3 start;
  glm::vec3 end;
  glm::vec3 position;
  bool isHovered;
};

glm::vec3 start = glm::vec3(0.0f);
glm::vec3 end = glm::vec3(0.0f);

std::vector<std::vector<Tile>> tmap(5,std::vector<Tile>(5));

void sel(GLFWwindow *window,Shader &shader,glm::vec3 &cursorPos){
  Rectangle r;
  r.origin = glm::vec3(0.0f);
  r.size = glm::vec3(500.0f,500.0f,0.0f);
  static glm::vec3 p1 = glm::vec3(0.0f);
  static glm::vec3 p2 = glm::vec3(0.0f);
  static bool isHeld = false;
  if(point_rect_collide(cursorPos,r)){
    if(!isHeld && glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT)==GLFW_PRESS){
      isHeld = true;
      p1 = cursorPos;
      p2 = cursorPos;
      start = cursorPos;
      end = cursorPos;
    }
    if(isHeld){
      p2 = cursorPos;
      end = cursorPos;
    }
    if(isHeld && glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT)==GLFW_RELEASE){
      isHeld = false;
      //p2 = cursorPos;
    }
  }

  shader.use();
  shader.setValue("p1",p1);
  shader.setValue("p2",p2);
}

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
  
  glm::vec3 origin = {500.0f,500.0f,0.0f};
  glm::vec3 size = {1000.0f,400.0f,0.0f};
  Shader shader = Shader("./Assets/vert.glsl","./Assets/frag.glsl");
  Shader selShader = Shader("./Assets/vert.glsl","./Assets/selFrag.glsl");
  Button bt = Button("./Assets/image.jpg",origin,size);
  Texture tex = Texture("./Assets/tile2.jpg");
  tex.setSamplerValue(shader,"buttonTex",0);

  VBO vbo(verticesPlane,GL_STATIC_DRAW);
  VAO vao;

  vao.setAttribPointer(vbo,0,3,8,0);
  vao.setAttribPointer(vbo,1,2,8,6);

  glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);
  double xpos = WIDTH/2.0f;
  double ypos = HEIGHT/2.0f;
  
  float xoff = 960.0f;
  float yoff = 540.0f;
  for(int y = 0;y<5;y++){
    for(int x = 0;x<5;x++){
      float xpos = xoff + (x * 100.0f);
      float ypos = yoff + (y*100.0f);

      Tile tmp;
      tmp.position = glm::vec3(xpos,ypos,0.0f);
      tmp.start = start;
      tmp.end = end;

      tmap[y][x] = tmp;
    }
  }
  
  Camera2D cam(window,100.0f);
  float dt = 0.0f;
  float lf = 0.0f;
  
  TextureAtlas atlas(window,1920.0f,1080.0f,13,25);

  while(!glfwWindowShouldClose(window)){
    float cf = (float)glfwGetTime();
    dt = cf - lf;
    lf = cf;
    
    processInput(window);
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glfwGetCursorPos(window,&xpos,&ypos);
    glm::vec3 cPos = screenToWorld(xpos,ypos,atlas.getViewMatrix(),atlas.getProjectionMatrix());
    
    atlas.renderTextureAtlas(dt,cPos);

    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
