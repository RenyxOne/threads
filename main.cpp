#include <iostream>
#include <thread>
#include <mutex>
#include <windows.h>

using namespace std;

const int CountPhilosophers = 10;
mutex forks[CountPhilosophers];
int eatsCount[CountPhilosophers] = {0};

[[noreturn]] void Philosopher(int number) {
    int firstFork = number;
    int secondFork = number + 1;

    if (number == CountPhilosophers - 1) {
        firstFork = 0;
        secondFork = number;
    }
    while (true) {
        forks[firstFork].lock();
        forks[secondFork].lock();

        eatsCount[number]++;
        Sleep(2000);

        forks[firstFork].unlock();
        forks[secondFork].unlock();

        Sleep(500);
    }
}

void printEatsCount() {
    while (true) {
        for (int i = 0; i < CountPhilosophers; i++)
            cout << eatsCount[i] << "  ";
        cout << endl;
        Sleep(1000);
    }
}

int main() {
    thread* threads[CountPhilosophers];
    for (int i = 0; i < CountPhilosophers; i++){
        threads[i] = new thread(Philosopher, i);
    }

    printEatsCount();
//    for (int i = 0; i < CountPhilosophers; i++){
//        threads[i]->join();
//    }
}