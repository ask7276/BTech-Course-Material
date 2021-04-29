-- function definition to convert input comma seperated string of numbers into array of numbers
convertStringToArray :: String -> [Int]

convertStringToArray temp_array = 
    do
    let array_of_numbers = ( read temp_array :: [Int] )
    array_of_numbers

-- function to calculate gcd of 2 numbers
myGCD :: Int -> Int -> Int

myGCD num1 num2 = 
    if num1 /= 0
        then do
        let temp = num2 `rem` num1
        myGCD temp num1
    else num2

-- calculate the lcm of numbers in array
returnLCMofList :: Int -> [Int] -> Int

returnLCMofList current_lcm [] = current_lcm
returnLCMofList current_lcm (first_number:numbers_array) = 
    do
        let ans = current_lcm * first_number `div` (myGCD current_lcm first_number)
        returnLCMofList ans numbers_array 

-- check if there is negative integer in array
checkIfNegative :: [Int] -> Int

checkIfNegative [] = 0
checkIfNegative (first_number:numbers_array) = 
    if first_number <= 0
        then 1
    else checkIfNegative numbers_array

-- output the appropriate ans
myOutput :: [Int] -> IO()

myOutput numbers_array = 
    -- if there is negative number or 0 in the array show exception
    if checkIfNegative numbers_array == 1
        then putStrLn "Please enter a list of positive numbers."
    else 
        do
            let ans = returnLCMofList 1 numbers_array
            putStr "LCM of list of numbers given as input is "
            print(ans)
        
    
main = do
    print([1,2,3,4,5,6,7,8])
    myOutput [1,2,3,4,5,6,7,8]
    print([2,5,-1,7])
    myOutput [2,5,-1,7]
    print([4,0,6])
    myOutput [4,0,6]
    print([13,7,91])
    myOutput [13,7,91]
    putStrLn "Please input a comma seperated list of integers enclosed in square brackets like [1,2,3,4,5]"
    temp_array <- getLine
    let numbers_array = convertStringToArray temp_array
    myOutput numbers_array