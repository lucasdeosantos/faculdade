program basico_2;

const  MIN = 0;
       MAX = 200; 

type    matriz = array [MIN..MAX,MIN..MAX] of real;
        vetor = array [MIN..MAX] of real;

var n: longint;
    m: matriz;
    vetor_y,vetor_x: vetor;
    
procedure ler_matriz(var w: matriz; var tam: longint);
var i,j: longint;
begin
    for i:= 1 to tam do
        for j:= 1 to tam do
            read(w[i,j]);
end;

procedure ler_vetor(var v: vetor; var tam: longint);
var i: longint;
begin
    for i:= 1 to tam do
        read(v[i]);
end;

procedure resultado_equacao(var w: matriz; var vy,vx: vetor; var tam: longint);
var i, j, k, l: longint;
    soma: real;
    nao: boolean;
begin
    nao:= false;
    l:= 1;
    i:= 1;
    while (i <= tam) and not nao do
    begin
        j:= 1;
        k:= 1;
        soma:= 0;
        while j <= tam do
        begin
            soma:= soma + w[i,j]*vx[k];
            j:= j + 1;
            k:= k + 1;
        end;
        if soma <> vy[l] then
            nao:= true;
        l:= l + 1;
        i:= i + 1;
    end;
    if nao = true then
        writeln('nao')
    else
        writeln('sim');
end;

begin
    read(n);
    ler_matriz(m,n);
    ler_vetor(vetor_y,n);
    ler_vetor(vetor_x,n);
    resultado_equacao(m,vetor_y,vetor_x,n);
end.