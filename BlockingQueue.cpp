#include <iostream>
#include <format>
#include <deque>
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>

/*
*  Implementatio of the Blocking Queue with Deque
*/
template <typename T>
class BlockingQueueWithDeque
{
public:
    BlockingQueueWithDeque() { m_maxSize = 10; }
    BlockingQueueWithDeque(int size) { m_maxSize = size; }

    void push(const T& data) {
        std::unique_lock<std::mutex> lock(m_mtxData);
        if (size() < m_maxSize) {
            m_Data.push_back(data);
            lock.unlock();
            m_Cv.notify_one();
        }
    }
    void pop(T& data) {
        std::unique_lock<std::mutex> lock(m_mtxData);
        //while (m_Data.empty()) {
        //    m_Cv.wait(lock);
        //}
        m_Cv.wait(lock, [&] {return !m_Data.empty(); });

        data = m_Data.front();
        m_Data.pop_front();
    }
    T& front() {
        std::unique_lock<std::mutex> lock(m_mtxData);
        m_Cv.wait(lock, [&] { return  !m_Data.empty(); });
        T& data = m_Data.front();
        return data;
    }
    T& back() {
        std::unique_lock<std::mutex> lock(m_mtxData);
        m_Cv.wait(lock, [&] { return  !m_Data.empty(); });
        T& data = m_Data.back();
        return data;
    }
    bool empty() { return m_Data.empty(); }
    size_t size() { return m_Data.size(); }

private:
    std::deque <T> m_Data;
    std::condition_variable m_Cv;
    std::mutex m_mtxData;
    int m_maxSize;
};

BlockingQueueWithDeque<int> testBlockingQueue(10);

void static producer(int qty) {
    for (int i = 0; i < qty; i++) {
        int item = i + 1;
        testBlockingQueue.push(item);
        std::cout << std::format("Master-Producer produced {}\n", item);
    }
}

void static consumer(int qty, int id) {
    int consumed_count = 0;
    for (int i = 0; i < qty; i++) {
        int item = 0; // get item from buffer
        testBlockingQueue.pop(item);
        consumed_count++;
        std::cout << std::format("Consumer {} conjumed item id {} - total consumed {}\n", id, item, consumed_count);
    }
}

int main() {
    std::thread producer_main(producer, 330);
    std::thread consumer_one(consumer, 110, 1);
    std::thread consumer_two(consumer, 110, 2);
    std::thread consumer_three(consumer, 110, 3);

    producer_main.join();
    consumer_one.join();
    consumer_two.join();
    consumer_three.join();

    return 0;
}