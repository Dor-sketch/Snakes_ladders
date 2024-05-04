#pragma once

# include <gtk/gtk.h>
# include <cairo.h>
# include <vector>
#include <random>


class Game{
    public:
        Game();
        static void movePlayer(std::vector<int>& player, int roll); // Add player parameter
        static std::vector<std::vector<int>> generateBoard(int n);
        static void on_roll_button_clicked(GtkWidget *button, gpointer user_data);
        static gboolean draw_cell(GtkWidget *widget, cairo_t *cr, gpointer user_data);
        static std::pair<int, int> getPos(int num);
        static std::vector<std::vector<int>> board;
        static void reset();


        bool on_draw(cairo_t* cr);
        static GtkWidget *window;
        static GtkWidget *drawing_area;
        static GtkWidget *roll_button;
        static GtkWidget *vbox;
        int n = 10;
        int cellSize = 60;
        int cellPadding = 5;
        int playerSize = 20;
        static std::vector<std::vector<int>> playerPos; // Change to 2D vector
};