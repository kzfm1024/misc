runcode([], C, N, [N*C]).
runcode([H|T], H, N, Z) :- N1 is N + 1, runcode(T, H, N1, Z).
runcode([H|T], C, N, [N*C|Z]) :- H \== C, runcode(T, H, 1, Z).

runcode2([], []).
runcode2([H|T], Z) :- runcode(T, H, 1, Z).
