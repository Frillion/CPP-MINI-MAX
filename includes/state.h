#include <functional>
#include <string>
#include <unordered_map>

struct position{ 
    int x;
    int y;
    bool operator==(const position& other) const{
        return this->x == other.x && this->y == other.y;
    }
};

template <>
struct std::hash<position>
{
  std::size_t operator()(const position& k) const
  {
    using std::hash;

    // Compute individual hash values for first,
    // second and third and combine them using XOR
    // and bit shifting:

    return ((hash<int>()(k.x)
             ^ (hash<int>()(k.y) << 1)) >> 1);
  }
};

class State{
    int width;
    int height;
    std::unordered_map<position, std::string> board;
    std::string turn;

    State(int width,int height){
        this->width = width;
        this->height = height;
        this->turn = "white";
        for(int i = 1; i < width + 1; i++){
            board[{i,1}] = "W";
            board[{i,2}] = "W";
            board[{i,this->height}] = "B";
            board[{i,this->height - 1}] = "B";
        }
        for(int x = 1; x < width; x++){
            for(int y = 3; y < this->height - 1; y++){
                board[{x,y}] = "";
            }
        }
    }
};





void foo(){
    int x;
    int y;
}
