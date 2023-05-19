program basico_1;

const  MIN = 0;
       MAX = 200; 

type matriz = array [MIN..MAX,MIN..MAX] of longint;

var l,c: longint;
    m: matriz;
    
procedure ler_matriz(var w: matriz; var tam_l,tam_c: longint);
var i,j: longint;
begin
    for i:= 1 to tam_l do
        for j:= 1 to tam_c do
            read(w[i,j]);
end;

procedure nulas(var w: matriz; var tam_l,tam_c: longint);
var i, j, cont_l, cont_c: longint;
    nao: boolean; 
begin
    cont_l:= 0;
    i:= 1;
    while i <= tam_l do
    begin
        nao:= false;
        j:= 1;
        while (j <= tam_c) and not nao do
        begin
            if w[i,j] <> 0 then
                nao:= true;
            j:= j + 1;
        end;
        if nao = false then
            cont_l:= cont_l + 1;
        i:= i + 1;
    end;
    
    cont_c:= 0;
    j:= 1;
    while j <= tam_c do
    begin
        nao:= false;
        i:= 1;
        while (i <= tam_l) and not nao do
        begin
            if w[i,j] <> 0 then
                nao:= true;
            i:= i + 1;
        end;
        if nao <> true then
            cont_c:= cont_c + 1;
        j:= j + 1;
    end;
    writeln('linhas: ',cont_l);
    writeln('colunas: ',cont_c);
end;

begin
    read(l,c);
    ler_matriz(m,l,c);
    nulas(m,l,c);
end.
