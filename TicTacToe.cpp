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

    // Method to draw the game board graphically
    void drawBoard() {
        window.clear(sf::Color::White); // Ensure the background is white

        // Draw grid lines
        sf::RectangleShape line(sf::Vector2f(300, 5));
        line.setFillColor(sf::Color::Black);
        for (int i = 1; i < 3; i++) {
            // Vertical lines
            line.setPosition(i * 100, 0);
            line.setSize(sf::Vector2f(5, 300));
            window.draw(line);

            // Horizontal lines
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

    // Method to handle player move
    void makeMove(int row, int col) {
        if (row < 0 || row >= 3 || col < 0 || col >= 3) {
            std::cout << "Invalid move. Try again." << std::endl;
            return;
        }
        if (board[row][col] != ' ') {
            std::cout << "Position already occupied. Try again." << std::endl;
            return;
        }
        board[row][col] = currentPlayer;
        currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
    }

    // Method to check for a win
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

    // Minimax algorithm to evaluate best move for AI player
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

    // Method to play the game
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
int main() {
    TicTacToe game;
    game.playGame();
    return 0;
}
