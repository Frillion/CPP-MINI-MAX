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

struct move{
    position from;
    position to;

    position delta() const {
        return {this->to.x - this->from.x, this->to.y - this->from.y};
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
    const static position piece_moves[];
    int width;
    int height;
    std::unordered_map<position, std::string> board;
    std::string turn;

    State(int width,int height);
    bool is_valid_move(const move& mv);

public:
    bool apply_move(const move& mv);
    std::vector<move> get_legal_moves();
};
