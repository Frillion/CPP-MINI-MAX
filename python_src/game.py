from board import Board

class Game:
    """
    This class encapsulates a Board instance and provides methods to apply moves,
    check for win/draw conditions, and display the game state.
    """

    def __init__(self, width, height):
        self.board = Board(width, height)
        self.winner = None
        self.is_draw = False


    def apply_move(self, x1, y1, x2, y2):
        """Apply a move and check for game termination."""
        success = self.board.apply_move(x1, y1, x2, y2)
        if not success:
            return False

        if self.check_win():
            return True  # Game Over

        return True  # Continue playing


    def check_win(self):
        """Check if the game is won, lost, or a draw."""
        for (x, y), piece in self.board.board.items():

            if piece == "W" and y == self.board.height:  # White wins by reaching last row
                self.winner = "white"
                return True
            
            if piece == "B" and y == 1:  # Black wins by reaching first row
                self.winner = "black"
                return True
            

        # Check if the current player has no legal moves
        current_moves = self.board.get_legal_moves()

        if not current_moves:  # No legal moves for current player
            # Switch turns and check if the opponent ALSO has no legal moves
            self.board.turn = "white" if self.board.turn == "black" else "black"
            opponent_moves = self.board.get_legal_moves()
            self.board.turn = "white" if self.board.turn == "black" else "black"  # Restore turn

            if not opponent_moves:  # No legal moves for either player means that it's a draw
                self.is_draw = True
                return True
            
            else:
                # If only one player is out of moves, the other wins
                self.winner = "black" if self.board.turn == "white" else "white"
                return True


        return False
