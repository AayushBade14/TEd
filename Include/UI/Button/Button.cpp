#include "./Button.hpp"

Button::Button(const std::string &path,glm::vec3 &origin,glm::vec3 &size){
  buttonRect.origin = origin;
  buttonRect.size = size;
  
  Texture tmp = Texture(path);
  texture = tmp.getID();

  glGenBuffers(1,&vbo);
  glGenVertexArrays(1,&vao);

  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER,vbo);
  glBufferData(GL_ARRAY_BUFFER,verticesPlane.size()*sizeof(float),verticesPlane.data(),GL_STATIC_DRAW);

  glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(6*sizeof(float)));
  glEnableVertexAttribArray(1);
  
  glBindVertexArray(0);
}

void Button::render(GLFWwindow *window,Shader &shader,glm::vec3 &cursorPos){
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model,buttonRect.origin);
  model = glm::scale(model,buttonRect.size);

  glm::mat4 view = glm::mat4(1.0f);
  glm::mat4 projection = glm::ortho(0.0f,1920.0f,0.0f,1080.0f);
  shader.use();
  shader.setValue("model",model);
  shader.setValue("view",view);
  shader.setValue("projection",projection);
  
  int val = 0;
  shader.use();
  shader.setValue("buttonTex",val);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D,texture);

  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLES,0,6);
  glBindVertexArray(0);
  
  glBindTexture(GL_TEXTURE_2D,0);

  static bool wasPressedLastFrame = false;
  static int ti = 0;
  if(point_rect_collide(cursorPos,buttonRect))
    isHovered = true;
  else
    isHovered = false;
  
  bool mouseDown = glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT)==GLFW_PRESS;

  if(isHovered && mouseDown && !wasPressedLastFrame)
    isActive = true;
  else
    isActive = false;
  
  if(isActive)
    std::cout<<"BUTTON-PRESSED: X"<<ti++<<std::endl;
  wasPressedLastFrame = mouseDown;
}
