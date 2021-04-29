%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Sample queries
% ?- assertz(faultynode(56)).
% ?- assertz(faultynode(59)).
% ?- retract(faultynode(48)).
% ?- retract(faultynode(68)).
% ?- shortest_path(11,99, Result).

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


% To make the predicates dynamic
:- (dynamic faultynode/1).

% To include the predicates for the generated graph
:- include('Mazedata.pl').

find_path(Src, Dest, Route, Path_Length) :-
    traverse(Src, Dest, [Src], Reverse_Path, Path_Length, 1),   % the function call which creates the path to the destination
    nb_getval(min_path_length, Val),        % quering the minimum length of any path found till now connecting Src and Dest
    Path_Length<Val,                        % proceed only if the new generated path is smaller than the current minimum length
    nb_setval(min_path_length, Path_Length),    % assign min_path_length as the new length of path, which is smaller than previous min_path_length
    reverse(Reverse_Path, Route),           % saving the reverse of Reverse_Path in Route
    nb_setval(shortest_found_path, Route).  % saving the current path as min_path since length of current path is less than previous min_path which was stored in min_path_length

traverse(Src, Dest, P, [Dest|P], L, _) :-
    mazelink(Src, Dest),                    % checking if there exist edge between Src and Dest
    L=1.                                    % setting L = 1 in base case of recursion. 

traverse(Src, Dest, Visited_Nodes, Route, L, Current_Depth) :-
    mazelink(Src, C),                       % taking all nodes which are neighbours of Src
    C\==Dest,                               % checking C is not equal to Dest
    \+ member(C, Visited_Nodes),            % checking C is not already visited
    \+ faultynode(C),                       % checking C is not a faulty node
    Current_Depth2 is Current_Depth+1,      % getting the minimum length of recursion will be.
    nb_getval(min_path_length, Value),      % getting min_path_length value
    Current_Depth2<Value,                   % proceding the unfolding of the recursion only if the minimum possible length of recursion for this path is less than min_path_length
    traverse(C, Dest, [C|Visited_Nodes], Route, L1,Current_Depth2),   % recursive call to traverse with updated visited variable value
    L is L1+1,                              % length of path
    Value>L.                                % making sure that this new path length is less than previous minimum length otherwise we may have 2 paths with same length which is not necessary.
       
pseudo_shortest_path(Src, Dest, _, _) :-
    nb_setval(min_path_length, 1000),               % setting the value of global variable min_path_length as 1000
    nb_setval(shortest_found_path, [Src]),          % setting the value of global variable shortest_found_path as [Src]
    forall(find_path(Src, Dest, _, _), write('')),  % for all solutions of find_path functions
    nb_getval(shortest_found_path, Valll),          % getting the value of shortest_found_path and printing it
    write('Minimum path is '),
    write(Valll),nl,
    nb_getval(min_path_length, Length),             % getting the value of min_path_length and printing it
    write('Length of minimum path is '),
    write(Length),nl,!.

pseudo_shortest_path(_, _, _, _) :-
    write('No path exist').                         % printing no path exist if such case happens.

shortest_path(Src, Dest, Result) :-
    pseudo_shortest_path(Src, Dest, Result, _).