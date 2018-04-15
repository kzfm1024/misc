/*-----------------------------*/
/*  クイックソート cp_qsort.c  */
/*-----------------------------*/
void quick_sort(int top, int end);
int d[10];

int main(void)
{
    int i;

    d[0] = 60; d[1] = 20; d[2] = 10; d[3] = 90; d[4] = 50;
    d[5] = 30; d[6] = 0;  d[7] = 40; d[8] = 80; d[9] = 70;

    quick_sort(0, 9);
    for (i=0; i<=9; i++) {
        printf("%d ", d[i]);
    }
    printf("\n");
    return 0;
}

void quick_sort(int top, int end)
{
    int key, wk, i, j;

    key = d[(top+end)/2];
    i = top-1; j = end+1;
    while (1) {
        while (d[++i] < key)
            ;
        while (d[--j] > key)
            ;
        if (i >= j) break;
        wk = d[i]; d[i] = d[j]; d[j] = wk;
    }
    if (top < i-1) quick_sort(top, i-1);  // 左半分をクイックソート
    if (j+1 < end) quick_sort(j+1, end);  // 右半分をクイックソート
}
