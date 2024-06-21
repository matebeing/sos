#include "raylib.h"
#include <string.h>
#include <locale.h>
#include "utils.c"
#include <ctype.h> 
#include <stdlib.h>
#include <stdio.h>

#define MAX_INPUT_CHARS 9
#define NUM_OBSTACLES 10
#define OBSTACLE_WIDTH 50
#define OBSTACLE_HEIGHT 50
#define OBSTACLE_SPEED 4


// Local Variables
static const int screenWidth = 1400;
static const int screenHeight = 800;

char code[500][500];
char translation[500][500];
int w = 0;

// Structs & Enum
typedef enum {
    MENU,
    PHASE1,
    PHASE2,
    GAMEOVER
} Scene;

typedef struct {
    Rectangle rect;
    Texture2D texture;
    Color color;
    int speed;
} Obstacle;

typedef struct pSZA {
    int hp;
    int x;
    int y;
} pSZA;

void removeLife(pSZA *SZA) {
    SZA->hp = SZA->hp - 1;
}

int main(void) {
    FILE* file;

    int lastScore = 0;

    file = fopen("file.txt", "r+");

    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    if (fscanf(file, "%d", &lastScore)) {
        printf("Score read from file: %d\n", lastScore);
    }
    else {
        printf("Error reading score from file.\n");
    }

    pSZA* SZA;
    SZA = (pSZA*)malloc(sizeof(pSZA));

    int score = 0;

    if (SZA == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    setlocale(LC_ALL, "Portuguese_Brazil");

    InitWindow(screenWidth, screenHeight, "SOS (SZA IS MISSING)");
    InitAudioDevice();

    SetTargetFPS(60);


    // Initialize obstacles
    Texture2D obstacleTextures[] = {
        LoadTexture("resources/plastictrash.png"),
        LoadTexture("resources/coke.png"),
        LoadTexture("resources/wooden.png"),
        LoadTexture("resources/shark.png")
    };

    Obstacle obstacles[NUM_OBSTACLES];

    for (int i = 0; i < NUM_OBSTACLES; i++) {
        obstacles[i].rect.x = screenWidth + i * 200;
        obstacles[i].rect.y = GetRandomValue(0, screenHeight - OBSTACLE_HEIGHT);
        obstacles[i].rect.width = OBSTACLE_WIDTH;
        obstacles[i].rect.height = OBSTACLE_HEIGHT;
        
        int randomTextureIndex = GetRandomValue(0, sizeof(obstacleTextures) / sizeof(Texture2D) - 1);
        obstacles[i].texture = obstacleTextures[randomTextureIndex];

        obstacles[i].color = RED;
        obstacles[i].speed = OBSTACLE_SPEED;
    }

    // SCENE ENUM
    Scene actualScene = MENU;

    // Define as coordenadas e dimensões do botão
    Rectangle buttonRec = { (screenWidth - 273) / 2, 380, 273, 90};

    // Font
    const char msg[256] = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHI\nJKLMNOPQRSTUVWXYZ[]^_`abcdefghijklmn\nopqrstuvwxyz{|}~¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓ\nÔÕÖ×ØÙÚÛÜÝÞßàáâãäåæçèéêëìíîïðñòóôõö÷\nøùúûüýþÿ";
    Font fontTtf = LoadFontEx("resources/fonts/Karantina-Bold.ttf", 32, 0, 250);
    SetTextLineSpacing(16);

    // Load background texture
    Texture2D background = LoadTexture("resources/menu/background.png");
    Texture2D banner = LoadTexture("resources/menu/banner.png");
    Texture2D button = LoadTexture("resources/menu/button.png");
    Texture2D sza = LoadTexture("resources/largeSZA.png");
    Texture2D bgsea = LoadTexture("resources/bgsea.png");
    Texture2D radio = LoadTexture("resources/morseradio.png");


    // AUDIOS
    Music music = LoadMusicStream("resources/menu/sosaudio.mp3");

    PlayMusicStream(music);

    // INPUT
    char name[MAX_INPUT_CHARS + 1] = "\0";
    int letterCount = 0;

    Rectangle textBox = { screenWidth / 2.02f - 110, 600, 225, 50 };
    bool mouseOnText = false;

    // TIMER
    int framesCounter = 0;
    int seconds = 3600;

    int framesTotais = 3600; // 60 segundos * 60 frames por segundo
    int framesContador = framesTotais;



    SZA->hp = 3;
    SZA->x = (screenWidth - 164) / 2;
    SZA->y = 410;

    while (!WindowShouldClose()) {

        for (int i = 0; i < NUM_OBSTACLES; i++) {
            obstacles[i].rect.x -= obstacles[i].speed;
            if (obstacles[i].rect.x + obstacles[i].rect.width <= 0) {
                obstacles[i].rect.x = screenWidth;
                obstacles[i].rect.y = GetRandomValue(0, screenHeight - OBSTACLE_HEIGHT);
            }
        }
       
        if (CheckCollisionPointRec(GetMousePosition(), textBox)) mouseOnText = true;
        else mouseOnText = false;

        if (actualScene == PHASE2) {
            if (IsKeyDown(KEY_UP)) {
                if (SZA->y > 200) {
                    SZA->y -= 10;
                    printf("%d", SZA->y);
                }
             
            }
            if (IsKeyDown(KEY_DOWN)) {
                if (SZA->y < 600) {
                    SZA->y += 10;
                    printf("%d", SZA->y);
                }
            }
        }

        if (mouseOnText)
        {
            SetMouseCursor(MOUSE_CURSOR_IBEAM);
            int key = GetCharPressed();
            while (key > 0)
            {
                if ((key >= 32) && (key <= 125) && (letterCount < MAX_INPUT_CHARS))
                {
                    name[letterCount] = (char)key;
                    name[letterCount + 1] = '\0'; 
                    letterCount++;
                }
                key = GetCharPressed();  
            }


            if (IsKeyPressed(KEY_ENTER)) {
                char word[20];
                strcpy(word, name);

                //strcat(word, "");

                int resultado = strcmp(name, translation[w]);

                if (resultado == 0) {
                    w++;
                    printf("%s", name);
                    letterCount = 0;
                    name[letterCount] = '\0';
                }
                else {
                    w = 0;
                    letterCount = 0;
                    name[letterCount] = '\0';
                }
            }
            if (IsKeyPressed(KEY_BACKSPACE))
            {
                letterCount--;
                if (letterCount < 0) letterCount = 0;
                name[letterCount] = '\0';
            }
        }
        else SetMouseCursor(MOUSE_CURSOR_DEFAULT);

        if (mouseOnText) framesCounter++;
        else framesCounter = 0;

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mousePoint = GetMousePosition();
            if (CheckCollisionPointRec(mousePoint, buttonRec)) {
                actualScene = PHASE1;
                PauseMusicStream(music);

            }
        }

        if (actualScene == GAMEOVER && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            actualScene = MENU;
        }

        // Draw
        BeginDrawing();
        UpdateMusicStream(music);
        ClearBackground(BLACK);

        switch (actualScene) {
        case MENU:
            
            // Textures
            DrawTexture(background, 0, 0, WHITE);
            DrawTexture(banner, (screenWidth - 172) / 2, 100, WHITE);

            // PLAY BUTTON
            DrawTexture(button, (screenWidth - 273) / 2, 380, WHITE);
            DrawTextEx(fontTtf, "PLAY", (Vector2) { (float)(screenWidth - MeasureText("PLAY", (float)fontTtf.baseSize)) / 2, 410.0f }, (float)fontTtf.baseSize, 10, WHITE);


            // CREDITS
            DrawTextEx(fontTtf, "Coded By Mate", (Vector2) { (float)(screenWidth - MeasureText("Coded By Mate", (float)fontTtf.baseSize)) / 2 + 18, 700}, (float)fontTtf.baseSize - 15.0, 10, WHITE);


            break;

        case GAMEOVER:

            DrawText("GAME OVER", (screenWidth - MeasureText("GAME OVER", 50)) / 2, 250, 50, WHITE);
            DrawText("SZA DROWNED", (screenWidth - MeasureText("SZA DROWNED", 50)) / 2, 300, 50, BLUE);
            DrawText("CLICK ANYWHERE TO GET BACK TO MENU", (screenWidth - MeasureText("CLICK ANYWHERE TO GET BACK TO MENU", 20)) / 2, 500, 20, WHITE);

            break;

        case PHASE1:

            strcpy(code[0], "... --- ...");
            strcpy(code[1], ".-.. --- ... -");
            strcpy(code[2], "--- -.-. . .- -.");
            strcpy(code[3], ".- .-.. --- -. .");
            strcpy(code[4], ".- ..-. .-. .- .. -..");
            strcpy(code[5], "... - .- .-. ...- .. -. --.");

            strcpy(translation[0], "SOS");
            strcpy(translation[1], "LOST");
            strcpy(translation[2], "OCEAN");
            strcpy(translation[3], "ALONE");
            strcpy(translation[4], "AFRAID");
            strcpy(translation[5], "STARVING");

            ClearBackground(BLACK);
            DrawTexture(background, 0, 0, WHITE);
            DrawTexture(radio, (screenWidth - 907) / 2, 280, WHITE);


            DrawText("PHASE 1", (screenWidth - MeasureText("PHASE 1", 30)) / 2, 50, 30, WHITE);
            DrawText("DECODE THE MESSAGE", (screenWidth - MeasureText("DECODE THE MESSAGE", 30)) / 2, 100, 30, WHITE);
            DrawText("SZA IS MISSING AND SENDING SIGNALS", (screenWidth - MeasureText("SZA IS MISSING AND SENDING SIGNALS", 20)) / 2, 150, 20, WHITE);
            
            framesContador--;

            if (w < 6) {
                DrawText(code[w], (screenWidth - MeasureText(code[w], 80)) / 2, 420, 80, BLACK);
            }
            else {
                DrawText("WELL DONE", (screenWidth - MeasureText("WELL DONE", 50)) / 2, 350, 50, WHITE);
                actualScene = PHASE2;
            }


            if ((framesContador / 60) % 60 > 0) {
                DrawTextEx(fontTtf, TextFormat("Time left: %02d:%02d", framesContador / 3600, (framesContador / 60) % 60), (Vector2) { (float)(screenWidth - MeasureText(TextFormat("Time left: %02d:%02d", framesContador / 3600, (framesContador / 60) % 60), (float)fontTtf.baseSize)) / 2, 200.0f }, (float)fontTtf.baseSize - 15.0, 10, WHITE);

                //DrawText(TextFormat("Time left: %02d:%02d", framesContador / 3600, (framesContador / 60) % 60), (screenWidth - MeasureText(TextFormat("Time left: %02d:%02d", framesContador / 3600, (framesContador / 60) % 60), 15)) / 2, 200, 15, WHITE);
            }
            else {
                framesContador = 3600;
                actualScene = GAMEOVER;
            }
  

            DrawRectangleRec(textBox, BLACK);
            if (mouseOnText) DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, WHITE);
            else DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, DARKGRAY);
            DrawText(name, (int)textBox.x + 5, (int)textBox.y + 8, 40, WHITE);

            if (mouseOnText)
            {
                if (letterCount < MAX_INPUT_CHARS)
                {
                    // Draw blinking underscore char
                    if (((framesCounter / 20) % 2) == 0) DrawText("_", (int)textBox.x + 8 + MeasureText(name, 40), (int)textBox.y + 12, 40, WHITE);
                }
            }
            break;
        case PHASE2:

            DrawTexture(background, 0, 0, WHITE);

            score += 1;


            for (int i = 0; i < NUM_OBSTACLES; i++) {
                DrawTexture(obstacles[i].texture, obstacles[i].rect.x, obstacles[i].rect.y, WHITE);
               // DrawRectangleRec(obstacles[i].rect, obstacles[i].color);

                if (CheckCollisionRecs((Rectangle) { SZA->x, SZA->y, 100, 75}, obstacles[i].rect)) {
                    removeLife(SZA);
                    obstacles[i].rect.x = screenWidth;
                    if (SZA->hp <= 0) {
                        actualScene = GAMEOVER;
                        SZA->hp = 3;

                        fprintf(file, "%d", score);
                        fclose(file);

                        score = 0;

                        file = fopen("file.txt", "r+");

                        if (fscanf(file, "%d", &lastScore)) {
                            printf("Score read from file: %d\n", lastScore);
                        }
                        else {
                            printf("Error reading score from file.\n");
                        }


                    }
                    //actualScene = GAMEOVER; // Handle collision, for example, transition to GAMEOVER scene
                }
            }

            //DrawRectangle(SZA->x, SZA->y, 75, 75, RED);
            DrawTexture(sza, SZA->x, SZA->y, WHITE);

            DrawText("PHASE 2", (screenWidth - MeasureText("PHASE 1", 30)) / 2, 50, 30, WHITE);
            DrawText("AVOID THE OBSTACLES", (screenWidth - MeasureText("AVOID THE OBSTACLES", 30)) / 2, 100, 30, WHITE);
            DrawText("SZA IS FLOATING AWAY", (screenWidth - MeasureText("SZA IS FLOATING AWAY", 20)) / 2, 150, 20, WHITE);

            DrawText(TextFormat("HP %d/3", SZA->hp), 150, 50, 30, WHITE);
            DrawText(TextFormat("Score %d", score), (screenWidth - 300), 50, 30, WHITE);           
            DrawText(TextFormat("Last Score %d", lastScore), (screenWidth - 300), 100, 20, GRAY);


            break;

        }


        EndDrawing();
    }

    // Unload background texture
    UnloadTexture(bgsea);
    UnloadTexture(background);
    free(SZA);
    CloseAudioDevice();     // Close audio device

    CloseWindow();

    return 0;
}
