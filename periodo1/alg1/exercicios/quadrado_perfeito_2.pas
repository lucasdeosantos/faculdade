program quadrado_magico_2;

const   MIN = 0;
        MAX = 200;
        
type matriz = array [MIN..MAX,MIN..MAX] of longint;

var n: longint;
    m: matriz;
    
procedure ler_matriz(var w: matriz; var tam: longint);
var i, j: longint;
begin
    for i:= 1 to tam do
        for j:= 1 to tam do
            read(w[i,j]);
end;

function quadrado_perfeito(var w: matriz; var tam: longint): boolean;
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
        quadrado_perfeito:= false
    else    
        quadrado_perfeito:= true;
        
end;

procedure sub_matrizes(var w: matriz; var tam: longint);
var i, j, ri, rj, cont, cont_quadrados_magicos, inicio_i, inicio_j, fim_i, fim_j: longint;
    r: matriz;
begin
    cont_quadrados_magicos:= 0;
    cont:= 2;
    inicio_i:= 1;
    inicio_j:= 1;
    fim_i:= cont;
    fim_j:= cont;

    while cont <= tam do
    begin
        while fim_i <= tam do
        begin
            while fim_j <= tam do
            begin
                i:= inicio_i;
                ri:= 1;
                while i <= fim_i do
                begin
                    rj:= 1;
                    j:= inicio_j;
                    while j <= fim_j do
                    begin
                        r[ri,rj]:= w[i,j];
                        j:= j + 1;
                        rj:= rj + 1;
                    end;
                    i:= i + 1;
                    ri:= ri + 1;
                end;
                if quadrado_perfeito(r,cont) = true then
                    cont_quadrados_magicos:= cont_quadrados_magicos + 1;
                    
                inicio_j:= inicio_j + 1;
                fim_j:= fim_j + 1;
            end;
            inicio_i:= inicio_i + 1;
            fim_i:= fim_i + 1;
            inicio_j:= 1;
            fim_j:= cont;
        end;
        cont:= cont + 1;
        inicio_i:= 1;
        fim_i:= cont;
        inicio_j:= 1;
        fim_j:= cont;
    end;
    
    writeln(cont_quadrados_magicos);
end;

begin
    read(n);
    ler_matriz(m,n);
    sub_matrizes(m,n);
end.