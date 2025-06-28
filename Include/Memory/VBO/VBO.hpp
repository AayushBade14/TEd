#pragma once

#include <glad/glad.h>

#include <iostream>
#include <vector>

class VBO{
  public:
    VBO(std::vector<float> &vertices,GLenum method);
    VBO();
    ~VBO();
    
    VBO& operator =(const VBO& other){
      if(this == &other)
        return *this;
      
      ID = other.getID();
      
      return *this;
    }
  
    void bind();
    void unbind();
    
    unsigned int getID();

  private:
    unsigned int ID;
}
