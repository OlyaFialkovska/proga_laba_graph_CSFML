#include <SFML/Graphics.h>
#include <math.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define PI 3.14159265
#define NUM_POINTS 1000

float A = 300.0f;
float B = 50.0f;
float D = 50.0f;
int N = 10; // Кількість проміжків розбиття

void drawAxes(sfRenderWindow *window, float mx, float my) {
    sfVertexArray *axes = sfVertexArray_create();
    sfVertexArray_setPrimitiveType(axes, sfLines);

    // Draw X-axis
    for (int i = -8; i <= 8; ++i) {
        sfVertex vertex;
        vertex.position.x = WINDOW_WIDTH / 2 + i * 50 * mx;
        vertex.position.y = WINDOW_HEIGHT / 2;
        vertex.color = sfWhite;
        sfVertexArray_append(axes, vertex);
    }

    // Draw Y-axis
    for (int i = -5; i <= 5; ++i) {
        sfVertex vertex;
        vertex.position.x = WINDOW_WIDTH / 2;
        vertex.position.y = WINDOW_HEIGHT / 2 - i * 50 * my;
        vertex.color = sfWhite;
        sfVertexArray_append(axes, vertex);
    }

    // Draw axis numbers
    sfFont* font = sfFont_createFromFile("arial.ttf");
    sfText* text = sfText_create();
    sfText_setFont(text, font);
    sfText_setCharacterSize(text, 12);
    sfText_setFillColor(text, sfWhite);

    // X-axis numbers
    for (int i = -8; i <= 8; ++i) {
        char number[10];
        sprintf(number, "%.1f", i * (PI / 2));
        sfText_setString(text, number);
        sfText_setPosition(text, (sfVector2f){WINDOW_WIDTH / 2 + i * 50 * mx - 10, WINDOW_HEIGHT / 2 + 10});
        sfRenderWindow_drawText(window, text, NULL);
    }

    // Y-axis numbers
    for (int i = -5; i <= 5; ++i) {
        char number[10];
        sprintf(number, "%.1f", i * 100.0f * my);
        sfText_setString(text, number);
        sfText_setPosition(text, (sfVector2f){WINDOW_WIDTH / 2 - 30, WINDOW_HEIGHT / 2 - i * 50 * my - 10});
        sfRenderWindow_drawText(window, text, NULL);
    }

    sfRenderWindow_drawVertexArray(window, axes, NULL);
    sfVertexArray_destroy(axes);
    sfFont_destroy(font);
    sfText_destroy(text);
}

void updateGraph(sfRenderWindow *window, sfVertexArray *lines, float mx, float my) {
    sfVertexArray_clear(lines);
    float dx = 2 * PI / NUM_POINTS; // Крок зміни аргументу

    // Малюємо графік функції
    for (int i = 0; i < NUM_POINTS; ++i) {
        float t = i * dx;
        float g = (A / B) * t;
        float x = (A - B) * cos(t) + D * cos(g);
        float y = (A - B) * sin(t) - D * sin(g);

        sfVertex vertex = { { WINDOW_WIDTH / 2 + x * mx, WINDOW_HEIGHT / 2 - y * my }, sfWhite };
        sfVertexArray_append(lines, vertex);
    }

    sfRenderWindow_drawVertexArray(window, lines, NULL);
}

int main() {
    sfVideoMode mode = { WINDOW_WIDTH, WINDOW_HEIGHT, 32 };
    sfRenderWindow *window = sfRenderWindow_create(mode, "Graph", sfClose, NULL);

    sfVertexArray *lines = sfVertexArray_create();

    float mx = 1.0f;
    float my = 1.0f;

    sfFont *font = sfFont_createFromFile("arial.ttf");
    sfText *text = sfText_create();
    sfText_setString(text, "FIALKOVSKA OLGA KV-34");
    sfText_setFont(text, font);
    sfText_setCharacterSize(text, 16);
    sfText_setFillColor(text, sfWhite);
    sfFloatRect textBounds = sfText_getLocalBounds(text);
    sfText_setPosition(text, (sfVector2f){WINDOW_WIDTH - textBounds.width - 10, 10}); // Позиція тексту

    while (sfRenderWindow_isOpen(window)) {
        sfEvent event;
        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
            else if (event.type == sfEvtMouseWheelScrolled) {
                if (event.mouseWheelScroll.delta > 0) {
                    mx *= 1.1f;
                    my *= 1.1f;
                } else if (event.mouseWheelScroll.delta < 0) {
                    mx *= 0.9f;
                    my *= 0.9f;
                }
            }
        }

        sfRenderWindow_clear(window, sfBlack);
        drawAxes(window, mx, my);
        updateGraph(window, lines, mx, my);
        sfRenderWindow_drawText(window, text, NULL); // Відображення тексту
        sfRenderWindow_display(window);
    }

    sfVertexArray_destroy(lines);
    sfText_destroy(text);
    sfFont_destroy(font);
    sfRenderWindow_destroy(window);

    return 0;
}
