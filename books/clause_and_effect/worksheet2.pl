drinks(john, martini).
drinks(mary, gin).
drinks(susan, vodka).
drinks(john, gin).
drinks(fred, gin).

pair(X, Y, Z) :- drinks(X, Z), drinks(Y, Z), X \== Y.
