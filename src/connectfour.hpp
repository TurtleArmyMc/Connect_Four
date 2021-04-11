#pragma once

class ConnectFour {
  private:
    short board[7][6] = {{0}}; // board[col][row]. Top left is [0][0]
    short player = 1;
    bool gameComplete = false;
    bool draw = false;

    short addChip(short c);
    bool columnFull(short c);

    bool checkDraw();
    bool checkWin(short c, short r);
    bool checkWinVertical(short c);
    bool checkWinHorizontal(short r);
    bool checkWinDiagonal(short c, short r);

  public:
    ConnectFour();
    bool validColumn(short c);
    bool makeMove(short c);
    short getPlayer();
    bool getGameComplete();
    bool getDraw();
    short getChip(short c, short r);
};