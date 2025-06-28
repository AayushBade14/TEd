#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class Shader{
  public:
    Shader(const std::string &vertexPath,const std::string &fragmentPath);
    ~Shader();
    
    Shader& operator=(const Shader &other){
      if(this == &other)
        return *this;

      ID = other.getID();

      return *this;
    }
  
    void use(); // method to use the shader program
    
    unsigned int getID(); 
    
    // method to set uniform value by reference
    template <typename T>
    void setValue(const std::string &name,T &value){
      unsigned int loc = glGetUniformLocation(ID,name.c_str());

      if constexpr(std::is_same<T,int>::value)
        glUniform1i(loc,value);
      else if constexpr(std::is_same<T,bool>::value)
        glUniform1i(loc,(int)value);
      else if constexpr(std::is_same<T,float>::value)
        glUniform1f(loc,value);
      else if constexpr(std::is_same<T,glm::vec2>::value)
        glUniform2fv(loc,1,glm::value_ptr(value));
      else if constexpr(std::is_same<T,glm::vec3>::value)
        glUniform3fv(loc,1,glm::value_ptr(value));
      else if constexpr(std::is_same<T,glm::mat4>::value)
        glUniformMatrix4fv(loc,1,GL_FALSE,glm::value_ptr(value));
    }
    // method to set uniform value by value
    template <typename T>
    void setValue(const std::string &name,T value){
      unsigned int loc = glGetUniformLocation(ID,name.c_str());

      if constexpr(std::is_same<T,int>::value)
        glUniform1i(loc,value);
      else if constexpr(std::is_same<T,bool>::value)
        glUniform1i(loc,(int)value);
      else if constexpr(std::is_same<T,float>::value)
        glUniform1f(loc,value);
      else if constexpr(std::is_same<T,glm::vec2>::value)
        glUniform2fv(loc,1,glm::value_ptr(value));
      else if constexpr(std::is_same<T,glm::vec3>::value)
        glUniform3fv(loc,1,glm::value_ptr(value));
      else if constexpr(std::is_same<T,glm::mat4>::value)
        glUniformMatrix4fv(loc,1,GL_FALSE,glm::value_ptr(value));
    }

  private:
    unsigned int ID;

    std::string loadFile(const std::string &path); // parses and loads shader file data
    unsigned int compileShader(const std::string &src,bool isVertex); // compiles shader
    void createShaderProgram(unsigned int &vertex,unsigned int &fragment); // creates shader prog.
};
