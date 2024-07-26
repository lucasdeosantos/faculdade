module Tratamento where

sr :: String -> String
sr nome = "Sr." ++ nome

sra :: String -> String
sra nome = "Sra." ++ nome

srta :: String -> String
srta nome = "Srta." ++ nome

adicionarTratamento :: [String] -> (String -> String) -> [String]
adicionarTratamento [] _ = []
adicionarTratamento (h:t) pronome = pronome h : adicionarTratamento t pronome
