#include <stdio.h>

int main()
{
    int n, m;
    printf("Introduceti dim mat (n m): ");
    scanf("%d %d", &n, &m);

    int A[n][m], B[n][m], C[n][m];

    printf("Introduceti el A:\n");
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            scanf("%d", &A[i][j]);

    printf("Introduceti el B:\n");
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            scanf("%d", &B[i][j]);

    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            C[i][j] = A[i][j] + B[i][j];

    printf("Rez:\n");
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
            printf("%d ", C[i][j]);
        printf("\n");
    }

    return 0;
}
