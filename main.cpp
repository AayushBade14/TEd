#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "./Include/Shader/Shader.hpp"
#include "./Include/Texture/Texture.hpp"
#include "./Include/Memory/VBO/VBO.hpp"
#include "./Include/Memory/VAO/VAO.hpp"

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
  
  bool Theld = false
  if(glfwGetKey(window,GLFW_KEY_T)==GLFW_PRESS){
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    Theld = true;
  }
  if(Theld && glfwGetKey(window,GLFW_KEY_T)==GLFW_RELEASE){
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    Theld = false;
  }

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
  
  std::vector<float> verticesPlane = {
    // vertex             // normals          // texcoords
    0.0f,0.0f,0.0f,       0.0f,0.0f,1.0f,     0.0f,0.0f,
    1.0f,1.0f,0.0f,   0.0f,0.0f,1.0f,     1.0f,1.0f,
    0.0f,1.0f,0.0f,     0.0f,0.0f,1.0f,     0.0f,1.0f,

    1.0f,1.0f,0.0f,   0.0f,0.0f,1.0f,     1.0f,1.0f,
    0.0f,0.0f,0.0f,       0.0f,0.0f,1.0f,     0.0f,0.0f,
    1.0f,0.0f,0.0f,     0.0f,0.0f,1.0f,     1.0f,0.0f
  };


  glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);

  while(!glfwWindowShouldClose(window)){
    processInput(window);
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
