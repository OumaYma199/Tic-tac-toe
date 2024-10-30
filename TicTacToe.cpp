#include <iostream>
#include <vector>

// TicTacToe class definition
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
class TicTacToe {
private:
    sf::RenderWindow window;
    std::vector<std::vector<char>> board; // 3x3 game board
    char currentPlayer; // 'X' or 'O'
    char aiPlayer; // AI player's symbol
    sf::Font font;

public:
    // Constructor to initialize the game board
    TicTacToe() : currentPlayer('X'), aiPlayer('O') {
        board.resize(3, std::vector<char>(3, ' '));
        window.create(sf::VideoMode(300, 300), "Tic Tac Toe");
        if (!font.loadFromFile("/usr/share/fonts/truetype/msttcorefonts/arial.ttf")) {
            std::cerr << "Error: Could not load font arial.ttf" << std::endl;
            exit(1);
        }
    }

    /// Draw the game board graphically
    ///
    /// \brief Draw the game board with the current game state
    ///
    /// This function draws the game board, which consists of a white background,
    /// black grid lines, and the X and O symbols on the board. It is called
    /// automatically by the game loop.
    ///
    /// \param None
    ///
    /// \return void
    ///
    /// Example of usage:
    /// \code
    /// TicTacToe game;
    /// game.drawBoard();
    /// \endcode
    void drawBoard() {
        window.clear(sf::Color::White); // Ensure the background is white

        // Draw grid lines
        sf::RectangleShape line(sf::Vector2f(300, 5));
        line.setFillColor(sf::Color::Black);

        // Draw vertical lines
        for (int i = 1; i < 3; i++) {
            line.setPosition(i * 100, 0);
            line.setSize(sf::Vector2f(5, 300));
            window.draw(line);
        }

        // Draw horizontal lines
        for (int i = 1; i < 3; i++) {
            line.setPosition(0, i * 100);
            line.setSize(sf::Vector2f(300, 5));
            window.draw(line);
        }

        // Draw X and O
        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(50);
        text.setFillColor(sf::Color::Black);

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] != ' ') {
                    text.setString(std::string(1, board[i][j]));
                    text.setPosition(j * 100 + 25, i * 100 + 10);
                    window.draw(text);
                }
            }
        }

        window.display();
    }

    /// Make a move for the current player
    ///
    /// \param row The row of the move (0-2)
    /// \param col The column of the move (0-2)
    ///
    /// \return void
    ///
    /// \details This method checks if the move is valid, if the position is already occupied, and if so, prints an error message. If not, it makes the move and switches the current player.
    ///
    /// \example
    /// \code
    /// TicTacToe game;
    /// game.makeMove(1, 1);
    /// \endcode
    /// This example makes a move for the current player at position (1,1).
    void makeMove(int row, int col) {
        // Vérifie si le mouvement est valide
        if (row < 0 || row >= 3 || col < 0 || col >= 3) {
            std::cout << "Invalid move. Try again." << std::endl;
            return;
        }

        // Vérifie si la position est déjà occupée
        if (board[row][col] != ' ') {
            std::cout << "Position already occupied. Try again." << std::endl;
            return;
        }

        // Effectue le mouvement
        board[row][col] = currentPlayer;

        // Change le joueur actuel
        currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
    }

    /// Check if there is a win
    ///
    /// \brief Check if the current state of the game board is a win
    ///
    /// This method checks if the current state of the game board is a win.
    /// It checks each row and column for a win, and also checks the two
    /// diagonals.
    ///
    /// \param None
    ///
    /// \return true if there is a win, false otherwise
    ///
    /// \details This method is called by the game loop to check for a win
    /// after each move. If there is a win, the game loop will exit and the
    /// winner will be printed to the console.
    ///
    /// \example
    /// \code
    /// TicTacToe game;
    /// // Make some moves
    /// if (game.checkWin()) {
    ///     std::cout << "Player " << (game.currentPlayer == 'X' ? 'O' : 'X') << " wins!" << std::endl;
    /// }
    /// \endcode
    bool checkWin() {
        // Check rows and columns
        for (int i = 0; i < 3; i++) {
            if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != ' ') {
                return true;
            }
            if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != ' ') {
                return true;
            }
        }
        // Check diagonals
        if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != ' ') {
            return true;
        }
        if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != ' ') {
            return true;
        }
        return false;
    }

    /// Minimax algorithm to evaluate best move for AI player
    ///
    /// This function implements the Minimax algorithm to evaluate the best move
    /// for the AI player. It takes a boolean parameter indicating whether it is
    /// the maximizing player (true) or the minimizing player (false).
    ///
    /// \param isMaximizing true if the AI player is the maximizing player, false
    /// otherwise
    ///
    /// \return an integer representing the score of the best move, with higher
    /// scores indicating better moves for the maximizing player, and lower
    /// scores indicating better moves for the minimizing player
    ///
    /// \example
    /// \code
    /// TicTacToe game;
    /// int score = game.minimax(true); // Evaluate the best move for the AI player
    /// \endcode
    int minimax(bool isMaximizing) {
        // Check for terminal states
        if (checkWin()) {
            return isMaximizing ? -1 : 1;
        }
        bool movesLeft = false;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == ' ') {
                    movesLeft = true;
                    break;
                }
            }
        }
        if (!movesLeft) return 0;

        int bestScore = isMaximizing ? -1000 : 1000;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == ' ') {
                    board[i][j] = isMaximizing ? aiPlayer : (aiPlayer == 'X' ? 'O' : 'X');
                    int score = minimax(!isMaximizing);
                    board[i][j] = ' ';
                    bestScore = isMaximizing ? std::max(bestScore, score) : std::min(bestScore, score);
                }
            }
        }
        return bestScore;
    }

    /// Play the game
    ///
    /// This method runs the game until the window is closed. It handles user interactions,
    /// processes AI moves, and checks for win conditions.
    ///
    /// \param None
    ///
    /// \return void
    ///
    /// \example
    /// \code
    /// TicTacToe game;
    /// game.playGame();
    /// \endcode
    ///
    /// This example demonstrates how to run the game by calling the playGame() method
    /// on a TicTacToe object.
    void playGame() {
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                } else if (event.type == sf::Event::MouseButtonPressed && currentPlayer != aiPlayer) {
                    int row = event.mouseButton.y / 100;
                    int col = event.mouseButton.x / 100;
                    makeMove(row, col);
                    drawBoard();
                    if (checkWin()) {
                        std::cout << "Player " << (currentPlayer == 'X' ? 'O' : 'X') << " wins!" << std::endl;
                        window.close();
                    } else {
                        currentPlayer = aiPlayer;
                    }
                }
            }
            if (currentPlayer == aiPlayer && window.isOpen()) {
                int bestScore = -1000;
                int bestMove = -1;
                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 3; j++) {
                        if (board[i][j] == ' ') {
                            board[i][j] = aiPlayer;
                            int score = minimax(false);
                            board[i][j] = ' ';
                            if (score > bestScore) {
                                bestScore = score;
                                bestMove = i * 3 + j;
                            }
                        }
                    }
                }
                makeMove(bestMove / 3, bestMove % 3);
                drawBoard();
                if (checkWin()) {
                    std::cout << "Player " << (currentPlayer == 'X' ? 'O' : 'X') << " wins!" << std::endl;
                    window.close();
                } else {
                    currentPlayer = (aiPlayer == 'X') ? 'O' : 'X';
                }
            }
        }
    }
};
/**
 * \brief Main entry point for the program
 *
 * Creates a TicTacToe object and starts the game loop.
 *
 * \return 0 if the program exits successfully
 *
 * \details This function is the main entry point for the program. It creates a
 * TicTacToe object and starts the game loop. The game loop waits for the user
 * to make a move by clicking on the game board, and then makes a move for the
 * AI player. The game loop continues until the game is won or a draw is
 * reached.
 *
 * \example
 * \code
 * TicTacToe game;
 * game.playGame();
 * \endcode
 * This example creates a TicTacToe object and starts the game loop.
 *
 * \pre None
 * \post The game loop has been started and the game is being played.
 */
int main() {
    TicTacToe game;
    game.playGame();
    return 0;
}
