#include "game.h"
#include <iostream>
#include <random>
#include <unordered_set>
#include <utility>
    #include <cmath>
    #include <ctime>
GtkWidget *Game::window;
GtkWidget *Game::drawing_area;
GtkWidget *Game::roll_button;
GtkWidget *Game::vbox;
std::vector<std::vector<int>> Game::board;
std::vector<std::vector<int>> Game::playerPos;
#include <iomanip>

static std::vector<std::vector<double>> playerColors = {
    {1, 0, 0}, // red
    {0, 1, 0}, // green
    {0, 0, 1}, // blue
               // Add more colors as needed
};
void printBoardWithNums(std::vector<std::vector<int>> board) {
  for (int i = 0; i < board.size(); i++) {
    for (int j = 0; j < board[0].size(); j++) {
      auto val = board[i][i % 2 == 1 ? j : board[0].size() - j - 1];
      std::cout << std::setw(3) << val << " ";
    }
    std::cout << std::endl;
  }
}
Game::Game() {
  // Initialize the player position

  playerPos = {{1}, {1}};

  // Initialize the board
  board = generateBoard(10);

  // Create the main window
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "Snakes and Ladders");
  gtk_window_set_default_size(GTK_WINDOW(window), 600, 600);
  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  // Create the drawing area
  drawing_area = gtk_drawing_area_new();
  gtk_widget_set_size_request(drawing_area, 600, 600);

  // Create the roll button
  roll_button = gtk_button_new_with_label("Roll");
  g_signal_connect(G_OBJECT(roll_button), "clicked",
                   G_CALLBACK(Game::on_roll_button_clicked), this);

  // Attach the drawing callback to the drawing area
  g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(Game::draw_cell),
                   this);

  // Create the vertical box for layout
  vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
  gtk_box_pack_start(GTK_BOX(vbox), drawing_area, TRUE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), roll_button, FALSE, FALSE, 0);
  gtk_container_add(GTK_CONTAINER(window), vbox);

  // Show all widgets
  gtk_widget_show_all(window);
}

std::vector<std::vector<int>> Game::generateBoard(int n) {
  std::vector<std::vector<int>> board(n, std::vector<int>(n, -1));
  std::unordered_set<int> used;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dis(
      2,
      n * n -
          1); // 1 and n*n should not be the start or end of a snake or ladder
  // Generate ladders and snakes randomly
  for (int i = 0; i < n; i++) {
    int start = dis(gen);
    int end = dis(gen);
    if (start == end || used.count(start) || used.count(end)) {
      i--; // Try again
      continue;
    }
    int startRow = n - (start - 1) / n - 1;
    int startCol = (start - 1) % n;
    if (startRow % 2 == 1) { // Reverse direction for every second row
      startCol = n - startCol - 1;
    }
    if (start < end) { // Ladder
      board[startRow][startCol] = end;
    } else { // Snake
      board[startRow][startCol] = start;
    }
    used.insert(start);
    used.insert(end);
  }
  return board;
}
void Game::movePlayer(std::vector<int> &playerPos, int roll) {
  // Move the player
  std::cout << "Roll: " << roll << std::endl;
  auto pos = getPos(playerPos[0]);
  auto nextPos = getPos(playerPos[0] + roll);
  std::cout << "Player position: " << playerPos[0] << std::endl;
  std::cout << "Next row: " << nextPos.first << " col: " << nextPos.second
            << std::endl;

  playerPos[0] += roll;
  if (playerPos[0] > board.size() * board[0].size()) {
    // If the player's position exceeds the board size, set it to the last cell
    playerPos[0] = board.size() * board[0].size();
    return;
  }
  if (nextPos.first < 0 || nextPos.first >= board.size() ||
      nextPos.second < 0 || nextPos.second >= board[0].size()) {
    // If the player's position exceeds the board size, set it to the last cell
    playerPos[0] = board.size() * board[0].size();
    return;
  }
  if (board[nextPos.first][nextPos.second] != -1) {
    std::cout << "Landed on a snake or ladder" << std::endl;
    std::cout << "Next cell: " << board[nextPos.first][nextPos.second]
              << std::endl;
    // If the player lands on a snake or a ladder, move to the new position
    playerPos[0] = board[nextPos.first][nextPos.second];
  }
  if (playerPos[0] > board.size() * board[0].size()) {
    // If the player's position exceeds the board size, set it to the last cell
    playerPos[0] = board.size() * board[0].size();
  }

  // Redraw the drawing area
  gtk_widget_queue_draw(drawing_area);
}
gboolean Game::draw_cell(GtkWidget *widget, cairo_t *cr, gpointer data) {
  Game *game = static_cast<Game *>(data);
  std::pair<int, int> pos = game->getPos(game->playerPos[0][0]);
  int n = game->board.size();
  int cellSize = 600 / n;
  int cellPadding = cellSize / 10;
  int playerSize = cellSize / 2;

  cairo_text_extents_t extents;
  // Draw the cells
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      // Draw cell with gradient for 3D effect
      cairo_pattern_t *pat = cairo_pattern_create_linear(
          j * cellSize, i * cellSize, (j + 1) * cellSize, (i + 1) * cellSize);


    // Get the current time
    std::time_t t = std::time(0);
    // Use the sin function to generate a value that oscillates over time
    double r = std::sin(2.0 * M_PI * (t % 60) / 60.0) / 2.0 + 0.5;
    double g = std::sin(2.0 * M_PI * ((t + 20) % 60) / 60.0) / 2.0 + 0.5;
    double b = std::sin(2.0 * M_PI * ((t + 40) % 60) / 60.0) / 2.0 + 0.5;

    // Use the generated values for the RGB colors
    cairo_pattern_add_color_stop_rgb(pat, 0, r, g, b);
    cairo_pattern_add_color_stop_rgb(pat, 1, r * 0.6, g * 0.6, b * 0.6);
      cairo_set_source(cr, pat);
      cairo_rectangle(cr, j * cellSize, i * cellSize, cellSize, cellSize);
      cairo_fill(cr);
      cairo_pattern_destroy(pat);

      // Draw cell numbers
      int cellNum = n * (n - i) - j;
      if (i % 2 == 1) { // Reverse direction for every second row
        cellNum = n * (n - i - 1) + j + 1;
      }
      std::string cellNumStr = std::to_string(cellNum);
      cairo_text_extents(cr, cellNumStr.c_str(), &extents);
      int x = j * cellSize + (cellSize - extents.width) / 2;
      int y = (i + 1) * cellSize - (cellSize - extents.height) / 2;
      cairo_move_to(cr, x, y);
      cairo_set_source_rgb(cr, 0, 0, 0); // black
      cairo_show_text(cr, cellNumStr.c_str());

      // Draw snakes and ladders
      if (game->board[i][j] != -1) {
        std::pair<int, int> nextPos = game->getPos(game->board[i][j]);
        cairo_set_line_width(cr, cellPadding / 2);
        if (game->board[i][j] > i * n + j + 1) { // snake
          cairo_set_source_rgb(cr, 1, 0, 1);     // red
          // Draw a Bezier curve
          cairo_move_to(cr, j * cellSize + cellPadding,
                        i * cellSize + cellPadding);
          double ctrl_x1 = j * cellSize + cellPadding;
          double ctrl_y1 =
              (i * cellSize + nextPos.first * cellSize) / 2 + cellPadding;
          double ctrl_x2 = nextPos.second * cellSize + cellPadding;
          double ctrl_y2 =
              (i * cellSize + nextPos.first * cellSize) / 2 + cellPadding;
          cairo_curve_to(cr, ctrl_x1, ctrl_y1, ctrl_x2, ctrl_y2,
                         nextPos.second * cellSize + cellPadding,
                         nextPos.first * cellSize + cellPadding);
          cairo_stroke(cr);
        } else {                             // ladder
          cairo_set_source_rgb(cr, 0, 1, 0); // green
          // Draw two parallel lines
          for (int k = -1; k <= 1; k += 2) {
            cairo_move_to(cr, (j + k * 0.1) * cellSize + cellPadding,
                          i * cellSize + cellPadding);
            cairo_line_to(cr,
                          (nextPos.second + k * 0.1) * cellSize + cellPadding,
                          nextPos.first * cellSize + cellPadding);
            cairo_stroke(cr);
          }
          // Draw rungs
          for (double l = 0.2; l < 1; l += 0.2) {
            cairo_move_to(
                cr,
                (j - 0.1 + l * (nextPos.second - j)) * cellSize + cellPadding,
                (i + l * (nextPos.first - i)) * cellSize + cellPadding);
            cairo_line_to(
                cr,
                (j + 0.1 + l * (nextPos.second - j)) * cellSize + cellPadding,
                (i + l * (nextPos.first - i)) * cellSize + cellPadding);
            cairo_stroke(cr);
          }
        }
      }
    }
  }

  for (int currentPlayer = 0; currentPlayer < game->playerPos.size();
       currentPlayer++) {
    pos = game->getPos(game->playerPos[currentPlayer][0]);
    // Draw the player with shadow for 3D effect
    cairo_set_source_rgb(cr, 0, 0, 0); // black
    cairo_rectangle(cr, pos.second * cellSize + (cellSize - playerSize) / 2 + 5,
                    pos.first * cellSize + (cellSize - playerSize) / 2 + 5,
                    playerSize, playerSize);
    cairo_fill(cr);
    cairo_set_source_rgb(cr, playerColors[currentPlayer][0],
                         playerColors[currentPlayer][1],
                         playerColors[currentPlayer][2]);
    cairo_rectangle(cr, pos.second * cellSize + (cellSize - playerSize) / 2,
                    pos.first * cellSize + (cellSize - playerSize) / 2,
                    playerSize, playerSize);
    cairo_fill(cr);
  }
  return FALSE;
}

void Game::reset() {
  playerPos = {{1}, {1}};
  board = generateBoard(10);
  gtk_widget_queue_draw(drawing_area);
}

void Game::on_roll_button_clicked(GtkWidget *button, gpointer user_data) {
  static int playerTurn = 0;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dis(1, 6);
  int roll = dis(gen);
  movePlayer(playerPos[playerTurn], roll);
  printBoardWithNums(board);
  if (playerPos[playerTurn][0] == board.size() * board[0].size()) {
    std::cout << "Player " << playerTurn + 1 << " wins!" << std::endl;
    reset();
  }
  playerTurn = (playerTurn + 1) % playerPos.size();
}

std::pair<int, int> Game::getPos(int num) {
  int n = board.size();
  int row = n - (num - 1) / n - 1;
  int col = (num - 1) % n;
  if ((n - row) % 2 == 0)
    col = n - 1 - col;
  return std::make_pair(row, col);
}



