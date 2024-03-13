program repeticao_em_matriz;

const   MIN = 1;
        MAX = 100;

type matriz = array [MIN..MAX,MIN..MAX] of longint;

var l,c: longint;
    m: matriz;

procedure ler_matriz(var w: matriz;var tam_l, tam_c: longint);
var i,j: longint;
begin
    for i:= 1 to tam_l do
        for j:= 1 to tam_c do
            read(w[i,j]);
end;

function repetidos(var w: matriz; tam_l, tam_c: longint): boolean;
var i, j, p, q: longint;
    achou: boolean;
begin
    achou:= false;
    i:= 1;
    while (i <= tam_l) and not achou do
    begin
        j:= 1;
        while (j <= tam_c) and not achou do
        begin
            p:= 1;
            while (p <= tam_l) and not achou do
            begin
                q:= 1;
                while (q <= tam_c) and not achou do
                begin
                    if (w[p,q] = w[i,j]) and ((p <> i) or (q <> j)) then
                        achou:= true;
                    q:= q + 1;
                end;
                p:= p + 1;
            end;
            j:= j + 1;
        end;
        i:= i + 1;
    end;
    repetidos:= achou;
end;

begin
    read(l,c);
    ler_matriz(m,l,c);
    if repetidos(m,l,c) then
        writeln('sim')
    else
        writeln('nao');
end.