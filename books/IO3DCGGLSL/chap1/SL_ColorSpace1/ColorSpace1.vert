
void main(void)
{
	//頂点座標の出力
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_FrontColor = gl_Color;  //フラグメントシェーダーへ
}