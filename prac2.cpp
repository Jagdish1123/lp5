#include <iostream>
#include <vector>
#include <omp.h>
#include <cstdlib>
#include <ctime>
using namespace std;

void bubbleSeq(vector<int>& a) {
    int n = a.size();

    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (a[j] > a[j + 1])
                swap(a[j], a[j + 1]);
}

void bubblePar(vector<int>& a) {
    int n = a.size();

    for (int i = 0; i < n; i++) {

        if (i % 2 == 0) {

            #pragma omp parallel for
            for (int j = 0; j < n - 1; j += 2)
                if (a[j] > a[j + 1])
                    swap(a[j], a[j + 1]);

        } else {

            #pragma omp parallel for
            for (int j = 1; j < n - 1; j += 2)
                if (a[j] > a[j + 1])
                    swap(a[j], a[j + 1]);
        }
    }
}
void merge(vector<int>& a, int l, int m, int r) {

    vector<int> temp(r - l + 1);
    int ptr = 0, i = l, j = m + 1;

    while (i <= m && j <= r) {
        if (a[i] < a[j])
            temp[ptr++] = a[i++];
        else
            temp[ptr++] = a[j++];
    }

    while (i <= m)
        temp[ptr++] = a[i++];

    while (j <= r)
        temp[ptr++] = a[j++];

    for (int k = 0; k < r - l + 1; k++) {
        a[l + k] = temp[k];
    }
}

void mergeSeq(vector<int>& a, int l, int r) {
    if (l >= r) return;

    int m = (l + r) / 2;
    mergeSeq(a, l, m);
    mergeSeq(a, m + 1, r);
    merge(a, l, m, r);
}

void mergePar(vector<int>& a, int l, int r) {
    if (l >= r) return;

    int m = (l + r) / 2;

    #pragma omp task //creates a task to be executed by a thread
    mergePar(a, l, m);

    #pragma omp task //creates a task to be executed by a thread
    mergePar(a, m + 1, r);

    #pragma omp taskwait //waits for all the tasks to complete

    merge(a, l, m, r);
}
int main() {

    int n = 10000;
    vector<int> arr(n);

    // srand(time(0)); //srand is used to seed the random number generator
    for (int i = 0; i < n; i++)
        arr[i] = n - i;
    

    double t1, t2;
    vector<int> a;

    cout << "Testing " << n << " elements\n\n";

    /* Bubble Seq */
    a = arr;
    t1 = omp_get_wtime(); // omp_get_wt// full form of omp_get_wtime() is open multiprocessing get wall time
    bubbleSeq(a);
    t2 = omp_get_wtime();
    cout << "Seq Bubble: " << t2 - t1 << "\n";

    /* Bubble Par */
    a = arr;
    t1 = omp_get_wtime();
    bubblePar(a);
    t2 = omp_get_wtime();
    cout << "Par Bubble: " << t2 - t1 << "\n";

    /* Merge Seq */
    a = arr;
    t1 = omp_get_wtime();
    mergeSeq(a, 0, n - 1);
    t2 = omp_get_wtime();
    cout << "Seq Merge:  " << t2 - t1 << "\n";

    /* Merge Par */
    a = arr;
    t1 = omp_get_wtime();

    #pragma omp parallel
    {
        #pragma omp single
        mergePar(a, 0, n - 1);
    }

    t2 = omp_get_wtime();
    cout << "Par Merge:  " << t2 - t1 << "\n";

    return 0;
}