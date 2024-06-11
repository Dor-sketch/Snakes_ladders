#include "game.h"
#include <cmath>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <random>
#include <unordered_set>
#include <utility>
#include <QGraphicsTextItem>

std::pair<int, int>
calculatePosition(const std::vector<int> &playerPos,
                  const std::vector<std::vector<int>> &notes) {
  std::pair<int, int> pos = {playerPos[0] / notes.size(),
                             playerPos[0] % notes.size()};

  if (pos.first % 2 == 1) {
    pos.second = notes.size() - pos.second - 1;
  }

  return pos;
}

bool isValidPosition(const std::pair<int, int> &pos,
                     const std::vector<std::vector<int>> &notes) {
  std::cout << "Position: " << pos.first << " " << pos.second << std::endl;
  return !(pos.first < 0 || pos.first >= notes.size() || pos.second < 0 ||
           pos.second >= notes[0].size());
}

/* Print the board with cell numbers - for debugging */
void printBoardWithNums(std::vector<std::vector<int>> board) {
  for (int i = 0; i < board.size(); i++) {
    for (int j = 0; j < board[0].size(); j++) {
      auto val = board[i][i % 2 == 1 ? j : board[0].size() - j - 1];
      std::cout << std::setw(3) << val << " ";
    }
    std::cout << std::endl;
  }
}
void Game::init() {
  std::cout << "Initializing player position and board" << std::endl;
  // Initialize the player position
  playerPos = {{1}, {1}};

  // Initialize the board
  board = generateBoard(10);
  std::cout << "Board initialized" << std::endl;
}


void Game::initWindow() {
  std::cout << "Initializing window" << std::endl;
  // Create the main window
  this->setWindowTitle("Snakes and Ladders");
  this->resize(600, 600);

  // Create the drawing area
  drawing_area = new QGraphicsView(this);
  drawing_area->setMinimumSize(600, 600);

  // Create the graphics scene
  scene = new QGraphicsScene(this);
  drawing_area->setScene(scene);

  // Create the roll button
  roll_button = new QPushButton("Roll", this);
  connect(roll_button, &QPushButton::clicked, this,
          &Game::on_roll_button_clicked);

  // Create the vertical box for layout
  vbox = new QVBoxLayout;
  vbox->addWidget(drawing_area);
  vbox->addWidget(roll_button);

  // Set the layout
  QWidget *container = new QWidget;
  container->setLayout(vbox);
  setCentralWidget(container);

  // Draw the initial board
  drawBoard();
}

// Implementation of constructor
Game::Game(QWidget *parent)
    : QMainWindow(parent),
      drawing_area(nullptr),
      roll_button(nullptr),
      vbox(nullptr),
      scene(nullptr),
      playerColors(
          {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}, {1, 1, 0}, {1, 0, 1}, {0, 1, 1}}),
      board(),
      playerPos() {
  std::cout << "Game constructor" << std::endl;
  init();
  initWindow();
}

std::vector<std::vector<int>> Game::generateBoard(int n) {
  std::cout << "Generating board" << std::endl;
  std::vector<std::vector<int>> board(n, std::vector<int>(n, -1));
  auto snakes = std::vector<std::pair<int, int>>{{16, 6}, {47, 26}, {49, 11},
                                                 {56, 53}, {62, 19}, {64, 60},
                                                 {87, 24}, {93, 73}, {95, 75},
                                                 {98, 78}};
  auto ladders = std::vector<std::pair<int, int>>{{1, 38}, {4, 14}, {9, 31},
                                                  {21, 42}, {28, 84}, {36, 44},
                                                  {51, 67}, {71, 91}, {80, 100}};
  for (auto &snake : snakes) {
    auto pos = getPos(snake.first);
    board[pos.first][pos.second] = snake.second;
  }
  for (auto &ladder : ladders) {
    auto pos = getPos(ladder.first);
    board[pos.first][pos.second] = ladder.second;
  }
  return board;
}
void Game::drawBoard() {
  std::cout << "Drawing board" << std::endl;
  int cellSize = 50;
  int playerSize = 20;

  scene->clear();

  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
    std::cout<< "hi" << std::endl;
      draw_gradient_cell(i, j, cellSize);
      draw_cell_numbers(i, j, cellSize, 10);
      draw_snakes_and_ladders(i, j, cellSize);
    }
  }

  std::cout << "Player positions size: " << playerPos.size() << std::endl;

  for (int i = 0; i < playerPos.size(); i++) {
    std::cout << "Drawing player " << i << " at position " << playerPos[i][0] << std::endl;
    draw_player(i, cellSize, playerSize);
  }
}

void Game::draw_player(int currentPlayer, int cellSize, int playerSize) {
  auto pos = getPos(playerPos[currentPlayer][0]);
  std::cout << "Player " << currentPlayer << " position: " << pos.first << ", " << pos.second << std::endl;

  int x = pos.first * cellSize + cellSize / 2;
  int y = pos.second * cellSize + cellSize / 2;

  QGraphicsEllipseItem *player = new QGraphicsEllipseItem(
      x - playerSize / 2, y - playerSize / 2, playerSize, playerSize);
  player->setBrush(QBrush(QColor(playerColors[currentPlayer][0] * 255,
                                 playerColors[currentPlayer][1] * 255,
                                 playerColors[currentPlayer][2] * 255)));
  scene->addItem(player);
}

void Game::draw_snakes_and_ladders(int i, int j, int cellSize) {
  std::cout << "Drawing snakes and ladders at [" << i << "][" << j << "]" << std::endl;
  if (board[i][j] != -1) {
    auto pos = getPos(board[i][j]);
    std::cout << "Board value: " << board[i][j] << ", Position: (" << pos.first << ", " << pos.second << ")" << std::endl;

    int x1 = i * cellSize + cellSize / 2;
    int y1 = j * cellSize + cellSize / 2;
    int x2 = pos.first * cellSize + cellSize / 2;
    int y2 = pos.second * cellSize + cellSize / 2;

    std::cout << "Drawing line from (" << x1 << ", " << y1 << ") to (" << x2 << ", " << y2 << ")" << std::endl;

    QGraphicsLineItem *line = new QGraphicsLineItem(x1, y1, x2, y2);
    line->setPen(QPen(Qt::black, 2));
    scene->addItem(line);
  }
}


void Game::draw_gradient_cell(int i, int j, int cellSize) {
  QLinearGradient gradient(0, 0, cellSize, cellSize);
  gradient.setColorAt(0, Qt::white);
  gradient.setColorAt(1, Qt::gray);
  QGraphicsRectItem *rect = new QGraphicsRectItem(i * cellSize, j * cellSize,
                                                  cellSize, cellSize);
  rect->setBrush(gradient);
  scene->addItem(rect);
}

void Game::draw_cell_numbers(int i, int j, int cellSize, int n) {
  int cell = i * n + j + 1;
  QString text = QString::number(cell);

  QGraphicsTextItem *cellText = new QGraphicsTextItem(text);
  cellText->setPos(i * cellSize + cellSize / 2, j * cellSize + cellSize / 2);
  scene->addItem(cellText);
}

void Game::on_roll_button_clicked() {
  std::cout << "Roll button clicked" << std::endl;
  // Roll the dice
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dis(1, 6);
  int roll = dis(gen);
  std::cout << "Rolled a " << roll << std::endl;

  // Move the player
  movePlayer(playerPos[0], roll);

  // Redraw the board
  drawBoard();
}

void Game::movePlayer(std::vector<int> &player, int roll) {
  player[0] += roll;

  if (player[0] >= 10 * 10) {
    player[0] = 10 * 10;
  } else {
    int row = 10 - (player[0] - 1) / 10 - 1;
    int col = (player[0] - 1) % 10;
    if ((10 - row) % 2 == 0)
      col = 10 - 1 - col;
    if (board[row][col] != -1)
      player[0] = board[row][col];
  }
  std::cout << "Moved player to position " << player[0] << std::endl;
}
std::pair<int, int> Game::getPos(int num) {
std::cout <<"bibibi" <<std::endl;
  int row = 10 - (num - 1) / 10 - 1;
  int col = (num - 1) % 10;
  if ((10 - row) % 2 == 0)
    col = 10 - 1 - col;
  std::cout << "getPos: num=" << num << ", row=" << row << ", col=" << col << std::endl;
  return {row, col};
}


Game::~Game() {
  std::cout << "Destroying game" << std::endl;
  delete drawing_area;
  delete roll_button;
  delete vbox;
  delete scene;
}
