program prova_3;

const   MIN = 0;
        MAX = 1024;
        
type matriz = array[MIN..MAX,MIN..MAX] of longint;

var n,e: longint;
    m: matriz;

procedure ler_matriz(var w: matriz;var tam: longint);
var i,j: longint;
begin
    for i:= 1 to tam do
        for j:= 1 to tam do
            read(w[i,j]);
end;

procedure max_pooling(var w: matriz;var tam: longint);
var i, j, ri, rj, maior, inicio, fim: longint;
    r: matriz;
begin
    ri:= 1; i:= 1;
    rj:= 1; j:= 1;
    inicio:= 1;
    maior:= w[i,j];
    fim:= tam div 2;
    while ri <= 2 do
    begin
        i:= inicio;
        while i <= (inicio + 1) do
        begin
            j:= fim - 1;
            while j <= fim do
            begin
                if w[i,j] > maior then
                    maior:= w[i,j];
                j:= j + 1;
            end;
        i:= i + 1;
        end;
        if (i = j) and (i < tam) then
        begin
            inicio:= 1;
            fim:= tam;
            r[ri,rj]:= maior;
            rj:= rj + 1;
            maior:= w[inicio,fim - 1];
        end
        else if (j > i) then
        begin
            inicio:= tam div 2 + 1;
            fim:= tam div 2;
            r[ri,rj]:= maior;
            ri:= ri + 1;
            rj:= 1;
            maior:= w[inicio,fim - 1];
        end
        else if (i > j) then 
        begin
            inicio:= tam div 2 + 1;
            fim:= tam;
            r[ri,rj]:= maior;
            rj:= rj + 1;
            maior:= w[inicio,fim - 1];
        end
        else
        begin
            r[ri,rj]:= maior;
            ri:= ri + 1;
        end;
    end;
    writeln;
    for ri:= 1 to tam_l do
    begin
        for rj:= 1 to tam_c do
            write(r[ri,rj],' ');
        writeln;
    end;
end;

procedure average_pooling(var w: matriz;var tam: longint);
var i, j, ri, rj, media, inicio, fim: longint;
    r: matriz;
begin
    ri:= 1;
    rj:= 1;
    inicio:= 1;
    fim:= tam div 2;
    media:= 0;
    while ri <= 2 do
    begin
        i:= inicio;
        while i <= (inicio + 1) do
        begin
            j:= fim - 1;
            while j <= fim do
            begin
                media:= media + w[i,j];
                j:= j + 1;
            end;
        i:= i + 1;
        end;
        if (i = j) and (i < tam) then
        begin
            inicio:= 1;
            fim:= tam;
            r[ri,rj]:= media div ((tam div 2)*(tam div 2));
            rj:= rj + 1;
            media:= 0;
        end
        else if (j > i) then
        begin
            inicio:= tam div 2 + 1;
            fim:= tam div 2;
            r[ri,rj]:= media div ((tam div 2)*(tam div 2));
            ri:= ri + 1;
            rj:= 1;
            media:= 0;
        end
        else if (i > j) then 
        begin
            inicio:= tam div 2 + 1;
            fim:= tam;
            r[ri,rj]:= media div ((tam div 2)*(tam div 2));
            rj:= rj + 1;
            media:= 0;
        end
        else
        begin
            r[ri,rj]:= media div ((tam div 2)*(tam div 2));
            ri:= ri + 1;
        end;
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
    read(n,e);
    ler_matriz(m,n);
    if e = 1 then
        max_pooling(m,n)
    else if e = 2 then
        average_pooling(m,n)
    else
        writeln('erro');
end.
