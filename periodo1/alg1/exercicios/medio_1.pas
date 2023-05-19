program medio_1;

const  MIN = 0;
       MAX = 100; 

type matriz = array [MIN..MAX,MIN..MAX] of longint;

var l,c: longint;
    m: matriz;
    
procedure ler_matriz(var w: matriz; var tam_l, tam_c: longint);
var i,j: longint;
begin
    for i:= 1 to tam_l do
        for j:= 1 to tam_c do
            read(w[i,j]);
end;

procedure vizinhos(var w: matriz; var tam_l, tam_c: longint);
var i, j, inicio, fim: longint;
    r: matriz;
begin
    inicio:= 1;
    fim:= tam_c;
    i:= inicio;
    while i <= tam_l do
    begin
        j:= 1;
        while j <= tam_c do 
        begin
            if (i >= inicio + 1) and (j <= fim - 1) then
                r[i,j]:= w[i,j+1] + w[i,j-1] + w[i-1,j-1] + w[i-1,j] + w[i-1,j+1] + w[i+1,j-1] + w[i+1,j] + w[i+1,j+1]
            else if (i = inicio) and (j = inicio) then
                r[i,j]:= w[i,j+1] + w[i+1,j] + w[i+1,j+1]
            else if (i = inicio) and (j = fim) then
                r[i,j]:= w[i,j-1] + w[i+1,j] + w[i+1,j-1]
            else if (i = fim) and (j = inicio) then
                r[i,j]:= w[i-1,j] + w[i-1,j+1] + w[i,j+1]
            else if (i = fim) and (j = fim) then
                r[i,j]:= w[i,j-1] + w[i-1,j-1] + w[i-1,j]
            else if (i = inicio) and (j <= fim - 1) then
                r[i,j]:= w[i,j+1] + w[i,j-1] + w[i+1,j] + w[i+1,j-1] + w[i+1,j+1]
            else if (i = fim) and (j <= fim - 1) then
                r[i,j]:= w[i,j+1] + w[i,j-1] + w[i-1,j] + w[i-1,j-1] + w[i-1,j+1]
            else if (i >= inicio + 1) and (j = inicio) then
                r[i,j]:= w[i-1,j] + w[i+1,j] + w[i,j+1] + w[i-1,j+1] + w[i+1,j+1]
            else if (i >= inicio) and (j = fim) then 
                r[i,j]:= w[i-1,j] + w[i+1,j] + w[i,j-1] + w[i-1,j-1] + w[i+1,j-1];
                
            j:= j + 1;
        end;
        i:= i + 1;
    end;
    writeln;
    for ri:= 1 to tam_l do
    begin
        for rj:= 1 to tam_c do
            write(r[ri,rj],' ');
        writeln;
    end;
end;

begin
    read(l,c);
    ler_matriz(m,l,c);
    vizinhos(m,l,c);
end.
