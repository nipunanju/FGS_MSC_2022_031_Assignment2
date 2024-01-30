#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <time.h>

void bubbleSort1(int arry[], int n)  {
    int tmp;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arry[j] > arry[j + 1]) {
                tmp = arry[j];
                arry[j] = arry[j + 1];
                arry[j + 1] = tmp;
            }
        }
    }

}

void quicksort_parallel_implementation(int arr[], int low, int high) {
    if (low < high) {
        int pivot = arr[high];
        int i = low - 1;

        #pragma omp parallel for
        for (int j = low; j < high; j++) {
            if (arr[j] <= pivot) {
                i++;
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }

        int temp = arr[i + 1];
        arr[i + 1] = arr[high];
        arr[high] = temp;

        #pragma omp task
        quicksort_parallel_implementation(arr, low, i);
        #pragma omp task
        quicksort_parallel_implementation(arr, i + 2, high);
    }
}

void quicksort(int arr[], int size)
{
    #pragma omp parallel
    {
        #pragma omp single nowait
        quicksort_parallel_implementation(arr, 0, size - 1);
    }
}


int* read_numbers(int *outSize) {
    FILE *file;
    int count = 0;
    char *filename = "./input.txt";
    int *buf = (int *) malloc(sizeof(int) * 3001000);

    file = fopen(filename, "r");

    if (file == NULL) {
        printf("Error opening file.\n");
        return NULL;
    }

    int num;
    char delimiter;

    while (fscanf(file, "%d%c", &num, &delimiter) == 2) {
        // Process the number (you can do whatever you need with it)
        buf[count++] = num;

        if (delimiter == ',') {
        }
    }

    fclose(file);

    int *numbers = (int *)malloc(sizeof(int)*count);
    memcpy(numbers, buf, count * sizeof(int));

    *outSize = count + 1;

    return (int *)numbers;
}


int main() {

    FILE *fp;
    double start, msec, stop;

    int *arry;
    int n = sizeof(arry) / sizeof(arry[0]);

    start = omp_get_wtime();
    arry = read_numbers(&n);
    quicksort(arry, n);
    stop = omp_get_wtime();

    msec = stop - start;

    printf("Sorted array of %d items. time taken: %f seconds. THREAD COUNT USED %d.\n", n, msec, omp_get_max_threads());

    fp = fopen("output.txt", "w+");
    for (int i = 0; i < n; i++) {
        fprintf(fp, "%d ", arry[i]);
    }
    fclose(fp);
    printf("\n");
    return 0;
}
