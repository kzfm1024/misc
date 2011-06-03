#include <stdio.h>
#define UNIX             YES
/* scalar contents: */
#define HASHTABSIZE      256    /* size of has table */
#define CELLTABSIZE     1024    /* size of cell table */
#define BLOCKSIZE        512    /* block size for cell allocation */
#define EVALSTKSIZE     1024    /* size of evalution stack */
#define VARSTKSIZE      1024    /* size of variable stack */
#define ARGSTKSIZE      1024    /* size of argument stack */
#define CATSTKSIZE       256    /* size of catch stack */
#define CHANBUFSIZE      126    /* size of channel buffer */
#define STRBUFSIZE       126    /* size of string buffer */
#define MAXCOLS           80    /* max no. of columns on the screen */
#define SMALLINTLOW     -128    /* least small integer */
#define SMALLINTHIGH     127    /* greatest small integer */

/* values for flag in symbol structure: */
#define UNBOUND         0       /* unbound symbol */
#define CONSTANT        1       /* constant - cannot be changed */
#define VARIABLE        2       /* bound variable */
#define FUNCTION        3       /* non-binary function */
#define LBINARY         4       /* binary lam */
#define VBINARY         5       /* binary vlam */
#define UBINARY         6       /* binary ulam */
#define MBINARY         7       /* binary mlam */
#define INTERNAL        8       /* internal object - not in symbol table */

/* values for flag in cell structutre: */
#define VOID            10      /* void object */
#define INTOBJ          11      /* integer number */
#define REALOBJ         12      /* real number */
#define STROBJ          13      /* string */
#define CHANOBJ         14      /* channel for I/O */
#define VECTOROBJ       15      /* vector */
#define LISTOBJ         16      /* list */
#define SETOBJ          17      /* set */
#define MARK            128     /* mark bit - for garbage collection */
#define MASK7           127     /* for masking bit 7 in flag */

/* channel kinds: */
#define INCHAN          0       /* input channel flag */
#define OUTCHAN         1       /* output chanel flag */
#define INOUTCHAN       2       /* input-output channel flag */
/* values for flag in printaux and bufprint: */
#define PRINT           0       /* flag == PRINT  ==> print */
#define PRINC           1       /* flag == PRINC  ==> princ */
#define LENGTH          2       /* flag == LENGTH ==> prlen */
#define STRIP           3       /* |symbol| ==> symbol */

typedef unsigned char byte;     /* the basic byte unit */
typedef union {int i,*j;} word; /* the basic word unit */
typedef float real;             /* real type - can be changed to double */

struct symbol {                 /* symbol-cell structure */
    byte flag;                  /* symbol type, always < VOID */
    struct cell *bind;          /* symbol binding */
    struct cell *prop;          /* symbol property list */
    char *name;                 /* symbol name */
    struct symbol *link;        /* link to next symbol */
};

struct cell {                   /* cons-cell structure */
    byte flag;                  /* cell type, always >= VOID */
    union {
        int inum;               /* integer number */
        real rnum;              /* real number */
        char *str;              /* string */
        struct channel *chan;   /* channel */
        struct {                /* for list/set construction */
            struct cell *car;   /* car pointer */
            struct cell *cdr;   /* cdr pointer */
        } pair;
        struct {                /* for vector construction */
            struct cell *dim;   /* vector dimension */
            struct cell **vec;  /* vector block */
        } vect;
    } part;
};

struct channel {                /* I/O channel structure */
    char ch;                    /* current character */
    unsigned short int tok;     /* current token */
    unsigned short int pos;     /* current position in buf */
    unsigned short int len;     /* no. of chars in buf */
    char *buf;                  /* channel buffer */
    byte mode;                  /* one of INCHAN,OUTCHAN,INOUTCHAN */
    FILE *file;                 /* the file associated with channel */
};

struct variable {               /* variable structure for variable stack */
    struct symbol *sym;         /* variable symbol */
    byte flag;                  /* its flag */
    struct cell *bind;          /* its binding */
};

typedef struct symbol  *kernsym;  /* symbol pointer */
typedef struct cell    *kerncell; /* cell pointer */
typedef struct channel *iochan;   /* I/O channel */

/* macros: */
#define ISnotbinary(p)          ((p)->flag <  LBINARY)
#define ISunbound(p)            ((p)->flag == UNBOUND)
#define ISconst(p)              ((p)->flag == CONSTANT)
#define ISvar(p)                ((p)->flag == VARIABLE)
#define ISfun(p)                ((p)->flag == FUNCTION)
#define ISlbin(p)               ((p)->flag == LBINARY)
#define ISvbin(p)               ((p)->flag == VBINARY)
#define ISubin(p)               ((p)->flag == UBINARY)
#define ISmbin(p)               ((p)->flag == MBINARY)
#define ISinternal(p)           ((p)->flag == INTERANL)

#define ISsym(p)                ((p)->flag <  VOID)
#define IScell(p)               ((p)->flag >= VOID)
#define ISvoid(p)               ((p)->flag == VOID)
#define ISint(p)                ((p)->flag == INTOBJ)
#define ISreal(p)               ((p)->flag == REALOBJ)
#define ISstr(p)                ((p)->flag == STROBJ)
#define ISchan(p)               ((p)->flag == CHANOBJ)
#define ISvector(p)             ((p)->flag == VECTOROBJ)
#define ISlist(p)               ((p)->flag == LISTOBJ)
#define ISset(p)                ((p)->flag == SETOBJ)
#define ISmarked(p)             (((p)->flag & MARK) == MARK)

#define CELLinum                part.inum
#define CELLrnum                part.rnum
#define CELLstr                 part.str
#define CELLchan                part.chan
#define CELLcar                 part.pair.car
#define CELLcdr                 part.pair.cdr
#define CELLdim                 part.vect.dim
#define CELLvec                 part.vect.vec

#define CONVbyte(p)             ((byte) (p))
#define CONVint(p)              ((int) (p))
#define CONVintp(p)             ((int *) (p))
#define CONVreal(p)             ((real) (p))
#define CONVstr(p)              ((char *) (p))
#define CONVchan(p)             ((iochan) (p))
#define CONVsym(p)              ((kernsym) (p))
#define CONVcell(p)             ((kerncell) (p))
#define CONVvector(p)           ((kerncell *) (p))
#define NIL                     ((kerncell) nil)
#define TTT                     ((kerncell) ttt)

#define READin()                readaux(_inchan,0)
#define READchan(chan)          readaux((chan)->CELLchan,0)
#define PRINTout(p)             printaux(PRINT,(p),_outchan)
#define PRINTchan(p,chan)       printaux(PRINT,(p),(chan)->CELLchan)
#define TERPRIout()             bufprint(PRINT,_outchan,"\n")
#define TERPRIchan(chan)        bufprint(PRINT,(chan)->CELLchan,"\n")
#define INTERNALsym(isym)       \
    (isym = CONVsym(new(sizeof(struct symbol))))->flag = INTERANL

#define CHECKlargs(fun,n)                        \
    if (argtop - CONVint(argstk[argtop]) != (n)) \
        error(fun,err_args,0)
#define CHECKvargs(fun,m,n)                       \
    if (argtop - CONVint(argstk[argtop]) < (m) || \
        argtop - CONVint(argstk[argtop]) > (n))   \
        error(fun,err_args,0)
#define CHECKvargs1(fun,n)                      \
    if (argtop - CONVint(argstk[argtop]) < (n)) \
        error(fun,err_args,0)
#define CHECKvargs2(fun,n)                      \
    if (argtop - CONVint(argstk[argtop]) > (n)) \
        error(fun,err_args,0)
#define EVALpush(obj)                                       \
    (++evaltop < celltop ? evalstk[evaltop] = (obj)         \
                         : CONVcell(faterr(err_evalstk)))
#define EVALpop()               --evaltop
#define CELLpush(obj)                               \
    (--celltop > evaltop ? evalstk[celltop] = (obj) \
                         : CONVcell(faterr(err_evalstk)))
#define CELLpop()               ++celltop
#define VARpush(s,f,b)                          \
    if (++vartop < VARSTKSIZE) {                \
        varstk[vartop].sym = (s);               \
        varstk[vartop].flag = (f);              \
        varstk[vartop].bind = (b);              \
    } else faterr(err_varstk)
#define VARpop()                                          \
    { varstk[vartop].sym->flag = varstk[vartop].flag;     \
      varstk[vartop].sym->bind = varstk[vartop--].bind; }
#define ARGpush(obj)                                    \
    if (++argtop < EVALSTKSIZE) argstk[argtop] = (obj); \
    else faterr(err_argstk)
#define ARGpop()                --argtop
#define ARGSpop()               argtop = CONVint(argstk[argtop]) - 1

#define ARGidx1                 CONVint(argstk[argtop])
#define ARGnum1                 argstk[ARGidx1]
#define ARGnum2                 argstk[ARGidx1 + 1]
#define ARGnum3                 argstk[ARGidx1 + 2]
#define ARGnum4                 argstk[ARGidx1 + 3]

#define GETint(yyy,xxx) \
    (ISint(xxx) ? xxx->CELLinum : CONVint(error(yyy,err_int,xxx)))
#define GETreal(yyy,xxx) \
    (ISreal(xxx) ? xxx->CELLrnum : CONVint(error(yyy,err_real,xxx)))
#define GETnum(yyy,xxx)                                             \
    (ISint(xxx) ? xxx->CELLinum                                     \
     : (ISreal(xxx) ? xxx->CELLrnum                                 \
        : CONVint(error(yyy,err_num,xxx))))
#define GETstr(yyy,xxx)                                             \
    (ISint(xxx) ? xxx->CELLstr                                      \
     : (ISsym(xxx) ? CONVsym(xxx)->name                             \
        : CONVstr(error(yyy,err_str,xxx))))
#define GETchan(yyy,xxx) \
    (ISchan(xxx) ? xxx->CELLchan : CONVchan(error(yyy,err_chan1,xxx)))
#define CHECKsym1(yyy,xxx) \
    if (xxx->flag >= VOID) error(yyy,err_sym1,xxx)
#define CHECKsym2(yyy,xxx)                          \
    if (xxx->flag >= VOID || xxx->flag == CONSTANT) \
        error(yyy,err_sym2,xxx)
#define CHECKpair(yyy,xxx) \
    if (xxx->flag < LISTOBJ) error(yyy,err_pair,xxx)
#define CHECKlist(yyy,xxx) \
    if (xxx->flag < LISTOBJ && xxx != NIL) error(yyy,err_list,xxx)

/*-------------------- external declarations --------------------*/
extern char     *err_args, *err_pars;
extern char     *err_evalstk, *eval_varstk, *err_argstk, *err_catstk;
extern char     *err_memory, *err_list;
extern char     *err_int, *err_real, *err_num, *err_str, *err_chan1, *err_chan2;
extern char     *err_sym1, *err_sym2, *err_pair, *err_list, *err_var, *err_dom;
extern kerncell catres;
extern kerncell golabel;
extern kerncell _tempstr;
extern kerncell inchan, outchan, errchan;
extern iochan   _inchan, _outchan, _errchan;
extern char     strbuf[];
extern struct variable varstk[];
extern kerncell evalstk[], argstk[];
extern int      evaltop, celltop, vartop, argtop, _argtop;
extern kerncell read_and_eval, top_lev_call, top_lev_tags;
extern int      (*org_interrupt)(); /* original interrupt handler */

/* internals: */
extern kernsym _bquotesym, _commasym, _atsym;
extern kernsym _toptagsym, _errtagsym, _rettagsym, _gotagsym;
extern kernsym _tempsym, _cxxrsym;

/* constants: */
extern kernsym nil, ttt, eofsym, inchansym, outchansym, errchansym;

/* unbounds: */
extern kernsym lamsym, vlamsym, ulamsym, mlamsym;

/* symt.c: */
extern kernsym addsym(char *name);
extern kernsym findsym(char *name);
extern kernsym mksym(char *name);
extern kernsym _mksym(char *name);
extern kernsym newsym(char *name, byte flag, kerncell bind);

/* cellt.c: */
extern char     *new(int size);
extern void     initcelltab();
extern kerncell freshcell();
extern kerncell collectgarb();
extern void     mark(kerncell obj);
extern kerncell mkinum(int inum);
extern kerncell mkrnum(real rnum);
extern kerncell mkstr(char *str);
extern kerncell _mkstr(char *str);
extern kerncell mkchan(iochan chan);
extern kerncell mkcell(kerncell head, kerncell tail);
extern kerncell mkset(kerncell head, kerncell tail);



/* FIXME */
extern kerncell Lcxxr();
