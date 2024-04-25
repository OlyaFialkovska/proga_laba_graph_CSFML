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
    float xmax = 2 * PI; // Максимальное значение аргумента
    float xmin = 0;      // Минимальное значение аргумента
    float dx = (xmax - xmin) / (numPoints - 1); // Шаг изменения аргумента
    float ymin = 0;      // Минимальное значение функции
    float ymax = 0;      // Максимальное значение функции

    float t, g, x, y;

    // Вычисляем минимальное и максимальное значения функции
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

    // Малюем координатные прямые Ox и Oy
    sfVertexArray *Ox = sfVertexArray_create();
    sfVertexArray *Oy = sfVertexArray_create();
    sfVertex vertex;

    // Строим ось Y
    for (int i = -525; i < 525; ++i) {
        vertex.position.x = WINDOW_WIDTH / 2; // Центрируем по X
        vertex.position.y = WINDOW_HEIGHT / 2 - i; // Центрируем по Y
        vertex.color = sfColor_fromRGB(255, 255, 255);
        sfVertexArray_append(Oy, vertex);
    }

    // Строим ось X
    for (int i = -400; i < 400; ++i) {
        vertex.position.x = WINDOW_WIDTH / 2 + i; // Центрируем по X
        vertex.position.y = WINDOW_HEIGHT / 2; // Центрируем по Y
        vertex.color = sfColor_fromRGB(255, 255, 255); // Белый цвет
        sfVertexArray_append(Ox, vertex);
    }

    // Отображаем координатные прямые
    sfRenderWindow_drawVertexArray(window, Ox, NULL);
    sfRenderWindow_drawVertexArray(window, Oy, NULL);

    // Малюем координатные оси
    sfVertex xAxis[] = { {0, WINDOW_HEIGHT / 2}, {WINDOW_WIDTH, WINDOW_HEIGHT / 2} };
    sfVertex yAxis[] = { {WINDOW_WIDTH / 2, 0}, {WINDOW_WIDTH / 2, WINDOW_HEIGHT} };
    sfVertexArray *axes = sfVertexArray_create();
    sfVertexArray_append(axes, xAxis[0]);
    sfVertexArray_append(axes, xAxis[1]);
    sfVertexArray_append(axes, yAxis[0]);
    sfVertexArray_append(axes, yAxis[1]);
    sfVertexArray_setPrimitiveType(axes, sfLines);
    sfRenderWindow_drawVertexArray(window, axes, NULL);

    // Малюем график функции
    for (int i = 0; i < numPoints; ++i) {
        t = xmin + i * dx;
        g = (A / B) * t;

        x = (A - B) * cos(t) + D * cos(g);
        y = (A - B) * sin(t) - D * sin(g);

        sfVertex vertex = { { WINDOW_WIDTH / 2 + x * mx, WINDOW_HEIGHT / 2 - y * my }, sfWhite };
        sfVertexArray_append(lines, vertex);
    }

    sfRenderWindow_drawVertexArray(window, lines, NULL);

    // Добавляем метки на оси X и Y в соответствии с масштабом
    sfFont *font = sfFont_createFromFile("arial.ttf");
    sfText *text = sfText_create();
    sfText_setFont(text, font);
    sfText_setCharacterSize(text, 10);
    sfText_setFillColor(text, sfWhite);
    sfText_setOutlineColor(text, sfBlack);
    sfText_setOutlineThickness(text, 1);

    // Метки на ось X
    int xInterval = 50 * mx; // Интервал между метками на оси X
    for (int i = -400; i < 400; i += xInterval) {
        char buffer[16];
        snprintf(buffer, sizeof(buffer), "%d", i);
        sfText_setString(text, buffer);
        sfFloatRect textBounds = sfText_getLocalBounds(text);
        sfText_setPosition(text, (sfVector2f){WINDOW_WIDTH / 2 + i - textBounds.width / 2, WINDOW_HEIGHT / 2 + 5}); // Позиция текста
        sfRenderWindow_drawText(window, text, NULL);
    }
    // Метки на ось Y
    int yInterval = 50 * my; // Интервал между метками на оси Y
    for (int i = -525; i < 525; i += yInterval) {
        char buffer[16];
        snprintf(buffer, sizeof(buffer), "%d", -i);
        sfText_setString(text, buffer);
        sfFloatRect textBounds = sfText_getLocalBounds(text);
        sfText_setPosition(text, (sfVector2f){WINDOW_WIDTH / 2 + 5, WINDOW_HEIGHT / 2 - i - textBounds.height / 2}); // Позиция текста
        sfRenderWindow_drawText(window, text, NULL);
    }

    sfText_destroy(text);
    sfFont_destroy(font);
}

int main() {
    sfVideoMode mode = { WINDOW_WIDTH, WINDOW_HEIGHT, 32 };
    sfRenderWindow *window = sfRenderWindow_create(mode, "Graph", sfClose, NULL);

    sfVertexArray *lines = sfVertexArray_create();

    float mx = 1.0f; // Масштаб по оси X
    float my = 1.0f; // Масштаб по оси Y

    while (sfRenderWindow_isOpen(window)) {
        sfEvent event;
        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
            else if (event.type == sfEvtMouseWheelScrolled) {
                if (event.mouseWheelScroll.delta > 0) {
                    mx *= 1.1f; // Увеличение масштаба при прокрутке вверх
                    my *= 1.1f;
                } else if (event.mouseWheelScroll.delta < 0) {
                    mx *= 0.9f; // Уменьшение масштаба при прокрутке вниз
                    my *= 0.9f;
                }
            }
        }

        sfRenderWindow_clear(window, sfBlack);
        updateGraph(window, lines, mx, my);
        sfRenderWindow_display(window);
    }

    sfVertexArray_destroy(lines);
    sfRenderWindow_destroy(window);

    return 0;
}

