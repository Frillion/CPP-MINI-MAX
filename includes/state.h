#include <cstdlib>
#include <memory>
#include <unordered_map>
#include <cmath>
#include <vector>

#ifndef STATE_REPR
#define STATE_REPR

enum Player{
    WHITE = 1,
    BLACK = -1,
    NONE = 2
};

enum Outcome{
    BLACK_W = -1,
    WHITE_W = 1,
    NONE_W = 2,
    DRAW = 0
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

    move(int x_from, int y_from, int x_to, int  y_to){
        from.x = x_from;
        from.y = y_from;
        to.y = y_to;
        to.x = x_to;
    }

    move(const position& from, const position& to){
        this->from = from;
        this->to = to;
    }

    move(const move& other){
        this->from.x = other.from.x;
        this->from.y = other.from.y;
        this->to.x = other.to.x;
        this->to.y = other.to.y;
    }

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
    Player turn;
    std::unordered_map<position, Player> board;
    bool is_valid_move(const move& mv);

public:
    State(int width,int height);
    State(State& other);
    const std::unordered_map<position,Player>& get_board()const {return this->board;}
    const int& get_width()const {return this->width;}
    const int& get_height()const {return this->height;}
    const Player& get_turn()const {return this->turn;}
    std::unique_ptr<State> apply_move(const move& mv);
    std::vector<move> get_legal_moves();
    Outcome check_win();
};

#endif
