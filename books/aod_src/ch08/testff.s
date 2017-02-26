# サブルーチン findfirst(v,w,b) は、bから始まるwワードのメモリの中から
# 最初に現れるvを探し、vが見つかった場所(0, 1, 2, ..)か、
# 見つからなかった場合には-1 を返す。
# _startから実行するとサブルーチンの簡単なテストができる。
#

.data # データ領域
x:
	.long 1
	.long 5
	.long 3
	.long 168
	.long 8888
.text 			# コード領域
.globl _start 		# 必要
_start: 		# 特別な処理をしない限り、このラベルが必要
			# スタックに引数を積んで、呼び出す
	push $x+4 	# 5の場所から検索を開始する
	push $4 	# 4ワードを検索する
	push $168 	# 168を検索する
	call findfirst
done:
	movl %edi, %edi # ブレークポイント用のダミー命令
findfirst:
	# ワードのブロックから指定された値が最初に現れるところを検索する
	# EBX には検索対象の値が設定される
	# ECX には検索範囲のワード数が設定される
	# EAX には現在検索しているワードへのポインタが設定される
	# 返り値 (EAX) には見つかったワードの番号が設定される(見つからなければ-1)
	# スタックから引数を取得する
	movl 4(%esp), %ebx
	movl 8(%esp), %ecx
	movl 12(%esp), %eax
	movl %eax, %edx	# ブロックの開始点を保存する
	# ループの先頭；現在のワードを検索する値と比較する
top:	cmpl (%eax), %ebx
	jz found
	decl %ecx 	# 検索するワード数カウンタを1減らす
	jz notthere 	# カウンタが0なら、検索する値が存在しなかった
	addl $4, %eax 	# そうでなければ、次のワードに移る
	jmp top
found:
	subl %edx, %eax # ブロックの先頭からのオフセット値を得る
	shrl $2, %eax 	# バイトからオフセット値に変換するために4で割る
	ret
notthere:
	movl $-1, %eax
	ret
