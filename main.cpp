#include <gtk/gtk.h>
#include <cairo.h>
#include <vector>
#include <random>
#include "game.h"

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    Game game;
    gtk_main();
    return 0;
}