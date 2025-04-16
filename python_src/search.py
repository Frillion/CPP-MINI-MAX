from copy import deepcopy
from heuristic import evaluate_state
import time
state_expansions = 0
total_state_expansion = 0

def negamax_alpha_beta(game, depth, alpha, beta, color, agent_color):
    """ This function implements the Negamax algorithm with alpha-beta pruning for the game """
    
    global state_expansions
    state_expansions += 1
    # If we reach depth 0 or the game ends
    if depth == 0 or game.winner or game.is_draw:
        return color * evaluate_state(game, agent_color) 

    best_value = float('-inf')

    for move in game.board.get_legal_moves():
        new_game = deepcopy(game)
        new_game.apply_move(*move)

        # Negamax recursive call with flipped sign for the opponent
        value = -negamax_alpha_beta(new_game, depth - 1, -beta, -alpha, -color, agent_color)

        best_value = max(best_value, value)
        alpha = max(alpha, best_value)

        if alpha >= beta:  # Pruning
            break

    return best_value
    

def get_best_move(game, depth, agent_color):
    """ Select the best move using Negamax with Alpha-Beta Pruning, ensuring the agent is always maximizing """
    
    best_move = None
    best_value = float('-inf')
    color = 1  # The agent (whoever they are) is always maximizing

    for move in game.board.get_legal_moves():
        new_game = deepcopy(game)
        new_game.apply_move(*move)
        
        eval = -negamax_alpha_beta(new_game, depth, float('-inf'), float('inf'), -color, agent_color)

        if eval > best_value:
            best_value = eval
            best_move = move

    return best_move


def get_best_move_with_timer(game, max_depth, time_limit, agent_color):
    global state_expansions
    global total_state_expansion
    state_expansions = 0  # Reset state expansion counter
    best_move = None
    total_start_time = time.time()  # Mark the overall start time
    iteration_data = []  # List to record (depth, iteration runtime)
    depth = 1

    while depth <= max_depth:
        # Check overall time limit before starting this depth iteration.
        if time.time() - total_start_time > time_limit:
            break

        iteration_start_time = time.time()  # Start time for this depth iteration

        # Run search at current depth.
        move = get_best_move(game, depth, agent_color)
        if move:
            best_move = move

        iteration_runtime = time.time() - iteration_start_time
        iteration_data.append((depth, iteration_runtime))
        depth += 1

    total_runtime = time.time() - total_start_time
    total_state_expansion += state_expansions

    # Print out information for each iteration.
    for d, rt in iteration_data:
        print(f"Depth: {d}, Iteration runtime: {rt:.3f} sec")
    print(f"Total runtime: {total_runtime:.3f} sec")
    print(f"Number of state expantions: {state_expansions}")
    print(f"Total number of state expantions: {total_state_expansion}")
    
    return best_move

