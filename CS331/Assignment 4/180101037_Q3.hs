-- function definition to convert input comma seperated string of numbers into array of numbers
convertStringToArray :: String -> [Int]

convertStringToArray temp_array = 
    do
    let array_of_numbers = ( read temp_array :: [Int] )
    array_of_numbers


-- defining tree and tree related functions
data Tree a = Nil | Node (Tree a) a (Tree a) 

-- function definition of insert in BST
insertInTree :: Tree Int -> Int -> Tree Int

insertInTree Nil node = Node Nil node Nil
insertInTree (Node left_tree node right_tree) new_node = 
    if node == new_node
        then Node left_tree node right_tree
    else if node < new_node
        then Node left_tree node (insertInTree right_tree new_node)
    else 
        Node (insertInTree left_tree new_node) node right_tree

-- function definition of to create BST from number list
createBST :: [Int] -> Tree Int

createBST [] = Nil
createBST (first_element:tail) = tempCreate (Node Nil first_element Nil) tail
    where
        tempCreate tree [] = tree
        tempCreate tree (first_element:tail) = tempCreate (insertInTree tree first_element) tail

-- inorder traversal of the BST 
inorderTreeTraversal Nil = []
inorderTreeTraversal (Node left_tree node right_tree) = left_tree_traversal ++ [node] ++ right_tree_traversal
    where
        left_tree_traversal = inorderTreeTraversal left_tree
        right_tree_traversal = inorderTreeTraversal right_tree

-- pre order traversal of BST
preorderTreeTraversal Nil = []
preorderTreeTraversal (Node left_tree node right_tree) = [node] ++ left_tree_traversal ++ right_tree_traversal
    where
        left_tree_traversal = preorderTreeTraversal left_tree
        right_tree_traversal = preorderTreeTraversal right_tree

-- post order traversal of BST
postorderTreeTraversal Nil = []
postorderTreeTraversal (Node left_tree node right_tree) = left_tree_traversal ++ right_tree_traversal ++ [node]
    where
        left_tree_traversal = postorderTreeTraversal left_tree
        right_tree_traversal = postorderTreeTraversal right_tree


main = do
    putStrLn "SOme sample cases and their outputs"
    let temp_BST = createBST [2,3,4,5,6]
    print([2,3,4,5,6])
    let inorderTraversal = inorderTreeTraversal temp_BST
    putStr "Inorder traversal of tree gives "
    print(inorderTraversal)
    let preorderTraversal = preorderTreeTraversal temp_BST
    putStr "Preorder traversal of tree gives "
    print(preorderTraversal)
    let postorderTraversal = postorderTreeTraversal temp_BST
    putStr "Postorder traversal of tree gives "
    print(postorderTraversal)
    let temp_BST = createBST [8,3,9,1,4,2,0,6,7,5]
    print([8,3,9,1,4,2,0,6,7,5])
    let inorderTraversal = inorderTreeTraversal temp_BST
    putStr "Inorder traversal of tree gives "
    print(inorderTraversal)
    let preorderTraversal = preorderTreeTraversal temp_BST
    putStr "Preorder traversal of tree gives "
    print(preorderTraversal)
    let postorderTraversal = postorderTreeTraversal temp_BST
    putStr "Postorder traversal of tree gives "
    print(postorderTraversal)
    let temp_BST = createBST [2]
    print([2])
    let inorderTraversal = inorderTreeTraversal temp_BST
    putStr "Inorder traversal of tree gives "
    print(inorderTraversal)
    let preorderTraversal = preorderTreeTraversal temp_BST
    putStr "Preorder traversal of tree gives "
    print(preorderTraversal)
    let postorderTraversal = postorderTreeTraversal temp_BST
    putStr "Postorder traversal of tree gives "
    print(postorderTraversal)

    putStrLn ""

    putStrLn "Please input a comma seperated list of integers enclosed in square brackets like [1,2,3,4,5]"
    temp_array <- getLine
    let numbers_array = convertStringToArray temp_array
    let generated_BST = createBST numbers_array
    let inorderTraversal = inorderTreeTraversal generated_BST
    putStr "Inorder traversal of tree gives "
    print(inorderTraversal)
    let preorderTraversal = preorderTreeTraversal generated_BST
    putStr "Preorder traversal of tree gives "
    print(preorderTraversal)
    let postorderTraversal = postorderTreeTraversal generated_BST
    putStr "Postorder traversal of tree gives "
    print(postorderTraversal)