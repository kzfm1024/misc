/**************************************************************/
/*    filename:cci.c ���C������                               */
/*    CCI-C (Rev 1.01) Copyright (c) 2009 Haruhiko Hayashi    */
/**************************************************************/
#include "cci.h"
#include "cci_prot.h"

int main(int argc, char *argv[])
{
  if (argc == 1) {
    puts("�p�@: cci prgfile [--code]");
    puts("      --code  �����R�[�h��\��");
    exit(1);
  }

  if (compile(argv[1])) {
    if (argc>=3 && strcmp(argv[2],"--code")==0) code_dump();
    else return execute();
  }
  return 0;
}
