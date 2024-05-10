#include <gtk/gtk.h>
#include <cairo.h>
#include <vector>
#include <random>
#include "game.h"
#include "tm.h"
#include <iostream>

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    if (argc > 1 && std::string(argv[1]) == "tm") {
        std::cout << "Turing Machine Game" << std::endl;
        TuringGame game;
    } else {
        Game game;
    }

    gtk_main();
    return 0;
}