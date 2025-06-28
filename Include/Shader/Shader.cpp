#include "./Shader.hpp"

Shader::Shader(const std::string &vertexPath,const std::string &fragmentPath){
  std::string vCode = loadFile(vertexPath);
  std::string fCode = loadFile(fragmentPath);

  unsigned int vertexShader = compileShader(vCode,true);
  unsigned int fragmentShader = compileShader(fCode,false);

  createShaderProgram(vertexShader,fragmentShader);
}

Shader::~Shader(){
  glDeleteProgram(ID);
}

void Shader::use(){
  glUseProgram(ID);
}

unsigned int Shader::getID(){
  return ID;
}

std::string Shader::loadFile(const std::string &path){
  std::string code;
  std::ifstream file;

  file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  try{
    file.open(path.c_str());

    std::stringStream stream;
    stream << file.rdbuf();

    file.close();

    code = stream.str();
  }
  catch(const std::ifstream::failure &e){
    std::cerr<<"ERROR: Reading Shader File (PATH: "<<path<<"). REASON: "<<e.what()<<std::endl;
  }

  return code;
}

unsigned int Shader::compileShader(const std::string &src,bool isVertex){
  const char *code = src.c_str();
  
  unsigned int shader = isVertex? glCreateShader(GL_VERTEX_SHADER) : glCreateShader(GL_FRAGMENT_SHADER);
  int success;
  char infoLog[512];
  
  glShaderSource(shader,1,&code,NULL);
  glCompileShader(shader);

  glGetShaderiv(shader,GL_COMPILE_STATUS,&success);
  if(!success){
    glGetShaderInfoLog(shader,512,NULL,infoLog);
    std::cerr<<"ERROR: Compiling "<< (isVertex? "Vertex-Shader" : "Fragment-Shader") << " (REASON: "<<infoLog<<")."<<std::endl;
  }

  return shader;
}

void Shader::createShaderProgram(unsigned int &vertex,unsigned int &fragment){

  int success;
  char infoLog[512];

  ID = glCreateProgram();
  glAttachShader(ID,vertex);
  glAttachShader(ID,fragment);      
  glLinkProgram(ID);

  glGetProgramiv(ID,GL_LINK_STATUS,&success);
  if(!success){
    glGetProgramInfoLog(ID,512,NULL,infoLog);
    std::cerr<<"ERROR: Creating Shader Program (REASON: "<<infoLog<<")."<<std::endl;
  }

  glDeleteShader(vertex);
  glDeleteShader(fragment);
}
