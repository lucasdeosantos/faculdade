program quadrado_magico;

const  MIN = 0;
       MAX = 100; 

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

procedure quadrado_perfeito(var w: matriz; var tam: longint);
var i, j, soma, soma_anterior, diagonal: longint;
    nao_e: boolean;
begin
    nao_e:= false;
    soma:= 0;
    soma_anterior:= 0;
    i:= 1;
    diagonal:= 1;
    
    while diagonal <= 2 do
    begin
        if diagonal = 1 then
        begin
            while i <= tam do
            begin
                j:= 1;
                while j <= tam do
                begin
                    soma_anterior:= soma_anterior + w[i,j];
                    j:= j + 1;
                    i:= i + 1;
                end;
            end;
        end
        else
        begin
            i:= 1;
            while i <= tam do
            begin
                j:= tam;
                while j >= 1 do
                begin
                    soma:= soma + w[i,j];
                    j:= j - 1;
                    i:= i + 1;
                end;
            end;
        end;
        diagonal:= diagonal + 1;
    end;
    if soma_anterior <> soma then
        nao_e:= true
    else
    begin
        i:= 1;
        while i <= tam do
        begin
            soma:= 0;
            j:= 1;
            while j <= tam do
            begin
                soma:= soma + w[i,j];
                j:= j + 1;
            end;
            if soma <> soma_anterior then
                nao_e:= true;
            i:= i + 1;
        end;
        
        j:= 1;
        while (j <= tam) and not nao_e do
        begin
            soma:= 0;
            i:= 1;
            while (i <= tam) and not nao_e do
            begin
                soma:= soma + w[i,j];
                i:= i + 1;
            end;
            if soma <> soma_anterior then
                nao_e:= true;
            j:= j + 1;
        end;
    end;
    
    if nao_e = true then
        writeln('nao')
    else
        writeln('sim');
end;

begin
    read(n);
    ler_matriz(m,n);
    quadrado_perfeito(m,n);
end.
