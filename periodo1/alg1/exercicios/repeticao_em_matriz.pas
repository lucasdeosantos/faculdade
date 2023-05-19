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
var i, j, aux_i, aux_j: longint;
    achou: boolean;
begin
    aux_i:= 1;
    aux_j:= 2;
    i:= 1; 
    achou:= false ;
    while (i <= tam_l) and not achou do
    begin
        j:= 1;
        while (j <= tam_c) and not achou do
        begin
            if j <> tam_c then
            begin
                aux_i:= i;    
                aux_j:= j + 1;
            end
            else
            begin
                aux_i:= i + 1;
                aux_j:= 1;
            end;
            while (aux_i <= tam_l) and not achou do
            begin
                while (aux_j <= tam_c) and not achou do
                begin
                    if w[i,j] = w[aux_i,aux_j] then
                        achou:= true;
                    aux_j:= aux_j + 1;
                end;
                aux_i:= aux_i + 1;
                aux_j:= 1;
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