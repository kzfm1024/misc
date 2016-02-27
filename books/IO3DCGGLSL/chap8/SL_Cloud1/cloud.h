//地形データ
#define nMeshC 64//最大分割数（両辺同じ）
float sizeXC = 100.0;
float sizeYC = 100.0;
float dataC[(nMeshC+1)*(nMeshC+1)];//高さデータ
float betaC = 2.0;//fBmの指数β
float sigma0C = 0.25;//標準偏差
int seedC = 0;//乱数のｼｰﾄﾞ
float bias = 0.3;//大きくすると晴れ間が多くなる
//地形affine変換用変数
//float posC[] = { 0.0, 0.0, 0.0 };//中心のxyz座標
//float scaleC[] = { 1.0, 1.0, 1.0 };//大きさ(倍率)
//float angleC[] = { 0.0, 0.0, 0.0 };//回転角度

void makeCloudData()
{
  int i, j;
  CMidpoint2 mp = CMidpoint2();
  mp.numDiv = 4;
  mp.numMax = nMeshC;
  mp.sigma0 = sigma0C;
  mp.beta = betaC;

  //初期高度データ
  for(j = 0; j <= mp.numDiv; j++)
		for(i = 0; i <= mp.numDiv; i++) mp.h[i][j] = - bias + (float)rand() / (float)RAND_MAX;;
  mp.create(seedC, dataC);//第1引数は乱数のseed（値を変えると異なったパターンが作成）
/*
	for(j = 0; j <= mp.numDiv; j++) 
		for(i = 0; i <= mp.numDiv; i++) 
		{//負ならば0とする
			if( dataT[j * (mp.numMax+1) + i] <= 0.0) dataT[j * (mp.numMax+1) + i] = 0.0;
		}*/
//	calcDirection();
}

