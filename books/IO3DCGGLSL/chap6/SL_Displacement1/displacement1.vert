
varying vec3 P;
varying vec3 N;
varying vec3 T;
varying vec3 B;
attribute vec3 tangent;
uniform sampler2D normalMap;
uniform float inv;
uniform float fDisp;

void main(void)
{
	if(inv == 1.0)//‰š“Ê”½“]‚È‚µ
		gl_Vertex.xyz +=  fDisp * gl_Normal.xyz * (texture2D(normalMap, gl_MultiTexCoord0.xy).b - 1.0);
	else
		gl_Vertex.xyz +=  fDisp * gl_Normal.xyz * (texture2D(normalMap, gl_MultiTexCoord0.xy).b);
  P = (gl_ModelViewMatrix * gl_Vertex).xyz;
  N = normalize(gl_NormalMatrix * gl_Normal).xyz;
  T = normalize(gl_NormalMatrix * tangent).xyz;
  B = cross(N, T);
  
  gl_TexCoord[0] = gl_MultiTexCoord0;
  gl_Position = ftransform() ;
}
