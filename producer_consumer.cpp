#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

std::queue<int> buffer;          // 共享队列
std::mutex mtx;                  // 互斥锁
std::condition_variable cv;      // 条件变量
const int MAX_BUFFER_SIZE = 10;  // 队列最大容量
bool finished = false;           // 标记生产是否结束

// 生产者
void producer(int count) {
    for (int i = 1; i <= count; ++i) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] {
            return buffer.size() < MAX_BUFFER_SIZE;
        });  // 等待队列有空间

        buffer.push(i);
        std::cout << "生产者生产: " << i << std::endl;

        cv.notify_all();  // 通知消费者
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    // 生产结束标记
    {
        std::lock_guard<std::mutex> lock(mtx);
        finished = true;
    }
    cv.notify_all();
}

// 消费者
void consumer(int id) {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] {
            return !buffer.empty() || finished;
        });  // 等待队列有数据或生产结束

        if (!buffer.empty()) {
            int item = buffer.front();
            buffer.pop();
            std::cout << "消费者 " << id << " 消费: " << item << std::endl;
            cv.notify_all();  // 通知生产者
        } else if (finished) {
            break;  // 没有数据且生产结束
        }
    }
}

int main() {
    std::thread prod(producer, 50);  // 生产50个数据
    std::thread cons1(consumer, 1);
    std::thread cons2(consumer, 2);
    std::thread cons3(consumer, 3);
    prod.join();
    cons1.join();
    cons2.join();
    cons3.join();

    std::cout << "所有任务完成！" << std::endl;
    return 0;
}
