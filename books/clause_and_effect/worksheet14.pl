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

reduce([X|T], X, T).
reduce([H|T], X, [H|L]) :- reduce(T, X, L).

path(X, X, L).
path(X, Y, L) :- a(X, Z), reduce(L, Z, L1), path(Z, Y, L1).

path2(X, X, L).
path2(X, Y, L) :- a(X, Z), reduce(L, X, L0), reduce(L0, Z, L1), path2(Z, Y, L1).
