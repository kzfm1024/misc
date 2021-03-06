
varying vec3 P;
varying vec3 N;
uniform float scale;//テクスチャの倍率
uniform int coord;//座標系の選択

void main(void)
{
  P = vec3(gl_ModelViewMatrix * gl_Vertex);
  N = normalize(gl_NormalMatrix * gl_Normal).xyz;
  if(coord == 0)//オブジェクト座標系 
    gl_TexCoord[0] = (gl_Vertex+vec4(0.5, 0.5, 0.5, 0.0)) * scale;
  else //視点座標系
    gl_TexCoord[0] = (vec4(P, 0.0)+vec4(0.5, 0.5, 0.5, 0.0)) * scale;
  gl_Position = ftransform();
}
