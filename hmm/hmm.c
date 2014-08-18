/*
 * 隠れマルコフモデルの演習問題実装 (C)
 * http://www.bioinfo.sfc.keio.ac.jp/class/bioinfo-a/PPT/bioinfo-a09s-6_HMM.pdf
 */
#include <stdio.h>
#include <string.h>
#include <math.h>

#define K (6)
#define L (5)
#define LOG0 (1.0)

#define TYPE_S (0)
#define TYPE_N (1)

static int type[K] = {
	TYPE_S, TYPE_N,
	TYPE_N, TYPE_N,
	TYPE_N, TYPE_N
};

static double t[K][K] = {
    /*  0    1    2    3    4    5 */
	{ 0.0, 1.0, 0.0, 0.0, 0.0, 0.0 }, /* 0 */
	{ 0.0, 0.0, 0.8, 0.2, 0.0, 0.0 }, /* 1 */
	{ 0.0, 0.0, 0.5, 0.0, 0.5, 0.0 }, /* 2 */
	{ 0.0, 0.0, 0.0, 0.0, 0.7, 0.3 }, /* 3 */
	{ 0.0, 0.0, 0.0, 0.0, 0.0, 1.0 }, /* 4 */
	{ 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 }  /* 5 */
};

double viterbi(int l, int i)
{
	int k, k_max;
	double p, p_max;

	switch(type[l])
	{
	case TYPE_S:
		if (i <= 0)
		{
			return 0.0;
		}
		else
		{
			return LOG0;
		}
	case TYPE_N:
		if (i <= 0)
		{
			return LOG0;
		}
		for(p_max = LOG0 - 1, k = 0;k < K;k ++)
		{
			if(tr[k][l] > 0.0)
			{
				p = viterbi(k, i - 1) + log(tr[k][l]);
			}
			else
			{
				p = LOG0;
			}

			if(p > p_max)
			{
				p_max = p;
				k_max = k;
			}
		}
		return p_max + log(e[l][ (int)cton(x[i - 1]) ]);
	}
}

main()
{
	static char x[L];
	strcpy(x, "atga");

	printf("%s\n", x);
}
