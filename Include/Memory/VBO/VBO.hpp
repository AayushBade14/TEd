#pragma once

#include <glad/glad.h>

#include <iostream>
#include <vector>

class VBO{
  public:
    VBO(std::vector<float> &vertices,GLenum method);
    ~VBO();
    
    void bind();
    void unbind();
    
    unsigned int getID() const;

  private:
    unsigned int ID;
};
