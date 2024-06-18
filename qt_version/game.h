#ifndef GAME_H
#define GAME_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <vector>
#include <utility>

class Game : public QMainWindow {
  Q_OBJECT

public:
  explicit Game(QWidget *parent = nullptr);
  ~Game();

  void init();
  void initWindow();
  std::vector<std::vector<int>> generateBoard(int n);
  void drawBoard();
  void draw_player(int currentPlayer, int cellSize, int playerSize);
  void draw_snakes_and_ladders(int i, int j, int cellSize);
  void draw_gradient_cell(int i, int j, int cellSize);
  void draw_cell_numbers(int i, int j, int cellSize, int n);
  void movePlayer(int playerIndex, int roll);
  std::pair<int, int> getPos(int num);
 QPointF getTargetPos(int, int, int);

private slots:
  int on_roll_button_clicked();

private:
  QGraphicsView *drawing_area;
  QPushButton *roll_button;
  QVBoxLayout *vbox;
  QGraphicsScene *scene;
  std::vector<std::vector<int>> board;
  std::vector<int> playerPos;
  std::vector<std::vector<int>> playerColors;
};

#endif // GAME_H
