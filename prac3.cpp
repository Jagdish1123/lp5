#include <iostream>
#include <vector>
#include <omp.h>
#include <climits>
#include <cstdlib>
#include <ctime>

using namespace std;

int main() {
    int n = 1e7;
    vector<int> a(n);

    srand(time(0));

    for (int i = 0; i < n; i++)
        a[i] = rand() % 100000;

    long long sum = 0;
    int min_val = INT_MAX;
    int max_val = INT_MIN;

    double t1, t2;

    /* -------- PARALLEL MIN/MAX/SUM -------- */
    t1 = omp_get_wtime();

    #pragma omp parallel for reduction(+:sum) reduction(min:min_val) reduction(max:max_val)
    for (int i = 0; i < n; i++) {
        sum += a[i];
        if (a[i] < min_val) min_val = a[i];
        if (a[i] > max_val) max_val = a[i];
    }

    t2 = omp_get_wtime();

    cout << "Time taken by parallel min/max/sum: " << t2 - t1 << " seconds\n";

    /* -------- SEQUENTIAL MIN/MAX/SUM -------- */
    long long sum_seq = 0;
    int min_seq = INT_MAX;
    int max_seq = INT_MIN;

    t1 = omp_get_wtime();

    for (int i = 0; i < n; i++) {
        sum_seq += a[i];
        if (a[i] < min_seq) min_seq = a[i];
        if (a[i] > max_seq) max_seq = a[i];
    }

    t2 = omp_get_wtime();

    cout << "Time taken by sequential min/max/sum: " << t2 - t1 << " seconds\n";

    /* -------- FINAL OUTPUT -------- */
    cout << "Minimum: " << min_seq << endl;
    cout << "Maximum: " << max_seq << endl;
    cout << "Sum: " << sum_seq << endl;
    cout << "Average: " << (double)sum_seq / n << endl;

    return 0;
}