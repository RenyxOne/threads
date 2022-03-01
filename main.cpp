#include <iostream>
#include <cmath>
#include <ctime>
#include <list>
#include <thread>

void printMatrix(int **matrix, int n, int m){
    for (int i = 0; i < n; i++){
        for (int j = 0; j < m; j++){
            std::cout << matrix[i][j] << '\t';
        }
        std::cout << std::endl;
    }
}

//void calc

int** createMatrixWithZeros (int n, int m) {
    int** matrix = new int*[n];
    for (int i = 0; i < n; i++) {
        matrix[i] = new int[m];
        for (int j = 0; j < m; j++){
            matrix[i][j] = 0;
        }
    }
    return matrix;
}

void calcOneElement(int **A, int **B, int size, int **dest, int i, int j){
    int res = 0;
    for (int k = 0; k < size; k++){
        res += A[i][k] * B[k][j];
    }
    dest[i][j] = res;
}

void calcOneResultLine(int **A, int **B, int size, int **dest, int i) {
    for (int j = 0; j < size; j++){
        int res = 0;
        for (int k = 0; k < size; k++){
            res += A[i][k] * B[k][j];
        }
        dest[i][j] = res;
    }
}

void pow2MatrixAsync(int** A, int** B, int n2, int m1, int** dest){
    std::list<std::thread*> threadList;
    for (int i = 0; i < n2; i++){
        threadList.push_front(new std::thread(calcOneResultLine, A, B, n2, dest, i));
//        for (int j = 0; j < m1; j++){
//            threadList.push_front(new std::thread(calcOneElement, A, B, n2, dest, i, j));
//        }
    }
    for (auto t : threadList){
        t->join();
        delete t;
    }

}

void pow2MatrixSync(int** A, int** B, int n2, int m1, int** dest){
    for (int i = 0; i < n2; i++){
        calcOneResultLine(A,B,m1,dest, i);
//        for (int j = 0; j < m1; j++){
//            calcOneElement(A, B, n2, dest, i, j);
//        }
    }
}

void testMults(int size){
    std::cout << "Test myltiply: to square matrix with size " << size << std::endl;
    int** matrix = createMatrixWithZeros(size,size);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++){
            matrix[i][j] = rand() % 10;
        }
    }

    std::cout << "Original matrix:" << std::endl;
    printMatrix(matrix,size,size);

    int** result = createMatrixWithZeros(size,size);
    pow2MatrixAsync(matrix, matrix, size, size, result);

    std::cout << "matrix^2:" << std::endl;
    printMatrix(result, size, size);


    std::cout << "Test finished" << std::endl << std::endl;
}

void testTime (int size) {
    int n = size;
    int m = size;
    int** matrix = createMatrixWithZeros(n,m);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++){
            matrix[i][j] = rand();
        }
    }

    int** result = createMatrixWithZeros(n,m);
    //sync mult
    time_t syncStart = clock();
    pow2MatrixSync(matrix, matrix, n, m, result);
    time_t syncEnd = clock();

    //async mult
    time_t asyncStart = clock();
    pow2MatrixAsync(matrix, matrix, n, m, result);
    time_t asyncEnd = clock();

    time_t syncTime = syncEnd - syncStart;
    time_t asyncTime = asyncEnd - asyncStart;

    std::cout << "Size matrix: " << n << "x" << m << std::endl;
    std::cout << "syncTime: " << syncTime << std::endl;
    std::cout << "asyncTime: " << asyncTime << std::endl;
}