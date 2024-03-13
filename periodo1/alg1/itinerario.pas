program quadrado_magico;

const  MIN = 0;
       MAX = 100; 

type    matriz = array [MIN..MAX,MIN..MAX] of longint;
        vetor = array [MIN..MAX] of longint;
        
var n,k: longint;
    m: matriz;
    v: vetor;
    
procedure ler_matriz(var w: matriz; var tam: longint);
var i,j: longint;
begin
    for i:= 1 to tam do
        for j:= 1 to tam do
            read(w[i,j]);
end;

procedure ler_vetor(var t: vetor; var tam: longint);
var i: longint;
begin
    for i:= 1 to tam do
        read(v[i]);
end;

procedure itinerario(var w: matriz; var t: vetor; var tam_w, tam_t: longint);
var i, custo: longint;
begin
    custo:= 0;
    i:= 1;
    while i < tam_t do
    begin
        custo:= custo + w[t[i],t[i+1]];
        i:= i + 1;
    end;
    writeln(custo);
end;

begin
    read(n);
    ler_matriz(m,n);
    read(k);
    ler_vetor(v,k);
    itinerario(m,v,n,k);
end.