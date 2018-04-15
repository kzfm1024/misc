/**********************************************************/
/*    filename:cci_tbl.c �L���\����                       */
/**********************************************************/
#include "cci.h"
#include "cci_prot.h"

/* ----------------------------�e�[�u�� */
#define TBL_MAX    1000      /* �L���\�̃T�C�Y   */
#define start_GTBL 1         /* ���p�̊J�n�ʒu */
#define LTBL_EMPTY 9999      /* �Ǐ��L���\��   */
SymTbl table[TBL_MAX+1];     /* �L���\           */
int start_LTBL = LTBL_EMPTY; /* �Ǐ��p�̊J�n�ʒu */
int tableCt = 0;             /* �g�p���̍ŏI�ʒu */

SymTbl *enter(SymTbl tb, SymKind kind) /* �L���\�o�^ */
{
  int tbNo;

  if ((kind==varId || kind==paraId) && tb.dtTyp==VOID_T) {
    err_s("�ϐ��^���(void)");
    tb.dtTyp = INT_T;                                            /* int�ɂ��� */
  }
  tb.nmKind = kind;
  nameChk(tb);                                       /* �����ƕϐ����̏d��chk */

  if (tableCt >= TBL_MAX) { err_s("�L���\ over"); exit(1); }
  tbNo = ++tableCt;                                             /* �L���\�ԍ� */
  table[tbNo] = tb;                                           /* �L���\�ɓo�^ */

  if (kind == paraId) ++table[tbNo].level;  /* �������֐��u���b�N�ɓ���钲�� */
  if (kind == varId) set_adrs(&table[tbNo]);          /* �ϐ��Ȃ�A�h���X�ݒ� */
  if (kind == fncId) table[tbNo].adrs = -tbNo;    /* �L���\�ԍ��̕��l���֐��� */
                                          /* �Ԓn�Ƃ��ĉ��ݒ�(�Ԓn������̈�) */
  return &table[tbNo];                                /* �L���\�|�C���^��Ԃ� */
}

void localTBL_open(void) /* �Ǐ��L���\�J�n */
{
  start_LTBL = tableCt + 1;                     /* �Ǐ��p�L���\�J�n�ʒu�ݒ� */
}

void localTBL_close(SymTbl *fp) /* �Ǐ��L���\�I�� */
{                         /* �������o�^����̈�Ɏc�����߈��������������� */
  tableCt = start_LTBL - 1 + fp->args;                  /* ���p�ŏI�ʒu�� */
  start_LTBL = LTBL_EMPTY;                                /* �Ǐ��L���\�N���A */
}

SymTbl *search(char *s) /* �L���\����(�T�[�r�X����) */
{
  SymTbl *p;
  static SymTbl *dmy_p=NULL, wkTb = {"tmp$name2",varId,INT_T,0,0,0,0};

  p = search_name(s);
  if (p == NULL) {                                    /* �Ȃ���Ή��ϐ���Ԃ� */
    err_ss("����`�̎��ʎq", s);
    if (dmy_p == NULL) dmy_p = enter(wkTb, varId);              /* ���ϐ��o�^ */
    p = dmy_p;
  }
  return p;
}

SymTbl *search_name(char *s) /* �L���\���� */
{
  int i;                         /* ��:�Ǐ��L���\���Ȃ��Ƃ���start_LTBL��9999 */
  for (i=tableCt; i>=start_LTBL; i--) {                 /* ��ɋǏ��̈��T�� */
    if (strcmp(table[i].name,s)==0) return table + i;
  }
  for (; i>=start_GTBL; i--) {                          /* ���ɑ��̈��T�� */
    if (table[i].nmKind!=paraId && strcmp(table[i].name,s)==0)
      return table + i;
  }
  return NULL;                                                    /* �Ȃ����� */
}

void nameChk(SymTbl tb) /* �����ƕϐ��̖��O�d���`�F�b�N */
{
  SymTbl *p;
  extern int blkNest;                                         /* (cci_pars.c) */
  int nest = blkNest;

  if (tb.nmKind!=paraId && tb.nmKind!=varId) return;
  if ((p = search_name(tb.name)) == NULL) return;
  if (tb.nmKind == paraId) ++nest;                  /* ���������p�ɋL���撲�� */
  if (p->level >= nest) err_ss("���ʎq���d�����Ă���", tb.name);
}

void del_fncTable(SymTbl *f1, SymTbl *f2) /* �d���o�^�̉��� */
{
  int i;
  if (f1 == NULL) return;
  if (f1->dtTyp!=f2->dtTyp || f1->args!=f2->args)  return;
  if (f1->nmKind==protId && f2->nmKind==fncId) {  /* f1���v���g�Af2���{�̂Ȃ� */
    for (i=0; i<=f2->args; i++) { *(f1+i) = *(f2+i); }      /* f2��f1�ɃR�s�[ */
  }
  tableCt -= (f2->args + 1);                              /* �V�����o�^����� */
  return;
}

int b_flg(SymTbl *tp) /* �Ǐ��ϐ��Ȃ�1��Ԃ� */
{
  if (tp->level == 0) return 0; else return 1;                   /* ���Ȃ�0 */
}

SymTbl *tb_ptr(int n) /* �L���\�A�h���X��Ԃ� */
{
  return &table[n];
}
