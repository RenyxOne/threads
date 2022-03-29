#include <iostream>
#include <thread>
#include <fstream>
#include <windows.h>

using namespace std;

char fileName[] = "chat.txt";

void ReadProcess(char* from, char *to) {
    CreateSemaphore(NULL, 0, 5, to);
    auto semaphore = OpenSemaphoreA(SEMAPHORE_ALL_ACCESS, TRUE, to);
    while (true) {
        WaitForSingleObject(semaphore, INFINITE);
        fstream file(fileName, fstream::in);
        system("cls");
        while (!file.eof()) {
            char buf[100];
            file.getline(buf, sizeof(buf));
            cout << buf;
        }
        cout << endl;
    }
}

void WriteProcess(char* from, char *to) {
    CreateSemaphore(NULL, 0, 5, from);
    auto semaphore = OpenSemaphoreA(SEMAPHORE_ALL_ACCESS, TRUE, from);

    while (true) {
        string t;
        cin >> t;
        fstream file(fileName, fstream::out);
        file << from << ": " << t;
        file.close();
        ReleaseSemaphore(semaphore,1, NULL);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << "input args" << std::endl;
        return 0;
    }

    thread test(ReadProcess, argv[1], argv[2]);
    WriteProcess(argv[1], argv[2]);
}