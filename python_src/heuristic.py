def evaluate_state(game, agent_color):

    """Evaluates the board from the agent's perspective using a simple heuristic"""
    
    #Check for win
    if game.winner == "white":
        return 100 if agent_color == "white" else -100
    
    elif game.winner == "black":
        return -100 if agent_color == "white" else 100
    
    elif game.is_draw:
        return 0  

    # Advancement heuristic
    white_max = 0  
    black_min = game.board.height 
    piece_advantage = 0  # metric for capturing pieces

    for (x, y), piece in game.board.board.items():
        if piece == "W":
            white_max = max(white_max, y)  
           
            if agent_color == "white":
                piece_advantage += 1  # if there are more white pieces, then it's better for white
        

        elif piece == "B":
            black_min = min(black_min, y) 
            
            if agent_color == "black":
                piece_advantage += 1  # if there are more black pieces, then it's better for black

    heuristic_value = ((black_min - 1) - (game.board.height - white_max)) + (piece_advantage * 10)

    # Adjust evaluation based on agent's perspective
    return heuristic_value if agent_color == "white" else -heuristic_value

