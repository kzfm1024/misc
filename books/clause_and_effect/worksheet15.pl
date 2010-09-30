herd([], [], []).
herd([sheep|T], [sheep|S], G) :- herd(T, S, G).
herd([goat|T], S, [goat|G]) :- herd(T, S, G).
herd([X|T], S, G) :- herd(T, S, G).

herd2([], [], [], []).
herd2([sheep|T], [sheep|S], G, Z) :- herd2(T, S, G, Z).
herd2([goat|T], S, [goat|G], Z) :- herd2(T, S, G, Z).
herd2([X|T], S, G, [X|Z]) :- herd2(T, S, G, Z).

alternate([], [], []).
alternate([H1|[H2|T]], [H1|X], [H2|Y]) :- alternate(T, X, Y).
