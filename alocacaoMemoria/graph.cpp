#include <iostream>
#include <chrono>
#include <string>
using namespace std;

int manualDivisao(int dividendo, int divisor) {
    int quociente = 0;
    while (dividendo >= divisor) {
        dividendo -= divisor;
        quociente++;
    }
    return quociente;
}

int main() {
    int dividendo = 4200000;
    int divisor = 7;
    int resultadosManuais[60];
    int resultadosBarra[60];

    for( int i=0;i<60;++i){
    // Realizando a divisão utilizando o operador da linguagem /
    auto start = std::chrono::high_resolution_clock::now();
    int resultado1 = dividendo / divisor;
    auto stop = std::chrono::high_resolution_clock::now();
    std::chrono::nanoseconds duracao1 = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);


    // Realizando a divisão utilizando algoritmo
    start = std::chrono::high_resolution_clock::now();
    int resultado2 = manualDivisao(dividendo, divisor);
    stop = std::chrono::high_resolution_clock::now();
    std::chrono::nanoseconds duracao2 = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);

    resultadosManuais[i] = duracao2.count();
    resultadosBarra[i] = duracao1.count(); 

    }
    for(int i = 0; i < 60; i++){
        cout<<"("<<i+1<<","<<resultadosBarra[i]<<") ";
    }

    cout<<endl;

    for(int i = 0; i < 60; i++){
        cout<<"("<<i+1<<","<<resultadosManuais[i]<<") ";
    }

    return 0;
}