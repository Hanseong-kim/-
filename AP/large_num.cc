#include <iostream>
#include <algorithm> // std::max
using namespace std;

float largest(float const *xs, size_t n){
    float l = *xs; // xs[0] == *xs
    for (size_t i = 0; i < n; i++){
        if (xs[i] > l){
            l = max(l, xs[i]); // l = l > xs[i] ? l : xs[i];
        }
    }
    return l;
}

int main(){
    float nums[1000];
    size_t n;
    cout << "how many numbers? " << flush;
    cin >> n;
    for (size_t i = 0; i < n; i++){
        cout << "number " << i + 1 << ": " << flush;
        cin >> nums[i];
    }
    cout << "largest number: " << largest(nums, n) << endl;
    return 0;
}