#version 460 core

out vec4 fragColor;

in vec2 TexCoord;

uniform sampler2D buttonTex;
uniform vec3 p1;
uniform vec3 p2;

void main(){
  vec2 uv_start = p1.xy/vec2(500.0,500.0);
  vec2 uv_end = p2.xy/vec2(500.0,500.0);

  vec2 selectedUV = mix(uv_start,uv_end,TexCoord);

  fragColor = texture(buttonTex,selectedUV);
}
