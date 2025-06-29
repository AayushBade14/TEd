#pragma once

#include "../VBO/VBO.hpp"

class VAO{
  public:
    VAO();
    ~VAO();
    
    void bind();
    void unbind();

    unsigned int getID() const;

    void setAttribPointer(VBO &vbo,int loc,int nrVal,int stride,int start);

  private:
    unsigned int ID;
};
