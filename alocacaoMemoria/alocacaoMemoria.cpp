#include <windows.h>
#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <psapi.h>
#include <string>
#include <vector>
#include <ctime>
#include <random>

// Função de medição de memoria (biblioteca windows.h + psapi.h)
double getMemory()
{
    PROCESS_MEMORY_COUNTERS memInfo;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &memInfo, sizeof(memInfo)))
    {
        return static_cast<double>(memInfo.WorkingSetSize) / (1024.0 * 1024.0);
    }
    return 0.0;
}

// Função para medir tempo de execução de usuário e sistema (biblioteca windows.h + psapi.h)
void getTime(double &userTimeMs, double &systemTimeMs)
{
    FILETIME creationTime, exitTime, kernelTime, userTime;

    if (GetProcessTimes(GetCurrentProcess(), &creationTime, &exitTime, &kernelTime, &userTime))
    {
        ULARGE_INTEGER uTime, sTime;

        uTime.LowPart = userTime.dwLowDateTime;
        uTime.HighPart = userTime.dwHighDateTime;

        sTime.LowPart = kernelTime.dwLowDateTime;
        sTime.HighPart = kernelTime.dwHighDateTime;

        userTimeMs = static_cast<double>(uTime.QuadPart) / 10.0;
        systemTimeMs = static_cast<double>(sTime.QuadPart) / 10.0;
    }
    else
    {
        userTimeMs = systemTimeMs = 0.0;
    }
}

// Template para exibir os conteúdos dos vetores para serem colocados no código LaTeX
template <typename Container>
void exibeResultado(const Container &arr)
{
    for (size_t i = 0; i < arr.size(); i++)
    {
        std::cout << "(" << i << ", " << arr[i] << ") ";
    }
    std::cout << std::endl;
}

// Função gera nome (mudar para biblioteca random)
std::string geraNome()
{
    static std::mt19937 generator(static_cast<unsigned int>(std::time(nullptr)));
    const std::string CHARS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    static std::uniform_int_distribution<> charDist(0, CHARS.size()); // Range para pegar caracteres de CHARS

    std::string nome;
    int tamanho = 100 + charDist(generator) % 401; // Tamanho entre 100 e 500

    for (int i = 0; i < tamanho; i++)
    {
        nome.push_back(CHARS[charDist(generator)]);
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
    // Define a seed do rand para o horário atual do computador
    srand(static_cast<unsigned int>(time(0)));

    // Constantes a serem utilizadas
    const int START = 10000;
    const int PLUS = 5000;
    const int LIMIT = 1000000;

    // Alocação da Estrutura na memória
    Obj *obj = new Obj[LIMIT];

    // Vetores para guardarem as informações
    std::vector<uintptr_t> enderecosIniciais;
    std::vector<double> temposDeExecucao;
    std::vector<double> diferencasMemoria;
    std::vector<double> temposUsuario;
    std::vector<double> temposSistema;

    double memoriaInicial = getMemory(); // Mede memória inicial
    double userTimeInicio, systemTimeInicio;
    getTime(userTimeInicio, systemTimeInicio);

    for (int k = 0; k < START; k++)
    {
        obj[k].myName = geraNome();
        obj[k].num1 = rand() % 1000000;
        obj[k].num2 = rand() % 1000000;
    }

    double memoriaFinal = getMemory(); // Mede memória final a cada execução do código
    double userTimeFim, systemTimeFim;
    getTime(userTimeFim, systemTimeFim);

    enderecosIniciais.push_back(reinterpret_cast<uintptr_t>(&obj[0])); // Endereço inicial da iteração
    diferencasMemoria.push_back(memoriaFinal - memoriaInicial);
    temposUsuario.push_back(userTimeFim - userTimeInicio);
    temposSistema.push_back(systemTimeFim - systemTimeInicio);

    for (int i = START; i <= LIMIT; i += PLUS)
    {
        std::cout << "Alocando para " << i << " objetos." << std::endl;

        getTime(userTimeInicio, systemTimeInicio); // Mede o tempo atual antes da execução do código

        for (int k = i - PLUS; k < i; k++)
        {
            obj[k].myName = geraNome();
            obj[k].num1 = rand() % 1000000;
            obj[k].num2 = rand() % 1000000;
        }

        memoriaFinal = getMemory();              // Mede memória final a cada execução do código
        getTime(userTimeFim, systemTimeFim); // Mede o tempo atual para calcular tempo total

        // Parte do código que coloca as informações nos vetores
        enderecosIniciais.push_back(reinterpret_cast<uintptr_t>(&obj[i - PLUS])); // Endereço inicial da iteração
        diferencasMemoria.push_back(memoriaFinal - memoriaInicial);
        temposUsuario.push_back(userTimeFim - userTimeInicio);
        temposSistema.push_back(systemTimeFim - systemTimeInicio);
    }

    delete[] obj; // Libera a memória alocada

    // Exibe os resultados
    std::cout << "Endereços Iniciais: ";
    exibeResultado(enderecosIniciais);

    std::cout << "Diferença de Memória (MB): ";
    exibeResultado(diferencasMemoria);

    std::cout << "Tempo de Usuário (milissegundos): ";
    exibeResultado(temposUsuario);

    std::cout << "Tempo de Sistema (milissegundos): ";
    exibeResultado(temposSistema);

    return 0;
}
