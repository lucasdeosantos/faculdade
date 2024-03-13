#include <bits/stdc++.h>
using namespace std;

int main() {
    cin.tie();
    ios_base::sync_with_stdio(0);

    int n, tipo, tipoAnterior, idade, anterior, atual, isFifo, isLifo, isPriority;
    isFifo = isLifo = isPriority = 1;
    tipoAnterior = 0;
    cin >> n;
    deque<int> fifo, lifo, priority;

    for(int i = 0; i < n; i++) {
        cin >> tipo >> idade;
        if (!tipo) {
            fifo.push_back(idade);
            lifo.push_back(idade);
            priority.push_back(idade);
        }
        else {
            if (fifo.empty() && lifo.empty() && priority.empty()) break;

            if (!tipoAnterior)
                anterior = idade;

            atual = idade;

            if (!fifo.empty()) {
                if (atual == fifo.front()) {
                    fifo.pop_front();
                }
                else {
                    fifo.clear();
                    isFifo = 0;
                }
            }

            if (!lifo.empty()) {
                if (atual == lifo.back()) {
                    lifo.pop_back();
                }
                else {
                    lifo.clear();
                    isLifo = 0;
                }
            }

            if (atual > anterior) {
                priority.clear();
                isPriority = 0;
            }

            anterior = atual;
        }
        tipoAnterior = tipo;
    }
    
    if (isFifo && !isLifo && !isPriority) {
        cout << "fifo" << "\n";
    }
    else if (!isFifo && isLifo && !isPriority) {
        cout << "lifo" << "\n";
    }
    else if (!isFifo && !isLifo && isPriority) {
        cout << "priority" << "\n";
    }
    else if (!isFifo && !isLifo && !isPriority) {
        cout << "unknown" << "\n";
    }
    else {
        cout << "ambiguous" << "\n";
    }
}
