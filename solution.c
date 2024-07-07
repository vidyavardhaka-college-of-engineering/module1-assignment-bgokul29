#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define N 3

// State space tree nodes
typedef struct Node {
    struct Node* parent;
    int mat[N][N];
    int x, y;
    int cost;
    int level;
} Node;

// Function to print N x N matrix
void printMatrix(int mat[N][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", mat[i][j]);
        }
        printf("\n");
    }
}

// Function to allocate a new node
Node* newNode(int mat[N][N], int x, int y, int newX, int newY, int level, Node* parent) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->parent = parent;
    memcpy(node->mat, mat, sizeof(node->mat));
    int temp = node->mat[x][y];
    node->mat[x][y] = node->mat[newX][newY];
    node->mat[newX][newY] = temp;
    node->cost = INT_MAX;
    node->level = level;
    node->x = newX;
    node->y = newY;
    return node;
}

int row[] = { 1, 0, -1, 0 };
int col[] = { 0, -1, 0, 1 };

// Function to calculate the number of misplaced tiles (Hamming distance)
int calculateCost(int initial[N][N], int final[N][N]) {
    int count = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (initial[i][j] && initial[i][j] != final[i][j])
                count++;
        }
    }
    return count;
}

// Function to check if (x, y) is a valid matrix coordinate
int isSafe(int x, int y) {
    return (x >= 0 && x < N && y >= 0 && y < N);
}

// Print path from root node to destination node
void printPath(Node* root) {
    if (root == NULL)
        return;
    printPath(root->parent);
    printMatrix(root->mat);
    printf("\n");
}

// Comparison function to be used in the priority queue
int comp(const void* lhs, const void* rhs) {
    Node* a = *(Node**)lhs;
    Node* b = *(Node**)rhs;
    return (a->cost + a->level) - (b->cost + b->level);
}

// Function to solve N*N - 1 puzzle algorithm using Branch and Bound
void solve(int initial[N][N], int x, int y, int final[N][N]) {
    Node* pq[10000]; // Priority queue (heap)
    int pqSize = 0;

    Node* root = newNode(initial, x, y, x, y, 0, NULL);
    root->cost = calculateCost(initial, final);
    pq[pqSize++] = root;

    while (pqSize) {
        qsort(pq, pqSize, sizeof(Node*), comp);
        Node* min = pq[0];
        memmove(pq, pq + 1, (--pqSize) * sizeof(Node*));

        if (min->cost == 0) {
            printPath(min);
            return;
        }

        for (int i = 0; i < 4; i++) {
            int newX = min->x + row[i];
            int newY = min->y + col[i];
            if (isSafe(newX, newY)) {
                Node* child = newNode(min->mat, min->x, min->y, newX, newY, min->level + 1, min);
                child->cost = calculateCost(child->mat, final);
                pq[pqSize++] = child;
            }
        }
    }
}

// Driver code
int main() {
    int initial[N][N] = {
        {1, 2, 3},
        {5, 6, 0},
        {7, 8, 4}
    };

    int final[N][N] = {
        {1, 2, 3},
        {5, 8, 6},
        {0, 7, 4}
    };

    int x = 1, y = 2;

    solve(initial, x, y, final);

    return 0;
}
