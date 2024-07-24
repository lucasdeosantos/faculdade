calcularReajuste :: Float -> Float
calcularReajuste salario = salario * 1.25

mediaPonderada :: Float -> Float -> Float -> Float -> Float -> Float -> Float
mediaPonderada n1 p1 n2 p2 n3 p3 = n1 * p1 + n2 * p2 + n3 * p3

converterTemperatura :: Float -> Float
converterTemperatura temp = temp * (9.0 / 5.0) + 32.0

calcularIdade :: Int -> Int
calcularIdade ano = 2032 - ano

calcularMinutos :: Int -> Int -> Int
calcularMinutos hora minutos = hora * 60 + minutos

somaQuadrados1 :: Int -> Int -> Int -> Int
somaQuadrados1 x y z = x * x + y * y + z * z

somaQuadrados2 :: Float -> Float -> Float -> Float
somaQuadrados2 x y z = x ** 2 + y ** 2 + z ** 2

somaQuadrados3 :: Int -> Int -> Int -> Int
somaQuadrados3 x y z = x ^ 2 + y ^ 2 + z ^ 2

verificarImpar1 :: Int -> Bool
verificarImpar1 n
    | mod n 2 == 0 = False
    | otherwise = True

verificarImpar2 :: Int -> Bool
verificarImpar2 n = not (even n)

verificarDivisibilidade :: Int -> Int -> Bool
verificarDivisibilidade x y
    | mod x y == 0 = True
    | otherwise = False

calcularSalario :: Float -> Float
calcularSalario salarioBase = salarioBase + salarioBase * 0.05 - salarioBase * 0.07
