#include "kernel.h"

kernsym symtab[HASHTABSIZE];    /* symbol table */

/* initialize the symbol table */
int initsymtab()
{
    int i;

    for (i = 0; i < HASHTABSIZE; i++)
        symtab[i] = NULL;
}

/* the hash function */
int hash(const char *name)
{
    int hashaddr = 0;

    while (*name)
        hashaddr += *name++;
    return (hashaddr % HASHTABSIZE);
}

/* add a symbol to the symbol table */
kernsym addsym(const char *name)
{
    int hashaddr = hash(name);
    int len;
    kernsym newsym, sym;

    newsym = CONVsym(new(sizeof(struct symbol)));
    newsym->flag = UNBOUND;
    newsym->name = new((len = strlen(name)) + 1);
    strcpy(newsym->name, name);
    *(newsym->name + len) = 0;
    if ((sym = symtab[hashaddr]) == NULL || strcmp(name, sym->name) < 0)
    {
        /* insert in front of list */
        symtab[hashaddr] = newsym;
        newsym->link = sym;
    }
    else if (sym->link == NULL)
    {
        /* append to the end of list */
        sym->link = newsym;
        newsym->link = NULL;
    }
    else
    {
        /* inset in list */
        while (strcmp(name, sym->link->name) > 0 && (sym = sym->link)->link)
            ;
        newsym->link = sym->link;
        sym->link = newsym;
    }
    newsym->prop = NIL;
    return newsym;
}

/* find a symbol in the symbol table */
kernsym findsym(const char *name)
{
    kernsym sym = symtab[hash(name)];
    int cmp;

    while (sym != NULL && (cmp = strcmp(name, sym->name)) > 0)
        sym = sym->link;

    if (sym == NULL || cmp < 0) 
        return NULL;
    return sym;
}

/* mkae a symbol object */
kernsym mksym(const char *name)
{
    kernsym sym = findsym(name);

    if (sym == NULL && ISunbound(sym = addsym(name)) && *name == 'c')
    {
        while (*++name == 'a' || *name == 'd') ; /* look for c..r form */
        if (*name == 'r' && *++name == 'd')
        {
            sym->flag = LBINARY;
            sym->bind = CONVcell(Lcxxr);
        }
    }

    return sym;
}

/* make a temporary symbol */
kernsym _mksym(const char *name)
{
    _tempsym->name = name;
    return _tempsym;
}

/* make a new symbol object */
/* assumes that symbol is not already in the symbol table */
kernsym newsym(const char *name, byte flag, kerncell bind)
{
    kernsym sym;

    sym = addsym(name);
    sym->flag = flag;
    sym->bind = bind;
    return sym;
}
