% special cases for sublist function.
sublist([], List2) :- write('yes'), nl, halt.
sublist([H|List1], []) :- write('no'),nl, halt.

% if the first elements of the lists are same, then apply the sublist function on tails.
sublist([X | List1_Tail], [X | List2_Tail]) :- sublist(List1_Tail, List2_Tail).

% if the first elements of the lists are different, then apply the sublist function with first list and tail of 2nd list
sublist([X | List1_Tail], [Y | List2_Tail]) :- sublist([X| List1_Tail], List2_Tail).
