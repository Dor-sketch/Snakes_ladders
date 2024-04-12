#include "game.h"
#include <random>
#include <iostream>
#include <unordered_set>
#include <utility>

GtkWidget *Game::window;
GtkWidget *Game::drawing_area;
GtkWidget *Game::roll_button;
GtkWidget *Game::vbox;
std::vector<std::vector<int>> Game::board;
std::vector<int> Game::playerPos;



Game::Game() {
    // Initialize the player position
    playerPos.push_back(1);

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
    g_signal_connect(G_OBJECT(roll_button), "clicked", G_CALLBACK(Game::on_roll_button_clicked), this);

    // Attach the drawing callback to the drawing area
    g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(Game::draw_cell), this);

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
    std::uniform_int_distribution<int> dis(2, n * n - 1); // 1 and n*n should not be the start or end of a snake or ladder
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
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            std::cout << board[i][j] << " ";
        }
        std::cout << std::endl;
    }
    return board;
}

void Game::movePlayer(int roll) {
    // Move the player
    playerPos[0] += roll;
    if (playerPos[0] > board.size() * board.size()) playerPos[0] = board.size() * board.size();

    // Redraw the drawing area
    gtk_widget_queue_draw(drawing_area);
}gboolean Game::draw_cell(GtkWidget* widget, cairo_t* cr, gpointer data) {
    Game* game = static_cast<Game*>(data);
    std::pair<int, int> pos = game->getPos(game->playerPos[0]);
    int n = game->board.size();
    int cellSize = 600 / n;
    int cellPadding = cellSize / 10;
    int playerSize = cellSize / 2;
    for (const auto &row : game->board) {
        for (int cell : row) {
            std::cout << cell << " ";
        }
        std::cout << std::endl;
    }
    cairo_text_extents_t extents;

    // Draw the cells
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            // Draw cell with gradient for 3D effect
            cairo_pattern_t* pat = cairo_pattern_create_linear(j * cellSize, i * cellSize, (j + 1) * cellSize, (i + 1) * cellSize);
            cairo_pattern_add_color_stop_rgb(pat, 0, 0.8, 0.8, 0.8); // light gray
            cairo_pattern_add_color_stop_rgb(pat, 1, 0.6, 0.6, 0.6); // dark gray
            cairo_set_source(cr, pat);
            cairo_rectangle(cr, j * cellSize, i * cellSize, cellSize, cellSize);
            cairo_fill(cr);
            cairo_pattern_destroy(pat);
            // Draw cell numbers
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    int cellNum = n * i + j + 1;
                    std::string cellNumStr = std::to_string(cellNum);
                    cairo_text_extents(cr, cellNumStr.c_str(), &extents);
                    int x = j * cellSize + (cellSize - extents.width) / 2;
                    int y = (i + 1) * cellSize - (cellSize - extents.height) / 2;
                    if (i % 2 == 1) { // Reverse direction for every second row
                        x = (n - j - 1) * cellSize + (cellSize - extents.width) / 2;
                    }
                    cairo_move_to(cr, x, y);
                    cairo_set_source_rgb(cr, 0, 0, 0); // black
                    cairo_show_text(cr, cellNumStr.c_str());
                }
            }

            // Draw snakes and ladders
            std::pair<int, int> endPos = game->getPos(game->board[i][j]);
            if (game->board[i][j] > n * i + j + 1 && endPos != std::make_pair(i, j)) { // Ladder
                cairo_pattern_t* pat = cairo_pattern_create_linear(j * cellSize + cellSize / 2, i * cellSize + cellSize / 2, endPos.second * cellSize + cellSize / 2, endPos.first * cellSize + cellSize / 2);
                cairo_pattern_add_color_stop_rgb(pat, 0, 0, 1, 0); // Green at the start
                cairo_pattern_add_color_stop_rgb(pat, 1, 0, 0.5, 0); // Darker green at the end
                cairo_set_source(cr, pat);
                cairo_move_to(cr, j * cellSize + cellSize / 2, i * cellSize + cellSize / 2);
                cairo_line_to(cr, endPos.second * cellSize + cellSize / 2, endPos.first * cellSize + cellSize / 2);
                cairo_stroke(cr);
                cairo_pattern_destroy(pat);
            } else if (game->board[i][j] < n * i + j + 1 && endPos != std::make_pair(i, j)) { // Snake
                cairo_pattern_t* pat = cairo_pattern_create_linear(j * cellSize + cellSize / 2, i * cellSize + cellSize / 2, endPos.second * cellSize + cellSize / 2, endPos.first * cellSize + cellSize / 2);
                cairo_pattern_add_color_stop_rgb(pat, 0, 1, 0, 0); // Red at the start
                cairo_pattern_add_color_stop_rgb(pat, 1, 0.5, 0, 0); // Darker red at the end
                cairo_set_source(cr, pat);
                cairo_move_to(cr, j * cellSize + cellSize / 2, i * cellSize + cellSize / 2);
                cairo_curve_to(cr, j * cellSize + cellSize / 2, endPos.first * cellSize, endPos.second * cellSize + cellSize / 2, endPos.first * cellSize, endPos.second * cellSize + cellSize / 2, endPos.first * cellSize + cellSize / 2);
                cairo_stroke(cr);
                cairo_pattern_destroy(pat);
            }
        }
    }

    // Draw the player with shadow for 3D effect
    cairo_set_source_rgb(cr, 0, 0, 0); // black
    cairo_rectangle(cr, pos.second * cellSize + (cellSize - playerSize) / 2 + 5, pos.first * cellSize + (cellSize - playerSize) / 2 + 5, playerSize, playerSize);
    cairo_fill(cr);
    cairo_set_source_rgb(cr, 1, 0, 0); // red
    cairo_rectangle(cr, pos.second * cellSize + (cellSize - playerSize) / 2, pos.first * cellSize + (cellSize - playerSize) / 2, playerSize, playerSize);
    cairo_fill(cr);

    return FALSE;
}
void Game::on_roll_button_clicked(GtkWidget *button, gpointer user_data) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(1, 6);
    int roll = dis(gen);
    movePlayer(roll);
}

std::pair<int, int> Game::getPos(int num) {
    int n = board.size();
    int row = n - (num - 1) / n - 1;
    int col = (num - 1) % n;
    if ((n - row) % 2 == 0) col = n - 1 - col;
    return std::make_pair(row, col);
}
