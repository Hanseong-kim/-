#include <iostream>
#include <cassert>

using namespace std;
enum class month{
    jan, feb, mar, apr, may, jun, jul, aug, sep, oct, nov, dec
};

static int num_days(month m){
    switch (m){
        case month::jan:
        case month::mar: 
        case month::may: 
        case month::jul: 
        case month::oct: 
        case month::dec: 
            return 31;
        case month::feb: return 28;
        default:
            return 30;
    }
    return -1; // error
}

struct date
{
    month m;
    int day; // range from 1 to 31
};

int date_difference(date const* d1, date const* d2){
    // d1 is earlier than d2
    if (d1->m > d2->m)  return -date_difference(d2, d1);
    assert(d1->m <= d2->m);
    int days = d2->day - d1->day;
    for (month m = d1->m; m < d2->m; m = (month)((int)m + 1)){
        days += num_days((month)m);
    }
    return days;
}

int main(){
    
    date d1 = {month::jun, 6};
    date d2 = {month::aug, 30};
    cout << date_difference(&d1, &d2) << endl;
    return 0;
}