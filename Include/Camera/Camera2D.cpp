#include "./Camera2D.hpp"

Camera2D::Camera2D(GLFWwindow *window,float camSpeed){
  this->m_window = window;
  this->m_cameraSpeed = camSpeed;
  glfwGetWindowSize(m_window,&m_screenWidth,&m_screenHeight);
  std::cout<<"CAMERA INIT! WIDTH: "<<m_screenWidth<<" HEIGHT: "<<m_screenHeight<<std::endl;
  this->m_cameraPosition = glm::vec3(m_screenWidth/2.0f,m_screenHeight/2.0f,0.0f);
}

void Camera2D::handleInput(float dt){
  if(glfwGetKey(m_window,GLFW_KEY_W)==GLFW_PRESS)
    m_cameraPosition.y += m_cameraSpeed * dt;
  if(glfwGetKey(m_window,GLFW_KEY_S)==GLFW_PRESS)
    m_cameraPosition.y -= m_cameraSpeed * dt;
  if(glfwGetKey(m_window,GLFW_KEY_A)==GLFW_PRESS)
    m_cameraPosition.x -= m_cameraSpeed * dt;
  if(glfwGetKey(m_window,GLFW_KEY_D)==GLFW_PRESS)
    m_cameraPosition.x += m_cameraSpeed * dt;
}

void Camera2D::update(float dt){
  handleInput(dt);

  m_viewMatrix = glm::translate(glm::mat4(1.0f),-m_cameraPosition);
  m_projectionMatrix = glm::ortho(
    m_cameraPosition.x - m_screenWidth/2.0f,
    m_cameraPosition.x + m_screenWidth/2.0f,
    m_cameraPosition.y - m_screenHeight/2.0f,
    m_cameraPosition.y + m_screenHeight/2.0f
  );
}

glm::vec3& Camera2D::getCameraPosition(){
  return m_cameraPosition;
}

glm::mat4& Camera2D::getViewMatrix(){
  return m_viewMatrix;
}

glm::mat4& Camera2D::getProjectionMatrix(){
  return m_projectionMatrix;
}
