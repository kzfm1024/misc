membercheck(X, [X|_) :- !.
membercheck(X, [_|L]) :- membercheck(X, L).

max(X, Y, X) :- X >= Y, !.
max(X, Y, Y).

drink(milk).
drink(beer) :- !.
drink(gin).

class(N, pos) :- N > 0.
class(0, zero).
class(N, neg) :- N < 0.

class2(N, pos) :- N > 0.
class2(0, zero) :- !.
class2(_, neg).


