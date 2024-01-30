#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <time.h>

void quicksort_serial_implementation(int arr[], int low, int high) {
    if (low < high) {
        int pivot = arr[high];
        int i = low - 1;

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

        quicksort_serial_implementation(arr, low, i);
        quicksort_serial_implementation(arr, i + 2, high);
    }
}

void quicksort_serial(int arr[], int size)
{
    quicksort_serial_implementation(arr, 0, size - 1);
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
    clock_t start, end;
    int *arry;
    int n = sizeof(arry) / sizeof(arry[0]);

    start = clock();
    arry = read_numbers(&n);
    quicksort_serial(arry, n);
    end = clock();

    double msec = ((double)end - start) / CLOCKS_PER_SEC;
    
    printf("Sorted array items %d. time taken: %f seconds.", n, msec);

    fp = fopen("output.txt", "w+");
    for (int i = 0; i < n; i++) {
        fprintf(fp, "%d ", arry[i]);
    }
    fclose(fp);
    printf("\n");
    return 0;
}
