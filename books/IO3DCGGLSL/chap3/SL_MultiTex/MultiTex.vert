varying vec3 P;
varying vec3 N;

void main(void)
{
  P = vec3(gl_ModelViewMatrix * gl_Vertex);
  N = normalize(gl_NormalMatrix * gl_Normal).xyz;
  gl_TexCoord[0] = gl_MultiTexCoord0;
  gl_TexCoord[1] = gl_TextureMatrix[1] * gl_MultiTexCoord0;
  gl_Position = ftransform();
}
