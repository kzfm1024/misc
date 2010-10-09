% 
% No permission to modify static_procedure `length/2'
%
length2([], 0).
%length2([H|T], N) :- length2(T, Nt), N is Nt + 1.
length2([_|T], N) :- length2(T, Nt), N is Nt + 1.

%length2(L, N) :- accumulate(L, 0, N).
%accumulate([], A, A).
%accumulate([H|T], A, N) :- A1 is A + 1, accumulate(T, A1, N).

sum([], 0).
sum([H|T], S) :- sum(T, St), S is H + St.
