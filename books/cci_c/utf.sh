#nkf -Lu -S -w cci.c > utf/cci.c

for i in cci.c cci.exe cci.h cci_code.c cci_misc.c cci_pars.c cci_prot.h cci_tbl.c cci_tkn.c cp_add.c cp_base.c cp_prime.c cp_qsort.c minicalc.c polish_p.c token_p.c
do
    echo $i
    nkf -Lu -S -w cci_c/$i > utf/$i
done
