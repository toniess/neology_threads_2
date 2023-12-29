#include <iostream>
#include <thread>
#include <mutex>

class Data {
public:
    Data(int val) : data(val) {}

    int getValue() const {
        return data;
    }

    void setValue(int val) {
        data = val;
    }

private:
    int data;
    std::mutex mtx;
    friend void swap(Data& a, Data& b);
    friend void swap_scoped(Data& a, Data& b);
    friend void swap_unique(Data& a, Data& b);
};



void swap(Data& a, Data& b) {
    std::lock_guard<std::mutex> lock_a(a.mtx);
    std::lock_guard<std::mutex> lock_b(b.mtx);

    int tmp = a.data;
    a.data = b.data;
    b.data = tmp;
}


void swap_scoped(Data& a, Data& b) {
    std::scoped_lock(a.mtx, b.mtx);

    int tmp = a.data;
    a.data = b.data;
    b.data = tmp;
}


void swap_unique(Data& a, Data& b) {
    std::unique_lock<std::mutex> lock_a(a.mtx, std::defer_lock);
    std::unique_lock<std::mutex> lock_b(b.mtx, std::defer_lock);
    std::lock(lock_a, lock_b);

    int tmp = a.data;
    a.data = b.data;
    b.data = tmp;
}


int main() {
    Data data1(5);
    Data data2(10);

    std::thread t1(swap, std::ref(data1), std::ref(data2));
    t1.join();

    std::thread t2(swap_scoped, std::ref(data1), std::ref(data2));
    t2.join();

    std::thread t3(swap_unique, std::ref(data1), std::ref(data2));
    t3.join();

    std::cout << "Data1: " << data1.getValue() << std::endl;
    std::cout << "Data2: " << data2.getValue() << std::endl;

    return 0;
}
