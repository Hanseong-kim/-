#include <iostream>
#include <cmath>
using namespace std;


struct point
{
    double x, y, z;
};

static double distance(point const* a, point const *b){
    double x_diff = a->x - b->x;
    double y_diff = a->y - b->y;
    double z_diff = a->z - b->z;
    return sqrt(x_diff * x_diff + y_diff * y_diff + z_diff * z_diff);
}


int main(){
    double x, y, z;
    cin >> x >> y >> z;
    point p = { .x = x, .y = y, .z = z};
    cin >> x >> y >> z;
    point q = {x, y, z};
    cout << distance(&p, &q) << endl;

    return 0;
}