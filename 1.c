#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 9
#define SUBGRID_SIZE 3

int sudoku[SIZE][SIZE] = {
    {6, 2, 4, 5, 3, 9, 1, 8, 7},
    {5, 1, 9, 7, 2, 8, 6, 3, 4},
    {8, 3, 7, 6, 1, 4, 2, 9, 5},
    {1, 4, 3, 8, 6, 5, 7, 2, 9},
    {9, 5, 8, 2, 4, 7, 3, 6, 1},
    {7, 6, 2, 3, 9, 1, 4, 5, 8},
    {3, 7, 1, 9, 5, 6, 8, 4, 2},
    {4, 9, 6, 1, 8, 2, 5, 7, 3},
    {2, 8, 5, 4, 7, 3, 9, 1, 6}
};

int valid[3] = {1, 1, 1}; // [0] for rows, [1] for columns, [2] for subgrids

void* check_rows(void* arg) {
    for (int i = 0; i < SIZE; i++) {
        int seen[SIZE+1] = {0};
        for (int j = 0; j < SIZE; j++) {
            int num = sudoku[i][j];
            if (num < 1 || num > SIZE || seen[num]) {
                valid[0] = 0;
                return NULL;
            }
            seen[num] = 1;
        }
    }
    return NULL;
}

void* check_columns(void* arg) {
    for (int j = 0; j < SIZE; j++) {
        int seen[SIZE+1] = {0};
        for (int i = 0; i < SIZE; i++) {
            int num = sudoku[i][j];
            if (num < 1 || num > SIZE || seen[num]) {
                valid[1] = 0;
                return NULL;
            }
            seen[num] = 1;
        }
    }
    return NULL;
}

void* check_subgrids(void* arg) {
    for (int grid = 0; grid < SIZE; grid++) {
        int seen[SIZE+1] = {0};
        int startRow = (grid / SUBGRID_SIZE) * SUBGRID_SIZE;
        int startCol = (grid % SUBGRID_SIZE) * SUBGRID_SIZE;
        
        for (int i = startRow; i < startRow + SUBGRID_SIZE; i++) {
            for (int j = startCol; j < startCol + SUBGRID_SIZE; j++) {
                int num = sudoku[i][j];
                if (num < 1 || num > SIZE || seen[num]) {
                    valid[2] = 0;
                    return NULL;
                }
                seen[num] = 1;
            }
        }
    }
    return NULL;
}

int main() {
    pthread_t threads[3];
    
    pthread_create(&threads[0], NULL, check_rows, NULL);
    pthread_create(&threads[1], NULL, check_columns, NULL);
    pthread_create(&threads[2], NULL, check_subgrids, NULL);
    
    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }
    
    if (valid[0] && valid[1] && valid[2]) {
        printf("The Sudoku solution is valid!\n");
    } else {
        printf("The Sudoku solution is invalid!\n");
        if (!valid[0]) printf("Rows check failed.\n");
        if (!valid[1]) printf("Columns check failed.\n");
        if (!valid[2]) printf("Subgrids check failed.\n");
    }
    
    return 0;
}