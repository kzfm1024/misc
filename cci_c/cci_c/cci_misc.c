/**********************************************************/
/*    filename:cci_misc.c �G�֐�                          */
/**********************************************************/
#include "cci.h"
#include "cci_prot.h"

#define MAX_ERR 10                                      /* ���e�ő�G���[�� */

/* ������1�o�C�g�ڂȂ犿���R�[�h�̃o�C�g���A�����łȂ����0��Ԃ� */
int is_kanji(int ch) /* �����Ȃ�\���o�C�g����Ԃ� */
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

/* �������[�h����: 'S':SHIFT-JIS 'E':EUC-JP 'U':UTF-8 0:����ȊO ��Ԃ� */
int get_kanji_mode(void) /* �����R�[�h��ʂ�Ԃ� */
{
  int ret = 0;
  static unsigned char kan[]="��";    /* �����R�[�h�𒲂ׂ邽�߂̃T���v������ */

  if (kan[0]==0x8a && kan[1]==0xbf)      ret = 'S';              /* SHIFT-JIS */
  else if (kan[0]==0xb4 && kan[1]==0xc1) ret = 'E';                 /* EUC-JP */
  else if (kan[0]==0xe6 && kan[1]==0xbc && kan[2]==0xa2) ret = 'U';  /* UTF-8 */
  return ret;
}

char *s_malloc(char *s) /* ������m�� */
{
  char *p = (char *)malloc(strlen(s)+1);                      /* +1��'\0'�̕� */
  if (p == NULL) { fprintf(stderr, "out of memory(malloc)\n"); exit(1); }
  else           { strcpy(p, s); return p; }
}

/* �m�F�t���� *var ��1���� */
void incVar(int *var, int size, char *errmsg) /* �m�F�t��+1 */
{
  if (*var >= size) err_fi(errmsg, size); else ++(*var);
}

void err_s(char *s) /* �G���[�\�� */
{
  err_ss(s, "");
}

void err_fi(char *fmt, int idt) /* �G���[�\�� */
{
  char ss[100];
  sprintf(ss, fmt, idt);
  err_ss(ss, "");
}

void err_ss(char *s1, char *s2) /* �G���[�\�� */
{
  extern int err_ct;                              /* ���G���[��(cci_pars.c) */
  static int olderr_lineno = -1;

  if (olderr_lineno == get_lineNo()) return;   /* ����s�ł͍ŏ���1�����\�� */
  if (++err_ct > MAX_ERR) {
    fprintf(stderr, "�G���[���� %d �𒴂��܂����B\n", MAX_ERR); exit(1);
  }
  fprintf(stderr, "#%d error: ", get_lineNo());
  if (*s2 == '\0') fprintf(stderr, "%s\n", s1);
  else             fprintf(stderr, "%s(%s)\n", s1, s2);
  olderr_lineno = get_lineNo();
}
