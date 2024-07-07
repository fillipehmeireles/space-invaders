#include "raylib.h"
#include "spaceinvaders.h"
#include <stdio.h>

static GameScreen game_screen;
static Rectangle border_right;
static Rectangle border_left;
static Rectangle border_bottom;
static SpaceCraft player_spacecraft;

Heart player_hearts[PLAYER_MAX_LIFE];

static int score = 0;
char score_text[sizeof(int) + 8] = " SCORE: 0";


void setScoreText()
{
    sprintf(score_text,"SCORE: %d",score);
}

void drawBullet(Bullet bullets[])
{
    for(int i = 0; i < MAX_BULLETS; i++)
    {
        if(bullets[i].shotted)
        {
            DrawRectangle(bullets[i].rec.x, bullets[i].rec.y, bullets[i].rec.width, bullets[i].rec.height, bullets[i].color);
        }

    }
}

void initBullets(Bullet bullets[])
{
    for(int i = 0; i < MAX_BULLETS; i++)
    {
        bullets[i] = (Bullet){.rec=(Rectangle){.x=0,.y=0,.width=10, .height=10}, .color=BLUE, .shotted=false};;
    }
}

void shootBullet(Bullet bullets[], Position bullet_initial_pos)
{
    for(int i = 0; i < MAX_BULLETS; i++)
    {
        if(!bullets[i].shotted)
        {
            bullets[i].shotted = true;
            bullets[i].rec.x= bullet_initial_pos.x;
            bullets[i].rec.y= bullet_initial_pos.y;
            break;
        }
    }
}

void checkEnemyShooted(SpaceCraft enemies[MAX_ENEMIES_ROWS][MAX_ENEMIES_PER_ROW],Bullet bullets[MAX_BULLETS])
{
    for(int i = 0; i < MAX_ENEMIES_ROWS; i++)
    {
        for(int j = 0; j < MAX_ENEMIES_PER_ROW; j++)
        {
            for(int k = 0; k < MAX_BULLETS; k++)
            {
                if(bullets[k].shotted && enemies[i][j].lifes > 0)
                {
                    if(CheckCollisionRecs(bullets[k].rec, enemies[i][j].rec))
                    {
                        enemies[i][j].lifes -= 1;
                        enemies[i][j].color = BLUE;
                        bullets[k].shotted = false; 
                        if(enemies[i][j].lifes <= 0)
                        {
                            score++;
                            setScoreText();
                        }
                    }
                }
            }
        }

    }

}

void updateBullets(Bullet bullets[])
{
    for(int i = 0; i < MAX_BULLETS; i++)
    {
        if(bullets[i].shotted)
        {
            bullets[i].rec.y -= BULLET_SPEED;
            if(bullets[i].rec.y <= 0)
            {
                bullets[i].shotted = false;
            }
        }
    }
}

void drawPlayerSpaceCraft(SpaceCraft player_spacecraft)
{

    DrawRectangle(player_spacecraft.rec.x , player_spacecraft.rec.y, player_spacecraft.rec.width, player_spacecraft.rec.height, player_spacecraft.color);

}

void movePlayer(SpaceCraft* player, KeyboardKey direction)
{
    int step = direction == KEY_LEFT ? -PLAYER_CALIBER/2: PLAYER_CALIBER/2;
    if((CheckCollisionRecs(player->rec, border_right)  && direction == KEY_RIGHT ) || (CheckCollisionRecs(player->rec, border_left) && direction == KEY_LEFT))
    {
        return;
    }
    player->rec.x += step;
}

void initEnemies(SpaceCraft enemies[MAX_ENEMIES_ROWS][MAX_ENEMIES_PER_ROW])
{
    for(int i = 0; i < MAX_ENEMIES_ROWS; i++)
    {
        for(int j = 0; j < MAX_ENEMIES_PER_ROW; j++)
        {
            enemies[i][j] = (SpaceCraft){.rec=(Rectangle){.x=0,.y=0,.width=30,.height=30},.color=YELLOW,.lifes=2, .step=CALIBER/2};
            enemies[i][j].rec = (Rectangle){.x=((float)game_screen.width /2 - (enemies[i][j].rec.x + (enemies[i][j].rec.width + (enemies[i][j].rec.width * 2)) * j)),.y=((float)game_screen.height/2 - ((enemies[i][j].rec.height + 10)* i)),.width=enemies[i][j].rec.width, .height=enemies[i][j].rec.height};
        }
    }
}

void drawEnemies(SpaceCraft enemies[MAX_ENEMIES_ROWS][MAX_ENEMIES_PER_ROW]) 
{
    //Color colors[MAX_ENEMIES_PER_ROW] = {RED, BLUE,YELLOW, WHITE, GREEN};
    for(int i = 0; i < MAX_ENEMIES_ROWS; i++)
    {
        for(int j = 0; j < MAX_ENEMIES_PER_ROW; j++)
        {
            if(enemies[i][j].lifes > 0)
            {
                DrawRectangle(enemies[i][j].rec.x,enemies[i][j].rec.y,enemies[i][j].rec.width,enemies[i][j].rec.height,enemies[i][j].color);
            }
        }
    }
}

void moveEnemies(SpaceCraft enemies[MAX_ENEMIES_ROWS][MAX_ENEMIES_PER_ROW])
{

    for(int i = 0; i < MAX_ENEMIES_ROWS; i++)
    {
        for(int j = 0; j < MAX_ENEMIES_PER_ROW; j++)
        { 

            enemies[i][j].rec.y += (float)CALIBER/12;
            if(CheckCollisionRecs(enemies[i][j].rec, border_right) || CheckCollisionRecs(enemies[i][j].rec, border_left))
            {
                enemies[i][j].step = -enemies[i][j].step;
            }

            enemies[i][j].rec.x += enemies[i][j].step;
        }
    }
}

void initHearts(Heart hearts[], int player_max_life)
{
    for(int i = 0; i < player_max_life; i++)
    {

        Texture2D heart_sprite = LoadTexture("resources/heart.png"); 
        hearts[i].sprite = heart_sprite;
        hearts[i].position = (Position){.x=BORDERS_SIZE + (30 * i+1),.y=BORDERS_SIZE + 25};
        hearts[i].active = true;
    }
}

void drawHearts(Heart hearts[], int player_max_life)
{
    for(int i = 0; i < player_max_life; i++)
    {
        if(hearts[i].active)
        {
            DrawTextureEx(hearts[i].sprite,(Vector2){.x=hearts[i].position.x, .y=hearts[i].position.y},0.0f,0.1f, WHITE);
        }
    }
}

void checkEnemyScore(SpaceCraft enemies[MAX_ENEMIES_ROWS][MAX_ENEMIES_PER_ROW])
{
    for(int i = 0; i < MAX_ENEMIES_ROWS; i++)
    {
        for(int j = 0; j < MAX_ENEMIES_PER_ROW; j++)
        {
            //if(CheckCollisionRecs(enemies[i][j].rec, border_bottom) || CheckCollisionRecs(enemies[i][j].rec, player_spacecraft.rec ))

            if(CheckCollisionRecs(enemies[i][j].rec, player_spacecraft.rec ))
            {
                for(int k = 0; k < PLAYER_MAX_LIFE; k++)
                {
                    if(player_hearts[k].active)
                    {
                        player_hearts[k].active = false;
                        enemies[i][j].lifes = 0;
                        goto life_damage;
                    }
                }
            }

        }
    }
life_damage:
    TraceLog(LOG_INFO, "Exited");

}

int start(GameScreen game_s)
{
    game_screen = game_s;
    InitWindow(0,0, "Space Invaders"); 
    SetWindowSize(game_screen.width, game_screen.height);
    SetTargetFPS(60);

    player_spacecraft = (SpaceCraft){.rec=(Rectangle){.x=(float)game_screen.width / 2 - player_spacecraft.rec.width,.y=(float)game_screen.height  - (50 + BORDERS_SIZE),.width=50, .height=50}, .color=RED, .lifes=3, .step=PLAYER_CALIBER};


    initHearts(player_hearts, player_spacecraft.lifes);
    Bullet bullets[MAX_BULLETS];
    initBullets(bullets);

    SpaceCraft enemies[MAX_ENEMIES_ROWS][MAX_ENEMIES_PER_ROW];
    initEnemies(enemies);

    border_right = (Rectangle){.x=game_screen.width - border_right.width,.y=0,.width=1,.height=game_screen.height };
    border_left = (Rectangle){.x=0,.y=0,.width=1,.height=game_screen.height };
    border_bottom = (Rectangle){.x=0,.y=game_screen.height - 1,.width=game_screen.width,.height=1 };

    int total_shots = 0;
    char total_shot_text[sizeof(int)];

    while (!WindowShouldClose())
    {    
        if(IsKeyPressed(KEY_SPACE))
        {
            shootBullet(bullets, (Position){.x=player_spacecraft.rec.x + (player_spacecraft.rec.width/2 - 10), .y=player_spacecraft.rec.y });
            total_shots++;
            sprintf(total_shot_text,"%d",total_shots);
        }
        if(IsKeyDown(KEY_LEFT)) 
        {
            movePlayer(&player_spacecraft,KEY_LEFT);
        }else if(IsKeyDown(KEY_RIGHT))
        {
            movePlayer(&player_spacecraft,KEY_RIGHT);
        }


        BeginDrawing();
        DrawRectangle( border_right.x, border_right.y, border_right.width, border_right.height, BLACK);
        DrawRectangle( border_left.x, border_left.y, border_left.width, border_left.height, BLACK);

        DrawRectangle( border_bottom.x, border_bottom.y, border_bottom.width, border_bottom.height, RED);

        ClearBackground(BLACK);

        DrawText(total_shot_text, 10, game_screen.height - 20, 20, WHITE);

        DrawText(score_text, BORDERS_SIZE, BORDERS_SIZE, 20, WHITE);


        drawHearts(player_hearts,player_spacecraft.lifes);

        checkEnemyShooted(enemies,bullets);
        checkEnemyScore(enemies);

        drawEnemies(enemies);
        moveEnemies(enemies);
        drawPlayerSpaceCraft(player_spacecraft);
        drawBullet(bullets);
        updateBullets(bullets);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}

