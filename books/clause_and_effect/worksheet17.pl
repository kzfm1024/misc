ms([], _, []).
ms([H|T], N, [C|L]) :- C is H + N, ms(T, C, L).
mapsum(A, B) :- ms(A, 0, B).

en([], _, []).
en([H|T], I, [n(H, I)|L]) :- I1 is I + 1,  en(T, I1, L).
enum(A, B) :- en(A, 1, B).
