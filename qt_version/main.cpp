#include <QApplication>
#include "game.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    Game game(nullptr); // Pass nullptr as the parent widget
    game.show();

    return app.exec();
}
