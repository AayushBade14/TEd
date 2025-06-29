#include "./VAO.hpp"

VAO::VAO(){
  glGenVertexArrays(1,&ID);
}

VAO::~VAO(){
  unbind();
  glDeleteVertexArrays(1,&ID);
}

void VAO::bind(){
  glBindVertexArray(ID);
}

void VAO::unbind(){
  glBindVertexArray(0);
}

void VAO::setAttribPointer(VBO &vbo,int loc,int nrVal,int stride,int start){
  bind();
  vbo.bind();
  glVertexAttribPointer(loc,nrVal,GL_FLOAT,GL_FALSE,stride * sizeof(float),(void*)(start * sizeof(float)));
  glEnableVertexAttribArray(loc);
  unbind();
}

unsigned int VAO::getID() const{
  return ID;
}

