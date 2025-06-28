#pragma once

#include "../VBO/VBO.hpp"

class VAO{
  public:
    VAO(VBO &vbo);
    ~VAO();
    
    VAO& operator =(const VAO &other){
      if(this == &other)
        return *this;
      
      ID = other.getID();

      return *this;
    }
  
    void bind();
    void unbind();

    unsigned int getID();
    
    void setAttribPointer(int loc,int nrVal,int stride,int start);

  private:
    unsigned int ID;
    VBO vbo;
};
