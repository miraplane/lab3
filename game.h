struct Point {
    int x;
    int y;
};

enum Direction {
    UP_RIGHT,
    UP_LEFT,
    DOWN_RIGHT,
    DOWN_LEFT,
    RIGHT,
    LEFT
};

enum PaddleDirection {
  UP,
  DOWN
};

const int paddleSize = 3;

class Paddle {
  public:
    Paddle(int maxBoardX, int maxBoardY, int positionX) {
      int positionY = maxBoardY / 2;
      
      position[0].x = positionX;
      position[0].y = positionY - 1;

      position[1].x = positionX;
      position[1].y = positionY;

      position[2].x = positionX;
      position[2].y = positionY + 1;

      maxX = maxBoardX;
      maxY = maxBoardY;
    }

    Point getPosition(int index) {
      return position[index];
    }
    
    int getMaxX() {
      return maxX;
    }
    
    int getMaxY() {
      return maxY;
    }

    void move(PaddleDirection direction) {
      int shift = 0;
      switch(direction) {
        case UP:
          if (position[0].y != 0) {
            shift -= 1;
          }
          break;
        case DOWN:
          if (position[paddleSize - 1].y != maxY - 1) {
            shift += 1;
          }
          break;
      }
    }

    bool containsPoint(Point &point) {
      for (int i = 0; i < paddleSize; i++) {
        if (point.x == position[i].x && point.y == position[i].y) {
          return true;
        }
      }
      return false;
    }
  
  private:
    Point position[paddleSize];
    int maxX;
    int maxY;
  
};

class Ball {
  public:
    Ball(int maxBoardX, int maxBoardY) {
      pos.x = 0;
      pos.y = maxBoardY / 2;

      direction = RIGHT;
      maxX = maxBoardX;
      maxY = maxBoardY;
    }

    Point getPosition() {
      return pos;
    }
    
    int getMaxX() {
      return maxX;
    }
    
    int getMaxY() {
      return maxY;
    }

    Direction getDirection() {
      return direction;
    }

    void update() {
      if (pos.y = 0) {
        if (direction == UP_RIGHT) {
          direction = DOWN_RIGHT;
        }
        else {
          direction = DOWN_LEFT;
        }
      }

      if (pos.y = maxY - 1) {
        if (direction == DOWN_RIGHT) {
          direction = UP_RIGHT;
        }
        else {
          direction = UP_LEFT;
        }
      }
      
      switch(direction) {
        case UP_RIGHT:
          pos.y -= 1;
          pos.x += 1;
          break;
        case UP_LEFT:
          pos.y -= 1;
          pos.x -= 1;
          break;  
        case DOWN_RIGHT:
          pos.y += 1;
          pos.x += 1;
          break;
        case DOWN_LEFT:
          pos.y += 1;
          pos.x -= 1;
          break;
        case RIGHT:
          pos.x += 1;
          break;
        case LEFT:
          pos.x -= 1;
          break;
      }
    }

      bool flewOut() {
        if(pos.x <= 0 || pos.x >= maxX - 1) {
          return true;
        }
        return false;
      }

      void contact(Paddle &paddle) {
        int paddleSegment = 0;
        if (pos.y == paddle.getPosition(1).y) {
          paddleSegment = 1;
        }
        if (pos.y == paddle.getPosition(2).y) {
          paddleSegment = 2;
        }
        
        if (pos.x + 1 == paddle.getPosition(0).x) {
          switch(paddleSegment) {
            case 0:
              direction = UP_LEFT;
              break;
            case 1:
              if (direction == RIGHT) {
                direction = LEFT;
              }
              if (direction == UP_RIGHT) {
                direction = UP_LEFT;
              }
              if (direction == DOWN_RIGHT) {
                direction = DOWN_LEFT;
              }
              break;
            case 2:
              direction = DOWN_LEFT;
              break;
          }
        }

        if (pos.x - 1 == paddle.getPosition(0).x) {
          switch(paddleSegment) {
            case 0:
              direction = UP_RIGHT;
              break;
            case 1:
              if (direction == LEFT) {
                direction = RIGHT;
              }
              if (direction == UP_LEFT) {
                direction = UP_RIGHT;
              }
              if (direction == DOWN_LEFT) {
                direction = DOWN_RIGHT;
              }
              break;
            case 2:
              direction = DOWN_RIGHT;
              break;
          }
        }
      }

   private:
    Point pos;
    Direction direction;
    int maxX;
    int maxY;
};

enum GameState {
    RUNNING,
    FINISHED
};

class Game {
  public:
    Game(int maxBoardX, int maxBoardY):
    ball(maxBoardX, maxBoardY),
    paddle1(maxBoardX, maxBoardY, 0),
    paddle2(maxBoardX, maxBoardY, maxBoardX) {
      maxX = maxBoardX;
      maxY = maxBoardY;
      state = RUNNING;
      winner = -1;
    }

  void update() {
    if (isRunning()) {
      ball.contact(paddle1);
      ball.contact(paddle2);     
      ball.update();
      if (ball.flewOut()) {
        Point ballPos = ball.getPosition();
        if (ballPos.x <= 0) {
          winner = 1;
        }
        else {
          winner = 0;
        }
        state = FINISHED;
        return;
      }
    }
  }

  bool isRunning(){
    return state == RUNNING;
  }

  int getWinner(){
    return winner;
  }

  Ball &getBall(){
    return ball;
  }

  Paddle &getPaddle(int paddleIndex){
    if (paddleIndex == 0) {
      return paddle1;
    }
    return paddle2;
  }

  void paddleMove(PaddleDirection direction, int paddleIndex){
    if (paddleIndex == 0) {
      paddle1.move(direction);
    }
    else {
      paddle2.move(direction);
    }
  }
  
  private:  
    Ball ball;
    Paddle paddle1;
    Paddle paddle2;
    GameState state;
    int winner;
    int maxX;
    int maxY;
  
};
