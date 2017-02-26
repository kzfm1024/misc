int main(void)
{
    char *char_ptr;
    int *int_ptr;
    int int_array[2];
    
    // char_ptr は配列の最初の要素を指す。
    char_ptr = (char *) int_array;
    
    // int_ptr をintの先頭から1バイト後ろに設定する。
    // intは1バイトにならないので、int_ptrはワード境界にそろっていないことになる。
    int_ptr = (int *) (char_ptr+1);
    
    *int_ptr = 1; // ここでバスエラーが生じる。
    
    return 0;
}

