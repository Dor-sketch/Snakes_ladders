#ifndef GAME_H
#define GAME_H

#include <QMainWindow>
#include <QPushButton>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QGraphicsScene>
#include <vector>
#include <QWidget>
#include <utility>

class Game : public QMainWindow {
    Q_OBJECT

public:
    explicit Game(QWidget *parent = nullptr);
    virtual ~Game();

private slots:
    void on_roll_button_clicked();

private:
    void init();
    void initWindow();
    std::vector<std::vector<int>> generateBoard(int n);
    void drawBoard();
    void draw_player(int currentPlayer, int cellSize, int playerSize);
    void draw_snakes_and_ladders(int i, int j, int cellSize);
    void draw_gradient_cell(int i, int j, int cellSize);
    void draw_cell_numbers(int i, int j, int cellSize, int n);
    void movePlayer(std::vector<int> &player, int roll);
    std::pair<int, int> getPos(int num);

    QGraphicsView *drawing_area;
    QGraphicsScene *scene;
    QPushButton *roll_button;
    QVBoxLayout *vbox;
    std::vector<std::vector<int>> board;
    std::vector<std::vector<int>> playerPos;
    std::vector<std::vector<int>> playerColors;
    int n = 10;
};

#endif // GAME_H
