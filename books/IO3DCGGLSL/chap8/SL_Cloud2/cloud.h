//雲データ
#define nMeshC 128   //最大分割数（両辺同じ）
float sizeXC = 100.0;
float sizeYC = 100.0;
float dataC[(nMeshC+1)*(nMeshC+1)];//雲の存在確率
float betaC = 3.0;   //fBmの指数β
float sigma0C = 0.25;//標準偏差
int seedC = 0;       //乱数のｼｰﾄﾞ
float bias = 0.5;    //大きくすると晴れ間が多くなる

void makeCloudData()
{
  int i, j;
  CMidpoint2 mp2 = CMidpoint2();
  mp2.numDiv = 8;
  mp2.numMax = nMeshC;
  mp2.sigma0 = sigma0C;
  mp2.beta = betaC;
  //初期データ
  for(j = 0; j <= mp2.numDiv; j++)
		for(i = 0; i <= mp2.numDiv; i++) 
			mp2.h[i][j] = - bias + (float)rand() / (float)RAND_MAX;;
  mp2.create(seedC, dataC);
}

