evens([], []).
evens([X|T], [X|L]) :- 0 is X mod 2, evens(T, L).
evens([X|T], L) :- 1 is X mod 2, evens(T, L).

%evens([1, 2, 3, 4, 5, 6], Q).

prohibit(bother).
prohibit(blast).
prohibit(drat).
prohibit(fiddlestick).

censor([], []).
censor([X|T], L) :- prohibit(X), censor(T, L).
censor([X|T], [X|L]) :- censor(T, L).

%censor([i, bother, my, blast, me, drat, mine, fiddlestick], Q).
