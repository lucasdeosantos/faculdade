program matrizes_internas;

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

procedure sub_matrizes(var w: matriz; var tam: longint);
var i, j, cont, inicio_i, inicio_j, fim_i, fim_j: longint;
begin
    writeln;
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
                while i <= fim_i do
                begin
                    j:= inicio_j;
                    while j <= fim_j do
                    begin
                        if j = fim_j then
                            writeln(w[i,j], ' ')
                        else
                            write(w[i,j], ' ');
                        j:= j + 1;
                    end;
                    i:= i + 1;
                end;
                writeln;
                inicio_j:= inicio_j + 1;
                fim_j:= fim_j + 1;
            end;
            inicio_i:= inicio_i + 1;
            fim_i:= fim_i + 1;
            inicio_j:= 1;
            fim_j:= cont;
        end;
        writeln;
        cont:= cont + 1;
        inicio_i:= 1;
        fim_i:= cont;
        inicio_j:= 1;
        fim_j:= cont;
    end;
end;

begin
    read(n);
    ler_matriz(m,n);
    sub_matrizes(m,n);
end.