/**********************************************************/
/*    filename:cci_misc.c 雑関数                          */
/**********************************************************/
#include "cci.h"
#include "cci_prot.h"

#define MAX_ERR 10                                      /* 許容最大エラー個数 */

/* 漢字第1バイト目なら漢字コードのバイト数、そうでなければ0を返す */
int is_kanji(int ch) /* 漢字なら構成バイト数を返す */
{
  int uch = (unsigned char)ch;
  static int kanji_mode = -1;

  if (kanji_mode == -1) kanji_mode = get_kanji_mode();
  switch (kanji_mode){
  case 'S':                                                      /* SHIFT-JIS */
    return  (0x81<=uch && uch<=0x9f) || (0xe0<=uch && uch<=0xfc) ? 2 : 0;
  case 'E': return uch==0x8f ? 3 : (uch>=0x80 ? 2 : 0);             /* EUC-JP */
  case 'U': return uch>=0xe0 ? 3 : (uch>=0xc0 ? 2 : 0);              /* UTF-8 */
  default:  return 0;
  }
}

/* 漢字モード判定: 'S':SHIFT-JIS 'E':EUC-JP 'U':UTF-8 0:それ以外 を返す */
int get_kanji_mode(void) /* 文字コード種別を返す */
{
  int ret = 0;
  static unsigned char kan[]="漢";    /* 漢字コードを調べるためのサンプル文字 */

  if (kan[0]==0x8a && kan[1]==0xbf)      ret = 'S';              /* SHIFT-JIS */
  else if (kan[0]==0xb4 && kan[1]==0xc1) ret = 'E';                 /* EUC-JP */
  else if (kan[0]==0xe6 && kan[1]==0xbc && kan[2]==0xa2) ret = 'U';  /* UTF-8 */
  return ret;
}

char *s_malloc(char *s) /* 文字列確保 */
{
  char *p = (char *)malloc(strlen(s)+1);                      /* +1は'\0'の分 */
  if (p == NULL) { fprintf(stderr, "out of memory(malloc)\n"); exit(1); }
  else           { strcpy(p, s); return p; }
}

/* 確認付きで *var を1増加 */
void incVar(int *var, int size, char *errmsg) /* 確認付き+1 */
{
  if (*var >= size) err_fi(errmsg, size); else ++(*var);
}

void err_s(char *s) /* エラー表示 */
{
  err_ss(s, "");
}

void err_fi(char *fmt, int idt) /* エラー表示 */
{
  char ss[100];
  sprintf(ss, fmt, idt);
  err_ss(ss, "");
}

void err_ss(char *s1, char *s2) /* エラー表示 */
{
  extern int err_ct;                              /* 総エラー個数(cci_pars.c) */
  static int olderr_lineno = -1;

  if (olderr_lineno == get_lineNo()) return;   /* 同一行では最初の1つだけ表示 */
  if (++err_ct > MAX_ERR) {
    fprintf(stderr, "エラー個数が %d を超えました。\n", MAX_ERR); exit(1);
  }
  fprintf(stderr, "#%d error: ", get_lineNo());
  if (*s2 == '\0') fprintf(stderr, "%s\n", s1);
  else             fprintf(stderr, "%s(%s)\n", s1, s2);
  olderr_lineno = get_lineNo();
}
