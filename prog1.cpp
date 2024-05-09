#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <mutex>
using namespace std;

const int n = 1000;
const int m = 100;
const int k = 1000;
const int num_threads = 4;
mutex mtx;

vector<vector<int>> A(n, vector<int>(m, 1)); // Заповнюємо матрицю A одиницями
vector<vector<int>> B(m, vector<int>(k, 1)); // Заповнюємо матрицю B одиницями
vector<vector<int>> C(n, vector<int>(k, 0)); // Результуюча матриця C

void matrix_multiply(int start_row, int end_row) {
    for (int i = start_row; i < end_row; ++i) {
        for (int j = 0; j < k; ++j) {
            for (int l = 0; l < m; ++l) {
                C[i][j] += A[i][l] * B[l][j];
            }
            // Виводимо результати "по ходу обчислень"
            //lock_guard<mutex> lock(mtx);
            //cout << "[" << i << "," << j << "]=" << C[i][j] << endl;
        }
    }
}

int main() {
    vector<thread> threads;
    auto start_time = chrono::high_resolution_clock::now();

    for (int t = 0; t < num_threads; ++t) {
        int start_row = (n * t) / num_threads;
        int end_row = (n * (t + 1)) / num_threads;
        threads.emplace_back(matrix_multiply, start_row, end_row);
    }

    for (auto& thread : threads) {
        thread.join();
    }

    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);
    cout << "Multiplication took " << duration.count() << " milliseconds." << endl;

    return 0;
}
