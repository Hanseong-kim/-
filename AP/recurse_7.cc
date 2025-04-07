#include <iostream>
#include <algorithm>
using namespace std;

struct position{
    int x, y;
}; // 튜플대신 구조체를 사용
// knight의 위치를 나타내는 구조체


//코드의 단점

bool move_knight(int sx, int sy, int ex, int ey, int moves, position* path){
    // Check if the number of moves is even and the difference in coordinates is odd
    if (sx < 0 || sx >= 8 || sy < 0 || sy >= 8) { return false;}
    if (moves == 0) {
        if(sx == ex && sy == ey){
            path[0].x = ex;
            path[0].y = ey;
            return true;
        }
        else return false;    
    }   
    int x_moves[8] = {1, 2, 2, 1, -1, -2, -2, -1};
    int y_moves[8] = {2, 1, -1, -2, 2, 1, -1, -2};
    for (int i = 0; i < 8; i++){
        if (move_knight(sx + x_moves[i], sy + y_moves[i], ex, ey, moves - 1, path + 1)){ 
            path[0].x = sx;
            path[0].y = sy;
            // cout << path[0].x << path[0].x << endl;
            return true;
        }
    }
    return false;
}

int main(){
    int start_x, start_y, end_x, end_y, num_moves;
    position path[100];
    cout << "Enter start x and y : " << flush;
    cin >> start_x >> start_y;
    cout << "Enter target x and y : " << flush;
    cin >> end_x >> end_y;
    cout << "Enter number of moves : " << flush;
    cin >> num_moves;
    if (num_moves < 0 || num_moves > 100){
        cout << "Invalid number of moves" << endl;
        return 1;
    }
    bool can_do_it = move_knight(start_x, start_y, end_x, end_y, num_moves, path);
    cout << can_do_it << endl; 
    if (can_do_it){
        for (int i = 0; i < num_moves + 1; i++){
            cout << path[i].x << " ," << path[i].y << endl;
        }
    }
} // 이거 공부 필요