//spline.h

//LU�����ɂ��A��1���������̉�@(��ʂ̏ꍇ)
//LU�������[�`��(LU���������̌W���z��Ɋi�[)
void LUDecomposition(int n, float a[][20])
{
    int i, j, k;
    float w;

    //�ŏ��̍s(Upper)
    for(j = 1; j < n; j++) a[0][j] = a[0][j] / a[0][0];

    for(k = 1; k < n-1; k++)
    {
        //Lower
        for(i = k; i < n; i++)
        {
            w = a[i][k];
            for(j = 0; j < k; j++) w -= a[i][j] * a[j][k];
            a[i][k] = w;
        }
        //Upper
        for(j = k+1; j < n; j++)
        {
            w = a[k][j];
            for(i = 0; i < k; i++) w -= a[k][i] * a[i][j];
            a[k][j] = w / a[k][k];
        }
    }
    w = a[n-1][n-1];
    for(j = 0; j < n-1; j++) w -= a[n-1][j] * a[j][n-1];
    a[n-1][n-1] = w;
}
//-----------------------------------------------------------------
//LU���������E�t����ɂ��A��1��������������
void SolutionByLU(int n, float a[][20], float* b)
{
    int i, j;
    float w;

    //���
    b[0] = b[0] / a[0][0];
    for(i = 1; i < n; i++)
    {
        w = b[i];
        for(j = 0; j < i; j++) w -= a[i][j] * b[j];
        b[i] = w / a[i][i];
    }
    //�t���
    for(i = n-2; i >= 0; i--)
    {
        w = b[i];
        for(j = i+1; j < n; j++) w -= a[i][j] * b[j];
        b[i] = w;
    }
}
//----------------------------------------------------------------------------
//�R���X�v���C���Ȑ��ɂ����(3�����j
void spline3( float p[][3], float q[][3], int numControl, int numDivision)
{
    int i, j, k, ip;
    int n = numControl;
    int m = numControl - 1;//segment��
    float s[20];//segment�̒���
    float a[20][20], b[20][3];
    float u, u0;
    float w0[3], w1[3], w2[3], w3[3];
    float bb[20];

    //segment��
    //�W���}�g���N�X������
    for(j = 0; j < n; j++){
        for(i = 0; i < n; i++) a[i][j] = 0.0;
        b[i][0] = b[i][1] = b[i][2] = 0.0;
    }
    for(i = 0; i < m; i++)
        s[i] = sqrt( (p[i+1][0]-p[i][0]) * (p[i+1][0]-p[i][0])
                   + (p[i+1][1]-p[i][1]) * (p[i+1][1]-p[i][1]) 
                   + (p[i+1][2]-p[i][2]) * (p[i+1][2]-p[i][2]) );

    if(p[n-1][0] == p[0][0] && p[n-1][1] == p[0][1]
        && p[n-1][2] == p[0][2]) goto syuuki;//�n�_�ƏI�_������

    //���R���E����
    a[0][0] = 2.0f * s[0];
    a[0][1] = s[0];
    for(i = 1; i < n-1; i++)
    {
        a[i][i-1] = s[i];
        a[i][i] = 2.0f * (s[i-1] + s[i]);
        a[i][i+1] = s[i-1];
    }
    a[n-1][n-2] = s[n-2];
    a[n-1][n-1] = 2.0f * s[n-2];

    //LU����
    LUDecomposition(n, a);

    //�萔�x�N�g��
    for(k = 0; k < 3; k++)//k=0:x, k=1:y, k=2:z
    {
        b[0][k] = 3.0f * (p[1][k] - p[0][k]);
        for(i = 1; i < n-1; i++)
        {
            b[i][k] = 3.0f * ((p[i][k]-p[i-1][k]) * s[i] * s[i]
              + (p[i+1][k]-p[i][k]) * s[i-1] * s[i-1]) / (s[i-1]*s[i]);
        }
        b[n-1][k] = 3.0f * (p[n-1][k] - p[n-2][k]);
    }

    //x���W�̉�
    for(i = 0; i < n; i++) bb[i] = b[i][0];
    SolutionByLU(n, a, bb);
    for(i = 0; i < n; i++) b[i][0] = bb[i];
    //�����W�̉�
    for(i = 0; i < n; i++) bb[i] = b[i][1];
    SolutionByLU(n, a, bb);
    for(i = 0; i < n; i++) b[i][1] = bb[i];
    //�����W�̉�
    for(i = 0; i < n; i++) bb[i] = b[i][2];
    SolutionByLU(n, a, bb);
    for(i = 0; i < n; i++) b[i][2] = bb[i];
    //1�����W����b[i][0],b[i][1],b[i][2]�Ɋi�[����Ă���
    //���
    for(k = 0; k < 3; k++)
    {
        for(i = 0; i < m; i++)
        {
            w0[k] = p[i][k];
            w1[k] = b[i][k];//1�����W��
            w2[k] = 3.0f * (p[i+1][k]-p[i][k])/(s[i]*s[i]) - (2.0f*b[i][k]+b[i+1][k])/s[i];
            w3[k] = 2.0f * (p[i][k]-p[i+1][k])/(s[i]*s[i]*s[i]) + (b[i][k]+b[i+1][k])/(s[i]*s[i]);
            u0 = s[i] / numDivision;
            for(j = 0; j < numDivision; j++)
            {
                u = u0 * (float)j;
                q[numDivision * i + j][k] = w0[k] + w1[k]*u + w2[k]*u*u + w3[k]*u*u*u;
            }
        }
    }
    //�ŏI�_
    q[m*numDivision][0] = p[n-1][0];
    q[m*numDivision][1] = p[n-1][1];
    q[m*numDivision][2] = p[n-1][2];
    return;

syuuki:;
    //�������E����
    a[0][0] = 2.0f * (s[0] + s[n-2]);
    a[0][1] = s[n-2];
    a[0][n-2] = s[0];
    for(i = 1; i < n-1; i++)
    {
        a[i][i-1] = s[i];
        a[i][i] = 2.0f * (s[i-1] + s[i]);
        a[i][i+1] = s[i-1];
    }

    //LU����
    LUDecomposition(n-1, a);

    //�萔�x�N�g��
    for(k = 0; k < 3; k++)//k=0:x, k=1:y, k=2:z
    {
        b[0][k] = 3.0f * ((p[1][k] - p[0][k])*s[n-2]*s[n-2]
                        - (p[n-2][k]-p[0][k]) *s[0]*s[0]) / (s[0]*s[n-2]);
        for(i = 1; i < n-1; i++)
        {
            b[i][k] = 3.0f * ((p[i][k]-p[i-1][k]) * s[i] * s[i]
              + (p[i+1][k]-p[i][k]) * s[i-1] * s[i-1]) / (s[i-1]*s[i]) ;
        }
    }

    //x���W�̉�
    for(i = 0; i < n-1; i++) bb[i] = b[i][0];
    SolutionByLU(n-1, a, bb);
    for(i = 0; i < n-1; i++) b[i][0] = bb[i];
    //�����W�̉�
    for(i = 0; i < n-1; i++) bb[i] = b[i][1];
    SolutionByLU(n-1, a, bb);
    for(i = 0; i < n-1; i++) b[i][1] = bb[i];
    //z���W�̉�
    for(i = 0; i < n-1; i++) bb[i] = b[i][2];
    SolutionByLU(n-1, a, bb);
    for(i = 0; i < n-1; i++) b[i][2] = bb[i];
    //1�����W����b[i][0],b[i][1],b[i][2]�Ɋi�[����Ă���
    //���
    for(k = 0; k < 3; k++)
    {
        for(i = 0; i < m; i++)
        {   ip = i+1;
            if(ip == m) ip = 0;
            w0[k] = p[i][k];
            w1[k] = b[i][k];//1�����W��
            w2[k] = 3.0f * (p[ip][k]-p[i][k])/(s[i]*s[i]) - (2.0f*b[i][k]+b[ip][k])/s[i];
            w3[k] = 2.0f * (p[i][k]-p[ip][k])/(s[i]*s[i]*s[i]) + (b[i][k]+b[ip][k])/(s[i]*s[i]);
            u0 = s[i] / numDivision;
            for(j = 0; j < numDivision; j++)
            {
                u = u0 * (float)j;
                q[numDivision * i + j][k] = w0[k] + w1[k]*u + w2[k]*u*u + w3[k]*u*u*u;
            }
        }
    }
    //�ŏI�_
    q[m*numDivision][0] = p[n-1][0];
    q[m*numDivision][1] = p[n-1][1];
    q[m*numDivision][2] = p[n-1][2];
}

//----------------------------------------------------------------------------
//�R���X�v���C���Ȑ��ɂ����(2�����j
void spline2( float p[][2], float q[][2], int numControl, int numDivision)
{
    int i, j, k, ip;
    int n = numControl;
    int m = numControl - 1;//segment��
    float s[20];//segment�̒���
    float a[20][20], b[20][2];
    float u, u0;
    float w0[3], w1[3], w2[3], w3[3];
    float bb[20];

    //segment��
    //�W���}�g���N�X������
    for(j = 0; j < n; j++){
        for(i = 0; i < n; i++) a[i][j] = 0.0;
        b[i][0] = b[i][1] = 0.0;
    }
    for(i = 0; i < m; i++)
        s[i] = sqrt( (p[i+1][0]-p[i][0]) * (p[i+1][0]-p[i][0])
                   + (p[i+1][1]-p[i][1]) * (p[i+1][1]-p[i][1]) );

    if(p[n-1][0] == p[0][0] && p[n-1][1] == p[0][1]) goto syuuki;//�n�_�ƏI�_������

    //���R���E����
    a[0][0] = 2.0f * s[0];
    a[0][1] = s[0];
    for(i = 1; i < n-1; i++)
    {
        a[i][i-1] = s[i];
        a[i][i] = 2.0f * (s[i-1] + s[i]);
        a[i][i+1] = s[i-1];
    }
    a[n-1][n-2] = s[n-2];
    a[n-1][n-1] = 2.0f * s[n-2];

    //LU����
    LUDecomposition(n, a);

    //�萔�x�N�g��
    for(k = 0; k < 2; k++)//k=0:x, k=1:y
    {
        b[0][k] = 3.0f * (p[1][k] - p[0][k]);
        for(i = 1; i < n-1; i++)
        {
            b[i][k] = 3.0f * ((p[i][k]-p[i-1][k]) * s[i] * s[i]
              + (p[i+1][k]-p[i][k]) * s[i-1] * s[i-1]) / (s[i-1]*s[i]);
        }
        b[n-1][k] = 3.0f * (p[n-1][k] - p[n-2][k]);
    }

    //x���W�̉�
    for(i = 0; i < n; i++) bb[i] = b[i][0];
    SolutionByLU(n, a, bb);
    for(i = 0; i < n; i++) b[i][0] = bb[i];
    //�����W�̉�
    for(i = 0; i < n; i++) bb[i] = b[i][1];
    SolutionByLU(n, a, bb);
    for(i = 0; i < n; i++) b[i][1] = bb[i];
    //1�����W����b[i][0],b[i][1]�Ɋi�[����Ă���
    //���
    for(k = 0; k < 2; k++)
    {
        for(i = 0; i < m; i++)
        {
            w0[k] = p[i][k];
            w1[k] = b[i][k];//1�����W��
            w2[k] = 3.0f * (p[i+1][k]-p[i][k])/(s[i]*s[i]) - (2.0f*b[i][k]+b[i+1][k])/s[i];
            w3[k] = 2.0f * (p[i][k]-p[i+1][k])/(s[i]*s[i]*s[i]) + (b[i][k]+b[i+1][k])/(s[i]*s[i]);
            u0 = s[i] / numDivision;
            for(j = 0; j < numDivision; j++)
            {
                u = u0 * (float)j;
                q[numDivision * i + j][k] = w0[k] + w1[k]*u + w2[k]*u*u + w3[k]*u*u*u;
            }
        }
    }
    //�ŏI�_
    q[m*numDivision][0] = p[n-1][0];
    q[m*numDivision][1] = p[n-1][1];
    return;

syuuki:;
    //�������E����
    a[0][0] = 2.0f * (s[0] + s[n-2]);
    a[0][1] = s[n-2];
    a[0][n-2] = s[0];
    for(i = 1; i < n-1; i++)
    {
        a[i][i-1] = s[i];
        a[i][i] = 2.0f * (s[i-1] + s[i]);
        a[i][i+1] = s[i-1];
    }

    //LU����
    LUDecomposition(n-1, a);

    //�萔�x�N�g��
    for(k = 0; k < 2; k++)//k=0:x, k=1:y
    {
        b[0][k] = 3.0f * ((p[1][k] - p[0][k])*s[n-2]*s[n-2]
                        - (p[n-2][k]-p[0][k]) *s[0]*s[0]) / (s[0]*s[n-2]);
        for(i = 1; i < n-1; i++)
        {
            b[i][k] = 3.0f * ((p[i][k]-p[i-1][k]) * s[i] * s[i]
              + (p[i+1][k]-p[i][k]) * s[i-1] * s[i-1]) / (s[i-1]*s[i]) ;
        }
    }

    //x���W�̉�
    for(i = 0; i < n-1; i++) bb[i] = b[i][0];
    SolutionByLU(n-1, a, bb);
    for(i = 0; i < n-1; i++) b[i][0] = bb[i];
    //�����W�̉�
    for(i = 0; i < n-1; i++) bb[i] = b[i][1];
    SolutionByLU(n-1, a, bb);
    for(i = 0; i < n-1; i++) b[i][1] = bb[i];
    //1�����W����b[i][0],b[i][1]�Ɋi�[����Ă���
    //���
    for(k = 0; k < 2; k++)
    {
        for(i = 0; i < m; i++)
        {   ip = i+1;
            if(ip == m) ip = 0;
            w0[k] = p[i][k];
            w1[k] = b[i][k];//1�����W��
            w2[k] = 3.0f * (p[ip][k]-p[i][k])/(s[i]*s[i]) - (2.0f*b[i][k]+b[ip][k])/s[i];
            w3[k] = 2.0f * (p[i][k]-p[ip][k])/(s[i]*s[i]*s[i]) + (b[i][k]+b[ip][k])/(s[i]*s[i]);
            u0 = s[i] / numDivision;
            for(j = 0; j < numDivision; j++)
            {
                u = u0 * (float)j;
                q[numDivision * i + j][k] = w0[k] + w1[k]*u + w2[k]*u*u + w3[k]*u*u*u;
            }
        }
    }
		    //�ŏI�_
    q[m*numDivision][0] = p[n-1][0];
    q[m*numDivision][1] = p[n-1][1];

}