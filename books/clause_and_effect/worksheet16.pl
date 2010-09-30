firstcol([], []).
%firstcol([[H|T]|R], [H|Hs]) :- firstcol(R, Hs).
firstcol([[H|_]|R], [H|Hs]) :- firstcol(R, Hs).

nextcols([], []).
%nextcols([[H|T]|R], [T|Ts]) :- nextcols(R, Ts).
nextcols([[_|T]|R], [T|Ts]) :- nextcols(R, Ts).

transpose([[]|_], []).
transpose(R, [H|C]) :- firstcol(R, H), nextcols(R, T), transpose(T, C).

chopcol([], [], []).
chopcol([[H|T]|R], [H|Hs], [T|Ts]) :- chopcol(R, Hs, Ts).

transpose2([[]|_], []).
transpose2(R, [H|C]) :- chopcol(R, H, T), transpose2(T, C).
