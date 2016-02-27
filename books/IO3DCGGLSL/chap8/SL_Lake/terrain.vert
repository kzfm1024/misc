varying vec3 P;
varying vec3 N;

void main(void)
{ 
	vec4 p = normalize(gl_Vertex);
	vec3 n = normalize(gl_Normal);
  P = vec3(gl_ModelViewMatrix * gl_Vertex);
  N = normalize(gl_NormalMatrix * n);
  gl_TexCoord[0].s = sqrt(n.x * n.x + n.y * n.y);
  gl_TexCoord[0].t = clamp(p.z*1.8, 0.0, 1.0);
  gl_Position = ftransform();
}
