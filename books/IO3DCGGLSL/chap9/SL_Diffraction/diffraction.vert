
attribute vec3 tangent;
uniform int changeTangent;
varying vec3 P, N, T;

void main()
{
  P = vec3(gl_ModelViewMatrix * gl_Vertex);//頂点
  N = normalize(gl_NormalMatrix * gl_Normal);//法線ベクトル
  T = normalize(gl_NormalMatrix * tangent);//接線ベクトル
  if(changeTangent == 1) T = cross(T, N);//接線ベクトルの切り替え
	gl_Position = ftransform();

}

