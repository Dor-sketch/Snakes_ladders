#pragma once

# include <gtk/gtk.h>
# include <cairo.h>
# include <vector>
#include <random>


class Game{
    public:
        Game();
        static void movePlayer(int roll);
        static std::vector<std::vector<int>> generateBoard(int n);
        static void on_roll_button_clicked(GtkWidget *button, gpointer user_data);
        static gboolean draw_cell(GtkWidget *widget, cairo_t *cr, gpointer user_data);
        static std::pair<int, int> getPos(int num);
        static std::vector<std::vector<int>> board;

        bool on_draw(cairo_t* cr);
        static GtkWidget *window;
        static GtkWidget *drawing_area;
        static GtkWidget *roll_button;
        static GtkWidget *vbox;
        int n = 10;
        int cellSize = 60;
        int cellPadding = 5;
        int playerSize = 20;
        static std::vector<int> playerPos;
};