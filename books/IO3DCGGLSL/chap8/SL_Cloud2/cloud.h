//�_�f�[�^
#define nMeshC 128   //�ő啪�����i���ӓ����j
float sizeXC = 100.0;
float sizeYC = 100.0;
float dataC[(nMeshC+1)*(nMeshC+1)];//�_�̑��݊m��
float betaC = 3.0;   //fBm�̎w����
float sigma0C = 0.25;//�W���΍�
int seedC = 0;       //�����̼���
float bias = 0.5;    //�傫������Ɛ���Ԃ������Ȃ�

void makeCloudData()
{
  int i, j;
  CMidpoint2 mp2 = CMidpoint2();
  mp2.numDiv = 8;
  mp2.numMax = nMeshC;
  mp2.sigma0 = sigma0C;
  mp2.beta = betaC;
  //�����f�[�^
  for(j = 0; j <= mp2.numDiv; j++)
		for(i = 0; i <= mp2.numDiv; i++) 
			mp2.h[i][j] = - bias + (float)rand() / (float)RAND_MAX;;
  mp2.create(seedC, dataC);
}

