program final_alg1;

const  MIN = 0;
       MAX = 200; 

type matriz = array [MIN..MAX,MIN..MAX] of longint;

var n, u, n_quadrados, maior_quadrado, maior_soma, quadrado_atual: longint;
    m: matriz;
    
procedure ler_matriz(var w: matriz; var tam: longint);
var i,j: longint;
begin
    for i:= 1 to tam do
        for j:= 1 to tam do
            read(w[i,j]);
end;

function quadrado(var w: matriz; var inicio, final: longint): longint;
var i, j, cont, soma: longint;

begin
    cont:= 1;
    soma:= 0;
    i:= inicio;
    if (final - inicio) <> 1 then
    begin
        while cont <= 2 do
        begin
            j:= inicio;
            while j <= final do
            begin
                soma:= soma + w[i,j];
                j:= j + 1;
            end;
            i:= final;
            cont:= cont + 1;
        end;
        j:= inicio;
        cont:= 1;
        while cont <= 2 do
        begin
            i:= inicio + 1;
            while i < final do
            begin
                soma:= soma + w[i,j];
                i:= i + 1;
            end;
            j:= final;
            cont:= cont + 1;
        end;
    end
    else
    begin
        while i <= final do
        begin
            j:= inicio;
            while j <= final do
            begin
                soma:= soma + w[i,j];
                j:= j + 1;
            end;
            i:= i + 1;
        end;
    end;
    quadrado:= soma;
    inicio:= inicio + 1;
    final:= final - 1;
end;

begin
    u:= 1; 
    n_quadrados:= 1;
    read(n);
    maior_soma:= -2147483648;
    ler_matriz(m,n);
    while n > u do
    begin
        quadrado_atual:= quadrado(m,u,n);
        writeln('Quadrado ', n_quadrados,': ', quadrado_atual);
        if quadrado_atual > maior_soma then
        begin
            maior_soma:= quadrado_atual;
            maior_quadrado:= n_quadrados;
        end;
        n_quadrados:= n_quadrados + 1;
    end;
        writeln('Quadrado com maior soma: ', maior_quadrado);
end.
