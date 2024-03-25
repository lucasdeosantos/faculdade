int verifica_delta() {
    int a = 1;
    int b = 2;
    int c = -1;

    if ((b*b) - (4*a*c) > 0)
        return 1;

    return 0;
}

int main() {
    return verifica_delta();
}
