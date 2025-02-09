#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// Informações dos arquivos que estou trabalhando
struct baralho {
    string nome;
    string ATK;
    string HP;
};

int tamanhoArquivoTexto(const string &arquivo) {
    ifstream entrada(arquivo);

    if (!entrada.is_open()) {
        cout << "Erro ao abrir o arquivo para salvar em texto." << endl;
        return 1;
    }

    int cont = 0;
    string leitura;
    while (getline(entrada, leitura)){ // Contabiliza linha por linha
        cont++;
    } 

    entrada.close(); 
    return cont /3; // Divide por tres por ser trios de informações 
}

int tamanhoArquivoBinario(const string &arquivo) {
    ifstream entrada(arquivo, ios::binary);
    if (!entrada.is_open()) {
        cout << "Erro ao abrir o arquivo binário." << endl;
        return 0;
    }

    int cont = 0;
    int cont2 = 1;
    char leitura;
    while (entrada.read(&leitura, sizeof(char))) { // Contabiliza caractere por caractere
        
        if (leitura == 0) {  
            cont2++;
            if(cont2 == 4){
                cont++;
                cont2 = 1;
            }
        }
    }

    entrada.close();   
    return cont / 3; 
}

// Função para aumentar o limite do vetor de cartas
baralho* aumentarLimite(baralho* vet, int &tam) {
    tam++;
    baralho* novovet = new baralho[tam];
   
    for (int cont = 0; cont < tam-1; cont++) { // Repassa as informações
        novovet[cont] = vet[cont];
    }
    
    delete[] vet; 
    return novovet;
}

int abrirBaralhoBinario(baralho vet[], int tam, const string& arquivo) {
    ifstream entrada(arquivo, ios::binary);
    if (!entrada.is_open()) {
        cout << "Erro ao abrir o arquivo binário." << endl;
        return 0;
    }
    int cont;
    unsigned int tamanho_nome, tamanho_ATK, tamanho_HP;
    string nome, ATK, HP;
    for (cont = 0; cont < tam; cont++) {

        // Converte o tipo int para char e garante o tamanho correto do dado
        if (!entrada.read(reinterpret_cast<char*>(&tamanho_nome), sizeof(unsigned int))) break; 
        nome.resize(tamanho_nome);
        if (!entrada.read(&nome[0], tamanho_nome)) break;

        if (!entrada.read(reinterpret_cast<char*>(&tamanho_HP), sizeof(unsigned int))) break;
        HP.resize(tamanho_HP);
        if (!entrada.read(&HP[0], tamanho_HP)) break;

        if (!entrada.read(reinterpret_cast<char*>(&tamanho_ATK), sizeof(unsigned int))) break;
        ATK.resize(tamanho_ATK);
        if (!entrada.read(&ATK[0], tamanho_ATK)) break;

        vet[cont].nome = nome;
        vet[cont].HP = HP;
        vet[cont].ATK = ATK;
    }

    entrada.close();
    return cont; 
}

void salvarBaralhoBinario(baralho vet[], int tam, const string& arquivo) {
    ofstream saida(arquivo, ios::binary);

    if (!saida.is_open()) {
        cout << "Erro ao abrir o arquivo para salvar em binário." << endl;
        return;
    }

    for (int cont = 0; cont < tam; cont++) {
        // Pega o tamanho correto das variaveis
        unsigned int tamanho_nome = vet[cont].nome.size();
        unsigned int tamanho_ATK = vet[cont].ATK.size();
        unsigned int tamanho_HP = vet[cont].HP.size();

        // Converte os tipos string para char e posteriomente em binario e salva no arquivo
        saida.write(reinterpret_cast<char*>(&tamanho_nome), sizeof(unsigned int));
        saida.write(vet[cont].nome.c_str(), tamanho_nome);

        saida.write(reinterpret_cast<char*>(&tamanho_HP), sizeof(unsigned int));
        saida.write(vet[cont].HP.c_str(), tamanho_HP);

        saida.write(reinterpret_cast<char*>(&tamanho_ATK), sizeof(unsigned int));
        saida.write(vet[cont].ATK.c_str(), tamanho_ATK);
    }

    saida.close();
    cout << "Baralho salvo no arquivo binário com sucesso." << endl;
}

int abrirBaralhoTexto(baralho vet[], int tam, const string& arquivo) {
    ifstream entrada(arquivo);

    if (!entrada.is_open()) {
        cout << "Erro ao abrir o arquivo de texto." << endl;
        return 0;
    }

    int cont = 0;
    while (cont < tam && entrada >> vet[cont].nome >> vet[cont].HP >> vet[cont].ATK) {
        cont++;
    }

    entrada.close();
    return 1; 
}

void salvarBaralhoTexto(baralho vet[], int tam, const string& arquivo) {
    ofstream saida(arquivo);

    if (!saida.is_open()) {
        cout << "Erro ao abrir o arquivo para salvar em texto." << endl;
        return;
    }

    for (int cont = 0; cont < tam; cont++) {
        saida << vet[cont].nome << " " << vet[cont].HP << " " << vet[cont].ATK << endl;
    }

    saida.close();
    cout << "Baralho salvo no arquivo de texto com sucesso." << endl;
}

baralho* importarCartasTexto(baralho *vet, int &tam, const string& arquivo) {
    ifstream entrada(arquivo);

    if (!entrada.is_open()) {
        cout << "Erro ao abrir o arquivo de texto." << endl;
        return 0;
    }
   
    int cont = tamanhoArquivoTexto(arquivo);
    int saveQuanti = tam;
    
    for(int cont2 = 0; cont2 < cont; cont2++){
        
        vet = aumentarLimite(vet, tam);
    }   
    
    
    while (saveQuanti < tam && entrada >> vet[saveQuanti].nome >> vet[saveQuanti].HP >> vet[saveQuanti].ATK) {
        saveQuanti++;
    }
    entrada.close();
    
    return vet;
}

baralho* importarCartasBinario(baralho *vet, int &tam, const string& arquivo) {
    ifstream entrada(arquivo, ios::binary);
    if (!entrada.is_open()) {
        cout << "Erro ao abrir o arquivo binário." << endl;
        return 0;
    }
    
    int cont = tamanhoArquivoTexto(arquivo);
    int saveQuanti = tam;
    
    for(int cont2 = 0; cont2 < cont; cont2++){
        
        vet = aumentarLimite(vet, tam);
    }   


    for (; saveQuanti < tam; saveQuanti++) {
        unsigned int tamanho_nome, tamanho_ATK, tamanho_HP;
        string nome, ATK, HP;

        if (!entrada.read(reinterpret_cast<char*>(&tamanho_nome), sizeof(unsigned int))) break;
        nome.resize(tamanho_nome);
        if (!entrada.read(&nome[0], tamanho_nome)) break;

        if (!entrada.read(reinterpret_cast<char*>(&tamanho_HP), sizeof(unsigned int))) break;
        HP.resize(tamanho_HP);
        if (!entrada.read(&HP[0], tamanho_HP)) break;

        if (!entrada.read(reinterpret_cast<char*>(&tamanho_ATK), sizeof(unsigned int))) break;
        ATK.resize(tamanho_ATK);
        if (!entrada.read(&ATK[0], tamanho_ATK)) break;

        vet[cont].nome = nome;
        vet[cont].HP = HP;
        vet[cont].ATK = ATK;
    }

    entrada.close();
    return vet; 
}

int main() {
    int tam = 0;
    string arquivoTexto = "/home/hiku/Trabalho3_Super_Trunfo/cartas.txt";
    string arquivoBinario = "/home/hiku/Trabalho3_Super_Trunfo/cartas.bin";
    //string arquivoTexto =   "/home/hiku/Trabalho3_Super_Trunfo/novopack.txt";
    //string arquivoBinario = "/home/hiku/Trabalho3_Super_Trunfo/novopack.bin";

    // Caso queira verificar se os arquivos tem tamanho igual
    if(tamanhoArquivoTexto(arquivoTexto) == tamanhoArquivoBinario(arquivoBinario)){
        tam = tamanhoArquivoTexto(arquivoTexto);
    }else{
        cout << "Decks com tamanhos diferentes!" << endl;
        return 0;
    }

    baralho* vet = new baralho[tam];
    int cont, escolha, quantidade;
    char opc = 'z';

    string endereco = "/home/hiku/Trabalho3_2.0/cartas.txt";
    string nome, hp, atk;

    while (opc != 'S') {
        cout << endl << "Gerador de Cartas" << endl;
        cout << "-----------------" << endl;
        cout << "(C)adastrar" << endl << "(I)mportar" << endl;
        cout << "(A)lterar" << endl << "(E)xcluir" << endl;
        cout << "(L)istar" << endl << "(S)air" << endl;
        cout << "(T) Mexer com um arquivo de texto" << endl;
        cout << "(B) Mexer com um arquivo binário" << endl;
        cout << endl << "Escolha uma opcao: "; 
        cin >> opc;

        switch (opc) {
            case 'C': // Cadastrar
                vet = aumentarLimite(vet, tam);

                cout << "Cadastrar Carta" << endl << "---------------" << endl;
                cout << "Nome: "; cin >> vet[tam-1].nome;
                cout << "HP: "; cin >> vet[tam-1].HP;
                cout << "ATK: "; cin >> vet[tam-1].ATK;
                cout << endl << "Carta cadastrada com sucesso." << endl;
                break;

            case 'I':  // Importar
                cout << "Importar Cartas" << endl << "---------------" << endl;

                cout << "(B)inario ou (T)exto? "; char escolhaImport; cin >> escolhaImport;

                if (escolhaImport == 'B' || escolhaImport == 'b') {
                   cout << "Digite o endereço do arquio: "; cin >> endereco; cout << endl;
                    vet = importarCartasTexto(vet,tam,endereco);
                        cout << endl << "Novas cartas Inseridas" << endl;
                    
                } else if (escolhaImport == 'T' || escolhaImport == 't') {
                    cout << "Digite o endereço do arquio: "; cin >> endereco; cout << endl;
                    vet = importarCartasTexto(vet,tam,endereco);
                        cout << endl << "Novas cartas Inseridas" << endl;
                }
                break;

            case 'A': // Alterar
                cout << "Atualizar Cartas" << endl << "---------------" << endl;
                for (cont = 0; cont < tam; cont++) {
                    cout << cont + 1 << ") " << "Brawler: " << vet[cont].nome << endl;
                }
                cout << "Digite o numero da carta para alterar: ";
                cin >> escolha;
                escolha--;  

                cout << "Alterando Carta: " << vet[escolha].nome << endl;
                cout << "Novo Nome: "; cin >> nome;
                cout << "Novo HP: "; cin >> hp;
                cout << "Novo ATK: "; cin >> atk;

                vet[escolha].nome = nome;
                vet[escolha].HP = hp;
                vet[escolha].ATK = atk;

                cout << "Dados atualizados com sucesso." << endl;
                break;

            case 'E': // Excluir
                cout << "Excluir Carta" << endl << "---------------" << endl;
                for (cont = 0; cont < tam; cont++) {
                    cout << cont + 1 << ")" << "Brawler: " << vet[cont].nome << endl;
                }
                cout << "Digite o numero da carta: "; 
                cin >> escolha;
                cout << "Carta " << vet[escolha - 1].nome << " foi removida do baralho" << endl;

                tam--;
                for (int cont1 = escolha - 1; cont1 < tam; cont1++) {
                    vet[cont1] = vet[cont1 + 1];
                }
                break;

            case 'L': // Listar
                cout << "Cartas no Baralho" << endl << "---------------" << endl;
                for (cont = 0; cont < tam; cont++) {
                    cout << "Carta " << cont + 1 << ":" << endl;
                    cout << "Brawler: " << vet[cont].nome << endl;
                    cout << "HP: " << vet[cont].HP << endl;
                    cout << "ATK: " << vet[cont].ATK << endl << endl;
                }
                break;

            case 'S': // Sair
                cout << "Saindo..." << endl;
                break;

            case 'T': // Abrir e Salvar em Arquivo de Texto
                cout << "Abrir e Salvar em Arquivo de Texto" << endl << "---------------" << endl;
                cout << "(A)brir ou (S)alvar? "; char escolhaTexto; cin >> escolhaTexto;

                if (escolhaTexto == 'A' || escolhaTexto == 'a') {
                    cout << "Abrindo arquivo de texto" << endl;
                    quantidade = abrirBaralhoTexto(vet, tam, arquivoTexto);
                    cout << "Cartas carregadas: " << quantidade << endl;
                    tam = quantidade;
                } else if (escolhaTexto == 'S' || escolhaTexto == 's') {
                    cout << "Salvando arquivo de texto" << endl;
                    salvarBaralhoTexto(vet, tam, arquivoTexto);
                }
                break;

            case 'B': // Abrir e Salvar em Arquivo Binário
                cout << "Abrir e Salvar em Arquivo Binário" << endl << "---------------" << endl;
                cout << "(A)brir ou (S)alvar? "; char escolhaBin; cin >> escolhaBin;

                if (escolhaBin == 'A' || escolhaBin == 'a') {
                    cout << "Abrindo arquivo binário" << endl;
                    quantidade = abrirBaralhoBinario(vet, tam, arquivoBinario);
                    cout << "Cartas carregadas: " << quantidade << endl;
                    tam = quantidade;
                } else if (escolhaBin == 'S' || escolhaBin == 's') {
                    cout << "Salvando arquivo binário" << endl;
                    salvarBaralhoBinario(vet, tam, arquivoBinario);
                }
                break;
            default:
                cout << "Opção inválida." << endl;
        }
    }

    delete[] vet; 
    return 0;
}
