#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define WIDTH 20
#define HEIGHT 20

int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int ntail;
enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };
enum Direction dir;

void setup()
{
    initscr();
    clear();
    noecho();
    cbreak();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    timeout(100);

    x = HEIGHT / 2;
    y = WIDTH / 2;
    fruitX = rand() % WIDTH;
    fruitY = rand() % HEIGHT;
    score = 0;
    dir = STOP;
}

void draw()
{
    clear();
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            if (i == 0 || i == HEIGHT - 1 || j == 0 || j == WIDTH - 1)
                mvprintw(i, j, "#");
            else if (i == x && j == y)
                mvprintw(i, j, "O");
            else if (i == fruitY && j == fruitX)
                mvprintw(i, j, "F");
            else
            {
                int isprint = 0;
                for (int k = 0; k < ntail; k++)
                {
                    if (tailX[k] == j && tailY[k] == i)
                    {
                        mvprintw(i, j, "o");
                        isprint = 1;
                    }
                }
                if (isprint == 0)
                    mvprintw(i, j, " ");
            }
        }
    }
    mvprintw(HEIGHT, 0, "Score: %d", score);
    refresh();
}

void input()
{
    int c = getch();
    switch (c)
    {
    case KEY_LEFT:
        dir = LEFT;
        break;
    case KEY_RIGHT:
        dir = RIGHT;
        break;
    case KEY_UP:
        dir = UP;
        break;
    case KEY_DOWN:
        dir = DOWN;
        break;
    case 'x':
        endwin();
        exit(0);
        break;
    }
}

void logic()
{
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;
    for (int i = 1; i < ntail; i++)
    {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (dir)
    {
    case LEFT:
        y--;
        break;
    case RIGHT:
        y++;
        break;
    case UP:
        x--;
        break;
    case DOWN:
        x++;
        break;
    default:
        break;
    }

    if (x < 0 || x >= HEIGHT || y < 0 || y >= WIDTH)
    {
        endwin();
        exit(0);
    }

    for (int i = 0; i < ntail; i++)
    {
        if (tailX[i] == x && tailY[i] == y)
        {
            endwin();
            exit(0);
        }
    }

    if (x == fruitY && y == fruitX)
    {
        score += 10;
        fruitX = rand() % WIDTH;
        fruitY = rand() % HEIGHT;
        ntail++;
    }
}

int main()
{
    setup();
    while (1)
    {
        draw();
        input();
        logic();
    }
    return 0;
}
