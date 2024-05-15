#pragma once

#include <cairo.h>
#include <gtk/gtk.h>
#include <random>
#include <vector>

class Game {
public:
  Game();
  static void movePlayer(std::vector<int> &player,
                         int roll); // Add player parameter
  static void reset();
  static void on_roll_button_clicked(GtkWidget *button, gpointer user_data);
  static std::pair<int, int> getPos(int num);
protected:
  static std::vector<std::vector<int>> board;
  static std::vector<std::vector<int>> playerPos;
  int n = 10;

private:
static void init();
     void initWindow();

  bool on_draw(cairo_t *cr);
  static std::vector<std::vector<int>> notes;
  static std::vector<std::vector<int>> generateBoard(int n);
  static std::vector<std::vector<int>> generateNotes(int n);
  #ifdef USE_MIDIFILE
  static std::vector<std::vector<int>> generateNotes(const std::string
  &midi_file_path);
  #endif
  static gboolean draw_cell(GtkWidget *widget, cairo_t *cr, gpointer user_data);
  static void drawBoard(cairo_t *cr);
  gboolean draw_player(cairo_t *cr, int currentPlayer, int cellSize,
                       int playerSize);
  gboolean draw_snakes_and_ladders(cairo_t *cr, int i, int j, int cellSize,
                                   int cellPadding);
  gboolean draw_gradient_cell(cairo_t *cr, int i, int j, int cellSize);
  gboolean draw_cell_numbers(cairo_t *cr, int i, int j, int cellSize, int n);

  static GtkWidget *window;
  static GtkWidget *drawing_area;
  static GtkWidget *roll_button;
  static GtkWidget *vbox;
  static std::vector<std::vector<double>> playerColors;
  int cellSize = 60;
  int cellPadding = 5;
  int playerSize = 20;
};
