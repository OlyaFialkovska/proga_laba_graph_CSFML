#include <SFML/Graphics.h>
#include <math.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define PI 3.14159265

float A = 300.0f;
float B = 50.0f;
float D = 50.0f;

void updateGraph(sfRenderWindow *window, sfVertexArray *lines, float mx, float my) {
    sfVertexArray_clear(lines);
    int numPoints = 1000;
    float xmax = 2 * PI; // Максимальне значення аргументу
    float xmin = 0;      // Мінімальне значення аргументу
    float dx = (xmax - xmin) / (numPoints - 1); // Крок зміни аргументу
    float ymin = 0;      // Мінімальне значення функції
    float ymax = 0;      // Максимальне значення функції

    float t, g, x, y;

    // Обчислюємо мінімальне та максимальне значення функції
    for (int i = 0; i < numPoints; ++i) {
        t = xmin + i * dx;
        g = (A / B) * t;

        x = (A - B) * cos(t) + D * cos(g);
        y = (A - B) * sin(t) - D * sin(g);

        if (i == 0) {
            ymin = ymax = y;
        } else {
            if (y < ymin) ymin = y;
            if (y > ymax) ymax = y;
        }
    }

    // Малюємо координатні прямі Ox та Oy
    sfVertexArray *Ox = sfVertexArray_create();
    sfVertexArray *Oy = sfVertexArray_create();
    sfVertex vertex;

    // Побудова осі Y
    for (int i = -525; i < 525; ++i) {
        vertex.position.x = WINDOW_WIDTH / 2; // Центруємо по X
        vertex.position.y = WINDOW_HEIGHT / 2 - i; // Центруємо по Y
        vertex.color = sfColor_fromRGB(255, 255, 255);
        sfVertexArray_append(Oy, vertex);

        // Додамо позначки на кожну соту одиницю координатної осі
        if (i % 100 == 0) {
            sfVertex tick1 = { {WINDOW_WIDTH / 2 - 5, WINDOW_HEIGHT / 2 - i}, sfWhite };
            sfVertex tick2 = { {WINDOW_WIDTH / 2 + 5, WINDOW_HEIGHT / 2 - i}, sfWhite };
            sfVertexArray_append(Oy, tick1);
            sfVertexArray_append(Oy, tick2);
        }
    }

    // Побудова осі X
    for (int i = -400; i < 400; ++i) {
        vertex.position.x = WINDOW_WIDTH / 2 + i; // Центруємо по X
        vertex.position.y = WINDOW_HEIGHT / 2; // Центруємо по Y
        vertex.color = sfColor_fromRGB(255, 255, 255); // Білий колір
        sfVertexArray_append(Ox, vertex);

        // Додамо позначки на кожну соту одиницю координатної осі
        if (i % 100 == 0) {
            sfVertex tick1 = { {WINDOW_WIDTH / 2 + i, WINDOW_HEIGHT / 2 - 5}, sfWhite };
            sfVertex tick2 = { {WINDOW_WIDTH / 2 + i, WINDOW_HEIGHT / 2 + 5}, sfWhite };
            sfVertexArray_append(Ox, tick1);
            sfVertexArray_append(Ox, tick2);
        }
    }

    // Відображаємо координатні прямі
    sfRenderWindow_drawVertexArray(window, Ox, NULL);
    sfRenderWindow_drawVertexArray(window, Oy, NULL);

    // Малюємо координатні осі
    sfVertex xAxis[] = { {0, WINDOW_HEIGHT / 2}, {WINDOW_WIDTH, WINDOW_HEIGHT / 2} };
    sfVertex yAxis[] = { {WINDOW_WIDTH / 2, 0}, {WINDOW_WIDTH / 2, WINDOW_HEIGHT} };
    sfVertexArray *axes = sfVertexArray_create();
    sfVertexArray_append(axes, xAxis[0]);
    sfVertexArray_append(axes, xAxis[1]);
    sfVertexArray_append(axes, yAxis[0]);
    sfVertexArray_append(axes, yAxis[1]);
    sfVertexArray_setPrimitiveType(axes, sfLines);
    sfRenderWindow_drawVertexArray(window, axes, NULL);

    // Малюємо графік функції
    for (int i = 0; i < numPoints; ++i) {
        t = xmin + i * dx;
        g = (A / B) * t;

        x = (A - B) * cos(t) + D * cos(g);
        y = (A - B) * sin(t) - D * sin(g);

        sfVertex vertex = { { WINDOW_WIDTH / 2 + x * mx, WINDOW_HEIGHT / 2 - y * my }, sfWhite };
        sfVertexArray_append(lines, vertex);
    }

    sfRenderWindow_drawVertexArray(window, lines, NULL);
}

int main() {
    sfVideoMode mode = { WINDOW_WIDTH, WINDOW_HEIGHT, 32 };
    sfRenderWindow *window = sfRenderWindow_create(mode, "Graph", sfClose, NULL);

    sfVertexArray *lines = sfVertexArray_create();
sfFont *font = sfFont_createFromFile("arial.ttf");
    sfText *text = sfText_create();
    sfText_setString(text, "FIALKOVSKA OLGA KV-34");
    sfText_setFont(text, font);
    sfText_setCharacterSize(text, 24);
    sfText_setFillColor(text, sfWhite);
    sfFloatRect textBounds = sfText_getLocalBounds(text);
    sfText_setPosition(text, (sfVector2f){WINDOW_WIDTH - textBounds.width - 10, 10}); // Позиція тексту
float mx = 1.0f;
    float my = 1.0f;
    while (sfRenderWindow_isOpen(window)) {
        sfEvent event;
        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
            else if (event.type == sfEvtMouseWheelScrolled) {
                if (event.mouseWheelScroll.delta > 0) {
                    mx *= 1.1f; // Збільшення масштабу при прокрутці вгору
                    my *= 1.1f;
                } else if (event.mouseWheelScroll.delta < 0) {
                    mx *= 0.9f; // Зменшення масштабу при прокрутці вниз
                    my *= 0.9f;
                }
            }
        }

        sfRenderWindow_clear(window, sfBlack);
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

