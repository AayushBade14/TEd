#version 460 core

out vec4 fragColor;

in vec2 TexCoord;

uniform sampler2D atlasTex;
uniform sampler2D gridTex;

uniform bool is_clicked;
uniform float uv_x;
uniform float uv_y;
uniform float nxf;
uniform float nyf;

void main(){
  
  if(is_clicked){
    float x = 1.0/nyf;
    float y = 1.0/nxf;
    fragColor = texture(atlasTex,vec2(TexCoord.x * x, TexCoord.y * y) + vec2(uv_x * x, uv_y * y));
  }
  else{
    fragColor = texture(gridTex,TexCoord);
  }
}
