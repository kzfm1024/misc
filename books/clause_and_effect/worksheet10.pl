sqlist([], []).
sqlist([X|T], [Y|L]) :- Y is X * X, sqlist(T, L).

sqterm([], []).
sqterm([X|T], [s(X, Y)|L]) :- Y is X * X, sqterm(T, L).

fullmap([], []).
fullmap([X|T], [Y|L]) :- transform(X, Y), fullmap(T, L).

envelope([], []).
envelope([X|T], [container(X)|L]) :- envelope(T, L).

transform(the, 17).
transform(cat, 23).
transform(sits, 46).
transform(on, 9).
transform(mat, 2).

%sqlist([1, 2, 3, 4, 5], X).

%envelope([apple, peach, cat, 37, john], X).

%fullmap([the, cat, sits, on, the, mat], X).
