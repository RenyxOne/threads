#include <iostream>
#include <cmath>
#include <ctime>
#include <list>
#include <thread>
#include <windows.h>
#include <atomic>

int counterForMutex = 0;
int counterForCommon = 0;
std::atomic<int> counterForAtomic = 0;

std::mutex door;
int threadCount = std::thread::hardware_concurrency();

void incCounterMutex(int count){
    std::lock_guard<std::mutex> t(door);
    for (int i = 0; i < count; i++) {
        counterForMutex++;
    }
}

void incCounterAtomic(int count){
    for (int i = 0; i < count; i++) {
        counterForAtomic++;
    }
}

void incCounterCommon(int count){
    for (int i = 0; i < count; i++) {
        counterForCommon++;
    }
}

template <class T>
void testCounter(int incCount, void(*func)(int count), T& counter, const char* str) {
    counter = 0;
    int size = (threadCount > incCount) ? 1 : incCount / threadCount;

    std::list<std::thread*> threadList;
    int maxThreads = threadCount < incCount ? threadCount : incCount;

    for (int i = 0; i < maxThreads; i++){
         threadList.push_front(new std::thread(func, size));
    }

    for (auto t : threadList){
        t->join();
        delete t;
    }
    std::cout << str << std::endl;
    std::cout << "theory: " << incCount << std::endl;
    std::cout << "real: " << counter << std::endl;
    std::cout << std::endl;
}

int main() {
//    testMults(2);
//    testTime(1000);
    int count = 1000000;
    clock_t start, end;
    start = clock();
    testCounter(threadCount * count, incCounterAtomic, counterForAtomic, "atomic");
    end = clock();
    std::cout << end - start << std::endl;
    start = clock();
    testCounter(threadCount * count, incCounterMutex, counterForMutex, "Mutex");
    end = clock();
    std::cout << end - start << std::endl;
    start = clock();
    testCounter(threadCount * count, incCounterCommon, counterForCommon, "Common");
    end = clock();
    std::cout << end - start << std::endl;
}