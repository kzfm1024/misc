
varying vec3 P;
varying vec3 N;

void main(void)
{ 
	vec4 p = normalize(gl_Vertex);
	vec3 n = normalize(gl_Normal);
  P = vec3(gl_ModelViewMatrix * gl_Vertex);
  N = normalize(gl_NormalMatrix * n);
  gl_TexCoord[0].x = sqrt(n.x * n.x + n.y * n.y);
  gl_TexCoord[0].y = p.z;
  gl_Position = ftransform();
}
