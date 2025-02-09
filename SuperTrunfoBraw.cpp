#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

// Informações usadas no jogo
struct cartas {
    string nome;
    string ATK;
    string HP;
};

void limpartela() {
    system("clear");
}

// Garante que o vetor tera o tamanho correto
int tamanhoarquivo(const string &arquivo) {
    ifstream entrada(arquivo, ios::binary);
    if (!entrada.is_open()) {
        cout << "Erro ao abrir o arquivo binário." << endl;
        return 0;
    }

    int cont = 0;
    int cont2 = 1;
    char leitura;
    while (entrada.read(&leitura, sizeof(char))) {
        if (leitura == 0) {
            cont2++;
            if (cont2 == 4) { // Le os quatro bits de espaço e acrescenta uma linha
                cont++;
                cont2 = 1;
            }
        }
    }

    entrada.close();
    return cont / 3; // Estou trabalhando com trios de informações
}

// Abre o arquivo binario 
int carregarcartas(cartas* vet, int tam, const string& arquivo) {
    ifstream entrada(arquivo, ios::binary);
    if (!entrada.is_open()) {
        cout << "Erro ao abrir o arquivo binário." << endl;
        return 0;
    }

    for (int cont = 0; cont < tam; cont++) {
        unsigned int tamanho_nome, tamanho_ATK, tamanho_HP;
        string nome, ATK, HP;

        // Esses blocos de ifs garante que os arquivos sejam pegos no tamanho correto 
        if (!entrada.read(reinterpret_cast<char*>(&tamanho_nome), sizeof(unsigned int))) break;
        nome.resize(tamanho_nome);
        if (!entrada.read(&nome[0], tamanho_nome)) break;

        if (!entrada.read(reinterpret_cast<char*>(&tamanho_HP), sizeof(unsigned int))) break;
        HP.resize(tamanho_HP);
        if (!entrada.read(&HP[0], tamanho_HP)) break;

        if (!entrada.read(reinterpret_cast<char*>(&tamanho_ATK), sizeof(unsigned int))) break;
        ATK.resize(tamanho_ATK);
        if (!entrada.read(&ATK[0], tamanho_ATK)) break;

        // Passa os dados obtidos para o vetor do jogo
        vet[cont].nome = nome;
        vet[cont].HP = HP;
        vet[cont].ATK = ATK;
    }

    entrada.close();
    return 1;
}

void distribuircartas(cartas* baralho, cartas deckJogador1[], cartas deckJogador2[], int totalcartas) {
    int usadas[totalcartas];

    // Preenche o vetor com a quantidade de cartas disponiveis
    for (int cont = 0; cont < totalcartas; cont++) {
        usadas[cont] = cont;
    }

    srand(time(NULL)); // Inicializa a somente para números aleatórios
    for (int cont = totalcartas - 1; cont > 0; cont--) {
        // Escolhe um índice aleatório entre 0 e cont(Maximo de cartas disponiveis)
        int valorAleatorio = rand() % (cont + 1);

        // Troca os valores
        int temp = usadas[cont];
        usadas[cont] = usadas[valorAleatorio];
        usadas[valorAleatorio] = temp;
    }

    // Distribui as cartas embaralhadas para os jogadores
    for (int cont = 0; cont < 4; cont++) {
        
        deckJogador1[cont] = baralho[usadas[cont]];

        deckJogador2[cont] = baralho[usadas[cont + 4]];
    }
}


void jogarrodada(cartas& carta1, cartas& carta2, string nome1, string nome2, int& pontos1, int& pontos2) {
    int atributo1, atributo2, vez = 1;

    // Mantem o jogo alternando entre os jogadores
    while (vez != 3) {
        if (vez == 1) {
            cout << endl << "Jogador 1:" << nome1 << endl;
            cout << "Carta: " << carta1.nome << endl;
            cout << "1) HP: " << carta1.HP << endl;
            cout << "2) ATK: " << carta1.ATK << endl;
            cout << "Escolha o atributo (1 para HP ou 2 para ATK): ";
            cin >> atributo1;
            vez = 2;
            limpartela();
        } else {
            cout << endl << "Jogador 2:" << nome2 << endl;
            cout << "Carta: " << carta2.nome << endl;
            cout << "1) HP: " << carta2.HP << endl;
            cout << "2) ATK: " << carta2.ATK << endl;
            cout << "Escolha o atributo (1 para HP ou 2 para ATK): ";
            cin >> atributo2;
            vez = 3; // Termina uma rodada
            limpartela();
        }
    }

    // Converte HP e ATK para inteiros
    int hp1 = stoi(carta1.HP);
    int hp2 = stoi(carta2.HP);
    int atk1 = stoi(carta1.ATK);
    int atk2 = stoi(carta2.ATK);

    // Sequencia de if baseado nas escolhas dos jogadores
    if (atributo1 == 1 && atributo2 == 1) {
        cout << "Jogador 1(" << nome1 << "): " << hp1 << " HP" << " vs " << "Jogador 2(" << nome2 << "): " << hp2 << " HP" << endl; // Ambos escolheram HP

        if (hp1 > hp2) {
            cout << nome1 << " venceu esta rodada" << endl;
            pontos1++;
        } else if (hp2 > hp1) {
            cout << nome2 << " venceu esta rodada" << endl;
            pontos2++;
        } else {
            cout << "Empate nesta rodada" << endl;
        }
    } else if (atributo1 == 2 && atributo2 == 2) {
        cout << "Jogador 1(" << nome1 << "): " << atk1 << " ATK" << " vs " << "Jogador 2(" << nome2 << "): " << atk2 << " ATK" << endl; // Ambos escolheram ATK

        if (atk1 > atk2) {
            cout << nome1 << " venceu esta rodada" << endl;
            pontos1++;
        } else if (atk2 > atk1) {
            cout << nome2 << " venceu esta rodada" << endl;
            pontos2++;
        } else {
            cout << "Empate nesta rodada" << endl;
        }
    } else if (atributo1 == 1 && atributo2 == 2) {
        cout << "Jogador 1(" << nome1 << "): " << hp1 << " HP" << " vs " << "Jogador 2(" << nome2 << "): " << atk2 << " ATK" << endl; // Jogador 1 escolheu HP e o jogador 2 escolheu ATK

        if (hp1 > atk2) {
            cout << nome1 << " venceu esta rodada" << endl;
            pontos1++;
        } else if (atk2 > hp1) {
            cout << nome2 << " venceu esta rodada" << endl;
            pontos2++;
        } else {
            cout << "Empate nesta rodada" << endl;
        }
    } else if (atributo1 == 2 && atributo2 == 1) {
        cout << "Jogador 1(" << nome1 << "): " << atk1 << " ATK" << " vs " << "Jogador 2(" << nome2 << "): " << hp2 << " HP" << endl; // Jogador 1 escolheu ATK e o jogador 2 escolheu HP

        if (atk1 > hp2) {
            cout << nome1 << " venceu esta rodada" << endl;
            pontos1++;
        } else if (hp2 > atk1) {
            cout << nome2 << " venceu esta rodada" << endl;
            pontos2++;
        } else {
            cout << "Empate nesta rodada" << endl;
        }
    }

    cout << "\nPlacar: " << nome1 << " " << pontos1 << " x " << pontos2 << " " << nome2 << endl;
    cin.ignore();
    cin.get();
}

void jogo(cartas* baralho, int totalcartass) {
    // Recebe os nomes dos jogadores
    string nome1, nome2;
    cout << "Jogador 1: ";
    cin >> nome1;
    cout << "Jogador 2: ";
    cin >> nome2;

    cout << "As cartas serão distribuidas!" << endl << "-----------------" << endl;

    
    cartas deckJogador1[4], deckJogador2[4];
    distribuircartas(baralho, deckJogador1, deckJogador2, totalcartass);

    cout << "Cartas distribuidas, o jogo ira iniciar" << endl << "-----------------" << endl;
    int pontos1 = 0, pontos2 = 0, rodada = 0;

    while (rodada < 4) {
        cout << "Rodada: " << rodada + 1 << endl;
        jogarrodada(deckJogador1[rodada], deckJogador2[rodada], nome1, nome2, pontos1, pontos2);
        rodada++;
    }

    limpartela();
    cout << "Final: " << nome1 << " " << pontos1 << " x " << pontos2 << " " << nome2 << endl;

    if (pontos1 > pontos2)
        cout << nome1 << " venceu o jogo" << endl;
    else if (pontos2 > pontos1)
        cout << nome2 << " venceu o jogo" << endl;
    else
        cout << "O jogo terminou empatado" << endl;
}

int main() {
    string arquivobaralho = "/home/hiku/Trabalho3_Super_Trunfo/cartas.bin";
    int totalcartas = tamanhoarquivo(arquivobaralho);

    if (totalcartas == 0) {
        cerr << "Não foi possível carregar as cartas. Verifique o arquivo." << endl;
        return 1;
    }

    cartas baralho[totalcartas];
    carregarcartas(baralho, totalcartas, arquivobaralho);

    char opcao = 'Z';
    while (opcao != 'S') {
        cout << endl << "Super Trunfo Braw" << endl << "--------------------" << endl;
        cout << "Iniciar nova partida? [S/N] ";
        cin >> opcao;

        if (opcao == 'S' || opcao == 's') {
            jogo(baralho, totalcartas);
        }
    }

    return 0;
}