#include <iostream>
#include <chrono>
#include <vector>
#include <thread>
#include <fstream>

std::vector<std::vector<int>> A, B;

//Função para ler as matrizez
std::vector<std::vector<int>> ler_matriz(const std::string &nomeArq, int &linhas, int &colunas) {
    std::ifstream file(nomeArq);

    if (!file.is_open()) {
        std::cout << "Erro ao abrir " << nomeArq << std::endl;
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

//Função executada por cada thread
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

    //Salvar resultado dessa thread
    std::string nome = "resultado_thread_" + std::to_string(id) + ".txt";
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
        std::cout << "Uso: .\\threads.exe matriz1.txt matriz2.txt T" << std::endl;
        return 1;
    }
    int n1, m1, n2, m2;

    //Lê as matrizes antes de criar threads
    A = ler_matriz(argv[1], n1, m1);
    B = ler_matriz(argv[2], n2, m2);

    if (m1 != n2) {
        std::cout << "Erro: dimensoes incompatíveis!" << std::endl;
        return 1;
    }

    int T = std::stoi(argv[3]);

    std::vector<std::thread> threads;

    int linhas_por_thread = n1 / T;
    int resto = n1 % T;
    int inicio = 0;

    for (int i = 0; i < T; i++) {
        int fim = inicio + linhas_por_thread + (i < resto ? 1 : 0);

        threads.emplace_back(calcular_parte, i, inicio, fim, m1, m2);

        inicio = fim;
    }
    for (auto &t : threads) {
        t.join();
    }

    std::cout << "Execucao com threads concluida!" << std::endl;

    return 0;
}