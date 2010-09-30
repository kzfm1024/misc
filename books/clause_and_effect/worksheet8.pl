max([], A, A).
max([H|T], A, M) :- H > A, max(T, H, M).
max([H|T], A, M) :- H =< A, max(T, A, M).

%
% Practice 1
%
maximum([], M) :- maximum([], M).
maximum([H|T], M) :- max(T, H, M).

%
% Practice 3
%
min([], A, A).
min([H|T], A, M) :- H < A, min(T, H, M).
min([H|T], A, M) :- H >= A, min(T, A, M).

minmax([], Min, Max) :- maximum([], Min, Max).
minmax([H|T], Min, Max) :- min(T, H, Min), max(T, H, Max).

%max([3, 1, 4, 1, 5, 8, 2, 6], 0, N).
%max([2, 4, 7, 7, 7, 2, 1, 6], 5, N).
