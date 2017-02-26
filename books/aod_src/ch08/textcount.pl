#! /usr/bin/perl

# コマンドラインに指定したテキストファイルを読んで、
# 語と、行と、段落の数を数える。

open(INFILE, $ARGV[0]);

$line_count = 0;
$word_count = 0;
$par_count = 0;

$now_in_par = 0; # まだ段落の中ではない

while ($line = <INFILE>) {
    $line_count++;
    if ($line ne "\n") {
        if ($now_in_par == 0) {
            $par_count++;
            $now_in_par = 1;
        }
        @words_on_this_line = split(" ",$line);
        $word_count += scalar(@words_on_this_line);
    }
    else {
        $now_in_par = 0;
    }
}

print "$word_count $line_count $par_count\n";
