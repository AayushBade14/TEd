#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "./Include/Shader/Shader.hpp"
#include "./Include/Texture/Texture.hpp"
#include "./Include/Memory/VBO/VBO.hpp"
#include "./Include/Memory/VAO/VAO.hpp"
#include "./Include/Data/Data.hpp"
#include "./Include/Collision/Collision.hpp"
#include "./Include/UI/Button/Button.hpp"

#include <iostream>

#define WIDTH 1920
#define HEIGHT 1080
#define TITLE "TEd"

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
    }
    if(isHeld)
      p2 = cursorPos;

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
  Texture tex = Texture("./Assets/image.jpg");
  tex.setSamplerValue(shader,"buttonTex",0);

  VBO vbo(verticesPlane,GL_STATIC_DRAW);
  VAO vao;

  vao.setAttribPointer(vbo,0,3,8,0);
  vao.setAttribPointer(vbo,1,2,8,6);

  glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);
  double xpos = WIDTH/2.0f;
  double ypos = HEIGHT/2.0f;

  while(!glfwWindowShouldClose(window)){
    processInput(window);
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glfwGetCursorPos(window,&xpos,&ypos);
    glm::vec3 cPos = {(float)xpos,(float)HEIGHT-(float)ypos,0.0f};
    
    tex.assignTextureUnit(0);
    
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model,glm::vec3(500.0f,500.0f,0.0f));
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::ortho(0.0f,1920.0f,0.0f,1080.0f);
    shader.use();
    shader.setValue("model",model);
    shader.setValue("view",view);
    shader.setValue("projection",projection);
  
    vao.bind();
    glDrawArrays(GL_TRIANGLES,0,6);
    vao.unbind();
    tex.unbind();

    model = glm::mat4(1.0f);
    model = glm::translate(model,glm::vec3(960.0f,540.0f,0.0f));
    model = glm::scale(model,glm::vec3(500.0f,500.0f,0.0f));
    tex.assignTextureUnit(0);
    selShader.use();
    selShader.setValue("model",model);
    selShader.setValue("view",view);
    selShader.setValue("projection",projection);
    sel(window,selShader,cPos); 

    vao.bind();
    glDrawArrays(GL_TRIANGLES,0,36);
    vao.unbind();

    tex.unbind();
    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
