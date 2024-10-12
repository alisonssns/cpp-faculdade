#include <windows.h>
#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <psapi.h>
#include <string>
#include <chrono>
#include <ctime>
#include <random>  // Inclui a biblioteca random para a geração de números aleatórios

// Função de medição de memória (biblioteca windows.h + psapi.h)
double GetMemoryUsageInMB()
{
    PROCESS_MEMORY_COUNTERS memInfo;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &memInfo, sizeof(memInfo)))
    {
        return static_cast<double>(memInfo.WorkingSetSize) / (1024.0 * 1024.0);
    }
    return 0.0;
}

// Função para medir tempo de execução de usuário e sistema (biblioteca windows.h + psapi.h)
void GetUserAndSystemTime(double &userTimeSec, double &systemTimeSec)
{
    FILETIME creationTime, exitTime, kernelTime, userTime;

    if (GetProcessTimes(GetCurrentProcess(), &creationTime, &exitTime, &kernelTime, &userTime))
    {
        ULARGE_INTEGER uTime, sTime;

        uTime.LowPart = userTime.dwLowDateTime;
        uTime.HighPart = userTime.dwHighDateTime;

        sTime.LowPart = kernelTime.dwLowDateTime;
        sTime.HighPart = kernelTime.dwHighDateTime;

        userTimeSec = static_cast<double>(uTime.QuadPart) / 10000000.0;
        systemTimeSec = static_cast<double>(sTime.QuadPart) / 10000000.0;
    }
    else
    {
        userTimeSec = systemTimeSec = 0.0;
    }
}

// Função gera nome usando a biblioteca random
std::string geraNome()
{
    std::string nome;
    nome.reserve(500);  // Reserva espaço na string para evitar realocações
    int tamanho = 100 + rand() % 401;  // Gera um tamanho entre 100 e 500 caracteres

    // Geração de número aleatório usando std::random
    std::random_device rd;  // Fonte de entropia para números aleatórios
    std::mt19937 gen(rd()); // Gerador de números aleatórios (Mersenne Twister)
    std::uniform_int_distribution<> distrib(0, 25); // Distribuição uniforme entre 0 e 25 (para letras do alfabeto)

    for (int i = 0; i < tamanho; i++)
    {
        nome += ('A' + distrib(gen)); // Gera letras aleatórias entre 'A' e 'Z'
    }
    return nome;
}

// Estrutura do objeto
struct Obj
{
    std::string myName;
    int num1;
    int num2;
};

int main()
{
    // Define a "seed" do rand para o horário atual do computador
    srand(static_cast<unsigned int>(time(0)));

    double timeUserS, timeSystemS;
    GetUserAndSystemTime(timeUserS, timeSystemS); // Tempo inicial de execução

    // Constantes a serem utilizadas
    const int START = 10000;
    const int PLUS = 5000;
    const int LIMIT = 1000000;

    // Alocação da Estrutura na memória
    Obj *obj = new Obj[LIMIT];

    for (int i = START; i <= LIMIT; i += PLUS)
    {
        double userTimeInicio, systemTimeInicio;
        GetUserAndSystemTime(userTimeInicio, systemTimeInicio); // Tempo antes da execução do loop
        double memoriaInicial = GetMemoryUsageInMB(); // Mede memória inicial

        // Preenchimento dos objetos com dados aleatórios
        for (int k = 0; k < PLUS; k++)
        {
            obj->myName = geraNome(); // Gera nome aleatório
            obj->num1 = rand() % 1000000;  // Gera número aleatório entre 0 e 999999
            obj->num2 = rand() % 1000000;  // Gera número aleatório entre 0 e 999999
        }

        double memoriaFinal = GetMemoryUsageInMB(); // Mede memória final após a execução
        double userTimeFim, systemTimeFim;
        GetUserAndSystemTime(userTimeFim, systemTimeFim); // Mede tempo final após a execução

        // Exibe o tempo de sistema e de usuário para cada iteração
        std::cout << "Tempo de sistema para " << i << " itens: " << userTimeFim - userTimeInicio << std::endl;
        std::cout << "Tempo de usuário para " << i << " itens: " << systemTimeFim - systemTimeInicio << std::endl;
    }

    double timeUserF, timeSystemF;
    GetUserAndSystemTime(timeUserF, timeSystemF); // Tempo total após a execução de todos os loops

    // Exibe os tempos totais de usuário e de sistema
    std::cout << "Tempo total de usuário: " << timeUserF - timeUserS << std::endl;
    std::cout << "Tempo total de sistema: " << timeSystemF - timeSystemS << std::endl;

    // Libera a memória alocada para os objetos
    delete[] obj;

    return 0;
}
