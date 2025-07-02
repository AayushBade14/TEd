#version 460 core

out vec4 fragColor;

in vec2 TexCoord;

uniform sampler2D atlas;

void main(){
  fragColor = texture(atlas,TexCoord);
}
