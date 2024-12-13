int binary_search(long long *P, int np, long long value) {
    int start = 0, end = np - 1, mid;

    while (start < end) {
        mid = (start + end) / 2;
        if (value < P[mid]) {
            end = mid;
        } 
        else {
            start = mid + 1;
        }
    }
    return start;
}
