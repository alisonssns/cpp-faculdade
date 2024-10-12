#include <iostream>
#include <vector>
#include <windows.h>
#include <psapi.h>

SIZE_T getMemoryUsage() {
    PROCESS_MEMORY_COUNTERS pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
    return pmc.WorkingSetSize;
}

void displayTotalMemory() {
    MEMORYSTATUSEX memoryStatus;
    memoryStatus.dwLength = sizeof(memoryStatus);
    GlobalMemoryStatusEx(&memoryStatus);

    std::cout << "Memória total do sistema: " 
              << memoryStatus.ullTotalPhys / (1024 * 1024) << " MB" << std::endl;
    std::cout << "Memória disponível: " 
              << memoryStatus.ullAvailPhys / (1024 * 1024) << " MB" << std::endl;
    std::cout << "Memória usada: " 
              << (memoryStatus.ullTotalPhys - memoryStatus.ullAvailPhys) / (1024 * 1024) 
              << " MB" << std::endl;
}

int main() {
    std::vector<int*> vectors;

    // Exibir a memória total do sistema
    displayTotalMemory();

    // Medir o uso de memória antes da criação dos vetores
    SIZE_T memoryBefore = getMemoryUsage();
    std::cout << "Uso de memória antes: " << memoryBefore / 1024 << " KB" << std::endl;

    // Criar vários vetores
    for (int i = 0; i < 10; ++i) {
        vectors.push_back(new int[1000]); // Cada vetor com 1000 inteiros
    }

    // Medir o uso de memória após a criação dos vetores
    SIZE_T memoryAfter = getMemoryUsage();
    std::cout << "Uso de memória após: " << memoryAfter / 1024 << " KB" << std::endl;

    // Liberar a memória alocada
    for (auto vec : vectors) {
        delete[] vec;
    }

    return 0;
}
