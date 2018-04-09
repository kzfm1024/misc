/**************************************************************/
/*    filename:cci.c メイン処理                               */
/*    CCI-C (Rev 1.01) Copyright (c) 2009 Haruhiko Hayashi    */
/**************************************************************/
#include "cci.h"
#include "cci_prot.h"

int main(int argc, char *argv[])
{
  if (argc == 1) {
    puts("用法: cci prgfile [--code]");
    puts("      --code  生成コードを表示");
    exit(1);
  }

  if (compile(argv[1])) {
    if (argc>=3 && strcmp(argv[2],"--code")==0) code_dump();
    else return execute();
  }
  return 0;
}
