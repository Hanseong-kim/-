#include <iostream>
#include <algorithm>
// static long long fib(int n){
//     if (n <= 1) return n;
//     else        return fib(n - 1) + fib(n - 2);
// }
// // static long long fib(int n){

static double square_root(double x, double low, double high){
    if (high - low < 0.0001) return low / 2;
    double mid = low + (high - low) / 2;
    if (mid * mid < x) return square_root(x, mid, high);
    else               return square_root(x, low, mid);
}
static double sqaure_root(double x){
    return square_root(x, 0, std::max(x,1.0));
}
int main(){
    using namespace std;
    int n;
    cin >> n;
    cout << sqaure_root(n) << endl;
    return 0;
}
//gdb쓰는거 배우기
