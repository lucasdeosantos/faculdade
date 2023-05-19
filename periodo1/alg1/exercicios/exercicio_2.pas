program exercicio_1;

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

procedure permutacao(var w: matriz; var tam: longint);
var i,j: longint;
    diferente: boolean;
begin
    diferente:= false;
    i:= 1;
    while (i <= tam) and not diferente do 
    begin
        j:= 1;
        while (j <= tam) and not diferente do 
        begin
            if (w[i,j] <> 0) and (w[i,j] <> 1) then
                diferente:= true;
            j:= j + 1;
        end;
        i:= i + 1;
    end;
    
    if diferente = true then
        writeln('nao')
    else
        writeln('sim');
end;

begin
    read(n);
    ler_matriz(m,n);
    permutacao(m,n);
end.