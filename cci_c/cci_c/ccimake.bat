:ccimake.bat  CCI-CをWindows/Visual C++でコンパイルするためのバッチファイル
cl /W3 /EHsc /D_CRT_SECURE_NO_WARNINGS cci.c cci_pars.c cci_tkn.c cci_tbl.c cci_code.c  cci_misc.c
