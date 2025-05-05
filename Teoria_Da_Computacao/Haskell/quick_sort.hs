-- Comentário
quickSort :: (Ord a) => [a] -> [a]
quickSort [] = [] -- lista vazia já está ordenada
quickSort (x:xs) = 
    let menor = quickSort [y | y <- xs, y <= x]  -- numeros menores ou iguais ao pivo
        maior = quickSort [y | y <- xs, y > x]   -- numeros maiores que o pivo
    in menor ++ [x] ++ maior -- combinacao das partes ordenadas com o pivo

main :: IO ()
main = do
    let lista = [5, 1, 9, 4, 6, 7, 3]
    putStrLn "lista original:"
    print lista
    putStrLn "lista ordenada:"
    print (quickSort lista)
