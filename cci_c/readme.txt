�t�^CD-ROM�ɂ���

���t�^CD-ROM�̓��e
�@���̕t�^CD-ROM�ɂ́A�w��������R���p�C���E�C���^�v���^�J���x�i�ѐ���Ò��A�\�t
�g�o���N �N���G�C�e�B�u���j�ō쐬�����T���v���v���O���������^���Ă��܂��B
�@�t�^CD-ROM�Ɏ��^�����\�[�X�R�[�h�́A�{���Ɍf�ڂ̃T���v���v���O�������A�ǎ҂���
�œ��͂����Ԃ��Ȃ����߂ɃT�[�r�X�񋟂�����̂ł���A�����ۏ؂�����̂ł͂���
�܂���B

���T���v�����p��̒���

(1) ����CD-ROM�ɂ͈ȉ��̃v���O�������i�[����Ă��܂��B�������Windows�p�̃\�[�X�v
���O�����ł��B�C���X�g�[���Ȃǂ͂���܂���̂ŁA�����̃t�@�C�����f�B�X�N��̓K
�؂ȃt�H���_�ɃR�s�[���Ă����p���������B

    cci.h       ���߃R�[�h��L���\����у}�N���Ȃǂ̒�`
    cci_prot.h  �S�֐��̃v���g�^�C�v
    cci.c       ���C������
    cci_pars.c  �\�����
    cci_tkn.c   �g�[�N������
    cci_tbl.c   �L���\����
    cci_code.c  �R�[�h�����ƃ������Ǘ��Ɖ��ߎ��s
    cci_misc.c  �G���[�o�͂Ȃǂ̎G�֐�
    ccimake.bat Windows/Visual C++�ŃR���p�C�����邽�߂̃o�b�`�t�@�C��
    cci.exe     �R���p�C���ςݎ��s�\�t�@�C���iWindows�R�}���h�v�����v�g�p�j

    cp_add.c    CCI-C�p�T���v���v���O����
    cp_prime.c  CCI-C�p�T���v���v���O����
    cp_qsort.c  CCI-C�p�T���v���v���O����
    cp_base.c   CCI-C�p�T���v���v���O����

    token_p.c   �����̓v���O�����i�{���f�ځj
    polish_p.c  �t�|�[�����h�L�@�v���O�����i�{���f�ځj
    minicalc.c  �d��v���O�����i�{���f�ځj

(2) ������ccimake.bat���g���ƁAWindows + Visual C++ �ŃR���p�C���ł��܂��B�ق���
�R���p�C�����g���Ƃ��́Accimake.bat�̓��e���Q�l�ɂ��Ă��������B
�@Windows�ȊO�̊��ŃR���p�C������Ƃ��͕����̃G���R�[�h��K�؂ɕϊ����Ă��������B
�@Linux��Mac OS X�ł́Ankf�R�}���h���g���ăG���R�[�h�ϊ����ł��܂��B
�@nkf�R�}���h�ŁA�V�t�gJIS��UTF-8�ɕϊ�����utf�f�B���N�g���ɏo�͂����������܂��B

    nkf -Lu -S -w cci.c > utf/cci.c

    -S   �ϊ������V�t�gJIS�ł���w��
    -w   �ϊ��悪UTF-8�ł���w��
    -Lu  ���s�R�[�h��Unix��LF�R�[�h�ɕϊ�����w��

�@�Ȃ�Linux��Mac OS X�ł́A�R���\�[���̃G���R�[�h��ύX�ł���̂ŁA������V�t�gJIS
�ɂ���΁A�\�[�X�R�[�h��ϊ�����K�v�͂���܂���B

(3) cci_misc.c�ɂ���is_kanji()�֐���get_kanji_mode()�֐��́A��������i�S�p������
��j���s�Ȃ����̂ŁA�V�t�gJIS�AEUC-JP�AUTF-8�̊e�G���R�[�h�Ɏ����Ή������Ă��܂��B
�����̃G���R�[�h�ł���΁A���֐��͂��̂܂ܗ��p�ł��܂��B

(4) CCI-C�̃R���p�C���Ɠ���́A���̊��Ŋm�F���Ă��܂��B

    Windows Vista + Visual Studio 2008(Visual C++)
    Windows7 + Visual Studio 2008(Visual C++)
    Windows7(64�r�b�g��) + Visual Studio 2008(Visual C++)
    CentOS 5.2 (Linux) + GNU C
    Fedora 12(Linux) + gcc
    Fedora 12((Linux,x64) + gcc
    Mac OS X 10.6 + GNU C

    ���F64�r�b�g�R���p�C���ł́uadrs = mallocG((int)strlen(s) + 1);�v�̂悤�ɁA
       (int)�L���X�g�����Ȃ��ƌx�������ꍇ�����邪�A�{�\�[�X�v���O�����ł�
       ���̋L�q�͓���Ă��Ȃ��B

(5) ����CD-ROM�́A�ǎ҂��{���f�ڃ\�[�X�v���O��������͂��镉�S���A�y������ړI��
�񋟂��Ă��܂��B����̊��S����ۏ؂�����̂ł͂Ȃ����Ƃ��������̏�ł����p������
���B


���t�^CD-ROM�̎��^���e�Ɋւ��钍�ӎ���
�E�{���̃T���v���v���O�����́A�w�K��⏕���邽�߂ɒ񋟂���Ă�����̂ł���A����
�@�E�Ɩ��p�r�ł̎g�p��O��Ƃ������̂ł͂���܂���B
�E�{�����w�����ꂽ�ǎ҂��A�w�K�̂��߂ɃR���s���[�^�ɃC���X�g�[�����邱�ƁA�\����
�@�����i�o�b�N�A�b�v�j����邱�Ƃ݂̂��������A��O�҂ɔz�z����s�ׂ��ւ��܂��B
�E�T���v���v���O�����́A���Ȃ̐ӔC�ɂ����ĉ^�p���Ă��������B�T���v���v���O������
�@�^�p�������ʐ����������Ȃ��Q�E���Q�ɂ��āA�M�ҁE�o�ŎЂ͂��̐ӂ𕉂��܂���B
�E�T���v���v���O�����ɑ΂��āA�E�C���X�`�F�b�N���s���Ă��܂����A����ɂ��M�ҁE
�@�o�ŎЂ��E�C���X�̕s���݂�ۏ؂�����̂ł͂���܂���B
�E�w�K�p�r�ŋ���@�ւ܂��͊�Ƃ̋��畔��Ȃǂ��T���v���v���O�����𕡐��E�z�z����
�@�ꍇ�́A���O�ɕM�ҁE�o�ŎЂ̋����𓾂�K�v������܂��B�ڍׂ́A�{���̊����ɋL��
�@����Ă���\�t�g�o���N �N���G�C�e�B�u������Ђ́u�ҏW���v�܂ł��₢���킹����
�@�����B

