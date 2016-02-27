
varying vec3 P, N;

void main()
{
	//vec4 pos = gl_Vertex;
  //gl_Vertex.x = gl_Vertex.x + 0.1 * sin(2.0 * 3.141592 * gl_Vertex.z + 1.0 * time);
  
  P = vec3(gl_ModelViewMatrix * gl_Vertex);//頂点
  N = normalize(gl_NormalMatrix * gl_Normal);//法線ベクトル
	gl_Position = ftransform();

}

