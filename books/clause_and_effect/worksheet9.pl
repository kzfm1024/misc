a(g, h).
a(d, a).
a(g, d).
a(e, d).
a(h, f).
a(e, f).
a(a, e).
a(a, b).
a(b, f).
a(b, c).
a(f, c).

%path(X, X, T).
path(X, X, _).
path(X, Y, T) :- a(X, Z), legal(Z, T), path(Z, Y, [Z|T]).

%legal(Z, []).
legal(_, []).
legal(Z, [H|T]) :- Z \== H, legal(Z, T).
