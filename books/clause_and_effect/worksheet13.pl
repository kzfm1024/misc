member(X, [X|_]).
member(X, [_|T])  :- member(X, T).

setify([], []).
setify([X|T], L) :- member(X, T), setify(T, L).
setify([X|T], [X|L]) :- setify(T, L).
