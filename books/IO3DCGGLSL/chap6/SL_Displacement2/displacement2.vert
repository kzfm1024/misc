
varying vec3 P;
varying vec3 N;
varying vec3 T;
varying vec3 B;
attribute vec3 tangent;
uniform sampler2D normalMap;//法線マップ
uniform float inv;
uniform float fDisp;
uniform float bias;

void main(void)
{ 
	if(inv == 1.0)
		gl_Vertex.xyz +=  fDisp * gl_Normal.xyz * (texture2D(normalMap, gl_MultiTexCoord0.xy).b - 1.0 +bias);
	else
		gl_Vertex.xyz +=  fDisp * gl_Normal.xyz * (texture2D(normalMap, gl_MultiTexCoord0.xy).b - bias);
  P = (gl_ModelViewMatrix * gl_Vertex).xyz;
  N = normalize(gl_NormalMatrix * gl_Normal).xyz;
  T = normalize(gl_NormalMatrix * tangent).xyz;
  B = cross(N, T);

  gl_TexCoord[0] = gl_MultiTexCoord0;
  gl_Position = ftransform() ;
}
