#include "game.h"
#include <iostream>
#include <random>
#include <QGraphicsTextItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsItemAnimation>
#include <QTimeLine>
#include <QBrush>
#include <QColor>
#include <QLinearGradient>
#include <QPen>
#include <iomanip>
#include <QtConcurrent/QtConcurrent>
#include <QFuture>
#include <QGraphicsDropShadowEffect>
std::pair<int, int> calculatePosition(const std::vector<int> &playerPos,
                                      const std::vector<std::vector<int>> &notes) {
  if (playerPos.empty() || notes.empty()) {
    return {-1, -1}; // Invalid position
  }
  int posIndex = playerPos[0];
  std::pair<int, int> pos = {posIndex / notes.size(), posIndex % notes.size()};

  if (pos.first % 2 == 1) {
    pos.second = notes.size() - pos.second - 1;
  }

  return pos;
}

bool isValidPosition(const std::pair<int, int> &pos,
                     const std::vector<std::vector<int>> &notes) {
  return !(pos.first < 0 || pos.first >= notes.size() || pos.second < 0 ||
           pos.second >= notes[0].size());
}

void printBoardWithNums(const std::vector<std::vector<int>> &board) {
  for (int i = 0; i < board.size(); i++) {
    for (int j = 0; j < board[0].size(); j++) {
      auto val = board[i][i % 2 == 1 ? j : board[0].size() - j - 1];
      std::cout << std::setw(3) << val << " ";
    }
    std::cout << std::endl;
  }
}


void Game::initWindow() {
  std::cout << "Initializing window" << std::endl;
  // Create the main window
  this->setWindowTitle("Snakes and Ladders");
  this->resize(1000, 1000);

  // Create the drawing area
  drawing_area = new QGraphicsView(this);
  drawing_area->setMinimumSize(1000, 1000);

  // Create the graphics scene
  scene = new QGraphicsScene(this);
  drawing_area->setScene(scene);

  // Create the roll button
  roll_button = new QPushButton("Roll", this);
  // Style the button to look like a cube
// Style the button to look like a cube
// Style the button to look like a cube
// Style the button to look like a cube
roll_button->setStyleSheet(
    "QPushButton {"
    "background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #f6f7fa, stop: 1 #dadbdf);"
    "border: 2px solid #8f8f91;"
    "border-radius: 15px;"
    "padding: 6px;"
    "font: bold 14px;"
    "color: #333;"
    "min-width: 80px;"
    "min-height: 30px;"
    "box-shadow: 2px 2px 5px rgba(0, 0, 0, 0.2);"
    "}"
    "QPushButton:hover {"
    "background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #e6e7ea, stop: 1 #c5c6ca);"
    "}"
    "QPushButton:pressed {"
    "background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1, stop: 0 #dadbdf, stop: 1 #f6f7fa);"
    "border-style: inset;"
    "box-shadow: inset 2px 2px 5px rgba(0, 0, 0, 0.3);"
    "}"
    "QPushButton:disabled {"
    "background-color: #efefef;"
    "color: #a1a1a1;"
    "border: 2px solid #d3d3d3;"
    "}"
);



// Connect the button click to the roll function
connect(roll_button, &QPushButton::clicked, this, [this]() {
    int roll_result = this->on_roll_button_clicked();
    roll_button->setText("Roll result: " + QString::number(roll_result));
});

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
  int cellSize =100;
  int playerSize = 20;

  scene->clear();

  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      draw_gradient_cell(i, j, cellSize);
      draw_cell_numbers(i, j, cellSize, 10);
      draw_snakes_and_ladders(i, j, cellSize);
    }
  }

  std::cout << "Player positions size: " << playerPos.size() << std::endl;
}

QPointF lastPos; // Add this member variable to track the last position

void Game::init() {
    std::cout << "Initializing player position and board" << std::endl;
    playerPos = {1}; // Initialize the player position properly
    lastPos = QPointF(25, 465); // Initialize the lastPos
    board = generateBoard(10);
    std::cout << "Board initialized" << std::endl;


}

void Game::draw_player(int currentPlayer, int cellSize, int playerSize) {
    std::cout << "Drawing player " << currentPlayer << std::endl;

    // Calculate the target position based on movement logic
    QPointF targetPos = getTargetPos(currentPlayer, cellSize, playerSize);

    // Extend the target position in the direction of movement
    QPointF direction = targetPos - lastPos;
    QPointF extendedTargetPos = targetPos + direction;

    // Calculate the number of steps to move
    int steps = qAbs(extendedTargetPos.x() - lastPos.x()) + qAbs(extendedTargetPos.y() - lastPos.y());

    // Create a time line for animation
    QTimeLine *timeLine = new QTimeLine(2000); // Duration of the animation in milliseconds
    timeLine->setFrameRange(0, 100);
    // Create the player ellipse item at the initial position
    QGraphicsEllipseItem *player = new QGraphicsEllipseItem(lastPos.x(), lastPos.y(), playerSize, playerSize);

    // Create a linear gradient for the fill
    QLinearGradient fillGradient(0, 0, playerSize, playerSize);
    fillGradient.setColorAt(0, QColor(playerColors[currentPlayer][0] * 255, playerColors[currentPlayer][1] * 255, playerColors[currentPlayer][2] * 255));
    fillGradient.setColorAt(0.5, Qt::white);
    fillGradient.setColorAt(1, QColor(playerColors[currentPlayer][0] * 255, playerColors[currentPlayer][1] * 255, playerColors[currentPlayer][2] * 255));

    // Set the brush with the fill gradient
    player->setBrush(QBrush(fillGradient));

    // Create a linear gradient for the border
    QLinearGradient borderGradient(0, 0, playerSize, playerSize);
    borderGradient.setColorAt(0, Qt::black);
    borderGradient.setColorAt(0.5, Qt::white);
    borderGradient.setColorAt(1, Qt::black);

    // Set a border for the player
    QPen pen(QBrush(borderGradient), 3); // Use the border gradient for the pen
    pen.setJoinStyle(Qt::RoundJoin); // Set the JoinStyle to RoundJoin
    player->setPen(pen);

    // Create a drop shadow effect
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect;
    shadow->setBlurRadius(15); // Increase the blur radius to 15
    shadow->setOffset(7, 7); // Increase the offset to 7, 7

    // Apply the shadow effect to the player
    player->setGraphicsEffect(shadow);
    scene->addItem(player);

    // Create the animation and set the player item and time line
    QGraphicsItemAnimation *animation = new QGraphicsItemAnimation;
    animation->setItem(player);
    animation->setTimeLine(timeLine);

    // Move the player step by step
    for (int i = 0; i <= 100; ++i) {
        // Calculate the intermediate position
        QPointF intermediatePos = lastPos + (extendedTargetPos - lastPos) * (i / 100.0);

        // Set the animation to move the player to the next position
        animation->setPosAt(i / 100.0, intermediatePos);

        std::cout << "IntermediatePos: " << intermediatePos.x() << " " << intermediatePos.y() << std::endl;

        // Calculate the board indices
        int boardRow = static_cast<int>(intermediatePos.y() / cellSize);
        int boardCol = static_cast<int>(intermediatePos.x() / cellSize);

        // Optionally handle board indices if needed
    }

    // Start the animation
    timeLine->start();

    // Update the last position after the animation
    lastPos = targetPos;
}


QPointF Game::getTargetPos(int currentPlayer, int cellSize, int playerSize) {
    auto pos = getPos(playerPos[currentPlayer]);
    int x = pos.second * cellSize + (cellSize - playerSize) / 2;
    int y = pos.first * cellSize + (cellSize - playerSize) / 2;
    return QPointF(x, y);
}

void Game::movePlayer(int playerIndex, int roll) {
    std::cout << "Moving player " << playerIndex << " by " << roll << " steps" << std::endl;

    playerPos[playerIndex] += roll;
    draw_player(playerIndex, 50, 20);
    if (playerPos[playerIndex] >= 100) {
        std::cout << "Player " << playerIndex << " has won!" << std::endl;
        playerPos[playerIndex] = 1;
    } else {
        int row = 10 - (playerPos[playerIndex] - 1) / 10 - 1;
        int col = (playerPos[playerIndex] - 1) % 10;
        if ((10 - row) % 2 == 0)
            col = 10 - 1 - col;
        if (board[row][col] != -1) {
            playerPos[playerIndex] = board[row][col];
            draw_player(playerIndex, 50, 20);
        }
    }
    std::cout << "Moved player to position " << playerPos[playerIndex] << std::endl;

}
void Game::draw_snakes_and_ladders(int i, int j, int cellSize) {
  if (board[i][j] != -1) {
    auto pos = getPos(board[i][j]);

    int x1 = j * cellSize + cellSize / 2;
    int y1 = i * cellSize + cellSize / 2;
    int x2 = pos.second * cellSize + cellSize / 2;
    int y2 = pos.first * cellSize + cellSize / 2;

    QLineF lineF(QPointF(x1, y1), QPointF(x2, y2));
    QGraphicsLineItem *line = new QGraphicsLineItem(lineF);

    QPen pen;
    pen.setWidth(5); // Increase the line width
    pen.setCapStyle(Qt::RoundCap); // Round the ends of the lines
    pen.setJoinStyle(Qt::RoundJoin); // Round the corners of the lines

    // Use different colors for snakes and ladders
    if (pos.first >= i) {
      pen.setColor(Qt::red); // Snake
    } else {
      pen.setColor(Qt::green); // Ladder
    }

    line->setPen(pen);
    line->setZValue(1);
    scene->addItem(line);

    // // Create a text item and set its font and color
    // QGraphicsTextItem *text = new QGraphicsTextItem(pos.first < i ? "S" : "L");
    // QFont font("Arial", cellSize / 4); // Adjust the font size based on the cell size
    // font.setBold(true); // Make the font bold
    // text->setFont(font);
    // text->setDefaultTextColor(Qt::white); // Set the text color to white

    // text->setPos((x1 + x2) / 2, (y1 + y2) / 2);
    // scene->addItem(text);
  }
}
void Game::draw_gradient_cell(int row, int col, int cellSize) {
  // Calculate cell boundaries
  int left = col * cellSize;
  int top = row * cellSize;
  int right = (col + 1) * cellSize;
  int bottom = (row + 1) * cellSize;

  // Create a radial gradient within the cell for a 3D effect
  QRadialGradient gradient((left + right) / 2, (top + bottom) / 2, cellSize);
  gradient.setColorAt(0, QColor(135, 206, 235)); // SkyBlue color at the center
  gradient.setColorAt(1, QColor(70, 130, 180)); // SteelBlue color at the edges

  // Create a rectangle item representing the cell
  QGraphicsRectItem *cellRect = new QGraphicsRectItem(left, top, cellSize, cellSize);
  cellRect->setBrush(gradient);

  // Create a drop shadow effect
  QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect;
  shadow->setOffset(2); // Smaller offset for a closer shadow
  shadow->setBlurRadius(10); // Smaller blur radius for a sharper shadow
  shadow->setColor(QColor(0, 0, 0, 80)); // Semi-transparent black color for the shadow

  // Apply the drop shadow effect to the cell
  cellRect->setGraphicsEffect(shadow);

  // Add the cell to the scene
  scene->addItem(cellRect);
}void Game::draw_cell_numbers(int i, int j, int cellSize, int n) {
    // Calculate the cell number based on the row and column
    int cell;
    if ((n - i - 1) % 2 == 0) {
        // For even rows, numbers go from left to right
        cell = (n - i - 1) * n + j + 1;
    } else {
        // For odd rows, numbers go from right to left
        cell = (n - i) * n - j;
    }

    QString text = QString::number(cell);

    // Create a text item and set its font and color
    QGraphicsTextItem *cellText = new QGraphicsTextItem(text);
    QFont font("Arial", cellSize / 4); // Adjust the font size based on the cell size
    font.setBold(true); // Make the font bold
    cellText->setFont(font);
    cellText->setDefaultTextColor(Qt::white); // Set the text color to white

    // Center the text within the cell
    cellText->setPos(j * cellSize + cellSize / 2 - cellText->boundingRect().width() / 2,
                     i * cellSize + cellSize / 2 - cellText->boundingRect().height() / 2);

    scene->addItem(cellText);
}

int Game::on_roll_button_clicked() {
  std::cout << "Roll button clicked" << std::endl;

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dis(1, 6);
  int roll = dis(gen);
  std::cout << "Rolled a " << roll << std::endl;

  drawBoard();
  movePlayer(0, roll);
  return roll;
}

std::pair<int, int> Game::getPos(int num) {
  int row = 10 - (num - 1) / 10 - 1;
  int col = (num - 1) % 10;
  if ((10 - row) % 2 == 0)
    col = 10 - 1 - col;
  return {row, col};
}

Game::~Game() {
  std::cout << "Destroying game" << std::endl;
  delete drawing_area;
  delete roll_button;
  delete vbox;
  delete scene;
}
