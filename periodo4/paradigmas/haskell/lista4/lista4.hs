primeiroElemento :: (String, String, Char) -> String
primeiroElemento (primeiro, _, _) = primeiro

segundoElemento :: (String, String, Char) -> String
segundoElemento (_, segundo, _) = segundo

terceiroElemento :: (String, String, Char) -> Char
terceiroElemento (_, _, terceiro) = terceiro

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


