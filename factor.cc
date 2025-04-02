/*
 * We want to read in positive integers and get back an array with 
 * all of its factors
 */

#include <iostream>
#include <cmath>
#include <vector>
using namespace std;

void factor(unsigned long n,unsigned long* fs, size_t* num_fs){
    *num_fs = 0; // Initialize the number of factors to 0
    for (unsigned long i = 2; i <= n; i++){
        while (n % i == 0){
            fs[(*num_fs)] = i;
            (*num_fs)++; // Increment the number of factors
            n /= i;
        }
    } // 
}

int main(){
    unsigned long fs[100];
    size_t num_fs;
    unsigned long n = 0;
    cin >> n;
    factor(n, fs, &num_fs);
    for (size_t i = 0; i < num_fs; i++){
        cout << fs[i] << " ";
    }
    cout << endl;
    return 0;
}