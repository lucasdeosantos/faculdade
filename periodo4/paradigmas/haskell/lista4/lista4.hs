primeiroElemento :: (String, String, Char) -> String
primeiroElemento (x, _, _) = x

segundoElemento :: (String, String, Char) -> String
segundoElemento (_, y, _) = y

terceiroElemento :: (String, String, Char) -> Char
terceiroElemento (_, _, z) = z

type Nome = String
type Titulo = String
type Genero = Char
type Pesquisador = (Nome, Titulo, Genero)
type Grupo = [Pesquisador]

base :: Int -> Pesquisador
base x
    | x == 1 = ("joao", "mestre", 'm')
    | x == 2 = ("jonas", "doutor", 'm')
    | x == 3 = ("joice", "mestre", 'f')
    | x == 4 = ("janete", "doutor", 'f')
    | x == 5 = ("jocileide", "doutor", 'f')
    | otherwise = ("ninguem", "", 'x')

pesquisadoresBase :: Int -> Grupo
pesquisadoresBase 0 = []
pesquisadoresBase n = [base n] ++ pesquisadoresBase (n - 1) 

doutores :: Grupo -> [Nome]
doutores pesquisadores = [ nome | (nome, titulo, genero) <- pesquisadores, titulo == "doutor"]

contMestres :: Grupo -> Int
contMestres [] = 0
contMestres (h:t)
    | segundoElemento h == "mestre" = 1 + contMestres t   
    | otherwise = contMestres t

contDoutores :: Grupo -> Int
contDoutores [] = 0
contDoutores (h:t)
    | segundoElemento h == "doutor" = 1 + contDoutores t   
    | otherwise = contDoutores t

contM :: Grupo -> Int
contM [] = 0
contM (h:t)
    | terceiroElemento h == 'm' = 1 + contM t   
    | otherwise = contM t

contF :: Grupo -> Int
contF [] = 0
contF (h:t)
    | terceiroElemento h == 'f' = 1 + contF t   
    | otherwise = contF t

contMD :: (Grupo -> Int) -> Grupo -> Int
contMD f pesquisadores = f pesquisadores
