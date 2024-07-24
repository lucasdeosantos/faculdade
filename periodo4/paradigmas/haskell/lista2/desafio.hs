num :: Int -> Float
num 1 = 5.0
num 2 = 10.0
num 3 = 15.0
num 4 = 20.0
num 5 = 25.0

auxSoma :: Int -> Float
auxSoma 0 = 0.0
auxSoma n = num n + auxSoma (n - 1) 

soma :: Float
soma = auxSoma 5

auxMedia :: Int -> Float
auxMedia n = soma / fromIntegral n

media :: Float
media = auxMedia 5
