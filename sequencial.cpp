#include <iostream>
#include <ctime>
#include <vector>
#include <fstream>
#include <chrono>
#include <cstdlib>



//Função para ler a matrizes
std::vector<std::vector<int>> ler_matriz(const std::string &nomeArq, int &linhas, int &colunas) {
    std::ifstream file(nomeArq);
    if (!file.is_open()) {
        std::cout << "Erro ao abrir o arquivo " << nomeArq << std::endl;
        exit(1);
    }

    //Lê as dimensões da matriz
    file >> linhas >> colunas;

    std::vector<std::vector<int>> matriz(linhas, std::vector<int>(colunas));

    //Lê os valores
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            file >> matriz[i][j];
        }
    }

    file.close();
    return matriz;
}

// Função para multiplicar matrizes
std::vector<std::vector<int>> multiplicar(
    const std::vector<std::vector<int>> &A,
    const std::vector<std::vector<int>> &B,
    int n1, int m1, int m2
) {
    std::vector<std::vector<int>> C(n1, std::vector<int>(m2, 0));

    for (int i = 0; i < n1; i++) {
        for (int j = 0; j < m2; j++) {
            for (int k = 0; k < m1; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    return C;
}

//Função para salvar resultado
void salvar_resultado(const std::string &nomeArquivo,
                      const std::vector<std::vector<int>> &C,
                      int linhas, int colunas,
                      double tempo) {

    std::ofstream file(nomeArquivo);

    if (!file.is_open()) {
        std::cout << "Erro ao salvar resultado!" << std::endl;  //Verificação
        exit(1);
    }
    file << linhas << " " << colunas << std::endl;

    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            file << C[i][j] << " ";
        }
        file << std::endl;
    }

    file << "Tempo: " << tempo << " segundos" << std::endl;

    file.close();
}
int main(int argc, char *argv[]) {

    if (argc != 3) {
        std::cout << "Uso: .\\sequencial.exe matriz1.txt matriz2.txt" << std::endl;
        return 1;
    }

    int n1, m1, n2, m2;

    //Lê as matrizes dos arquivos
    std::vector<std::vector<int>> A = ler_matriz(argv[1], n1, m1);
    std::vector<std::vector<int>> B = ler_matriz(argv[2], n2, m2);

    //Validação da multiplicação
    if (m1 != n2) {
        std::cout << "Erro: dimensoes incompatíveis!" << std::endl;
        return 1;
    }

    //Medir o tempo de execução
    auto inicio = std::chrono::high_resolution_clock::now();
    std::vector<std::vector<int>> C = multiplicar(A, B, n1, m1, m2);

    auto fim = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duracao = fim - inicio;

    //Salva o resultado
    salvar_resultado("resultado.txt", C, n1, m2, duracao.count());

    std::cout << "Multiplicacao concluida. Resultado salvo em resultado.txt" << std::endl;

    return 0;
}