#include "./Texture.hpp"

Texture::Texture(const std::string &path){
  loadTexture(path,GL_REPEAT,GL_REPEAT,GL_LINEAR_MIPMAP_LINEAR,GL_LINEAR);
}

Texture::Texture(const std::string &path,GLenum wrapS,GLenum wrapT,GLenum minF,GLenum magF){
  loadTexture(path,wrapS,wrapT,minF,magF);
}

Texture::~Texture(){
  unbind();
  glDeleteTextures(1,&ID);
}

void Texture::bind(){
  glBindTexture(GL_TEXTURE_2D,ID);
}

void Texture::unbind(){
  glBindTexture(GL_TEXTURE_2D,0);
}

void Texture::loadTexture(const std::string &path,GLenum wrapS,GLenum wrapT,GLenum minF,GLenum magF){

  glGenTextures(1,&ID);
  bind();

  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,wrapS);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,wrapT);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,minF);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,magF);

  stbi_set_flip_vertically_on_load(true);
  
  int width,height,nrChannels;
  unsigned char *data = stbi_load(path.c_str(),&width,&height,&nrChannels,0);

  GLenum Flag = nrChannels == 3 ? GL_RGB : GL_RGBA;

  if(data){
    glTexImage2D(GL_TEXTURE_2D,0,Flag,width,height,0,Flag,GL_UNSIGNED_BYTE,data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else{
    std::cerr<<"ERROR: Loading Texture (PATH: "<<path<<")."<<std::endl;
  }

  stbi_image_free(data);
  unbind();

  std::cout<<"Texture Loaded Successfully! (PATH: "<<path<<")."<<std::endl;
}

void Texture::setSamplerValue(Shader &shader,const std::string &name,int value){
  shader.use();
  shader.setValue(name,value);
}

void Texture::assignTextureUnit(int value){
  glActiveTexture(GL_TEXTURE0 + value);
  bind();
}

unsigned int Texture::getID() const{
  return ID;
}
