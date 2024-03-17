int v[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
int par = 0;

int main () {
    for(int i = 0; i < 10; ++i)
        if (v[i] % 2 == 0)
            par++;

    return par;
}
