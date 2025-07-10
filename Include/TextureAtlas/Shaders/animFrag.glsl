#version 460 core

out vec4 fragColor;

in vec2 TexCoord;

uniform sampler2D animSelectionBox;

void main(){
  vec4 tex = texture(animSelectionBox,TexCoord);
  if(tex.a < 0.1)
    discard;

  fragColor = tex;
}


