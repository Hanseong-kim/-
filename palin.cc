#include <iostream>
#include <string> // for the c++ string type
#include <cstring> // for the c string type
using namespace std;
/*
 * E.g., "radar" is a palindrome. 회문(앞에서 읽으나 뒤에서 읽으나 같은 단어)
 * "car" is not a palindrome
 *  "a" is a palindrome
 *
 *  For a stirng s of lenght >= 2, s is a palindrome if and only if
 *  s = aba whre a is a letter in our alphabet and b is a string of length >= 2
 *  and b is a palindrome
 */

bool is_palindrome(const char* s, size_t l){
    // 문자의 길이가 0, 1일때 
    if (l <= 1) return true;
    if (s[0] != s[l - 1]) return false; // 첫번째 문자와 마지막 문자가 다르면 false
    return is_palindrome(s + 1, l - 2); // 첫번째 문자와 마지막 문자를 제외한 나머지 문자열에 대해 재귀적으로 호출
}

bool is_palindrome(char const* s){
    return is_palindrome(s, strlen(s));
}

int main(){
    string x;
    cin >> x;
    cout << "Is it a palindrome? " << is_palindrome(x.c_str()) << endl;

    return 0;
}