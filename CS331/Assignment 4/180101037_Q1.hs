-- function definition to convert input comma seperated string of numbers into array of numbers
convertStringToArray :: String -> [Int]

convertStringToArray temp_array = 
    do
    let array_of_numbers = ( read temp_array :: [Int] )
    array_of_numbers

main = do
    print("[2,4,1,7,5]")
    let temp = convertStringToArray "[2,4,1,7,5]"
    print(temp)
    print("[5,33,7,7,0,00,666]")
    let temp = convertStringToArray "[5,33,7,7,0,00,666]"
    print(temp)

    putStrLn "Please input a comma seperated list of integers enclosed in square brackets like [1,2,3,4,5]"
    temp_array <- getLine
    let numbers_array = convertStringToArray temp_array
    print(numbers_array)