#include "../include/raylib.h"
#include "../include/raymath.h"
#include <iostream>
#include <string>

using namespace std;


const int screenWidth = 800;
const int screenHeight = 900;

int textX = screenHeight / 2;
int textAdd = 2;


bool isCollide( int colliderX, int objectX ) {
  if ( objectX >= colliderX and objectX <= colliderX ) return true;
  return false;

}


class Player1 {
public:

  int score = 1;

  int width = 15;
  int height = 100;

  int posX = 20;
  int posY = 100;

  void Update() {
    Draw();
    Controls();
  }

  void Draw() {
    DrawRectangle(posX, posY, width, height, WHITE);
  }

  void Controls() {
    
    if ( IsKeyDown(KEY_W) and posY > 100) {
      posY -= 10;
    }
    if ( IsKeyDown(KEY_S) and posY + height < screenHeight) {
      posY += 10;
    }
  
  }

};

class Player2 {
public:
  
  int score = 1;

  int width = 15;
  int height = 100;

  int posX = screenWidth - 35;
  int posY = 101;

  void Update() {
    Draw();
    Controls();
  }

  void Draw() {
    DrawRectangle(posX, posY, width, height, WHITE);
  }

  void Controls() {
    
    if ( IsKeyDown(KEY_UP) and posY > 100) {
      posY -= 10;
    }
    if ( IsKeyDown(KEY_DOWN) and posY + height < screenHeight) {
      posY += 10;
    }
  
  }

};


int randomGen() {
  srand((unsigned) time(NULL));

  int randd = (rand() % 701) + 100;

  return randd;
}

class Ball {
public:

  int radius = 16;
  

  int posX = screenWidth / 2 + radius / 2;
  int posY = randomGen();

  int factorX = -4;
  int factorY = 4;

  bool isEnd = false;

  int p1Score = 0;
  int p2Score = 0;

  Sound CollideSound;
  Sound LoseSound;

  Ball() {
    InitAudioDevice();
    LoseSound = LoadSound("../explosion.wav");
    CollideSound = LoadSound("../hitHurt.wav");
  }

  ~Ball() {
    UnloadSound(CollideSound);
    UnloadSound(LoseSound);
    CloseAudioDevice();
  }

  void Update(Player1 player1, Player2 player2) {
    Draw();
    Physics(player1, player2);
  }

  void Draw() {
    DrawCircle(posX, posY, radius, Color{229, 60, 53, 255});
  }

  void Physics(Player1 player1, Player2 player2) {
    if ( posX >= 46 and posX <= 50) { 
     if ( posY >= player1.posY - 20 and posY <= player1.posY + player1.height + 20) {
        factorX *= -1; 
        PlaySound(CollideSound);
      }
    }
    cout << posX <<endl;
    if ( posX + 8 == 764 ) {
     if ( posY >= player2.posY - 20 and posY <= player2.posY + player2.height + 20) {
        factorX *= -1; 
        PlaySound(CollideSound);
      }
    }



    if ( posY <= 100 ) {
      factorY *= -1;
      PlaySound(CollideSound);
    }

    if ( posY + radius >= screenHeight or posY == radius ) {
      factorY *= -1;
      PlaySound(CollideSound);
    }
    if ( posX < 0 ) {
      p2Score++;
      isEnd = true;
      PlaySound(LoseSound);
    } 
    
      
    if ( posX > screenWidth ) {
      p1Score++;
      isEnd = true;
      PlaySound(LoseSound);
    }


    posX += factorX;
    posY += factorY;
  }

};

class Game {
public:

  Player1 player1 = Player1();
  Player2 player2 = Player2();
  Ball ball = Ball();

  bool isStart = false;
  
  Color Menu = Color{255, 255, 255, 25};


  void Update() {
    TopMenu();
    if ( isStart ) {
      player1.Update();
      player2.Update();
      ball.Update(player1, player2);

      if ( ball.isEnd ) {
        isStart = false;
        Reset();
      }
    }

    else {
      player1.Draw();
      player2.Draw();
      ball.Draw();
      
      char text[] = "Press Space To Start!";

      textX += textAdd;

      if ( textX >= screenHeight / 2 + 50 or textX <= screenHeight / 2 - 50 ) {
        textAdd *= -1;
      }
 
      DrawRectangle(0, 0, screenWidth, screenHeight, Menu);
      DrawText(text, screenWidth / 2 - MeasureText(text, 30) / 2, textX, 30, WHITE);

    
      if (IsKeyPressed(KEY_SPACE)) isStart = true;

    } 

  }

  void TopMenu() {
    DrawRectangle(0, 70, screenWidth, 10, WHITE); 
    DrawText("Ping Pong", screenWidth / 2 - MeasureText("Ping Pong", 30) / 2, 20, 30, WHITE);
    DrawText(TextFormat("%d", ball.p1Score), 20, 20, 40, WHITE);
    DrawText(TextFormat("%d", ball.p2Score), screenWidth - 35, 20, 40, WHITE);

  }

  void Reset() {
    ball.posX = screenWidth / 2 + ball.radius / 2;
    ball.posY = randomGen();
    ball.isEnd = false;

    player1.posX = 20;
    player1.posY = 101;

    player2.posX = screenWidth - 35;
    player2.posY = 101;

  }

};

int main() {


  Game game = Game();

  InitWindow(screenWidth, screenHeight, "Ping Pong");

  SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

  while (!WindowShouldClose()){
    

    BeginDrawing();

    ClearBackground(Color{30, 30, 46, 255});
  
    game.Update();

    EndDrawing();
  }

    CloseWindow();        // Close window and OpenGL context

    return 0;
}
