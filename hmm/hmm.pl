#
# 隠れマルコフモデルの演習問題実装 (Perl)
# http://www.bioinfo.sfc.keio.ac.jp/class/bioinfo-a/PPT/bioinfo-a09s-6_HMM.pdf
#
@type = ( "TYPE_S", "TYPE_N",
		  "TYPE_N", "TYPE_N",
		  "TYPE_N", "TYPE_N" );

@x = split("", "atga");

@t = ...

@e = ...

	sub viterbi($$){
		my($l, $i) = @_;
		my($k, $k_max);
		my($p, $p_max);
		if($type[$l] eq "TYPE_S"){
			if($i <= 0){ return 0.0; }
			else { return $::LOG0; }
		}
		elsif($type[$l] eq "TYPE_N"){
			if($i <= 0){ return $::LOG0; }
			for($p_max = $::LOG0 - 1, $k = 0;$k < $::K;$k ++){
				if($tr[$k]->[$l] > 0.0){ $p = viterbi($k, $i - 1) + log($tr[$k]->[$l]); }
				else { $p = $::LOG0; }
				if($p > $p_max){ $p_max = $p; $k_max = $k; }
			}
			return $p_max + log($e[$l]->[ $cton{ $x[$i - 1] } ]);
		}
}
