#include "raylib.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "spaceinvaders.h"

#define BORDERS_SIZE 10
#define SHOT_COOLDOWN 0.5f
#define BULLET_SPEED 6.f
#define MAX_BULLETS 5
#define CALIBER 4
#define PLAYER_CALIBER 8
#define PLAYER_MAX_LIFE 3
#define MAX_ENEMIES 25
#define MAX_ENEMIES_ROWS 5
#define MAX_ENEMIES_PER_ROW 5
#define MAX_ENEMIES_BULLETS MAX_BULLETS *(MAX_ENEMIES_PER_ROW * MAX_ENEMIES_PER_ROW)

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
    Texture2D sprite;
} SpaceCraft;
void drawPlayerSpaceCraft();
void movePlayer(SpaceCraft *, KeyboardKey);

SpaceCraft enemies[MAX_ENEMIES_ROWS][MAX_ENEMIES_PER_ROW];
void initEnemies();
void drawEnemies();
void moveEnemies();
void checkEnemyScore();
void updateEnemyBullets();
typedef struct Heart
{
    Texture2D sprite;
    Position position;
    bool active;
} Heart;

void initHearts(Heart[], int);
void drawHearts(Heart[], int);
typedef struct Bullet
{
    Rectangle rec;
    Color color;
    bool shotted;
    Texture2D sprite;
} Bullet;
void initBullets(Bullet[]);
void shootBullet(Bullet[], Position);
void updateBullets(Bullet[]);
void drawBullet(Bullet[]);

void checkEnemyShooted(SpaceCraft[MAX_ENEMIES_ROWS][MAX_ENEMIES_PER_ROW], Bullet[MAX_BULLETS]);
static GameScreen game_screen;
static Rectangle border_right;
static Rectangle border_left;
static Rectangle border_bottom;
SpaceCraft player_spacecraft;

Heart player_hearts[PLAYER_MAX_LIFE];

static int score = 0;
char score_text[sizeof(int) + 8] = " SCORE: 0";
int shooter_idx, shooter_idy;

static Bullet enemies_bullets[MAX_ENEMIES_BULLETS];

void setScoreText()
{
    sprintf(score_text, "SCORE: %d", score);
}

void drawBullet(Bullet bullets[])
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (bullets[i].shotted)
        {
            DrawRectangle(bullets[i].rec.x, bullets[i].rec.y, bullets[i].rec.width, bullets[i].rec.height, bullets[i].color);
        }
    }
}

void drawEnemiesBullets()
{
    for (int i = 0; i < MAX_ENEMIES_BULLETS; i++)
    {
        if (enemies_bullets[i].shotted)
        {
            //DrawRectangle(enemies_bullets[i].rec.x, enemies_bullets[i].rec.y, enemies_bullets[i].rec.width, enemies_bullets[i].rec.height, enemies_bullets[i].color);

            DrawTextureEx(enemies_bullets[i].sprite, (Vector2){.x = enemies_bullets[i].rec.x, .y =enemies_bullets[i].rec.y}, 0.0f, 0.8f, WHITE);

        }
    }
}

void initBullets(Bullet bullets[])
{

    for (int i = 0; i < MAX_BULLETS; i++)
    {
        bullets[i] = (Bullet){.rec = (Rectangle){.x = 0, .y = 0, .width = 10, .height = 10}, .color = BLUE, .shotted = false};
    }
}

void initEnemiesBullets()
{

    Texture2D bullet_sprite = LoadTexture("resources/water_drop.png");
    for (int i = 0; i < MAX_ENEMIES_BULLETS; i++)
    {
        enemies_bullets[i] = (Bullet){.rec = (Rectangle){.x = 0, .y = 0, .width = 10, .height = 10}, .color = GREEN, .shotted = false, .sprite=bullet_sprite};
    }
}

void shootBullet(Bullet bullets[], Position bullet_initial_pos)
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (!bullets[i].shotted)
        {
            bullets[i].shotted = true;
            bullets[i].rec.x = bullet_initial_pos.x;
            bullets[i].rec.y = bullet_initial_pos.y;
            break;
        }
    }
}

void checkEnemyShooted(SpaceCraft enemies[MAX_ENEMIES_ROWS][MAX_ENEMIES_PER_ROW], Bullet bullets[MAX_BULLETS])
{
    for (int i = 0; i < MAX_ENEMIES_ROWS; i++)
    {
        for (int j = 0; j < MAX_ENEMIES_PER_ROW; j++)
        {
            for (int k = 0; k < MAX_BULLETS; k++)
            {
                if (bullets[k].shotted && enemies[i][j].lifes > 0)
                {
                    if (CheckCollisionRecs(bullets[k].rec, enemies[i][j].rec))
                    {
if (enemies[i][j].lifes <= 0)
                        {
                            score++;
                            setScoreText();
                            break;
                        }

                        enemies[i][j].lifes -= 1;
                        bullets[k].shotted = false;
                                            }
                }
            }
        }
    }
}

void updateBullets(Bullet bullets[])
{
    for (int i = 0; i < MAX_BULLETS; i++)
    {
        if (bullets[i].shotted)
        {
            bullets[i].rec.y -= BULLET_SPEED;
            if (bullets[i].rec.y <= 0)
            {
                bullets[i].shotted = false;
            }
        }
    }
}

void drawPlayerSpaceCraft()
{
            DrawTextureEx(player_spacecraft.sprite, (Vector2){.x = player_spacecraft.rec.x, .y =player_spacecraft.rec.y}, 0.0f, 1.f, WHITE);
}


void movePlayer(SpaceCraft *player, KeyboardKey direction)
{
    int step = direction == KEY_LEFT ? -PLAYER_CALIBER / 2 : PLAYER_CALIBER / 2;
    if ((CheckCollisionRecs(player->rec, border_right) && direction == KEY_RIGHT) || (CheckCollisionRecs(player->rec, border_left) && direction == KEY_LEFT))
    {
        return;
    }
    player->rec.x += step;
}

void initEnemies()
{
    Texture2D enemy_sprite = LoadTexture("resources/naridrin.png");
    for (int i = 0; i < MAX_ENEMIES_ROWS; i++)
    {
        for (int j = 0; j < MAX_ENEMIES_PER_ROW; j++)
        {
            enemies[i][j] = (SpaceCraft){.rec = (Rectangle){.x = 0, .y = 0, .width = enemy_sprite.width * 0.1f, .height = enemy_sprite.height * 0.1f}, .color = YELLOW, .lifes = 2, .step = CALIBER / 2};
            enemies[i][j].rec = (Rectangle){.x = ((float)game_screen.width / 2 - (enemies[i][j].rec.x + (enemies[i][j].rec.width + (enemies[i][j].rec.width * 2)) * j)), .y = ((float)game_screen.height / 2 - ((enemies[i][j].rec.height + 10) * i)), .width = enemy_sprite.width * 0.1f, .height = enemy_sprite.height * 0.1f};
            enemies[i][j].sprite = enemy_sprite;
        }
    }
}

void drawEnemies()
{
    for (int i = 0; i < MAX_ENEMIES_ROWS; i++)
    {
        for (int j = 0; j < MAX_ENEMIES_PER_ROW; j++)
        {
            if (enemies[i][j].lifes > 0)
            {
            DrawTextureEx(enemies[i][j].sprite, (Vector2){.x = enemies[i][j].rec.x, .y = enemies[i][j].rec.y}, 180.f, 0.1f, WHITE);
            }
        }
    }
}

void moveEnemies()
{

    for (int i = 0; i < MAX_ENEMIES_ROWS; i++)
    {
        for (int j = 0; j < MAX_ENEMIES_PER_ROW; j++)
        {

            enemies[i][j].rec.y += (float)CALIBER / 12;
            if (CheckCollisionRecs(enemies[i][j].rec, border_right) || CheckCollisionRecs(enemies[i][j].rec, border_left))
            {
                enemies[i][j].step = -enemies[i][j].step;
            }

            if (i % 2 == 0)
            {

                enemies[i][j].rec.x += -enemies[i][j].step;
            }
            else
            {
                enemies[i][j].rec.x += enemies[i][j].step;
            }
        }
    }
}

void initSpaceCraft()
{

    Texture2D spacecraft_sprite= LoadTexture("resources/spacecraft_full_health.png");

    player_spacecraft = (SpaceCraft){.rec = (Rectangle){.x = (float)game_screen.width / 2 - player_spacecraft.rec.width, .y = (float)game_screen.height - (50 + BORDERS_SIZE), .width = 50, .height = 50}, .lifes = 3, .step = PLAYER_CALIBER, .sprite=spacecraft_sprite};
}

void initHearts(Heart hearts[], int player_max_life)
{
    for (int i = 0; i < player_max_life; i++)
    {

        Texture2D heart_sprite = LoadTexture("resources/heart.png");
        hearts[i].sprite = heart_sprite;
        hearts[i].position = (Position){.x = BORDERS_SIZE + (30 * i + 1), .y = BORDERS_SIZE + 25};
        hearts[i].active = true;
    }
}

void drawHearts(Heart hearts[], int player_max_life)
{
    for (int i = 0; i < player_max_life; i++)
    {
        if (hearts[i].active)
        {
            DrawTextureEx(hearts[i].sprite, (Vector2){.x = hearts[i].position.x, .y = hearts[i].position.y}, 0.0f, 0.1f, WHITE);
        }
    }
}

void checkEnemyScore()
{
    bool player_damaged = false;
    for (int i = 0; i < MAX_ENEMIES_ROWS; i++)
    {

        if (player_damaged)
        {
            break;
        }

        for (int j = 0; j < MAX_ENEMIES_PER_ROW; j++)
        {
            if (enemies[i][j].lifes > 0)
            {
                if (CheckCollisionRecs(enemies[i][j].rec, player_spacecraft.rec) || CheckCollisionRecs(enemies[i][j].rec, border_bottom))
                {
                    for (int k = 0; k < PLAYER_MAX_LIFE; k++)
                    {
                        if (player_hearts[k].active)
                        {
                            player_hearts[k].active = false;
                            enemies[i][j].lifes = 0;
                            player_damaged = true;
                            player_spacecraft.lifes--;
                            break;
                        }
                    }
                }
            }
        }
    }
}

void checkEnemyScoreBullet()
{
    bool player_damaged = false;
    for(int i = 0; i < MAX_ENEMIES_BULLETS; i++)
    {
        if(player_damaged)
        {
            break;
        }

        if(enemies_bullets[i].shotted)
        {
            if(CheckCollisionRecs(enemies_bullets[i].rec, player_spacecraft.rec))
            {
                for (int k = 0; k < PLAYER_MAX_LIFE; k++)
                {
                    if (player_hearts[k].active)
                    {
                        player_hearts[k].active = false;
                        enemies_bullets[i].shotted = false;
                        player_damaged = true;
                        TraceLog(LOG_INFO, "this bullet is on x: %f, and on y: %f\n", enemies_bullets[i].rec.x, enemies_bullets[i].rec.y);
                        break;

                    }
                }
            }
        }
    }
}

void updateEnemyBullets()
{
    for (int i = 0; i < MAX_ENEMIES_BULLETS; i++)
    {
        if (enemies_bullets[i].shotted)
        {
            enemies_bullets[i].rec.y += 6.f;
            if(enemies_bullets[i].rec.y >= game_screen.height)
            {
                enemies_bullets[i].shotted = false;
            }
        }
    }
}

void enemyShoot(SpaceCraft enemies[MAX_ENEMIES_ROWS][MAX_ENEMIES_PER_ROW])
{
    for (int i = 0; i < MAX_ENEMIES_ROWS; i++)
    {
            if (enemies[shooter_idy][shooter_idx].lifes > 0)
            {
                if (!enemies_bullets[shooter_idx].shotted)
                {
                    enemies_bullets[shooter_idx].shotted = true;
                    enemies_bullets[shooter_idx].rec.x = enemies[shooter_idy][shooter_idx].rec.x;
                    enemies_bullets[shooter_idx].rec.y = enemies[shooter_idy][shooter_idx].rec.y;
                    break;
                }
            }
    }
}

int start(GameScreen game_s)
{
    game_screen = game_s;
    InitWindow(0, 0, "Space Invaders");
    SetWindowSize(game_screen.width, game_screen.height);
    SetTargetFPS(60);

    initSpaceCraft();
    initHearts(player_hearts, player_spacecraft.lifes);
    Bullet bullets[MAX_BULLETS];
    initBullets(bullets);

    initEnemies();

    initEnemiesBullets();
    border_right = (Rectangle){.x = game_screen.width - border_right.width, .y = 0, .width = 1, .height = game_screen.height};

    border_left = (Rectangle){.x = 0, .y = 0, .width = 1, .height = game_screen.height};
    border_bottom = (Rectangle){.x = 0, .y = game_screen.height - 1, .width = game_screen.width, .height = 1};

    int total_shots = 0;
    char total_shot_text[sizeof(int)];

    float interval = 0.5f;
    float timer = 0.0f;

    srand(time(NULL));
    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_SPACE))
        {
            shootBullet(bullets, (Position){.x = player_spacecraft.rec.x + (player_spacecraft.rec.width / 2 - 10), .y = player_spacecraft.rec.y});
            total_shots++;
            sprintf(total_shot_text, "%d", total_shots);
        }
        if (IsKeyDown(KEY_LEFT))
        {
            movePlayer(&player_spacecraft, KEY_LEFT);
        }
        else if (IsKeyDown(KEY_RIGHT))
        {
            movePlayer(&player_spacecraft, KEY_RIGHT);
        }

        BeginDrawing();
        DrawRectangle(border_right.x, border_right.y, border_right.width, border_right.height, BLACK);
        DrawRectangle(border_left.x, border_left.y, border_left.width, border_left.height, BLACK);

        DrawRectangle(border_bottom.x, border_bottom.y, border_bottom.width, border_bottom.height, RED);

        ClearBackground(BLACK);
        float dt = GetFrameTime();
        timer += dt;
        if (timer >= interval)
        {
            shooter_idx = rand() % 5;
            shooter_idy = rand() % 5;
            enemyShoot(enemies);
            timer = 0.0f;
        }

        DrawText(total_shot_text, 10, game_screen.height - 20, 20, WHITE);

        DrawText(score_text, BORDERS_SIZE, BORDERS_SIZE, 20, WHITE);

        drawHearts(player_hearts, player_spacecraft.lifes);

        checkEnemyShooted(enemies, bullets);
        checkEnemyScore();
        checkEnemyScoreBullet();

        drawEnemies();
        moveEnemies();
        drawPlayerSpaceCraft();
        drawBullet(bullets);
        updateBullets(bullets);
        updateEnemyBullets();

        drawEnemiesBullets();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
