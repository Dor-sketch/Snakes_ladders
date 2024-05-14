#include "parse_music.h"
#include <cstdlib>
#include <iostream>

// #include <QGuiApplication>
// #include <QScreen>
// #include <QPixmap>

// void takeScreenshot(const std::string& filename) {
//     QScreen *screen = QGuiApplication::primaryScreen();
//     if (const QWindow *window = QGuiApplication::focusWindow())
//         screen = window->screen();
//     if (!screen)
//         return;

//     QPixmap originalPixmap = screen->grabWindow(0);
//     originalPixmap.save(QString::fromStdString(filename), "PNG");
// }

int main() {
    // take screenshot of music sheet and process it
    std::cout<<"Taking screenshot of music sheet..."<<std::endl;
    system("screencapture -i screenshot.png");
    MusicParser parser("screenshot.png");
    parser.print();

    return 0;
}