//fft.h
class CFFT
{
public:
  float K1, K2, L1, L2;//遮断次数
  float beta, amp0;
  int flag, N;

  CFFT();
	CFFT(int k1,int k2,int l1,int l2,float beta0,float a0,int flag0, int nMesh);
	void makeTable(float* tblSin,float* tblCos);
  void makeAmpPhase(float* amp, float* phi);
  void makeFourier(float* amp, float* phi, float* sr, float* si);
  void fft1d(float* fr, float* fi, float* tblSin, float* tblCos);
  void ifft2d(float* sr, float* si, float* tblSin, float* tblCos);
};

CFFT::CFFT()
{
  K1 = 1;
  K2 = 63;
  L1 = 1;
  L2 = 63;
  beta = 3.0;
  amp0 = 0.1;
  flag = 1;//fft
  N = 128;
}

CFFT::CFFT(int k1,int k2,int l1,int l2,float beta0,float a0,int flag0, int nMesh)
{
  K1 = k1;
  K2 = k2;
  L1 = l1;
  L2 = l2;
  beta = beta0;
  amp0 = a0;
  flag = flag0;//fft
  N = nMesh;
}

//sin，cosテーブル作成
void CFFT::makeTable(float* tblSin,float* tblCos)
{
  short i;
  double cc,arg;

  cc = - 2.0 * M_PI * (double)flag / (double)N;
  for(i = 0; i < N; i++)
  {
	  arg = (double)i*cc;
	  tblSin[i] = sin(arg);
	  tblCos[i] = cos(arg);
  }
}

//振幅と位相を作成
void CFFT::makeAmpPhase(float* amp, float* phi)
{
  int k, l;

  for(l = 0; l < N; l++)
	for(k = 0; k < N; k++){ amp[k + l * N] = 0.0; phi[k + l * N] = 0.0;}

  for(l = L1; l < L2; l++)
  {
		for (k = K1; k < K2; k++)
		{
			phi[k+l*N] = (float)(2.0 * M_PI * (double)rand() / (double)RAND_MAX);
			amp[k+l*N] = (float)(0.5 * (amp0 / pow((double)(k*k+l*l), beta/4.0)));
			phi[N-k+l*N] = (float)(2.0 * M_PI * (double)rand() / (double)RAND_MAX);
			amp[N-k+l*N] = (float)(0.5 * (amp0 / pow((double)((N-k)*(N-k)+l*l), beta/4.0)));
		}
  }

}
//スペクトルデータ作成
void CFFT::makeFourier(float* amp, float* phi, float* sr, float* si)
{
  int k, l;

  for(l = 0; l < N; l++)
	for(k = 0; k < N; k++){ sr[k + l * N] = 0.0; si[k + l * N] = 0.0;}

  for(l = L1; l < L2; l++)
  {
		for (k = K1; k < K2; k++)
		{
			sr[k + l * N] = (float)(amp[k + l * N] * cos(phi[k + l * N]));
			si[k + l * N] = (float)(amp[k + l * N] * sin(phi[k + l * N]));
			sr[N-k + l * N] = (float)(amp[k + l * N] * cos(phi[N-k + l * N]));
			si[N-k + l * N] = (float)(amp[k + l * N] * sin(phi[N-k + l * N]));
		}
  }

  for (l = L1; l < L2; l++)
  {
		for (k = K1; k < K2; k++)
		{
			sr[k + (N - l) * N] = sr[N - k + l * N];
			si[k + (N - l) * N] = - si[N - k + l * N];
			sr[N - k + (N - l) * N] = sr[k + l * N];
			si[N - k + (N - l) * N] = - si[k + l * N];
		}
  }
}

void CFFT::fft1d(float* fr, float* fi, float* tblSin, float* tblCos)
{
  int i,m,iw,j,l,lp,lp2,n2,k;
  double c,s,wr,wi,xa,ya;

  //Bit Reversal
  j = 0;
  for(i = 0; i < N-1; i++)
  {
		if(i < j)
		{
			xa = fr[i]; fr[i] = fr[j]; fr[j] = xa;
			ya = fi[i]; fi[i] = fi[j]; fi[j] = ya;
		}
		n2 = N/2;
		while(j >= n2) {j = j-n2; n2 = n2/2; }
		j += n2;
  }

  //fft
  m = 0;
  n2 = N;
  while(n2 != 1) { m++; n2 = n2/2; }

  for(l=1; l<=m; l++)
  {
		lp = (short)pow(2.0,(double)l);
		lp2 = lp/2;
		k = 0;
		for(j=0; j<lp2; j++)
		{
			c = tblCos[k]; s = tblSin[k];
			k += N/lp;
			for(i=j; i<N; i += lp)
			{
				iw = i+lp2;
				//ﾊﾞﾀﾌﾗｲ演算
				wr = fr[iw]*c-fi[iw]*s;
				wi = fr[iw]*s+fi[iw]*c;
				fr[iw] = fr[i]-wr; fi[iw] = fi[i]-wi;
				fr[i] = fr[i]+wr; fi[i] = fi[i]+wi;
			}
		}
  }
  if(flag == 1)
  {//FFT
	  for(i=0; i<N; i++)
	  {
		  fr[i] /= (float)N; fi[i] /= (float)N;
	  }
  }
}

void CFFT::ifft2d(float* sr, float* si, float* tblSin, float* tblCos)
{
  float fr[128], fi[128];
  int i, j;

  //各行IFFT
  for (j = 0; j < N; j++)
  {
	  for (i = 0; i < N; i++) { fr[i] = sr[i + j*N];  fi[i] = si[i + j*N]; }

	  fft1d(fr, fi, tblSin, tblCos);
	  for (i = 0; i < N; i++)
		{
			sr[i + j*N] = fr[i]; si[i + j*N] = fi[i];
		}
  }
  
  //さらに各列IFFT
  for (i = 0; i < N; i++)
  {
		for (j = 0; j < N; j++) 
		{ fr[j] = sr[i + j*N];  fi[j] = si[i + j*N]; }

		fft1d(fr, fi, tblSin, tblCos);
		for (j = 0; j < N; j++)
		{
			sr[i + j*N] = fr[j]; //実部だけでよい
		}
  }
}



