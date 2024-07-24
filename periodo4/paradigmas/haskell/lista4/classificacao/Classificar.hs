module Classificar where

positivo :: Int -> Bool
positivo n
    | n > 0 = True
    | otherwise = False

negativo :: Int -> Bool
negativo n
    | n < 0 = True
    | otherwise = False

zero :: Int -> Bool
zero n
    | n == 0 = True
    | otherwise = False

classificarElementos :: [Int] -> (Int -> Bool) -> [Int]
classificarElementos [] _ = []
classificarElementos (h:t) condicao
    | condicao h == True = h : classificarElementos t condicao
    | otherwise = classificarElementos t condicao
