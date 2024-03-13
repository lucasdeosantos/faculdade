program exercicio_1;

const  MIN = 0;
       MAX = 200; 

type matriz = array [MIN..MAX,MIN..MAX] of longint;

var la,ca,lb,cb: longint;
    ma,mb: matriz;
    
procedure ler_matriz(var w: matriz; var tam_l,tam_c: longint);
var i,j: longint;
begin
    for i:= 1 to tam_l do
        for j:= 1 to tam_c do
            read(w[i,j]);
end;

procedure iguais(var wa,wb: matriz; var tam_la,tam_ca,tam_lb,tam_cb: longint);
var i,j: longint;
    diferente: boolean;
    
begin
    diferente:= false;
    if (tam_la = tam_cb) and (tam_ca = tam_lb) then
    begin
        i:= 1;
        while (i <= tam_la) and not diferente do
        begin
            j:= 1;
            while (j <= tam_ca) and not diferente do
            begin
                if wa[i,j] <> wb[j,i] then
                    diferente:= true;
                j:= j + 1;
            end;
        i:= i + 1;
        end;
        if diferente = true then
            writeln('nao')
        else
            writeln('sim');
    end
    else
    writeln('nao');
end;

begin
    read(la,ca);
    ler_matriz(ma,la,ca);
    read(lb,cb);
    ler_matriz(mb,lb,cb);
    iguais(ma,mb,la,ca,lb,cb);
end.