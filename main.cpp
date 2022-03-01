#include <iostream>
#include <cmath>
#include <ctime>
#include <list>
#include <thread>
#include <semaphore>

std::binary_semaphore
        smphSignalMainToThead(0),
        smphSignalTheadToMain(0);


void theadFunc(){
    while (true) {
        smphSignalMainToThead.acquire();
        std::cout << "hello";
    }
}

int main() {
    auto t = std::thread(theadFunc);
    while (true) {
        char a;
        std::cin >> a;
        if (a == '1')
            smphSignalMainToThead.release();
    }
}