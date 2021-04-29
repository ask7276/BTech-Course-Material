% this function checks the mid value between the left limit and right limit. 

% If the | Mid * Mid - X | < Accuracy, it prints the Mid value.

calculateSquareRoot(X, Left_Limit, Right_Limit, Accuracy) :- (
    Mid is (Left_Limit + Right_Limit)/2,
    Mid * Mid - X + Accuracy =< 0 ->
    calculateSquareRoot(X, Mid, Right_Limit, Accuracy)
    ;
    Mid is (Left_Limit + Right_Limit)/2,
    Mid * Mid - X - Accuracy >= 0 ->
    calculateSquareRoot(X, Left_Limit, Mid, Accuracy)
    ;
    Mid is (Left_Limit + Right_Limit)/2,
    write('Square root of the number is '),
    write(Mid)
).

% This function either directly writes the message on the terminal or calls the calcualteSquareRoot function 
% depending upon the case.

squareroot(X, Result, Accuracy) :- (
    X < 0 ->
    write('Entered number is negative. Square root exists only for non negative elements'),nl
    ;
    X =:= 0 -> 
    write('Answer is 0'),nl
    ;
    X < 1 -> 
    calculateSquareRoot(X, 2*Result, 1 , Accuracy)
    ;
    X =:= 1 ->
    write('Answer is 1'), nl
    ;
    calculateSquareRoot(X, 1, 2*Result, Accuracy)
).