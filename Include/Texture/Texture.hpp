#pragma once

#include "../Shader/Shader.hpp"

class Texture{
  public:
    Texture(const std::string &path);
    ~Texture() = default;

    void bind();
    void unbind();

    void setSamplerValue(Shader &shader,const std::string &name,int value);
    void assignTextureUnit(int value);
  
  private:
    unsigned int ID;
    void loadTexture(const std::string &path);
};
