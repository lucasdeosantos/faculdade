module Revisao where

somaLista :: [Int] -> Int
somaLista [] = 0
somaLista (h:t) = h + somaLista t

produtoLista :: [Int] -> Int
produtoLista [] = 1
produtoLista (h:t) = h * produtoLista t

ehPar :: Int -> Bool
ehPar n
    | mod n 2 == 0 = True
    | otherwise = False

fatorial :: Int -> Int
fatorial 0 = 1
fatorial n = n * fatorial (n - 1)

fibonacci :: Int -> Int
fibonacci 0 = 0
fibonacci 1 = 1
fibonacci n = fibonacci (n-1) + fibonacci (n - 2)

sequenciaFibonacci = [fibonacci x | x <- [0,1..]]

contar :: Eq a => a -> [a] -> Int
contar _ [] = 0
contar n (h:t)
    | h == n = 1 + contar n t
    | otherwise = contar n t

filtrarPares :: [Int] -> [Int]
filtrarPares lista = filter even lista
-- filtrarPares [] = []
-- filtrarPares (h:t)
--    | mod h 2 == 0 = h : filtrarPares t ; even h = h : filtrarPares t
--    | otherwise = filtrarPares t

reverterLista :: Eq a => [a] -> [a]
reverterLista [] = []
reverterLista (h:t) = reverterLista t ++ [h]

maior :: [Int] -> Int
maior [] = error "Lista Vazia"
maior [x] = x
maior (h:t) = max h (maior t)

elementosUnicos :: Eq a => [a] -> [a]
elementosUnicos lista = elementosUnicosAux lista []
  where
    elementosUnicosAux :: Eq a => [a] -> [a] -> [a]
    elementosUnicosAux [] resultado = resultado
    elementosUnicosAux (h:t) resultado
      | elem h resultado = elementosUnicosAux t resultado
      | otherwise = elementosUnicosAux t (resultado ++ [h])

tabuada :: Int -> Int -> [Int]
tabuada x y = [x * n | n <- [1..y]]

paraBinario :: Int -> [Int]
paraBinario 0 = [0]
paraBinario 1 = [1]
paraBinario n = paraBinario (div n 2) ++ [mod n 2] 

ehPalindromo :: Eq a => [a] -> Bool
ehPalindromo lista = lista == reverse lista

gerarLista :: Int -> [Int]
gerarLista n = [x | x <- [1..n]]

mapear :: (a -> b) -> [a] -> [b]
mapear _ [] = []
mapear f (h:t) = f h : mapear f t
-- mapear f list = map f list  

produtoPares :: [(Int, Int)] -> [Int]
produtoPares [] = []
produtoPares ((x, y) : t) = x + y : produtoPares t
                        -- [x + y] ++ produtoPares t    
                        
intersecao :: Eq a => [a] -> [a] -> [a]
intersecao c1 c2 = [x | x <- c1, elem x c2]

diferenca :: Eq a => [a] -> [a] -> [a]
diferenca c1 c2 = [x | x <- c1, notElem x c2]

duplicar :: [Int] -> [Int]
duplicar [] = []
duplicar (h:t) = h : h : duplicar t
-- duplicar (h:t) = [h] ++ [h] ++ duplicar t
