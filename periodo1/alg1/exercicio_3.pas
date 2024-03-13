program exercicio_1;

const  MIN = 0;
       MAX = 200; 

type matriz = array [MIN..MAX,MIN..MAX] of double;

var la,ca,lb,cb: longint;
    ma,mb: matriz;
    
procedure ler_matriz(var w: matriz; var tam_l,tam_c: longint);
var i,j: longint;
begin
    for i:= 1 to tam_l do
        for j:= 1 to tam_c do
            read(w[i,j]);
end;

procedure inversa(var wa,wb: matriz; var tam_la, tam_ca, tam_lb, tam_cb: longint);
var i, j, k, tam_lr, tam_cr: longint;
    r: matriz;
    diferente: boolean;
begin
    if tam_ca <> tam_lb then
        writeln('Matriz não pode ser multiplicada')
    else
    begin
       tam_lr:= tam_la; tam_cr:= tam_cb;
        for i:= 1 to tam_la do
            for j:= 1 to tam_cb do
            begin
                r[i,j]:= 0;
                for k:= 1 to tam_lb do 
                    r[i,j]:= r[i,j] + wa[i,k] * wb[k,j];
            end;
        diferente:= false;
        i:= 1;
        while (i <= tam_lr) and not diferente do
        begin
            j:= 1;
            while (j <= tam_cr) and not diferente do
            begin
                if (i = j) and (r[i,j] <> 1) then
                    diferente:= true
                else if (i <> j) and (r[i,j] <> 0) then
                    diferente:= true;
                j:= j + 1;
            end;
            i:= i + 1;
        end;
        if diferente = true then
            writeln('Não')
        else    
            writeln('Sim');
    end;
end;

begin
    read(la,ca);
    ler_matriz(ma,la,ca);
    read(lb,cb);
    ler_matriz(mb,lb,cb);
    inversa(ma,mb,la,ca,lb,cb);
end.