#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Filme
{
public:
    string titulo;
    string horario;

    Filme() {} // Construtor padrão para inicializar um objeto Filme

    void novoFilme()
    {
        cout << "Insira o título do filme: ";
        getchar();
        getline(cin, titulo);
        cout << "Insira o horário de exibição do filme: ";
        cin >> horario;
    }
};

class Assento
{
public:
    int numero;
    char letra;
    string tipo;
    bool disponivel;

    Assento(int num, char l, string t, bool d = true) : numero(num), letra(l), tipo(t), disponivel(d) {}
};

class Sala
{
public:
    vector<vector<Assento>> assentos;
    Filme filme;
    int numero;
    int x, y;

    void criarSala()
    {
        cout << "Insira a quantidade de linhas de cadeiras da sala: ";
        cin >> x;
        cout << "Insira a quantidade de colunas (cadeiras por linha) da sala: ";
        cin >> y;

        // Criando assentos
        for (int i = 0; i < y; i++)
        {
            vector<Assento> linha;
            for (int j = 0; j < x; j++)
            {
                Assento newAssento(j + 1, 'A' + i, "Normal", true); // Criando cada assento
                linha.push_back(newAssento);
            }
            assentos.push_back(linha);
        }

        filme.novoFilme(); // Inserir informações sobre o filme
        cout << "Insira o número da sala: ";
        cin >> numero;
    }

    void mostrarSala()
    {
        cout << "-------------------------------------------------------------------------" << endl;
        cout << "Sala " << numero << " - Filme: " << filme.titulo << " - Horário: " << filme.horario << endl;
        for (int k = 1; k <= x; k++)
        {
            cout << "  s" << k << " ";
        }
        cout << endl;
        for (int i = 0; i < y; i++)
        {
            char letra = 'A' + i;
            cout << letra << " ";

            for (int j = 0; j < x; j++)
            {
                cout << "[ ] ";
            }
            cout << endl;
        }
        cout << "-------------------------------------------------------------------------" << endl;
    }
};

int main()
{
    Sala sala;
    sala.criarSala();
    sala.mostrarSala();

    return 0;
}