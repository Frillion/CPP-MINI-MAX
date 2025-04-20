#include <cstdlib>
#include <memory>
#include <string>
#include <unordered_map>
#include <cmath>
#include <vector>

enum Player{
    WHITE = 1,
    BLACK = -1,
    NONE = 2
};

enum Outcome{
    BLACK_W = -1,
    WHITE_W = 1,
    DRAW = 2
};

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
    std::unordered_map<position, Player> board;
    Player turn;
    bool is_valid_move(const move& mv);

public:
    State(int width,int height);
    State(State& other);
    std::unique_ptr<State> apply_move(const move& mv);
    std::vector<move> get_legal_moves();
    bool check_win();
};

