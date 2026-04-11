#include <iostream>
#include <cstdlib>
#include <fstream>
#include <ctime>


//Inicia a função para gerar e salvar a matriz em um arquivo
void gerar_e_salvar(const std::string &nomeFile, int linhas, int colunas) {     
    std::ofstream file(nomeFile);

    if (!file.is_open()) {
        std::cout << "Erro ao abrir o arquivo! " << nomeFile << std::endl; // Verificação
        std::exit(1);
    }

    //Escreve as dimensões da matriz no começo do arquivo
    file << linhas << " " << colunas << std::endl;

    //Gera os valores aleatórios da matriz e escreve no arquivo
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            int valor = std::rand() % 101; // Essa variável guarda valores aleatórios de 0 a 100
            file << valor << " ";
        }
        file << std::endl;
    }

    file.close();

    std::cout << "O Arquivo '" << nomeFile << "' (" 
              << linhas << "x" << colunas << ") foi gerado!" << std::endl; 
    //Só para mostrar se o arquivo foi gerado mesmo e as dimensões da matriz.
}

int main(int argc, char *argv[]) {

    if (argc != 5) {
        std::cout << "Voce deve usar: .\\auxiliar.exe <n1> <m1> <n2> <m2>" << std::endl; 
        //Validar os argumentos (os 5 argumentos separados)
        return 1;
    }

    //Converter para inteiro os argumentos passados na linha de comando
    int n1 = std::atoi(argv[1]);
    int m1 = std::atoi(argv[2]);
    int n2 = std::atoi(argv[3]);
    int m2 = std::atoi(argv[4]);

    //Validar a multiplicação das matrizes
    if (m1 != n2) {
        std::cout << "Erro: m1 deve ser igual a n2 (" << m1 << " != " << n2 << ")" << std::endl;
        return 1;
    }

    std::srand(std::time(NULL));  //Para não gerar os mesmos números toda vez que executarmos o código

    //Finalmente, a geração e salvamento das matrizes
    gerar_e_salvar("matriz1.txt", n1, m1);
    gerar_e_salvar("matriz2.txt", n2, m2);

    return 0;
}
