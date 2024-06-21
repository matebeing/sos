#include "raylib.h"

void drawMorseScreen() {
	DrawText("GAME SCREEN", (400 - MeasureText("GAME SCREEN", 30)) / 2, 100, 30, LIGHTGRAY);
}