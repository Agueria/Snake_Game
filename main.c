#include <stdlib.h>
#include <unistd.h>
#include <curses.h>

#define HEIGHT 32
#define WIDTH 32

void setup();
void input();
void logic();
void draw();
void loop();

typedef struct Position {
    int x, y;
} position;

typedef struct Snake {
    int len;
    position *pos;
} snake_type;

typedef struct Game {
    snake_type snake;
    position fruit;
    int running;
    int score;
    char buffer[HEIGHT][WIDTH];
} game_type;

int i, j;
game_type game;
position dir = {1, 0};

void gen_new_fruit() {
    game.fruit.x = rand() % (WIDTH - 1) + 1;
    game.fruit.y = rand() % (HEIGHT - 1) + 1;
}

void setup() {
    game.running = 1;
    game.score = 0;
    game.snake.len = 5;
    game.snake.pos = malloc(game.snake.len * sizeof(position));
    for (i = 0; i < game.snake.len; i++) {
        game.snake.pos[i].x = 5 - i;
        game.snake.pos[i].y = 5;
    }
    gen_new_fruit();
}

void input() {
    int c = getch();
    switch (c) {
        case 'w':
            dir.y = -1;
            dir.x = 0;
            break;
        case 'a':
            dir.y = 0;
            dir.x = -1;
            break;
        case 's':
            dir.y = 1;
            dir.x = 0;
            break;
        case 'd':
            dir.y = 0;
            dir.x = 1;
            break;
        default:
            break;
    }
}

void logic() {
    position new_head = {game.snake.pos[0].x + dir.x, game.snake.pos[0].y + dir.y};
    for (i = game.snake.len - 1; i > 0; i--) {
        game.snake.pos[i] = game.snake.pos[i - 1];
    }
    game.snake.pos[0] = new_head;

    if (new_head.x == 0 || new_head.y == 0 || new_head.x == WIDTH - 1 || new_head.y == HEIGHT - 1) {
        game.running = 0;
    }

    if (new_head.x == game.fruit.x && new_head.y == game.fruit.y) {
        game.score++;
        game.snake.len++;
        game.snake.pos = realloc(game.snake.pos, game.snake.len * sizeof(position));
        gen_new_fruit();
    }

    for (i = 1; i < game.snake.len; i++) {
        if (new_head.x == game.snake.pos[i].x && new_head.y == game.snake.pos[i].y) {
            game.running = 0;
        }
    }
}

void draw() {
    clear();
    box(stdscr, 0, 0);

    mvprintw(game.fruit.y, game.fruit.x, "O");

    for (i = 0; i < game.snake.len; i++) {
        mvprintw(game.snake.pos[i].y, game.snake.pos[i].x, "o");
    }

    mvprintw(0, WIDTH + 1, "Score: %d", game.score);
    refresh();
}

void loop() {
    while (game.running) {
        input();
        logic();
        draw();
        usleep(100 * 1000);
    }

    mvprintw(HEIGHT / 2, WIDTH / 2, "Game Over");
    mvprintw(HEIGHT / 2 + 1, WIDTH / 2, "Score: %d", game.score);
    refresh();
    sleep(3);

    free(game.snake.pos);
}

int main() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    timeout(100);
    setup();
    loop();
    endwin();
    return 0;
}
