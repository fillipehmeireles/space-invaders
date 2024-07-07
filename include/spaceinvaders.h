
#ifndef spaceinvaders__h
#define spaceinvaders__h

#include "raylib.h"
#include <stdbool.h>

#define BORDERS_SIZE 10
#define SHOT_COOLDOWN 0.5f
#define BULLET_SPEED 6.f
#define MAX_BULLETS 5
#define CALIBER 4
#define PLAYER_CALIBER 8
#define MAX_ENEMIES 25
#define MAX_ENEMIES_ROWS 5
#define MAX_ENEMIES_PER_ROW 5
typedef struct Position
{
    float x;
    float y;
} Position;

typedef struct SpaceCraft
{
    Color color;
    int lifes;
    Rectangle rec;
    int step;
} SpaceCraft;
void drawPlayerSpaceCraft(SpaceCraft);
void movePlayer(SpaceCraft*, KeyboardKey);
void initEnemies(SpaceCraft[MAX_ENEMIES_ROWS][MAX_ENEMIES_PER_ROW]);
void drawEnemies(SpaceCraft[MAX_ENEMIES_ROWS][MAX_ENEMIES_PER_ROW]);
void moveEnemies(SpaceCraft[MAX_ENEMIES_ROWS][MAX_ENEMIES_PER_ROW]);
typedef struct Bullet
{
    Rectangle rec;
    Color color;
    bool shotted;
} Bullet;
void initBullets(Bullet[]);
void shootBullet(Bullet[], Position);
void updateBullets(Bullet[]);
void drawBullet(Bullet[]);


void checkEnemyShooted(SpaceCraft[MAX_ENEMIES_ROWS][MAX_ENEMIES_PER_ROW], Bullet[MAX_BULLETS]);

typedef struct GameScreen 
{
    int width;
    int height;
} GameScreen;
int start(GameScreen);

#endif
