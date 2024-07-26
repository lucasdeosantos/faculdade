maioridadeGuardas :: Int -> Bool
maioridadeGuardas idade 
    | idade >= 18 = True
    | otherwise = False

maioridadeIfElse :: Int -> Bool
maioridadeIfElse idade = if idade >= 18
    then True
    else False

classificarNumeroGuardas :: Int -> String
classificarNumeroGuardas n
    | n > 0 = "Positivo"
    | n < 0 = "Negativo"
    | otherwise = "Zero"

classificarNumeroIfElse :: Int -> String
classificarNumeroIfElse n =
    if n > 0 then "Positivo"
    else if n < 0 then "Negativo"
    else "Zero"

anoBisextoGuardas :: Int -> Bool
anoBisextoGuardas ano
    | mod ano 400 == 0 = True
    | mod ano 100 == 0 = False
    | mod ano 4 == 0 = True
    | otherwise = False

anoBisextoGuardasWhere :: Int -> Bool
anoBisextoGuardasWhere ano
    | divisivel 400 = True
    | divisivel 100 = False
    | divisivel 4 = True
    | otherwise = False
  where
    divisivel n = mod ano n == 0

anoBisextoIfElse :: Int -> Bool
anoBisextoIfElse ano =
    if mod ano 400 == 0 then True
    else if mod ano 100 == 0 then False
    else if mod ano 4 == 0 then True
    else False

anoBisextoIfElseWhere :: Int -> Bool
anoBisextoIfElseWhere ano = 
    if divisivel 400 then True
    else if divisivel 100 then False
    else if divisivel 4 then True
    else False
  where
    divisivel n = mod ano n == 0 

fibonacci :: Int -> Int
fibonacci 0 = 0
fibonacci 1 = 1 
fibonacci n = fibonacci (n - 1) + fibonacci (n - 2)

fibonacciGuardas :: Int -> Int
fibonacciGuardas n
    | n == 0 = 0
    | n == 1 = 1
    | n > 1 = fibonacciGuardas (n - 1) + fibonacciGuardas (n - 2)

fibonacciIfElse :: Int -> Int
fibonacciIfElse n =
    if n == 0 then 0
    else if n == 1 then 1
    else fibonacciIfElse (n - 1) + fibonacciIfElse (n - 2)

numeroDigitos :: Int -> Int
numeroDigitos 0 = 0
numeroDigitos n = 1 + numeroDigitos (div n 10) 
