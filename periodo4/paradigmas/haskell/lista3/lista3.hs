somaLista :: [Int] -> Int
somaLista [] = 0
somaLista (h:t) = h + somaLista t

removerPares :: [Int] -> [Int]
removerPares [] = []
removerPares (h:t)
    | mod h 2 == 0 = removerPares t
    | otherwise = h : removerPares t

inverterLista :: [Float] -> [Float]
inverterLista [] = []
inverterLista (h:t) = inverterLista t ++ [h]

nomesComecamComA :: [String] -> [String]
nomesComecamComA [] = []
nomesComecamComA (h:t)
    | head h == 'A' = h : nomesComecamComA t
    | otherwise = nomesComecamComA t

listaA = [3*x | x <- [0..5]]

listaB = [[x] | x <- [1..5]]

tuplaMultiplos :: Int -> (Int, Int, Int, Int)
tuplaMultiplos n = (2 * n, 3 * n, 4 * n, 5 * n)

tuplaParImpar :: Int -> (Int, String)
tuplaParImpar n = (divisao, parImpar)
  where
    divisao = div n 2
    parImpar = if mod n 2 == 0 then "PAR" else "IMPAR"

somaPositivosProdutoNegativos :: [Int] -> (Int, Int)
somaPositivosProdutoNegativos [] = (0, 1)
somaPositivosProdutoNegativos (h:t)
    | h > 0 = (soma + h, produto)
    | h < 0 = (soma, produto * t)
    | otherwise = (soma, produto)
  where
    (soma, produto) = somaPositivosProdutoNegativos t 

type Nome = String
type Nota = Int
type Tipo = String
type PontoTuristico = (Nome, Nota, Tipo)
type ListaPontosTuristicos = [PontoTuristico]
filtraPontosTuristicosCWB :: ListaPontosTuristicos -> Nota -> [(Nome, Nota)]
filtraPontosTuristicosCWB pontos referencia =
    [(nome, nota) | (nome, nota, tipo) <- pontos, nota > referencia, tipo == "parque" || tipo == "museu"]
