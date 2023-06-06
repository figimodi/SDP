#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <array>
#include <fstream>
#include <algorithm>

using namespace std;
mutex m;

void swap(int *xp, int *yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

// A function to implement bubble sort
void bubbleSort(int arr[], int n)
{
    int i, j;
    for (i = 0; i < n - 1; i++)

        // Last i elements are already in place
        for (j = 0; j < n - i - 1; j++)
            if (arr[j] > arr[j + 1])
                swap(&arr[j], &arr[j + 1]);
}

void f(const char *filename, int i, int *length, int **vector)
{
    ifstream infile(filename);
    int number;
    if (!infile.is_open())
    {
        cerr << "Error while opening the file" << endl;
        return;
    }
    infile >> number;
    // allocate array for ordering the vector
    *vector = (int *)malloc(number * sizeof(int));
    *length = number;
    int j = 0;
    while (infile >> number)
    {
        *(*vector + j) = number;
        j++;
    }
    bubbleSort(*vector, *length);
    infile.close();
}

int main(int argc, char *argv[])
{
    int count = argc - 2; // number of files to read
    vector<thread> threads;
    int *length = (int *)malloc(count * sizeof(int));
    int **vectors = (int **)malloc(count * sizeof(int *));
    int *output;
    int final_length = 0, out_index = 1;

    for (int i = 0; i < count; i++)
    {
        threads.emplace_back(f, argv[1 + i], i, &length[i], &vectors[i]);
    }

    for (int i = 0; i < count; i++)
    {
        threads[i].join();
    }

    for (int i = 0; i < count; i++)
    {
        final_length += length[i];
    }
    output = (int *)malloc(final_length * sizeof(int));
    for (int i = 0; i < count; i++)
    {
        for (int j = 0; j < length[i]; j++)
        {
            output[out_index + j] = vectors[i][j];
        }
        out_index += length[i];
    }
    output[0] = final_length;

    bubbleSort(output + 1, final_length);

    ofstream outf(argv[count + 1]);
    for (int i = 0; i <= final_length; i++)
    {
        outf << output[i] << " ";
    }
    outf.close();

    free(length);
    for (int i = 0; i < count; i++)
    {
        free(vectors[i]);
    }
    free(vectors);
    free(output);

    return 1;
}
