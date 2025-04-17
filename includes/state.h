#include <cstdlib>
#include <functional>
#include <string>
#include <unordered_map>
#include <cmath>
#include <vector>

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

    State(int width,int height);
    bool is_valid_move(position &curr_pos, position &next_pos);

public:
    bool apply_move(position &curr_pos, position &next_pos);
    std::vector<position> get_legal_moves();
};
