program inserir_linha;

const MIN = 0;
      MAX = 200;

type matriz = array [MIN..MAX,MIN..MAX] of longint;
     vetor = array [MIN..MAX] of longint;

var l, c, k: longint;
    m: matriz;
    v: vetor;

procedure ler_matriz(var w: matriz; var tam_l, tam_c: longint);
var i, j: longint;
begin
    for i:= 1 to tam_l do
        for j:= 1 to tam_c do
            read(w[i,j]);
end;

procedure ler_vetor(var r: vetor; var tam: longint);
var i: longint;
begin
    for i:= 1 to tam do
        read(r[i]);
end;

procedure insere_linha(var w: matriz; var r: vetor; var n, m, k: longint);
var i, j: longint;
begin
    for i:= n downto k do 
        for j:= 1 to m do 
            w[i+1,j]:= w[i,j];

    for j:= 1 to m do
        w[k,j]:= v[j]; 
    n:= n + 1; 
end;

procedure imprime_matriz(var w: matriz; var tam_l,tam_c: longint);
var i, j: longint;
begin
    writeln;
    for i:= 1 to tam_l do
    begin
        for j:= 1 to tam_c do
            write(w[i,j],' ');
        writeln;
    end;
end;

begin
    read(l,c);
    ler_matriz(m,l,c);
    read(k);
    ler_vetor(v,c);
    insere_linha(m,v,l,c,k);
    imprime_matriz(m,l,c);
end.