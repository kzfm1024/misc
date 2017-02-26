// psax.c; cursesライブラリの具体例

// 本コードはトップダウンに読んでください。まず、このコメントと
// グローバル変数、次に main() 最後にmain()から呼び出される関数の順です。

// シェルコマンド 'ps ax' を実行し、画面に収まる限りの最後の
// 出力行を保存します。ユーザはウィンドウの中で上下に移動できます。
// 現在ハイライト表示されているプロセスをkillすることができます。
//

// 使い方: psax

// ユーザコマンド：

// 'u': ハイライトを1行上に移動する
// 'd': ハイライトを1行下に移動する
// 'k': 現在のハイライト行のプロセスをkillする
// 'r': 'ps ax'を再実行して更新する
// 'q': 終了する

// 可能な改造には次の機能があるでしょう。画面をスクロールして
// 最終行だけでなく、すべての'ps ax'の出力を見られるようにする。
// 長い行を折り返す。プロセスをkillする前に、ユーザに確認する。
//

#define MAXROW 1000
#define MAXCOL 500

#include <curses.h> // 必要
#include <string.h>
WINDOW *scrn; // curses のウィンドウオブジェクトを指す

char cmdoutlines[MAXROW][MAXCOL]; // 'ps ax' の出力
                                  // malloc()を使った方がよい)

int ncmdlines, // cmdoutlines中の列数
    nwinlines, // xterm（もしくは同等の端末ソフト）のウィンドウに
               // 占める"ps ax"の行数
    winrow, // 画面上の現在行の位置
    cmdstartrow, // cmdoutlines を表示する最初の行
    cmdlastrow; // cmdoutlines を表示する最後の行

// winrowの行を太字のフォントで表示し直す
highlight()
{
    int clinenum;
    attron(A_BOLD); // この curses ライブラリ呼び出しにより、
                    // これから書き出すものは（他の書体が設定されるまで）
                    // 太字になります
    // 画面上のwinrowの行を太字で描画し直します
    //
    clinenum = cmdstartrow + winrow;
    mvaddstr(winrow,0,cmdoutlines[clinenum]);
    attroff(A_BOLD); // 太字モードを終了します
    refresh(); // 画面を更新します
}

// "ps ax" を実行し、出力を cmdoutlinesに保存する。
runpsax()
{
    FILE *p; char ln[MAXCOL]; int row; char* tmp;
    p = popen("ps ax","r"); // Unix のパイプを開く（プログラムの出力を
                            // ファイルのように読み出せる）
    for (row = 0; row < MAXROW; row++) {
        tmp = fgets(ln,MAXCOL,p); // パイプを1行読む
        if (tmp == NULL) break; // パイプが終了したらbreakでループを抜ける
        // 画面の幅を超えて保存したくないので、COLS文字に切り詰める。
        // curses ライブラリでは、画面の幅は COLSという変数に格納されている
        //
        strncpy(cmdoutlines[row],ln,COLS);
        cmdoutlines[row][MAXCOL-1] = 0;
    }
    ncmdlines = row;
    pclose(p); // パイプを閉じる
}

// コマンド出力のうち画面に収まる最後の部分を表示する
showlastpart()
{
    int row;
    clear(); // curses の画面消去を呼び出す
    // 'ps ax'の出力（の最後の部分）を画面に描画する準備をします。
    // 画面の行数以上に出力があるかどうかで2つの場合分けをする。
    // ます、出力がすべて画面に収まる場合
    if (ncmdlines <= LINES) { // LINES は cursesで管理される
                              // int型変数であり、
                              // 画面の行数と同じ。
        cmdstartrow = 0;
        nwinlines = ncmdlines;
    }
    else { // 次に出力が画面よりも大きい場合。
        cmdstartrow = ncmdlines - LINES;
        nwinlines = LINES;
    }
    cmdlastrow = cmdstartrow + nwinlines - 1;
    // 画面に行を表示する
    for (row = cmdstartrow, winrow = 0; row <= cmdlastrow; row++,winrow++)
        mvaddstr(winrow,0,cmdoutlines[row]); // カーソルを指定した場所に移動して、
                                             // そこに文字を描画するという
                                             // curses呼び出し
    refresh(); // 画面更新を画面に反映するには
               // cursesライブラリのこれを呼び出します
    // 最後の行をハイライト表示する
    winrow--;
    highlight();
}

// カーソルを上下の行に移動する
updown(int inc)
{
    int tmp = winrow + inc;
    // 画面の端を超えないようにする
    if (tmp >= 0 && tmp < LINES) {
        // ハイライト表示から、太字でない現在のフォント
        // （A_NORMAL）に戻すにするために、移動前に
        // 現在行を書き直す
        mvaddstr(winrow,0,cmdoutlines[cmdstartrow + winrow]);
        // 移動後の行をハイライト表示する
        winrow = tmp;
        highlight();
    }
}

// "ps ax" を（再）実行する
rerun()
{
    runpsax();
    showlastpart();
}

// ハイライト中のプロセスを kill する
prockill()
{
    char *pid;
    // strtok() はCのライブラリ関数（詳細はmanを参照）
    pid = strtok(cmdoutlines[cmdstartrow+winrow]," ");
    kill(atoi(pid),9); // シグナル9(SIGKILL;強制終了)をプロセスに
                       // 送るためのUnixシステムコール
    rerun();
}

main()
{
    char c;
    // ウィンドウを準備する。以下の3行でcursesライブラリを呼び出す
    // cursesプログラムの標準的な初期化手順である
    scrn = initscr();
    noecho(); // キー入力をエコーしない
    cbreak(); // キー入力は即時有効になる。Enterキー入力を必要としない
    // 'ps ax' を実行し出力を処理する
    runpsax();
    // ウィンドウに表示する
    showlastpart();
    // ユーザコマンドループ
    while (1) {
        // ユーザコマンドを受け取る
        c = getch();
        if (c == 'u') updown(-1);
        else if (c == 'd') updown(1);
        else if (c == 'r') rerun();
        else if (c == 'k') prockill();
        else break; // 終了する
    }
    // 元の設定を復元する
    endwin();
}
