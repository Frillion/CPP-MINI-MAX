class Board:
    """
    This acts as the internal board representation, which it keeps track of pieces and turns, and validates moves.
    """
    def __init__(self, width, height):
        
        self.width = width
        self.height = height
        self.board = {}
        self.turn = "white"
        self.initialize_board()

    def initialize_board(self):

        for x in range(1, self.width+1):
            self.board[(x, 1)] = "W"
            self.board[(x, 2)] = "W"
            self.board[(x, self.height)] = "B"       
            self.board[(x, self.height-1)] = "B"

        for x in range(1, self.width+1):
            for y in range(3, self.height-1):
                self.board[(x, y)] = None


    def is_valid_move(self, x1, y1, x2, y2):
        """Check if the given move is legal."""
        if not (1 <= x1 <= self.width and 1 <= y1 <= self.height):
            return False
        if not (1 <= x2 <= self.width and 1 <= y2 <= self.height):
            return False
        
        if self.board.get((x1, y1)) is None:
            return False  # No piece to move
        
        piece = self.board.get((x1, y1))
        if (piece == "W" and self.turn != "white") or (piece == "B" and self.turn != "black"):
            return False  # Wrong turn


        # Define movement restrictions
        dx, dy = abs(x2 - x1), abs(y2 - y1)
        if not ((dx == 2 and dy == 1) or (dx == 1 and dy == 2)):
            return False  

        if (piece == "W" and y2 <= y1) or (piece == "B" and y2 >= y1):
            return False  # Can't move backwards


        #Check the capturing moves now
        if self.board.get((x2, y2)) is not None:
            if dx != 1 or dy != 1: 
                return False
            
            if self.board.get((x2, y2)) == piece:
                return False  # Can't capture own piece
            
        else:
            if dx == 1 and dy == 1:
                return False  # Can't move diagonally without capturing a piece

        return True


    def apply_move(self, x1, y1, x2, y2):
        """Apply a move and update the board state."""
        if not self.is_valid_move(x1, y1, x2, y2):
            return False

        self.board[(x2, y2)] = self.board[(x1, y1)]
        self.board[(x1, y1)] = None
        self.turn = "black" if self.turn == "white" else "white"  # Switch turn
        return True
    

    def get_legal_moves(self):
        """Return all possible legal moves for the current player, including normal and capture moves."""
        moves = []
        for (x, y), piece in self.board.items():
            if piece and ((piece == "W" and self.turn == "white") or (piece == "B" and self.turn == "black")):
                if piece == "W":
                    knight_moves = [(2, 1), (1, 2), (-2, 1), (-1, 2)] # White moves upward
                    capture_moves = [(1, 1), (-1, 1)]  
                
                else:  
                    knight_moves = [(2, -1), (1, -2), (-2, -1), (-1, -2)]  # Black moves downward
                    capture_moves = [(1, -1), (-1, -1)] 

                # For normal knight moves:
                for dx, dy in knight_moves:
                    x2, y2 = x + dx, y + dy
                    if self.is_valid_move(x, y, x2, y2):
                        moves.append((x, y, x2, y2))


                # For capture moves:
                for dx, dy in capture_moves:
                    x2, y2 = x + dx, y + dy

                    if (1 <= x2 <= self.width) and (1 <= y2 <= self.height): 
                        target_piece = self.board.get((x2, y2))

                        if target_piece and target_piece != piece:  # Can't capture own pieces
                            moves.append((x, y, x2, y2))
        
        return moves
