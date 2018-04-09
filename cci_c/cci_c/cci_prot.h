/**********************************************************/
/*    filename:cci_prot.h 全関数のプロトタイプ            */
/**********************************************************/
/* -------------------- cci_pars.c 構文解析 */
int compile(char *fname);
void varDecl(void);
void fncDecl(void);
void fncChk(SymTbl *f1, SymTbl *f2);
void fncDecl_begin(void);
void fncDecl_end(void);
void set_main(void);
void block(int is_func);
void statement(void);
void continue_break_begin(TknKind stmnt);
void continue_break_end(void);
int get_loopTop(void);
void swch_begin(void);
void swch_end(void);
void expr_with_chk(TknKind k1, TknKind k2);
void expression(void);
void term(int n);
void factor(void);
void fncCall(SymTbl *fp);
void sys_fncCall(TknKind kd);
void set_type(void);
void set_name(void);
void set_aryLen(void);
void set_adrs(SymTbl *tp);
int opOrder(TknKind kd);
int is_global(void);
int mallocL(int size);
int good_format(char *form);
/* -------------------- cci_tkn.c トークン処理 */
void initChTyp(void);
void fileOpen(char *fname);
Token nextTkn(void);
int nextCh(void);
int is_ope2(int c1, int c2);
Token set_kind(Token tk);
Token chk_nextTkn(Token tk, TknKind kd);
int get_lineNo(void);
/* -------------------- cci_tbl.c 記号表処理 */
SymTbl *enter(SymTbl tb, SymKind kind);
void localTBL_open(void);
void localTBL_close(SymTbl *fp);
SymTbl *search(char *s);
SymTbl *search_name(char *s);
void nameChk(SymTbl tb);
void del_fncTable(SymTbl *f1, SymTbl *f2);
int b_flg(SymTbl *tp);
SymTbl *tb_ptr(int n);
/* -------------------- cci_code.c コード生成とメモリ管理と解釈実行 */
int gencode3(OpCode op, int fg, int dt);
void gencode_Unary(TknKind kd);
void gencode_Binary(TknKind kd);
void to_leftVal(void);
void remove_val(void);
int mallocS(char *s);
int mallocG(int n);
char *mem_adrs(int indx);
int get_const(int *var);
int const_fold(OpCode op);
int is_binaryOP(OpCode op);
int binaryExpr(OpCode op, int d1, int d2);
int is_code(int n, OpCode op, int dt);
int nextCodeCt(void);
void backPatch(int n, int adrs);
void backPatch_callAdrs(void);
void backPatch_RET(int fncAdrs);
void backPatch_BREAK(int looptop);
int execute(void);
void library(int fnc_typ);
void code_dump(void);
void exe_err(char *msg);
/* -------------------- cci_misc.c 雑関数 */
int is_kanji(int ch);
int get_kanji_mode(void);
char *s_malloc(char *s);
void incVar(int *var, int size, char *errmsg);
void err_s(char *s);
void err_fi(char *fmt, int idt);
void err_ss(char *s1, char *s2);
