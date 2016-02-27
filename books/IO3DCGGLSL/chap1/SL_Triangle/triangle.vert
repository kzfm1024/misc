
void main(void)
{
	gl_Position = gl_Vertex; //頂点座標をそのまま出力
	gl_FrontColor = gl_Color;//頂点カラーをそのまま出力
//	gl_FrontColor = vec4(1.0) - gl_Color;
}