-- function binary_search takes the input number whose square root has to be found, lower bound for square root, upper bound for square root and returns square root
binary_search :: Double -> Double -> Double -> Double

-- calls the binary_search with appropriate limits for square root 
mySqrt :: Double -> Double


binary_search number lower_bound upper_bound =
 	if number /= 0 && number /= 1
       	then do 
       	let mid = (lower_bound + upper_bound)/2
       	if mid * mid <= number && (mid + 0.000001) * (mid + 0.000001) >= number
       		then mid
       	else if mid * mid < number
       		then binary_search number mid upper_bound
       	else binary_search number lower_bound mid       
 	else if number ==1
 		then 1
 	else 0

mySqrt number = 
	if number >=1 
		-- for number greater than 1 square root is between 1 and that number
		then binary_search number 1 number
	-- for number between 0 to 1, square root of number is greater than number. So setting the limits as number, 1.
	else binary_search number number 1

main = do
	putStrLn "Some sample input cases and their outputs are shown below"
	putStr( "Number is " )
	putStrLn "0"
	putStr( "Square root of the number is " )
	print( mySqrt 0 )
	putStr( "Number is " )
	putStrLn "1"
	putStr( "Square root of the number is " )
	print( mySqrt 1 )
	putStr( "Number is " )
	putStrLn "0.5"
	putStr( "Square root of the number is " )
	print( mySqrt 0.5 )
	putStr( "Number is " )
	putStrLn "7845"
	putStr( "Square root of the number is " )
	print( mySqrt 7845 )
	putStr( "Number is " )
	putStrLn "123456789"
	putStr( "Square root of the number is " )
	print( mySqrt 123456789 )
	putStr( "Number is " )
	putStrLn "9801"
	putStr( "Square root of the number is " )
	print( mySqrt 9801 )
	putStr( "Number is " )
	putStrLn "0.007861"
	putStr( "Square root of the number is " )
	print( mySqrt 0.007861 )
	
	putStrLn ""
	putStrLn "Input the number whose square root has to be found"
	temp_number <- getLine 
	let number = ( read temp_number :: Double )
	putStr( "Square root of the number is " )
	print( mySqrt number )
	
