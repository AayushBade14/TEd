#pragma once

#include "../Shader/Shader.hpp"

class Texture{
  public:
    Texture(const std::string &path);
    ~Texture();
    
    Texture& operator=(const Texture &other){
      if(this == &other)
        return *this;

      ID = other.getID();

      return *this;
    }

    void bind();
    void unbind();
    
    unsigned int getID();
  
    void setSamplerValue(Shader &shader,const std::string &name,int value);
    void assignTextureUnit(int value);
  
  private:
    unsigned int ID;
    void loadTexture(const std::string &path);
};
