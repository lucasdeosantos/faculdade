int x = 11, y = 33, soma = 0;

int main () {
    if (x >= y)
        return 0;

    for (int i = x; i <= y; ++i)
        soma += i;

    if (soma < 255)
        return soma;
    
    return 255;
}
