int sum(int v[], int n) {
    int sum = 0;

    for(int i = 0; i < n; ++i)
        sum += v[i];

    if (sum > 255)
        return 255;

    return sum;
}

int main() {
    int v[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int n = 10;

    return sum(v, n);
}
