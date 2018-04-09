#nkf -Lu -S -w cci.c > utf/cci.c

for i in cci.c cci.exe cci.h cci_code.c cci_misc.c cci_pars.c cci_prot.h cci_tbl.c cci_tkn.c
do
    echo $i
    nkf -Lu -S -w cci_c/$i > utf/$i
done
