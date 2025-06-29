#pragma once

#include "../Shader/Shader.hpp"
#include "../ThirdParty/StbImage/stb_image.h"

class Texture{
  public:
    Texture(const std::string &path);
    Texture(const std::string &path,GLenum wrapS,GLenum wrapT,GLenum minF,GLenum magF);
    ~Texture();
    
    void bind();
    void unbind();
    
    unsigned int getID() const;
  
    void setSamplerValue(Shader &shader,const std::string &name,int value);
    void assignTextureUnit(int value);
  
  private:
    unsigned int ID;
    void loadTexture(const std::string &path,GLenum wrapS,GLenum wrapT,GLenum minF,GLenum magF);
};
