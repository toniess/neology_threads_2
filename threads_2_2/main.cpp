#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <vector>
#include <iomanip>

std::mutex g_display_mutex;

void setCursorPosition(int x, int y) {
    std::cout << "\033[" << y << ";" << x << "H";
    std::cout.flush();
}

void calculate(int threadNum, int calculationTime) {
    auto start = std::chrono::high_resolution_clock::now();

    std::unique_lock<std::mutex> lock(g_display_mutex);
    setCursorPosition(0, 2 + threadNum);
    std::cout << std::left
              << std::setw(17) << threadNum
              << std::setw(19) << std::this_thread::get_id()
              << std::setw(27) << "____________________"
              << std::setw(17) << "--" << std::endl;
    std::cout.flush();
    lock.unlock();

    int i = 0;
    while (i < 20) {
        lock.lock();
        setCursorPosition(37 + i, 2 + threadNum);
        std::cout << "█";
        std::cout.flush();
        lock.unlock();

        i++;
        std::this_thread::sleep_for(std::chrono::microseconds(calculationTime*10000));
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    lock.lock();
    setCursorPosition(64, 2 + threadNum);
    std::cout << elapsed.count();
    std::cout.flush();
    lock.unlock();
}

int main() {
    const int numThreads = 5; // Здесь указывается количество потоков
    const std::vector<int> calculationTime = {10, 15, 6, 5, 13}; // Здесь указывается время расчета

    std::thread threads[numThreads];

    std::cout << std::left
              << std::setw(25) << "№ потока"
              << std::setw(25) << "id потока"
              << std::setw(35) << "Прогресс"
              << std::setw(25) << "Время" << std::endl;


    for (int i = 0; i < numThreads; ++i) {
        threads[i] = std::thread(calculate, i, calculationTime[i]);
    }

    for (int i = 0; i < numThreads; ++i) {
        threads[i].join();
    }
    setCursorPosition(0, 2 + numThreads);

    return 0;
}
