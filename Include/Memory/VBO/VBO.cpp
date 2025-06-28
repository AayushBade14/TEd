#include "./VBO.hpp"

VBO::VBO(std::vector<float> &vertices,GLenum method){
  glGenBuffers(1,&ID);
  bind();
  glBufferData(GL_ARRAY_BUFFER,vertices.size() * sizeof(float),vertices.data(),method);
  unbind();
}

VBO::VBO(){
  // empty constructor
}

VBO::~VBO(){
  unbind();
  glDeleteBuffers(1,&ID);
}

void VBO::bind(){
  glBindBuffer(GL_ARRAY_BUFFER,ID);
}

void VBO::unbind(){
  glBindBuffer(GL_ARRAY_BUFFER,0);
}

unsigned int VBO::getID() const{
  return ID;
}
