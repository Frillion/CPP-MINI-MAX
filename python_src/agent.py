import random
from game import Game
from search import get_best_move_with_timer

#############

"""Agent acting in some environment"""


class Agent(object):

    # this method is called on the start of the new environment
    # override it to initialise the agent
    def start(self, role, width, height, play_clock):
        print("start called")
        return

    # this method is called on each time step of the environment
    # it needs to return the action the agent wants to execute as a string
    def next_action(self, last_move):
        print("next_action called")
        return "NOOP"

    # this method is called when the environment has reached a terminal state
    # override it to reset the agent
    def cleanup(self, last_move):
        print("cleanup called")
        return


#############

"""A random Agent for the KnightThrough game

 RandomAgent sends actions uniformly at random. In particular, it does not check
 whether an action is actually useful or legal in the current state.
 """


class RandomAgent(Agent):
    role = None
    play_clock = None
    my_turn = False
    width = 0
    height = 0

    # start() is called once before you have to select the first action. Use it to initialize the agent.
    # role is either "white" or "black" and play_clock is the number of seconds after which nextAction must return.
    def start(self, role, width, height, play_clock):
        self.play_clock = play_clock
        self.role = role
        self.my_turn = role != 'white'
        # we will flip my_turn on every call to next_action, so we need to start with False in case
        #  our action is the first
        self.width = width
        self.height = height
        # TODO: add your own initialization code here
        return

    def next_action(self, last_action):
        if last_action:
            if self.my_turn and self.role == 'white' or not self.my_turn and self.role != 'white':
                last_player = 'white'
            else:
                last_player = 'black'
            print("%s moved from %s to %s" % (last_player, str(last_action[0:2]), str(last_action[2:4])))
            # TODO: 1. update your internal world model according to the action that was just executed
        else:
            print("first move!")

        # update turn (above that line it myTurn is still for the previous state)
        self.my_turn = not self.my_turn
        if self.my_turn:
            # TODO: 2. run alpha-beta search to determine the best move

            # Here we just construct a random move (that will most likely not even be possible),
            # this needs to be replaced with the actual best move.
            x1 = random.randint(1, self.width)
            x2 = x1 + random.randint(1, 2) * random.choice([-1, 1])
            y1 = random.randint(1, self.height)
            direction = 1
            if self.role == 'black':
                direction = -1
            y2 = y1 + direction * random.randint(1, 2)
            return "(move " + " ".join(map(str, [x1, y1, x2, y2])) + ")"
        else:
            return "noop"

###################

class AI_Agent(Agent):
    """
    An implementation of our agent that uses iterative deepening Negamax search with alpha-beta pruning to choose moves.
    """

    role = None
    play_clock = None
    game = None  
    last_legal_move = None  # Cache our last legal move

    def start(self, role, width, height, play_clock):
        """Initializes the agent at the start of the game."""
        self.role = role.lower()  
        self.play_clock = play_clock
        self.game = Game(width, height)  # Create game state
        return


    def next_action(self, last_action):
        """Selects the best action using Negamax with Alpha-Beta Pruning."""
        if last_action:
            # Update game state with opponent's move.
            x1, y1, x2, y2 = last_action
            self.game.apply_move(x1, y1, x2, y2)

        # Don't do anything in the opponent's turn
        if self.game.board.turn != self.role:
            return "NOOP"

        legal_moves = self.game.board.get_legal_moves()
        if not legal_moves:
            # when a terminal state is detected and there are no legal moves,
            # instead of returning NOOP, we return our last legal move if we have one.
            if self.last_legal_move:
                return f"(move {self.last_legal_move[0]} {self.last_legal_move[1]} {self.last_legal_move[2]} {self.last_legal_move[3]})"
           
            else:
                return "NOOP"


        agent_col = self.role
        best_move = get_best_move_with_timer(
            self.game,
            max_depth=5,
            time_limit=self.play_clock - 1,
            agent_color=agent_col
        )

        if best_move and self.game.board.is_valid_move(*best_move):
            self.game.apply_move(*best_move)
            self.last_legal_move = best_move  # Cache this move.
            return f"(move {best_move[0]} {best_move[1]} {best_move[2]} {best_move[3]})"
       
        else:
            # If our search returns None despite having legal moves,
            # fall back to a random legal move.
            fallback = random.choice(legal_moves)
            self.game.apply_move(*fallback)
            self.last_legal_move = fallback
            return f"(move {fallback[0]} {fallback[1]} {fallback[2]} {fallback[3]})"

    def cleanup(self, last_action):
        """Resets the agent at the end of the game."""
        self.game = None
        print("Game Over. Resetting agent.")
        return
    
