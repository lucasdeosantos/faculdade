program final_alg1;

const  MIN = 0;
       MAX = 200; 

type matriz = array [MIN..MAX,MIN..MAX] of longint;

var n: longint;
    m: matriz;
    
procedure ler_matriz(var w: matriz; var tam: longint);
var i,j: longint;
begin
    for i:= 1 to tam do
        for j:= 1 to tam do
            read(w[i,j]);
end;

procedure quadrados(var w: matriz; var tam: longint);
var i, j, inicio, fim, q_atual, soma, maior_soma, q_maior_soma: longint;
begin
    soma:= 0;
    maior_soma:= -2147483648;
    q_atual:= 1;
    q_maior_soma:= q_atual;
    inicio:= 1;
    fim:= tam;
    
    while inicio < fim do
    begin
        i:= inicio;
        if (inicio - fim) <> 1 then 
        begin
            while i <= fim do
            begin
                j:= inicio;
                while j <= fim do
                begin
                soma:= soma + w[i,j];
                if (i <> inicio) and (i <> fim) then
                    j:= j + (fim - inicio)
                else
                    j:= j + 1;
                end;
            i:= i + 1;
            end;
        end
        else
        begin
            i:= inicio;
            while i <= fim do
            begin
                j:= inicio;
                while j <= fim do
                begin
                    soma:= soma + w[i,j];
                    j:= j + 1;
                end;
                i:= i + 1;
            end;
        end;
        
        writeln('Quadrado ', q_atual, ': ', soma);
        if soma > maior_soma then
        begin
            maior_soma:= soma;
            q_maior_soma:= q_atual;
        end;
        
        soma:= 0;
        q_atual:= q_atual + 1;
        inicio:= inicio + 1;
        fim:= fim - 1;
    end;
    writeln('Quadrado com a maior soma:', q_maior_soma);
end;
        
begin
    read(n);
    ler_matriz(m,n);
    quadrados(m,n);
end.