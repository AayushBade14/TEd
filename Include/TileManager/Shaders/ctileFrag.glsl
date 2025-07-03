#version 460 core

out vec4 fragColor;

in vec2 TexCoord;

uniform bool is_walkable;

void main(){
  if(is_walkable){
    fragColor = vec4(0.0,1.0,0.0,0.3); 
  }
  else{
    fragColor = vec4(1.0,0.0,0.0,0.3);
  }
}
