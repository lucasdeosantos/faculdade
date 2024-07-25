module Main where

import Tratamento

main :: IO ()
main = do
    let senhores = ["Tyrion", "Jaime", "Ned", "Robert", "Robb", "Jon"]
    let senhoras = ["Cercei", "Daenerys", "Catelyn", "Lysa", "Selise", "Arianne"]
    let senhoritas = ["Sansa", "Arya", "Myrcella"]

    let srs = adicionarTratamento senhores sr 
    let sras = adicionarTratamento senhoras sra
    let srtas = adicionarTratamento senhoritas srta

    putStrLn $ "Senhores: " ++ show srs
    putStrLn $ "Senhoras: " ++ show sras
    putStrLn $ "Senhoritas: " ++ show srtas
