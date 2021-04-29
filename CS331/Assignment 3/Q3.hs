import System.Environment

-- Ord a defines that an ordered list is taken as input
-- myQuickSort is used to sort the input list
myQuickSort :: (Ord a) => [a] -> [a]  

-- Returning empty list for empty input
myQuickSort [] = []  

-- using the last element as the pivot for dividing the array into 2 arrays
myQuickSort input_list = smaller_list ++ equal_list ++ greater_list
    where
    	-- setting the last number of the list as pivot
        pivot = last input_list
        -- sorting the list of smaller elements than pivot
        smaller_list = myQuickSort [element | element <- input_list, element < pivot]
        equal_list = [element | element <- input_list, element == pivot]
        -- sorting the list of greater elements than pivot
        greater_list = myQuickSort [element | element <- input_list, element > pivot]


main = do
	putStrLn "Some sample input cases and their outputs are shown below"
	putStr "List is "
	putStrLn "[1,2,3,4,5,6]"
	putStr( "Corresponding sorted list is " )
	print( myQuickSort [1,2,3,4,5,6] )
	putStr "List is "
	putStrLn "[9,8,7,6,5,4,3,2,1]"
	putStr( "Corresponding sorted list is " )
	print( myQuickSort [9,8,7,6,5,4,3,2,1] )
	putStr "List is "
	putStrLn "[97]"
	putStr( "Corresponding sorted list is " )
	print( myQuickSort [97] )
	putStr "List is "
	putStrLn "[65,945480,8756,497000,5,8794,8756,947,5]"
	putStr( "Corresponding sorted list is " )
	print( myQuickSort [65,945480,8756,497000,5,8794,8756,947,5] )
	putStr "List is "
	putStrLn "[5,8,9,1,0,0,0,0,0,0]"
	putStr( "Corresponding sorted list is " )
	print( myQuickSort [5,8,9,1,0,0,0,0,0,0] )
	putStr "List is "
	putStrLn "[7,7,7,7,7,7,7]"
	putStr( "Corresponding sorted list is " )
	print( myQuickSort [7,7,7,7,7,7,7] )
	putStr "List is "
	putStrLn "[54,98,3,5,884,657,481,9978,446,2,55,4,8,111,957,0]"
	putStr( "Corresponding sorted list is " )
	print( myQuickSort [54,98,3,5,884,657,481,9978,446,2,55,4,8,111,957,0] )
	
	putStrLn ""
	putStrLn ""
	-- getting the list of number as command line input with each number in string format
  	input <-getArgs
  	-- converting the numbers in string format to Int format
  	let number_list = [ read num :: Int | num <- input ]
  	putStr( "Sorted Input List is " )
  	print ( myQuickSort number_list)

