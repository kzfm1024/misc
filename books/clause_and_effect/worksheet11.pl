squint([], []).
squint([X|T], [Y|L]) :- integer(X), Y is X * X, squint(T, L).
squint([X|T], [X|L]) :- squint(T, L).
