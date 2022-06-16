#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <windows.h>

#define PIXEL "8"
#define NO_PIXEL " "
//#define WIDTH 64
#define WIDTH 128
//#define HEIGHT 24
#define HEIGHT 30
//#define PRINT_BUFFER_SIZE 4096
#define PRINT_BUFFER_SIZE (WIDTH * HEIGHT) + (WIDTH * 2)

#define SLEEP_TIME 50

bool getBit(const char *grid, size_t bit) {
    size_t fit = bit / 8;
    size_t remain = 7 - (bit % 8);
    char theByte = grid[fit];
    return ((theByte >> remain) & 0x01) == 0x01;
}

void setBit(char *grid, size_t bit, bool value) {
    size_t fit = bit / 8;
    size_t remain = 7 - (bit % 8);
    if (value) {
        grid[fit] = grid[fit] | (0x01 << remain);
    } else {
        grid[fit] = grid[fit] - (grid[fit] & (0x01 << remain));
    }
}

void loop(char *grid, char *gridBuffer, size_t width, size_t height) {
    memcpy(gridBuffer, grid, (width * height) / 8);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int count = 0;
            bool current = getBit(gridBuffer, x + (y * width));
            if (x > 0 && y > 0 && getBit(gridBuffer, (x - 1) + ((y - 1) * width))) {
                count++;
            }
            if (y > 0 && getBit(gridBuffer, (x) + ((y - 1) * width))) {
                count++;
            }
            if (x < (width - 1) && y > 0 && getBit(gridBuffer, (x + 1) + ((y - 1) * width))) {
                count++;
            }
            if (x > 0 && getBit(gridBuffer, (x - 1) + ((y) * width))) {
                count++;
            }
            if (x < (width - 1) && getBit(gridBuffer, (x + 1) + ((y) * width))) {
                count++;
            }
            if (x > 0 && y < (height - 1) && getBit(gridBuffer, (x - 1) + ((y + 1) * width))) {
                count++;
            }
            if (y < (height - 1) && getBit(gridBuffer, (x) + ((y + 1) * width))) {
                count++;
            }
            if (x < (width - 1) && y < (height - 1) && getBit(gridBuffer, (x + 1) + ((y + 1) * width))) {
                count++;
            }
            if (current && (count < 2 || count > 3)) {
                setBit(grid, x + (y * width), false);
            } else if (!current && (count == 3)) {
                setBit(grid, x + (y * width), true);
            }
        }
    }
}

void displayGrid(char *grid, size_t width, size_t height, char *printBuffer) {
    memset(printBuffer, 0, PRINT_BUFFER_SIZE);
    strcat_s(printBuffer, PRINT_BUFFER_SIZE, "\n");
    for (int i = 0; i < width; i++) {
        strcat_s(printBuffer, PRINT_BUFFER_SIZE, "-");
    }
    strcat_s(printBuffer, PRINT_BUFFER_SIZE, "\n");
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (getBit(grid, x + (y * width))) {
                strcat_s(printBuffer, PRINT_BUFFER_SIZE, PIXEL);
            } else {
                strcat_s(printBuffer, PRINT_BUFFER_SIZE, NO_PIXEL);
            }
        }
        strcat_s(printBuffer, PRINT_BUFFER_SIZE, "\n");
    }
//    for (int i = 0; i < width; i++) {
//        strcat_s(printBuffer, PRINT_BUFFER_SIZE, "-");
//    }
    printf("%s", printBuffer);
}

void setRandom(char *grid, size_t width, size_t height) {

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            grid[x + (y * width)] = rand();
        }
    }
}

int main() {
    char printBuffer[PRINT_BUFFER_SIZE];
    char grid[(WIDTH * HEIGHT) / 8];
    setRandom(grid, WIDTH, HEIGHT);
//    memset(grid, 0, ((WIDTH * (HEIGHT + 1)) / 8));
//    grid[25] = 0xC0;
//    grid[27] = 0x60;
//    grid[29] = 0x40;
//    displayGrid(grid, WIDTH, HEIGHT, printBuffer);

    char *gridBuffer = malloc((WIDTH * HEIGHT) / 8);

//    char *input = ' ';
    int frames = 0;
    while (frames < 1000) {
//        scanf_s("%c", &input);
        Sleep(SLEEP_TIME);
        loop(grid, gridBuffer, WIDTH, HEIGHT);
        displayGrid(grid, WIDTH, HEIGHT, printBuffer);
        frames++;
    }
    return 0;
}
