#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

// 每个线程循环次数
const int ITER = 100000000;
const int THREADS = 4;

// 情况一：未避免伪共享
struct DataNoPadding {
    std::atomic<int> value;
};

// 情况二：避免伪共享（填充到64字节）
struct alignas(64) DataWithPadding {
    std::atomic<int> value;
    char padding[64 - sizeof(std::atomic<int>)];  // 填充到一整行
};

void run_no_padding(DataNoPadding* data, int id) {
    for (int i = 0; i < ITER; ++i) {
        data[id].value.fetch_add(1, std::memory_order_relaxed);
    }
}

void run_with_padding(DataWithPadding* data, int id) {
    for (int i = 0; i < ITER; ++i) {
        data[id].value.fetch_add(1, std::memory_order_relaxed);
    }
}

int main() {
    // 未避免伪共享
    {
        std::vector<DataNoPadding> arr(THREADS);
        auto start = std::chrono::high_resolution_clock::now();

        std::vector<std::thread> threads;
        for (int i = 0; i < THREADS; ++i) {
            threads.emplace_back(run_no_padding, arr.data(), i);
        }
        for (auto& t : threads) t.join();

        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "未避免伪共享耗时: "
                  << std::chrono::duration<double>(end - start).count()
                  << " 秒\n";
    }

    // 避免伪共享
    {
        std::vector<DataWithPadding> arr(THREADS);
        auto start = std::chrono::high_resolution_clock::now();

        std::vector<std::thread> threads;
        for (int i = 0; i < THREADS; ++i) {
            threads.emplace_back(run_with_padding, arr.data(), i);
        }
        for (auto& t : threads) t.join();

        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "避免伪共享耗时: "
                  << std::chrono::duration<double>(end - start).count()
                  << " 秒\n";
    }

    return 0;
}
