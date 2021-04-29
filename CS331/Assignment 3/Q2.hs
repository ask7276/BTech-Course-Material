-- fibonacci calls the calculate function accordingly for what index the value of fibonacci number is queried.
fibonacci :: Integer -> Integer

-- calculates the fibonacci number in bottom up fashion
calculate :: Integer -> Integer -> Integer -> Integer

-- calculates and recursively calls calculate function with appropriate parameters. If we are trying to find the Nth fibonacci number, parameters to calculate function at any point is are index Fibonacci_number(N - index) Fibonacci_number(N - index +1)
calculate index num1 num2 = 
	if index /= 0
		then do 
		let num = num1 + num2
		let temp_index = index - 1
		-- calculate index Fibonacci_number(N - index) Fibonacci_number(N - index + 1) is calling 
		-- calculate (index - 1) Fibonacci_number(N - index + 1) Fibonacci_number(N - index + 2)
		calculate temp_index num2 num
	else num1


fibonacci index = 
	if index /= 1 && index /= 2
		then do
		calculate index 0 1
	else 1
	
	
main = do
	putStrLn "Some sample input cases and their outputs are shown below"
	putStr "Index is "
	putStrLn "1"
	putStr( "Corresponding fibonacci number is " )
	print( fibonacci 1 )
	putStr "Index is "
	putStrLn "2"
	putStr( "Corresponding fibonacci number is " )
	print( fibonacci 2 )
	putStr "Index is "
	putStrLn "5"
	putStr( "Corresponding fibonacci number is " )
	print( fibonacci 5 )
	putStr "Index is "
	putStrLn "10"
	putStr( "Corresponding fibonacci number is " )
	print( fibonacci 10 )
	putStr "Index is "
	putStrLn "37"
	putStr( "Corresponding fibonacci number is " )
	print( fibonacci 37 )
	putStr "Index is "
	putStrLn "1290"
	putStr( "Corresponding fibonacci number is " )
	print( fibonacci 1290 )
	putStr "Index is "
	putStrLn "200"
	putStr( "Corresponding fibonacci number is " )
	print( fibonacci 200 )
	
	putStrLn("")
	putStrLn "Input the index of which fibonacci number has to be found."
	temp_number <- getLine 
	let number = ( read temp_number :: Integer )
	putStr( "Corresponding fibonacci number is " )
	print( fibonacci number )
			
