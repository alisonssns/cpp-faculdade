#include <windows.h>
#include <iostream>
#include <cstdint>
#include <psapi.h>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>
#include <random>

// Função de medição de memória (usando windows.h + psapi.h)
double GetMemoryUsageInMB()
{
    PROCESS_MEMORY_COUNTERS memInfo;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &memInfo, sizeof(memInfo)))
    {
        // Converte de bytes para megabytes
        return static_cast<double>(memInfo.WorkingSetSize) / (1024.0 * 1024.0);
    }
    return 0.0;
}

// Função para medir tempo de execução de usuário e sistema (usando windows.h + psapi.h)
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

// Template para exibir os conteúdos dos vetores, útil para incluir em relatórios LaTeX
template <typename Container>
void exibeResultado(const Container &arr)
{
    for (size_t i = 0; i < arr.size(); i++)
    {
        std::cout << "(" << i << ", " << arr[i] << ") ";
    }
    std::cout << std::endl;
}

const std::string CHARS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

// Criação do gerador de números aleatórios (biblioteca random)
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> distChar(0, CHARS.size() - 1);
std::uniform_int_distribution<> distTamanho(100, 500); // Gera nome com tamanho entre 100 e 500
std::uniform_int_distribution<> distNum(0, 1000000);

// Função para gerar um nome aleatório (alterada para usar a biblioteca random)
std::string geraNome()
{
    // Caracteres possíveis no nome gerado
    std::string nome;
    nome.reserve(500); // Reserva espaço para até 500 caracteres
    int tamanho = distTamanho(gen);

    for (int i = 0; i < tamanho; i++)
    {
        nome.push_back(CHARS[distChar(gen)]); // Escolhe um caractere aleatório de CHARS
    }

    return nome;
}

// Estrutura representando o objeto
struct Obj
{
    std::string myName; // Nome gerado
    int num1;           // Número 1
    int num2;           // Número 2
};

int main()
{
    // Constantes para controle de iteração
    const int START = 10000;
    const int PLUS = 5000;
    const int LIMIT = 1000000;

    // Alocação de memória para a estrutura
    Obj *obj = new Obj[LIMIT];

    // Vetores para armazenar os dados
    std::vector<uintptr_t> enderecosIniciais;
    std::vector<double> temposDeExecucao;
    std::vector<double> diferencasMemoria;
    std::vector<double> temposUsuario;
    std::vector<double> temposSistema;
    double memoriaInicial = GetMemoryUsageInMB();

    for (int i = START; i <= LIMIT; i += PLUS)
    {
        std::cout << "Alocando para " << i << " objetos." << std::endl;

        // Mede memória inicial

        // Mede tempo inicial de usuário e sistema
        double userTimeInicio, systemTimeInicio;
        GetUserAndSystemTime(userTimeInicio, systemTimeInicio);

        // Gera dados aleatórios para os objetos
        for (int k = 0; k < PLUS; k++)
        {
            obj->myName = geraNome();
            obj->num1 = distNum(gen); // Usando random para num1
            obj->num2 = distNum(gen); // Usando random para num2
        }

        // Mede memória final e tempo final de execução
        double memoriaFinal = GetMemoryUsageInMB();
        double userTimeFim, systemTimeFim;
        GetUserAndSystemTime(userTimeFim, systemTimeFim);

        // Armazena os resultados nos vetores
        enderecosIniciais.push_back(reinterpret_cast<uintptr_t>(obj));
        diferencasMemoria.push_back(memoriaFinal - memoriaInicial);
        temposUsuario.push_back(userTimeFim - userTimeInicio);
        temposSistema.push_back(systemTimeFim - systemTimeInicio);
    }

    // Libera a memória alocada
    delete[] obj;

    // Exibe os resultados armazenados nos vetores
    std::cout << "Endereços Iniciais: ";
    exibeResultado(enderecosIniciais);

    std::cout << "Diferença de Memória (MB): ";
    exibeResultado(diferencasMemoria);

    std::cout << "Tempo de Usuário (segundos): ";
    exibeResultado(temposUsuario);

    std::cout << "Tempo de Sistema (segundos): ";
    exibeResultado(temposSistema);

    return 0;
}
