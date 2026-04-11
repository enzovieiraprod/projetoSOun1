#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <chrono>

std::vector<std::vector<int>> A, B;

//Função para ler as matrizes
std::vector<std::vector<int>> ler_matriz(const std::string &nomeArquivo, int &linhas, int &colunas) {
    std::ifstream file(nomeArquivo);

    if (!file.is_open()) {
        std::cout << "Erro ao abrir " << nomeArquivo << std::endl;
        exit(1);
    }

    file >> linhas >> colunas;

    std::vector<std::vector<int>> matriz(linhas, std::vector<int>(colunas));

    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            file >> matriz[i][j];
        }
    }

    file.close();
    return matriz;
}

//Função que cada processo executa
void calcular_parte(int id, int inicio, int fim, int m1, int m2) {

    auto t_inicio = std::chrono::high_resolution_clock::now();

    std::vector<std::vector<int>> C_parcial(fim - inicio, std::vector<int>(m2, 0));

    for (int i = inicio; i < fim; i++) {
        for (int j = 0; j < m2; j++) {
            for (int k = 0; k < m1; k++) {
                C_parcial[i - inicio][j] += A[i][k] * B[k][j];
            }
        }
    }

    auto t_fim = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracao = t_fim - t_inicio;

    std::string nome = "resultado_processo_" + std::to_string(id) + ".txt";
    std::ofstream file(nome);

    file << (fim - inicio) << " " << m2 << std::endl;

    for (int i = 0; i < (fim - inicio); i++) {
        for (int j = 0; j < m2; j++) {
            file << C_parcial[i][j] << " ";
        }
        file << std::endl;
    }

    file << "Tempo: " << duracao.count() << " segundos" << std::endl;

    file.close();
}

int main(int argc, char *argv[]) {

    if (argc != 4) {
        std::cout << "Uso: ./processos matriz1.txt matriz2.txt P" << std::endl;
        return 1;
    }

    int n1, m1, n2, m2;

    // Lê antes de criar processos
    A = ler_matriz(argv[1], n1, m1);
    B = ler_matriz(argv[2], n2, m2);

    if (m1 != n2) {
        std::cout << "Erro: dimensoes incompatíveis!" << std::endl;
        return 1;
    }

    int P = std::stoi(argv[3]);

    int linhas_por_proc = n1 / P;
    int resto = n1 % P;

    int inicio = 0;

    for (int i = 0; i < P; i++) {

        int fim = inicio + linhas_por_proc + (i < resto ? 1 : 0);

        pid_t pid = fork();

        if (pid == 0) {
            // Processo filho
            calcular_parte(i, inicio, fim, m1, m2);
            exit(0);
        }

        inicio = fim;
    }

    // Processo pai espera todos os filhos
    for (int i = 0; i < P; i++) {
        wait(NULL);
    }

    std::cout << "Execucao com processos concluida!" << std::endl;

    return 0;
}