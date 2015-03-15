
varying vec3 P;
varying vec3 N;
varying vec4 Reflect;//反射ベクトル
uniform samplerCube envSampler;
uniform float alpha;

uniform float refract;//相対屈折率
uniform float filmSpace ;//nano meter

const float ultraviolet = 380.0;
const float infrared = 780.0;

vec3 getRGB(float lambda)
{
  //正規化波長
	float x = (lambda - ultraviolet) / (infrared - ultraviolet);
	
	const float wR = 0.3, wB = 0.2, wV = 0.08;
	vec3 y = vec3(x) - vec3(0.7, 0.4, 0.2);	
	vec3 col ;
	col.r = max((1.0 - y.r * y.r / (wR*wR)), 0.0);
	if(x < 0.4) col.g = max((1.0 - y.g * y.g / (wB*wB)), 0.0);
	else        col.g = max((1.0 - y.g * y.g / (wR*wR)), 0.0);
	col.b = max((1.0 - y.b * y.b / (wB*wB)), 0.0);
	//violetを加味
	col.r += max((1.0 - (x-0.08) * (x-0.08) / (wV*wV)), 0.0) * 0.7;
	return col;
}

void main(void)
{
  vec3 L = normalize(gl_LightSource[0].position.xyz - P);
  N = normalize(N);
  
	vec3 V = normalize(-P);
	vec3 H = normalize(L + V);
	float powNH1 = pow(max(dot(N, H), 0.0), gl_FrontMaterial.shininess);
	float powNH2 = pow(max(dot(-N, H), 0.0), gl_FrontMaterial.shininess);
	vec4 specular = gl_FrontLightProduct[0].specular * powNH1;
	specular += gl_FrontLightProduct[0].specular * powNH2;

	//干渉による色の計算
	float dotNV = dot(N, V);
	float cosThetaR = sqrt(1.0 - (1.0 - dotNV*dotNV)/(refract * refract));
	float a = 2.0 * filmSpace * cosThetaR * refract;
	vec4 colorInter = vec4(0.0);
	for(int i = 0; i <= 5; i++)
	{
		float lambda = a / (float(i) + 0.5);
		if(lambda < ultraviolet || lambda > infrared) continue;
		colorInter.rgb += getRGB(lambda);
	}

	vec4 colorReflect = textureCube(envSampler, Reflect.stp) ;//反射環境の色
  vec3 col = (colorReflect+ specular).rgb;
	if(colorInter.rgb == vec3(0.0) ) 
	  gl_FragColor.rgb = col;
	else
	  gl_FragColor.rgb = colorInter.rgb * col;
	
	col = col*col*col*col;
	gl_FragColor.a = (col.r + col.g + col.b) * alpha;
}