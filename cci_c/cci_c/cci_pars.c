/**********************************************************/
/*    filename:cci_pars.c �\�����                        */
/**********************************************************/
#include "cci.h"
#include "cci_prot.h"

Token token;                                          /* ���ݏ������̃g�[�N�� */
int blkNest = 0;                 /* �u���b�N�̐[�� 0:��� 1:�֐� 2:�֐�����{} */
int err_ct = 0;                                               /* ���G���[�� */
SymTbl tmpTb = {"",noId,NON_T,0,0,0,0};                     /* �ꎞ�i�[�L���\ */
SymTbl *fncPt = NULL;                             /* �|�󒆂̊֐��̋L���\�ʒu */
TknKind last_statement;                                 /* �u���b�N���Ō�̕� */

int localAdrs;                                        /* �Ǐ��ϐ��A�h���X�Ǘ� */
#define START_LocalAdrs 1*INTSIZE                     /* �Ǐ��ϐ����t�J�n�Ԓn */
                                             /* �擪4�o�C�g�͖߂�Ԓn�p�Ɋm�� */
#define INT_P(p)   (int *)(p)                          /* int�^�|�C���^�ɕϊ� */
/*---------------------- continue,break�����p*/
#define LOOP_SIZ 20
struct {
  TknKind stkind;                                                 /* ���̎�� */
  int looptop;                              /* ���[�v�J�n�ʒu(continue�̔��) */
  int break_flg;                                   /* break��������ΐ^�ɂ��� */
} loopNest[LOOP_SIZ+1];                               /* ���[�v���̃l�X�g�Ǘ� */
int loopNest_ct = 0;                                      /* �g�p���̍ŏI�ʒu */
/*---------------------- switch�����p*/
#define SWTCH_SIZ 10
struct {
  int def_adrs;                                          /* default�̑Ή��Ԓn */
  int startCaseList;                                  /* caseList[]�̊J�n�ʒu */
} swchNest[SWTCH_SIZ+1];                              /* switch���̃l�X�g�Ǘ� */
int swchNest_ct = 0;                                      /* �g�p���̍ŏI�ʒu */

#define CASE_SIZ 100
struct {
  int value, adrs;                                      /* case�̒l�ƑΉ��Ԓn */
} caseList[CASE_SIZ+1];
int caseList_ct = 0;                                      /* �g�p���̍ŏI�ʒu */

int compile(char *fname) /* �R���p�C�� */
{
  initChTyp();                                                    /* ������\ */
  gencode2(CALL, -1);                         /* main�֐��ďo�B�Ԓn-1�͉��ݒ� */
  gencode1(STOP);                                           /* �v���O�����I�� */

  fileOpen(fname);
  token = nextTkn();
  while (token.kind != EofTkn) {
    switch (token.kind) {
    case Int: case Void:                                        /* �ϐ����֐� */
      set_type(); set_name();                             /* �^���Ɩ��O���i�[ */
      if (token.kind == '(') fncDecl(); else varDecl();      /* �֐�,�ϐ��錾 */
      break;
    case Semicolon:
      token = nextTkn();                                      /* ������������ */
      break;
    default:
      err_ss("�\���G���[", token.text);
      token = nextTkn();
    }
  }
  if (err_ct == 0) backPatch_callAdrs();            /* �֐��ďo����Ԓn���㖄 */
  *INT_P(mem_adrs(0)) = mallocG(0);            /* �ÓI�̈�T�C�Y��0�Ԓn�Ɋi�[ */

  if (err_ct > 0) fprintf(stderr, "%d�̃G���[���������܂����B\n", err_ct);
  return err_ct == 0;                                     /* �G���[�Ȃ��Ȃ�^ */
}

void varDecl(void) /* �ϐ��錾 */
{
  for (;;) {
    set_aryLen();                                         /* �z��Ȃ璷���ݒ� */
    enter(tmpTb, varId);                          /* �ϐ��o�^(�A�h���X���ݒ�) */
    if (token.kind != ',') break;                                 /* �錾�I�� */
    token = nextTkn(); set_name();                              /* ���̕ϐ��� */
  }
  token = chk_nextTkn(token, ';');                               /* ';'�̂͂� */
}

void fncDecl(void) /* �֐��錾 */
{
  SymTbl *f1;

  localAdrs = START_LocalAdrs;                /* �Ǐ��̈抄�t���J�E���^������ */
  f1 = search_name(tmpTb.name);
  if (f1!=NULL && f1->nmKind!=fncId && f1->nmKind!=protId) {
    err_ss("���ʎq���d�����Ă���", f1->name); f1 = NULL;
  }

  fncPt = enter(tmpTb, fncId);                                /* �L���\�ɓo�^ */
  token = nextTkn();
  localTBL_open();                                      /* �Ǐ��p�L���\��p�� */

  switch (token.kind) {                                           /* �������� */
  case Void: token = nextTkn(); break;                         /* ���� (void) */
  case ')':  break;                                            /* �����Ȃ� () */
  default:                                                        /* �������� */
    for (;;) {
      set_type(); set_name(); enter(tmpTb, paraId);               /* �����o�^ */
      ++(fncPt->args);                                        /* ��������+1 */
      if (token.kind != ',') break;                               /* �錾�I�� */
      token = nextTkn();                                          /* ���̈��� */
    }
  }
  token = chk_nextTkn(token, ')');                               /* ')'�̂͂� */

  if (token.kind == ';') fncPt->nmKind = protId;          /* �֐��v���g�^�C�v */
  set_adrs(fncPt);                          /* �֐������Ԓn�ƈ����A�h���X�ݒ� */
  if (f1 != NULL) fncChk(f1, fncPt);                    /* �֐��̐��������m�F */

  switch (token.kind) {
  case ';': token = nextTkn(); break;                         /* �v���g�^�C�v */
  case '{':
    if (IS_MAIN(fncPt)) set_main();                           /* main�֐����� */
    fncDecl_begin();                                          /* �֐��������� */
    block(1);                                                 /* �֐��{�̏��� */
    fncDecl_end();                                            /* �֐��o������ */
    break;
  default:  err_s("�֐��� ; �܂��� { ���Ȃ�"); exit(1);
  }
  localTBL_close(fncPt);                                /* �Ǐ��p�L���\������ */
  del_fncTable(f1, fncPt);                                /* �d���o�^�Ȃ���� */
  fncPt = NULL;                                               /* �֐������I�� */
}

void fncChk(SymTbl *f1, SymTbl *f2) /* �֐��d���`�F�b�N */
{
  if (f1->nmKind==fncId && f2->nmKind==fncId) {             /* �������֐��{�� */
    err_ss("�֐����Ē�`����Ă���", f1->name); return;
  }
                                              /* �Е�or�������֐��v���g�^�C�v */
  if (f1->dtTyp != f2->dtTyp || f1->args != f2->args) {
    err_ss("�֐��v���g�^�C�v���s��v", f1->name); return;
  }
  return;
}

void fncDecl_begin(void) /* �֐��������� */
{
  int i;
  gencode2(ADBR, 0);             /* �t���[���m�ہB�T�C�Y��fncDecl_end()�Ō㖄 */
  gencode3(STO, LOCAL_F, 0);                                    /* �ߔԒn�i�[ */
  for (i=fncPt->args; i>=1; i--)
    gencode3(STO, LOCAL_F, (fncPt+i)->adrs);                    /* �������i�[ */
}

void fncDecl_end(void) /* �֐��o������ */
{
  backPatch(fncPt->adrs, -localAdrs);   /* �t���[���T�C�Y�㖄(�֐��擪��ADBR) */
  if (last_statement != Return) {                   /* �֐�������return���Ȃ� */
    if (IS_MAIN(fncPt)) gencode2(LDI, 0);             /* main�֐��Ȃ�ߒl�ݒ� */
    else if (fncPt->dtTyp != VOID_T)                    /* �{�����n�̓Ǝ��d�l */
      err_s("�{�����n�ł͔�void�^�֐��̖����ɂ͒P�Ƃ�return�����K�v�ł��B");
  }
  backPatch_RET(fncPt->adrs);           /* RET���ߗ�֔��JMP�̔��Ԓn���㖄 */
  gencode3(LOD, LOCAL_F, 0);                  /* �ߔԒn���I�y�����h�X�^�b�N�� */
  gencode2(ADBR, localAdrs);                                /* �t���[������� */
  gencode1(RET);                                                  /* �߂薽�� */
}

void set_main(void) /* main�֐����� */
{
  if (fncPt->dtTyp != INT_T || fncPt->args != 0)       /* �^�ƈ������̊m�F */
    err_s("main�֐��̏������s��");
  backPatch(0, fncPt->adrs);                   /* 0�Ԓn��CALL���߂̔Ԓn���㖄 */
}

void block(int is_func) /* {}���̏��� */
{
  TknKind kd = Others;

  token = nextTkn();
  ++blkNest;
  if (is_func) {                              /* �֐��u���b�N�Ȃ�ϐ��錾���� */
    while (token.kind == Int) { set_type(); set_name(); varDecl(); }
  }

  while (token.kind != '}') {                                     /* �������� */
    kd = token.kind; statement();
  }
  last_statement = kd;                    /* �֐�������return�L���m�F�ɗp���� */

  --blkNest;
  token = nextTkn();
}

/* ���̊֐����̃��x������(��揈��)�̖������̂��߂Ɉȉ��̃}�N�����g�p���� */
#define UPLABEL(pos)  pos=nextCodeCt()              /* ��W�����v�p���x���@�\ */
#define JMP_UP(pos)   gencode2(JMP,pos)             /* JMP��UPLABEL��         */
#define JPT_UP(pos)   gencode2(JPT,pos)             /* JPT��UPLABEL��         */
#define JMP_DOWN(pos) pos=gencode2(JMP,0)           /* JMP��DWNLABEL��        */
#define JPF_DOWN(pos) pos=gencode2(JPF,0)           /* JPF��DWNLABEL��        */
#define DWNLABEL(pos) backPatch(pos,nextCodeCt())   /* ���W�����v�p���x���@�\ */
void statement(void) /* ���̏��� */
{
  TknKind kd;
  SymTbl *tp;
  DtType ret_typ = fncPt->dtTyp;
  int i, val;
  int LB_TOP, LB_EXP2, LB_EXP3, LB_BODY, LB_ELSE, LB_END, LB_TBL;

  kd = token.kind;

  if (kd==While || kd==Do || kd==Switch) continue_break_begin(kd);
  switch (kd) {
  case Break:
    if (loopNest_ct == 0) err_s("�s����break");
    else {
      gencode2(JMP, NO_FIX_BREAK_ADRS);                     /* �Y�������ʒu�� */
      loopNest[loopNest_ct].break_flg = TRUE;              /* break������L�� */
    }
    token = chk_nextTkn(nextTkn(), ';');                          /* ; �̂͂� */
    break;
  case Continue:
    gencode2(JMP, get_loopTop());                   /* ���[�v�J�n�s�փW�����v */
    token = chk_nextTkn(nextTkn(), ';');                          /* ; �̂͂� */
    break;
  case Case:
    token = nextTkn();
    get_const(NULL);                                          /* �萔���菀�� */
    expr_with_chk(0, ':');                                      /* [��]�̏��� */
    if (!get_const(&val))      err_s("case�����萔���łȂ�");
    else if (swchNest_ct == 0) err_s("�Ή�����switch�����Ȃ�");
    else {
      for (i=swchNest[swchNest_ct].startCaseList; i<=caseList_ct; i++) {
        if (caseList[i].value == val) {
          err_s("case���̒l���d�����Ă���"); break;
        }
      }
      incVar(&caseList_ct, CASE_SIZ, "case�傪%d�𒴂��܂����B");
      caseList[caseList_ct].value = val;                        /* case�l�ݒ� */
      caseList[caseList_ct].adrs = nextCodeCt();              /* �Ή��Ԓn�ݒ� */
    }
    statement();                                                  /* ���̏��� */
    break;
  case Default:
    if (swchNest_ct == 0)
      err_s("�Ή�����switch�����Ȃ�");
    else if (swchNest[swchNest_ct].def_adrs != -1)
      err_s("default���d�����Ă���");
    else
      swchNest[swchNest_ct].def_adrs = nextCodeCt();              /* �Ԓn�ݒ� */
    token = chk_nextTkn(nextTkn(), ':');                          /* : �̂͂� */
    statement();                                                  /* ���̏��� */
    break;
  case For:
    /*[��1]*/
    token = chk_nextTkn(nextTkn(), '(');                          /* ( �̂͂� */
    if (token.kind == ';')
      token = nextTkn();                                           /* ��1�Ȃ� */
    else {
      expr_with_chk(0, ';');                                   /* [��1]�̏��� */
      remove_val();                                               /* ���l�s�v */
    }

    /*[��2]*/
    UPLABEL(LB_EXP2);                               /* ����                   */
    if (token.kind == ';') {                        /*   ����2�Ȃ�            */
      gencode2(LDI, 1);                             /*   ���^(1)�ɂ���        */
      token = nextTkn();                            /*   ��                   */
    } else {                                        /*   ��                   */
      expr_with_chk(0, ';');                        /*   ��[��2]�̏���        */
    }                                               /*   ��                   */
    JPF_DOWN(LB_END);                               /* ������  false��        */
    JMP_DOWN(LB_BODY);                              /* ��������true��         */
                                                    /*   ������               */
    /*[��3]*/                                       /*   ������               */
    continue_break_begin(kd);                       /*   ������               */
    UPLABEL(LB_EXP3);                               /* ����������             */
    if (token.kind == ')')                          /*   ��������             */
      token = nextTkn();                            /*   ����������3�Ȃ�      */
    else {                                          /*   ��������             */
      expr_with_chk(0, ')');                        /*   ��������[��3]�̏���  */
      remove_val();                                 /*   �����������l�s�v     */
    }                                               /*   ��������             */
    JMP_UP(LB_EXP2);                                /* ����������             */
                                                    /*     ������             */
    /*[�{��]*/                                      /*     ������             */
    DWNLABEL(LB_BODY);                              /* ����������             */
    statement();                                    /*     ��  ��[��]�̏���   */
    JMP_UP(LB_EXP3);                                /* �����������J��Ԃ�     */
                                                    /*     ��                 */
    /*[����]*/                                      /*     ��                 */
    DWNLABEL(LB_END);                               /* �������I�[             */
    break;
  case If:
    token = nextTkn();
    expr_with_chk('(', ')');                        /*       [��]�̏���       */
    JPF_DOWN(LB_ELSE);                              /* ����  false��          */
    statement();                                    /*   ��  [��1]�̏���      */
    if (token.kind != Else) {                       /*   ��                   */
      DWNLABEL(LB_ELSE);                            /* ����else�Ȃ����̏I�[   */
      break;                                        /*   ��                   */
    }                                               /*   ��else���鎞         */
    JMP_DOWN(LB_END);                               /* ������                 */
    DWNLABEL(LB_ELSE);                              /* ������                 */
    token = nextTkn();                              /*     ��                 */
    statement();                                    /*     ��[��2]�̏���      */
    DWNLABEL(LB_END);                               /* ������                 */
    break;
  case While:
    token = nextTkn();
    UPLABEL(LB_TOP);                                /* ����                   */
    expr_with_chk('(', ')');                        /*   ��  [��]�̏���       */
    JPF_DOWN(LB_END);                               /* ������false��          */
    statement();                                    /*   ����[��]�̏���       */
    JMP_UP(LB_TOP);                                 /* �������J��Ԃ�         */
    DWNLABEL(LB_END);                               /* ������                 */
    break;
  case Do:
    token = nextTkn();
    UPLABEL(LB_TOP);                                /* ����                   */
    statement();                                    /*   ��[��]�̏���         */
    if (token.kind == While) {                      /*   ��                   */
      token = nextTkn();                            /*   ��                   */
      expr_with_chk('(', ')');                      /*   ��[��]�̏���         */
      token = chk_nextTkn(token, ';');              /*   ��; �̂͂�           */
      JPT_UP(LB_TOP);                               /* ����true��             */
    } else {
      err_s("do�I�[��while���Ȃ�");
    }
    break;
  case Switch:
    token = nextTkn();
    expr_with_chk('(', ')');                        /*      [��]�̏���        */
    JMP_DOWN(LB_TBL);                               /* ����  �e�[�u��������   */
    swch_begin();                                   /*   ��                   */
    statement();                                    /* case,default���̏���.  */
    JMP_DOWN(LB_END);                               /* ������������           */
    DWNLABEL(LB_TBL);                               /* ������                 */
    swch_end();                                     /*     ���e�[�u������     */
    DWNLABEL(LB_END);                               /* ������                 */
    break;
  case Return:
    token = nextTkn();
    if (token.kind == ';') {                                      /* �ߒl�Ȃ� */
      if (ret_typ != VOID_T) err_s("return���ɖ߂�l���Ȃ�");
    } else {                                                      /* �ߒl���� */
      expression();                                               /* �ߒl�쐬 */
      if (ret_typ == VOID_T) err_s("void�^�֐��ɒl��Ԃ�return��������");
    }
    gencode2(JMP, NO_FIX_RET_ADRS);                         /* �֐��o�������� */
    token = chk_nextTkn(token, ';');                              /* ; �̂͂� */
    break;
  case Printf: case Exit:                                   /* void�^�g���֐� */
    sys_fncCall(kd);
    token = chk_nextTkn(token, ';');                  /* printf() + b; ��h�~ */
    break;
  case Input:                                             /* ��void�^�g���֐� */
    expr_with_chk(0, ';');                                    /* �ʏ�̎���� */
    remove_val();                                                 /* ���l�s�v */
    break;
  case Incre: case Decre:                                      /* ++var --var */
    expr_with_chk(0, ';');
    remove_val();                                                 /* ���l�s�v */
    break;
  case Ident:                                           /* ���ʎq(�֐����ϐ�) */
    tp = search(token.text);                                    /* �L���\�ʒu */
    if ((tp->nmKind==fncId || tp->nmKind==protId) && tp->dtTyp==VOID_T) {
      fncCall(tp);                                  /* void�^�֐��͂����ŏ��� */
      token = chk_nextTkn(token, ';');                            /* ; �̂͂� */
    } else {
      expr_with_chk(0, ';');                                  /* �ʏ�̎���� */
      remove_val();                                               /* ���l�s�v */
    }
    break;
  case Lbrace:                                                    /* ������{} */
    block(0);                                             /* 0:��֐��u���b�N */
    break;
  case Semicolon:                                 /* ��,for�̎��s�����ŏo�� */
    token = nextTkn();
    break;
  case EofTkn:                             /* �Ō�� } ��Y�ꂽ�ꍇ�Ȃǂɔ��� */
    err_s("�Ӑ}���Ȃ��I���B'}'�s��? "); exit(1);
  default:
    err_ss("�s���ȋL�q", token.text);
    token = nextTkn();
  }
  if (kd==For || kd==While || kd==Do || kd==Switch) continue_break_end();
}

void continue_break_begin(TknKind stmnt) /* CB�J�n���� */
{
  incVar(&loopNest_ct, LOOP_SIZ, "���䕶�l�X�g��%d��𒴂��܂����B");
  loopNest[loopNest_ct].stkind = stmnt;                           /* ���̎�� */
  loopNest[loopNest_ct].looptop = nextCodeCt();                   /* �擪�Ԓn */
  loopNest[loopNest_ct].break_flg = FALSE;                 /* break���L���Ǘ� */
}

void continue_break_end(void) /* CB�I������ */
{
  if (loopNest[loopNest_ct].break_flg)                       /* break������� */
    backPatch_BREAK(loopNest[loopNest_ct].looptop);             /* �����㖄 */
  --loopNest_ct;                                        /* �l�X�g���ЂƂ� */
}

int get_loopTop(void) /* �������[�v�̐擪�Ԓn��Ԃ� */
{
  int i;
  for (i=loopNest_ct; i>0; i--) {
    if (loopNest[i].stkind != Switch) return loopNest[i].looptop;
  }
  err_s("continue�����[�v�����ɂȂ�");
  return 0;
}

void swch_begin(void) /* switch���̊J�n */
{
  incVar(&swchNest_ct, SWTCH_SIZ, "switch�l�X�g��%d��𒴂��܂����B");
  swchNest[swchNest_ct].def_adrs = -1;                   /* default�Ԓn���ݒ� */
  swchNest[swchNest_ct].startCaseList = caseList_ct+1;
}

void swch_end(void)     /* switch���̏I�� */
{
  int i, start = swchNest[swchNest_ct].startCaseList;

  for (i=start; i<=caseList_ct; i++) {
    gencode2(EQCMP, caseList[i].value);                     /* case�̒l�Ɣ�r */
    gencode2(JPT,  caseList[i].adrs);                       /* �Ή��W�����v�� */
  }
  gencode1(DEL);                                                /* ��r�l�폜 */
  if (swchNest[swchNest_ct].def_adrs != -1) {          /* default���x�������� */
    gencode2(JMP, swchNest[swchNest_ct].def_adrs);
  }
  caseList_ct = start - 1;                      /* caseList�J�E���g�����ɖ߂� */
  --swchNest_ct;                                        /* �l�X�g���ЂƂ� */
}

/* �O��̃g�[�N���`�F�b�N�t����expression()�����s�B�֗��̂��߂ɗp�� */
void expr_with_chk(TknKind k1, TknKind k2) /* ������ */
{
  if (k1 != 0) token = chk_nextTkn(token, k1);
  expression();
  if (k2 != 0) token = chk_nextTkn(token, k2);
}

void expression(void) /* ������ */
{
  term(2);
  if (token.kind == '=') {
    to_leftVal();                                             /* ���Ӓl�ɂ��� */
    token = nextTkn(); expression();
    gencode1(ASSV);                                     /* �������(���l�ݒ�) */
  }
}

void term(int n) /* ���֘A�����Bn�͗D�揇�� */
{
  TknKind kd;

  if (n == 8) { factor(); return; }
  term(n+1);
  while (n == opOrder(token.kind)) {                /* �������������Z�q������ */
    kd = token.kind;
    token = nextTkn(); term(n+1);
    gencode_Binary(kd);                                         /* �񍀉��Z�q */
  }
}

void factor(void) /* ���q�֘A���� */
{
  SymTbl *tp;
  TknKind kd = token.kind;

  switch (kd) {
  case Plus: case Minus: case Not: case Incre: case Decre:     /* + - ! ++ -- */
    token = nextTkn(); factor();
    if (kd==Incre || kd==Decre) to_leftVal();                 /* ���Ӓl�ɂ��� */
    gencode_Unary(kd);                                        /* �P�����ߐ��� */
    break;
  case IntNum:                                                    /* �����萔 */
    gencode2(LDI, token.intVal);
    token = nextTkn();
    break;
  case Lparen:                                                      /* ( �� ) */
    expr_with_chk('(', ')');
    break;
  case Printf: case Input: case Exit:
    if (kd != Input)
      err_ss("void�^�֐������̒��Ŏg���Ă���", token.text);
    sys_fncCall(kd);                                          /* �g���֐��ďo */
    break;
  case Ident:                                                       /* ���ʎq */
    tp = search(token.text);                                    /* �L���\�ʒu */
    switch (tp->nmKind) {
    case fncId: case protId:                                      /* �֐��ďo */
      if (tp->dtTyp == VOID_T)
        err_ss("void�^�֐������̒��Ŏg���Ă���", tp->name);
      fncCall(tp);                                                /* �֐��ďo */
      break;
    case varId: case paraId:                                          /* �ϐ� */
      if (tp->aryLen == 0) {                                      /* �P���ϐ� */
        gencode3(LOD, b_flg(tp), tp->adrs);
        token = nextTkn();
      } else {                                                        /* �z�� */
        token = nextTkn();
        if (token.kind == '[') {                                  /* []������ */
          gencode3(LDA, b_flg(tp), tp->adrs);               /* �z�񖼃A�h���X */
          expr_with_chk('[', ']');                              /* �Y�����擾 */
          gencode2(LDI, INTSIZE);                              /* 1�v�f�T�C�Y */
          gencode1(MUL);                                 /* �Y���~1�v�f�T�C�Y */
          gencode1(ADD);                          /* �����z��A�h���X�ɉ��Z */
          gencode1(VAL);                                          /* ���e��o */
        } else {                                                  /* []���Ȃ� */
          err_s("�Y���w�肪�Ȃ�");                              /* �����͑��s */
        }
      }
      if (token.kind==Incre || token.kind==Decre) {                 /* ��u++ */
        to_leftVal();                           /* ���Ӓl(�A�h���X�\��)�ɂ��� */
        if (token.kind==Incre) {gencode1(INC); gencode2(LDI,1); gencode1(SUB);}
        else                   {gencode1(DEC); gencode2(LDI,1); gencode1(ADD);}
        token = nextTkn();
      }
      break;
    }
    break;
  default:
    err_ss("�s���ȋL�q", token.text);
  }
}

void fncCall(SymTbl *fp) /* �֐��ďo */
{
  int  argCt = 0;

  token = chk_nextTkn(nextTkn(), '(');                            /* ( �̂͂� */
  if (token.kind != ')') {                                      /* ���������� */
    for (;;) {
      expression();                                           /* �������̏��� */
      ++argCt;                                                    /* ������ */
      if (token.kind != ',') break;                       /* , �Ȃ���������� */
      token = nextTkn();
    }
  }
  token = chk_nextTkn(token, ')');                                /* ) �̂͂� */

  if (argCt != fp->args)                                  /* �������`�F�b�N */
    err_ss("�֐��̈��������s��v", fp->name);
  gencode2(CALL, fp->adrs);                                       /* �֐��ďo */
}

void sys_fncCall(TknKind kd) /* �g���֐��ďo */
{
  int fnc_typ = 0;
  char *form;

  token = chk_nextTkn(nextTkn(), '(');                            /* ( �̂͂� */
  switch (kd) {
  case Exit: fnc_typ = EXIT_F; expression(); break;
  case Input: fnc_typ = INPUT_F; break;
  case Printf:
    if (token.kind != String)
      err_s("printf�̑�1�������s��");                           /* �������Ȃ� */
    gencode2(LDI, token.intVal);                        /* ������A�h���X�擾 */
    form = mem_adrs(token.intVal);                                    /* ���� */
    token = nextTkn();
    if (token.kind != ',')
      fnc_typ = PRINTF1_F;                                   /* ����1�^�C�v */
    else {
      fnc_typ = PRINTF2_F;                                   /* ����2�^�C�v */
      token = nextTkn(); expression();                         /* ��2�����]�� */
      if (token.kind == ',')
        err_s("printf�̈�����2�܂łł��B");                   /* ���������� */
      if (!good_format(form))                         /* ���S�̂��߂ɏ����m�F */
        err_s("printf�̏������s��");
    }
    break;
  }
  token = chk_nextTkn(token, ')');                                /* ) �̂͂� */
  gencode2(LIB, fnc_typ);
}

void set_type(void) /* �^�ݒ� */
{
  tmpTb.aryLen = tmpTb.adrs = tmpTb.args = 0;               /* �N���A���Ă��� */
  tmpTb.level = blkNest;                                     /* 0:��� 1:�Ǐ� */
  switch (token.kind) {
  case Int:  tmpTb.dtTyp = INT_T;  break;
  case Void: tmpTb.dtTyp = VOID_T; break;
  default:   err_ss("�^�w����", token.text);
             tmpTb.dtTyp = INT_T;                              /* Int�Ƃ݂Ȃ� */
  }
  token = nextTkn();
}

void set_name(void) /* ���O�ݒ� */
{
  if (token.kind == Ident) {
    tmpTb.name = s_malloc(token.text);                            /* ���O�ݒ� */
    token = nextTkn();
  } else {
    err_ss("�L�q���s�K��", token.text);
    tmpTb.name = "tmp$name1";                       /* ���肦�Ȃ����O�����ݒ� */
  }
}

void set_aryLen(void) /* �z��T�C�Y�ݒ� */
{
  tmpTb.aryLen = 0;
  if (token.kind != '[') return;                                /* �z��łȂ� */

  token = nextTkn();
  if (token.kind == ']') {                                        /* []�ł��� */
    err_s("�Y���w�肪�Ȃ�");
    token = nextTkn(); tmpTb.aryLen = 1; return;         /* 1���������Ƃɂ��� */
  }
  get_const(NULL);                                            /* �萔���菀�� */
  expr_with_chk(0, ']');                                        /* �Y�����擾 */
  if (get_const(&(tmpTb.aryLen))) {                             /* �萔���Ȃ� */
    if (tmpTb.aryLen <= 0) {
      tmpTb.aryLen = 1; err_s("�s���ȓY��");             /* 1���������Ƃɂ��� */
    }
  } else {
    err_s("�z�񕝎w�肪�����萔���łȂ�");
  }

  if (token.kind == '[') {
    err_ss("�������z��͐錾�ł��Ȃ�", token.text);
  }
}

void set_adrs(SymTbl *tp) /* �ϐ��E�����̔Ԓn���� */
{
  int i, size = INTSIZE;

  switch (tp->nmKind) {
  case varId:                                                        /*  �ϐ� */
    if (tp->aryLen > 0) size *= tp->aryLen;             /* �z��Ȃ�T�C�Y�v�Z */
    if (is_global()) tp->adrs = mallocG(size);                    /* ���ϐ� */
    else             tp->adrs = mallocL(size);                    /* �Ǐ��ϐ� */
    break;
  case fncId:                                                        /*  �֐� */
    tp->adrs = nextCodeCt();                          /* �֐������A�h���X�ݒ� */
    for (i=1; i<=tp->args; i++) {
      (tp+i)->adrs = mallocL(size);                     /* �����̃A�h���X���� */
    }
  }
}

int opOrder(TknKind kd) /* �񍀉��Z�q�̗D�揇�� */
{
  switch (kd) {
  case Multi: case Divi:   case Mod:                 return 7;
  case Plus:  case Minus:                            return 6;
  case Less:  case LessEq: case Great: case GreatEq: return 5;
  case Equal: case NotEq:                            return 4;
  case And:                                          return 3;
  case Or:                                           return 2;
  case Assign:                                       return 1;
  default:                                           return 0;
  }
}

int is_global(void) /* �O���[�o���̈�Ȃ�^ */
{
  return blkNest == 0;
}

int mallocL(int size) /* �Ǐ����������t */
{
  if (size < 0) size = 0;                                         /* ���S�΍� */
  localAdrs += size;
  return localAdrs - size;                                    /* ����t���Ԓn */
}

int good_format(char *form) /* �����n�����m�F */
{
  char *p;

  while ((p=strstr(form, "%%")) != NULL)                        /* %%���}�X�N */
    *p = *(p+1) = '\1';
  if ((p=strchr(form, '%')) == NULL) return FALSE;                 /* %���Ȃ� */
  ++p;
  if (*p == '-') ++p;                                           /* ����}�[�N */
  while (isdigit(*p)) { ++p; }                                      /* ���w�� */
  if (*p == '\0') return FALSE;                             /* �ϊ��������Ȃ� */
  if (strchr("cdioxX", *p) == NULL) return FALSE;     /* �����n�ϊ��������Ȃ� */
  if (strchr(p, '%') != NULL) return FALSE;                /* %�w�肪�������� */
  for (p=form; *p; p++) { if (*p == '\1') *p = '%'; }              /* %�ɖ߂� */
  return TRUE;                                                  /* ���������� */
}
