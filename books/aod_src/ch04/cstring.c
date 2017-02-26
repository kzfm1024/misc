#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *str;
    int len;
} CString;

CString *Init_CString(char *str)
{
    //
    CString *p = malloc(sizeof(CString));
    p->len = strlen(str);

    // 
    p->str = malloc(p->len + 1);
    strncpy(p->str, str, strlen(str) + 1);
    return p;
}

void Delete_CString(CString *p)
{
    free(p->str);
    free(p);
}

// CStringの最後の文字を削除して返す
//
char Chomp(CString *cstring)
{
    char lastchar = *(cstring->str + cstring->len - 1);
    // 文字列を1文字短くする
    *(cstring->str + cstring->len - 1) = '\0';
    cstring->len = strlen(cstring->str);
    return lastchar;
}

// char * を a CStringに追加する
//
CString *Append_Chars_To_CString(CString *p, char *str)
{
    char *newstr = malloc(p->len + strlen(str) + 1);
    p->len = p->len + strlen(str);
    
    // p->strを置き換える新しい文字列を作る
    snprintf(newstr, p->len + 1, "%s%s", p->str, str);
    // 古い文字列を解放し CString が新しい文字列を指すようにする
    free(p->str);
    p->str = newstr;
    
    return p;
}

int main(void)
{
    CString *mystr;
    char c;
    
    mystr = Init_CString("Hello!");
    printf("Init:\n str: `%s' len: %d\n", mystr->str, mystr->len);
    c = Chomp(mystr);
    printf("Chomp '%c':\n str:`%s' len: %d\n", c, mystr->str, mystr->len);
    mystr = Append_Chars_To_CString(mystr, " world!");
    printf("Append:\n str: `%s' len: %d\n", mystr->str, mystr->len);
    
    Delete_CString(mystr);
    
    return 0;
}
