#include "kernel.h"

/* check that elements of vars are all symbols */
/* returns the length of the vars list */
static int checkvars(kerncell vars)
{
    int count = 0;

    while (ISlist(vars))
    {
        ++count;
        if (!ISsym(vars->CELLcar) || ISconst(vars->CELLcar))
            error(evalsym, err_pars, vars->CELLcar);
        vars = vars->CELLcdr;
    }

    return count;
}

/* save the bindings of vars in varstk */
static void savevars(kerncell vars)
{
    kernsym var;

    while (ISlist(vars))
    {
        var = CONVsym(vars->CELLcar);
        /* NOTE: property lists are not stacked: */
        VARpush(var, var->flag, var->bind);
        var->flag = VARIABLE;
        vars = vars->CELLcdr;
    }
}

/* resotre the binding of variables */
static void restorevars(kerncell vars)
{
    while (ISlist(vars))
    {
        VARpop();
        vars = vars->CELLcdr;
    }
}

/* evaluate expr */
kerncell eval(kerncell expr)
{
    if (ISsym(expr))
    {
        if (IScont(expr) || ISvar(expr) || ISinternal(expr))
            return CONVsym(expr)->bind;
        error(evalsym, "unbound symbol", EVALpush(expr));
    }
    else if (ISlist(expr))
    {
        int save_celltop = celltop; /* save top of cell stack */
        kerncell save = evalcall(expr->CELLcar, EVALpush(expr), 0);
        EVALpop();
        celltop = save_celltop; /* restore top of cell stack */
        return save;
    }
    else
    {
        return expr;        /* any other object evaluates to itself */
    }
}

typedef kerncell (*kernfun)(void);
typedef kerncell (*kernfun_cxxr)(char*);

/* evaluats a function call */
kerncell evalcall(kerncell head, kerncell list, int stacked)
/* stacked: non-zero when args are already stacked */
{
    kerncell fox;
#if 0
    kerncell (*fun)();
#else
    kernfun fun;
#endif 
    int arg1;

start:
    if (ISlist(head))           /* ((...) arg1 ... argn) */
    {
        if ((fox = head->CELLcar) == CONVcell(lamsym))
            return evallam(head, list->CELLcdr, stacked);
        if (fox == CONVcell(vlamsym))
            return evalvlam(head, list->CELLcdr, stacked);
        if (fox == CONVcell(ulamsym))
            return expand(head, list, stacked);
        if (fox == CONVcell(mlamsym))
            return eval(expand(head, list, stacked));
        head = evalcall(head->CELLcar, head, 0);
    }
    if (ISsym(head))            /* (head arg1 ... argn) */
    {
        if (head == CONVcell(quotesym)) /* 'expr */
            return list->CELLcdr->CELLcar;
        
        switch (head->flag)     /* what kind of symbol is head? */
        {
        case FUNCTION:
            head = CONVsym(head)->bind; /* function binding */
            goto start;
        case LBINARY:
        case VBINARY:
            if (!stacked)
            {
                arg1 = argtop + 1;
                list = list->CELLcdr;
                while (ISlist(list))
                {
                    fox = eval(list->CELLcar); /* evaluate args */
                    ARGpush(fox);              /* push args onto arg stack */
                    list = list->CELLcdr;
                }
                ARGpush(CONVcell(arg1)); /* push position of 1st arg */
            }
#if 0
            fox = ((fun = CONVsym(head)->bind) != CONVcell(Lcxxr)
                   ? (*fun)()
                   : (*fun)(CONVsym(head)->name));
#else
            if (CONVsym(head)->bind != CONVcell(Lcxxr))
            {
                kernfun f = (kernfun)CONVsym(head)->bind;
                fox = (*f)();
            }
            else
            {
                kernfun_cxxr f = (kernfun_cxxr)CONVsym(head)->bind;
                fox = (*f)(CONVsym(head)->name);
            }
#endif
            if (!stacked)
                ARGSpop();
            return fox;
        case UBINARY:
            fox = (stacked ? mkargslist() : list->CELLcdr);
            ARGpush(fox);
            fun = (kernfun)CONVsym(head)->bind;
            fox = (*fun)();
            ARGpop();
            return fox;
        case MBINARY:
            fox = (stacked ? mkargslist() : list->CELLcdr);
            ARGpush(fox);
            fun = (kernfun)CONVsym(head)->bind;
            fox = (*fun)();
            ARGpop();
            return eval(stacked
                        ? fox   /* substitute the result */
                        : (ISlist(fox)
                           ? (list->CELLcar = fox->CELLcar,
                              list->CELLcdr = fox->CELLcdr, list)
                           : (list->CELLcar = CONVcell(voidsym),
                              list->CELLcdr = mkcell(fox, (kerncell)nil))));
        } /* switch */
    }
    if (ISvector(fox = head) ||
        ISvar(fox) && ISvector(fox = CONVsym(fox)->bind))
        return evalvector(fox, list->CELLcdr, stacked);
    error(evalsym, "undefined functon", head);
}

/* evaluate a lam application */
kerncell evallam(kerncell lam, kerncell args, int stacked)
/* stacked: non-zero when args are already stacked */
{
    int arg1, nvars;
    kerncell obj, vars;
    kerncell vs;

    lam = lam->CELLcdr;         /* drop lam head */
    if ((vars = lam->CELLcar) != NIL && !ISlist(vars))
        error(evalsym, err_pars, vars);
    
    nvars = checkvars(vars);    /* check that vars are all symbols */
    if (!stacked)
    {
        arg1 = argtop + 1;
        while (ISlist(args))
        {
            obj = eval(args->CELLcar); /* evaluate each argument and */
            ARGpush(obj);              /* push it onto arg stack */
            args = args->CELLcdr;
        }
        ARGpush(CONVcell(arg1)); /* push position of 1st arg */
    }
    else
    {
        arg1 = ARGidx1;
    }

    CHECKlargs(lamsym, nvars);  /* check number of args */
    savevars(vars);             /* save current binding of vars */
    for (vs = vars; ISlist(vs); vs = vs->CELLcdr) /* bind the vars to args */
        CONVsym(vs->CELLcar)->bind = argstk[arg1++];
    obj = NIL;
    lam = lam->CELLcdr;
    while (ISlist(lam))         /* eval each form in lam body */
    {
        obj = eval(lam->CELLcar);
        lam = lam->CELLcdr;
    }
    restorevars(vars);          /* restore the binding of vars */
    if (!stacked) ARGSpop();    /* pop arguments */

    return obj;                 /* return the value of last form */
}

/* evalualte a vlam application */
kerncell evalvlam(kerncell vlam, kerncell args, int stacked)
/* stacked: non-zero when args are already stacked */
{
    int arg1;
    int save_argtop = _argtop;  /* for nested vlam calls */
    kerncell obj, vars;

    vlam = vlam->CELLcdr;       /* drop vlam head */
    if (!ISlist(vars = vlam->CELLcar) || checkvars(vars) != 1)
        error(evalsym, err_pars, vars);

    if (!stacked)
    {
        arg1 = argtop + 1;
        while (ISlist(args))
        {
            obj = eval(args->CELLcar); /* evaluate each argument and */
            ARGpush(obj);              /* push it onto arg stack */
            args = args->CELLcdr;
        }
        ARGpush(CONVcell(arg1));
    }
    else
    {
        arg1 = ARGidx1;
    }
    
    _argtop = argtop;           /* save argtop - for use by 'arg' function: */
    savevars(vars);             /* save current binding of var */
    /* var is set to the number of arguments: */
    CONVsym(vars->CELLcar)->bind = mkinum(argtop - arg1);
    obj = NIL;
    vlam = vlam->CELLcdr;
    while (ISlist(vlam))
    {
        obj = eval(vlam->CELLcar); /* eval each form in vlam body */
        vlam = vlam->CELLcdr;
    }
    restorevars(vars);          /* restore the binding of var */
    _argtop = save_argtop;      /* restore _argtop */
    if (!stacked) ARGSpop();    /* pop arguments */

    return obj;                 /* return the value of last form */
}

/* expand ulam/mlam application */
kerncell expand(kerncell fun, kerncell list, int stacked)
/* stacked: non-zero when args are already stacked */
{
    kerncell fox, vars;
    int ismacro = fun->CELLcar == (kerncell)CONVsym(mlamsym);

    fun = fun->CELLcdr;         /* drop ulam/mlam */
    if (!ISlist(vars = fun->CELLcar) || checkvars(vars) != 1)
        error(evalsym, err_pars, vars);

    /* the list of arguments is treated as 1 arg: */
    fox = (stacked ? mkargslist() : list->CELLcdr);
    ARGpush(fox);
    savevars(vars);
    CONVsym(vars->CELLcar)->bind = argstk[argtop];
    fox = NIL;
    fun = fun->CELLcdr;
    while (ISlist(fun))
    {
        fox = eval(fun->CELLcar); /* evaluate body */
        fun = fun->CELLcdr;
    }
    restorevars(vars);
    ARGpop();
    return (ismacro && !stacked
            ? (ISlist(fox)      /* substitute the result */
               ? (list->CELLcar = fox->CELLcar,
                  list->CELLcdr = fox->CELLcdr, list)
               : (list->CELLcar = CONVcell(voidsym),
                  list->CELLcdr = mkcell(fox, (kerncell)nil), list))
            : fox);
}

/* vector application */
kerncell evalvector(kerncell head, kerncell tail, int stacked)
/* stacked: non-zero when args are already stacked */
{
    kerncell index;

    if (stacked
        ? (argtop - ARGidx1 != 1 || !ISint(index = ARGnum1))
        : (tail == NIL || tail->CELLcdr != NIL
           || !ISint(index = eval(tail->CELLcar))))
        error(evalsym, "bad vector index", index);

    if (index->CELLinum < 0 ||
        index->CELLinum >= head->CELLdim->CELLinum)
        error(evalsym, "vector index out of range", index);
    if (stacked)
        ARGSpop();

    return *(head->CELLvec + index->CELLinum);
}

/* make an argument list using the entries on argstk */
kerncell mkargslist()
{
    int argi = ARGidx1;
    kerncell arglist = NIL, list;

    while (argi < argtop)
    {
        if (arglist == NIL)
            arglist = list = mkcell(argstk[argi++], (kerncell)nil);
        else
            list = list->CELLcdr = mkcell(argstk[argi++], (kerncell)nil);
    }

    return arglist;
}

/* (eval 'expr) */
kerncell Leval()
{
    CHECKlargs(evalsym, 1);
    return eval(ARGnum1);
}

/* (call 'fun 'arg1 ... 'argn) */
kerncell Vcall()
{
    kerncell fox;

    CHECKlargs(callsym, 1);
    fox = ARGnum1;              /* the function to be called */
    argstk[argtop] = CONVcell(CONVint(argstk[argtop]) + 1);
    fox = evalcall(fox, (kerncell)nil, 1); /* do the call */
    argstk[argtop] = CONVcell(CONVint(argstk[argtop]) - 1);
    return fox;
}

/* (apply 'fun 'arglist) */
kerncell Lapply()
{
    kerncell arg1 = ARGnum1;
    kerncell arg2 = ARGnum2;
    kerncell fox = CONVcell(argtop + 1);

    CHECKlargs(applysym, 2);
    CHECKlist(applysym, arg2);
    while (ISlist(arg2))
    {
        ARGpush(arg2->CELLcar);
        arg2 = arg2->CELLcdr;
    }
    ARGpush(fox);
    fox = evalcall(arg1, (kerncell)nil, 1);
    ARGSpop();
    return fox;
}
