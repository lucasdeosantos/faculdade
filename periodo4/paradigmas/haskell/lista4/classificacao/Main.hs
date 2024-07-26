module Main where

import Classificar

main :: IO ()
main = do
    let arr = [-1, 2, -3, 4, -5, 6, -7, 8, -9, 0, 1, -2, 3, -4, 5, -6, 7, -8, 9]
    putStrLn $ "Positivos: " ++ show (classificarElementos arr positivo)
    putStrLn $ "Negativos: " ++ show (classificarElementos arr negativo)
    putStrLn $ "Positivos: " ++ show (classificarElementos arr zero)
