#include <iostream>
#include <string>
using namespace std;

void print_each_char(char const* s){
    for (; *s; s++){ // 문자열의 끝을 만날 때까지 반복
        cout << *s << " " << flush;
    }
}



int main(){
    string x = "hello";
    string y = x;

    x += " world";
    cout << x << endl;
    cout << y << endl;
    char a = x[0];
    char b = 65;
    char c = 'e';
    cout << a << b << c << endl;
    print_each_char(x.c_str());
    return 0;
}

