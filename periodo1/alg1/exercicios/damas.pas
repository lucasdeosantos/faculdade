program damas;

const   MIN = 0;
        MAX = 200;
        
type matriz = array [MIN..MAX,MIN..MAX] of longint;

var m: matriz;
    
procedure ler_matriz(var w: matriz);
var i, j: longint;
begin
    for i:= 1 to 8 do
        for j:= 1 to 8 do
            read(w[i,j]);
end;

procedure tomar(var w: matriz);
var i, j, inicio, fim, cont_pecas, cont_nenhum: longint;

begin
    writeln('');
    cont_pecas:= 0;
    cont_nenhum:= 0;
    inicio:= 1;
    fim:= 8;
    i:= 1;
    while i <= 8 do
    begin
        j:= 1;
        while j <= 8 do
        begin
            if w[i,j] = -1 then
            begin
                cont_pecas:= cont_pecas + 1;
                if (j = inicio) and (w[i+1,j+1] = 1) then
                    write(i,' ',j,' ')
                else if (j = fim) and (w[i+1,j-1] = 1) then
                    write(i,' ',j,' ')
                else if (i >= inicio) and (j < fim) and (j > inicio) and ((w[i+1,j+1] = 1) or (w[i+1,j-1] = 1)) then
                    write(i,' ', j,' ')
                else
                    cont_nenhum:= cont_nenhum + 1;
            end;
            j:= j + 1;
        end;
        i:= i + 1;
    end;
    if cont_pecas = cont_nenhum then
        write('nenhuma');
end;

procedure mover(var w: matriz);
var i, j, inicio, fim, cont_pecas, cont_nenhum: longint;

begin
    writeln('');
    cont_pecas:= 0;
    cont_nenhum:= 0;
    inicio:= 1;
    fim:= 8;
    i:= 1;
    while i <= 8 do
    begin
        j:= 1;
        while j <= 8 do
        begin
            if w[i,j] = -1 then
            begin
                cont_pecas:= cont_pecas + 1;
                if (j = inicio) and (w[i+1,j+1] = 0) then
                    write(i,' ',j,' ')
                else if (j = fim) and (w[i+1,j-1] = 0) then
                    write(i,' ',j,' ')
                else if (i >= inicio) and (j < fim) and (j > inicio) and ((w[i+1,j+1] = 0) or (w[i+1,j-1] = 0)) then
                    write(i,' ', j,' ')
                else
                    cont_nenhum:= cont_nenhum + 1;
            end;
            j:= j + 1;
        end;
        i:= i + 1;
    end;
    if cont_pecas = cont_nenhum then
        write('nenhuma');
end;

procedure nao_mover(var w: matriz);
var i, j, inicio, fim, cont_pecas, cont_nenhum: longint;

begin
    writeln('');
    cont_pecas:= 0;
    cont_nenhum:= 0;
    inicio:= 1;
    fim:= 8;
    i:= 1;
    while i <= 8 do
    begin
        j:= 1;
        while j <= 8 do
        begin
            if w[i,j] = -1 then
            begin
                cont_pecas:= cont_pecas + 1;
                if (j = inicio) and (w[i+1,j+1] = -1) then
                    write(i,' ',j,' ')
                else if (j = fim) and (w[i+1,j-1] = -1) then
                    write(i,' ',j,' ')
                else if (i >= inicio) and (j < fim) and (j > inicio) and ((w[i+1,j+1] = -1) or (w[i+1,j-1] = -1)) then
                    write(i,' ', j,' ')
                else
                    cont_nenhum:= cont_nenhum + 1;
            end;
            j:= j + 1;
        end;
        i:= i + 1;
    end;
    if cont_pecas = cont_nenhum then
        write('nenhuma');
end;

begin
    ler_matriz(m);
    tomar(m);
    mover(m);
    nao_mover(m);
end.
