#include <SFML/Graphics.h>
#include <math.h>
#include <stdio.h> // ��� sprintf

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define PI 3.14159265

// ������� ��� ���������� ����������� ������ ��������
int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

float A = 200.0f;
float B = 50.0f;
float D = 50.0f;

// �������, ������� ���������� �������� �������� �������
float function(float x) {
    // �������� �� ���� �������
    return sin(x);
}

void drawArrow(sfRenderWindow* window, sfConvexShape* arrow, sfVector2f position, float rotation, sfColor fillColor) {
    sfConvexShape_setPointCount(arrow, 3);
    sfConvexShape_setPoint(arrow, 0, (sfVector2f) {100, 200});
    sfConvexShape_setPoint(arrow, 1, (sfVector2f) {150, 150});
    sfConvexShape_setPoint(arrow, 2, (sfVector2f) {100, 100});

    sfConvexShape_setScale(arrow, (sfVector2f) {0.2f, 0.2});
    sfConvexShape_setRotation(arrow, rotation);
    sfConvexShape_setFillColor(arrow, fillColor);
    sfConvexShape_setPosition(arrow, position);

    sfRenderWindow_drawConvexShape(window, arrow, NULL);
}

void drawCenterLines(sfRenderWindow *window) {
    // �������� �������������� �����
// �������� �������������� �����
sfVertexArray *horizontalLine = sfVertexArray_create();
sfVertexArray_setPrimitiveType(horizontalLine, sfLines); // �������� ��� ������
sfVertex horizontalVertex1 = { {0, WINDOW_HEIGHT / 2}, sfWhite };
sfVertex horizontalVertex2 = { {WINDOW_WIDTH, WINDOW_HEIGHT / 2}, sfWhite };
sfVertexArray_append(horizontalLine, horizontalVertex1);
sfVertexArray_append(horizontalLine, horizontalVertex2);
sfRenderWindow_drawVertexArray(window, horizontalLine, NULL);

// �������� ������������ �����
sfVertexArray *verticalLine = sfVertexArray_create();
sfVertexArray_setPrimitiveType(verticalLine, sfLines); // �������� ��� ������
sfVertex verticalVertex1 = { {WINDOW_WIDTH / 2, 0}, sfWhite };
sfVertex verticalVertex2 = { {WINDOW_WIDTH / 2, WINDOW_HEIGHT}, sfWhite };
sfVertexArray_append(verticalLine, verticalVertex1);
sfVertexArray_append(verticalLine, verticalVertex2);
sfRenderWindow_drawVertexArray(window, verticalLine, NULL);


    // ������������ ��������
    sfVertexArray_destroy(horizontalLine);
    sfVertexArray_destroy(verticalLine);
}


void updateGraph(sfRenderWindow *window, sfVertexArray *lines, float *mx, float *my, float scale, sfText *textxVect[], sfText *textyVect[]) {
    sfVertexArray_clear(lines);
    int numPoints = 1000;
    float n = B / gcd(A, B); // ����� n, ������� �������� ����������, ����� B ���� �����
    float xmax = 2 * PI * n; // ����� ������������ �������� ���������
    float xmin = 0;      // ����������� �������� ���������
    float dx = (xmax - xmin) / (numPoints - 1); // ��� ��������� ���������
    float ymin = 0;      // ����������� �������� �������
    float ymax = 0;      // ������������ �������� �������

    float t, g, x, y;

    // ����������� ������������ � ������������� �������� �������
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

    // ����������� ������������ ���� Ox � Oy
    sfVertexArray *Ox = sfVertexArray_create();
    sfVertexArray *Oy = sfVertexArray_create();
    sfVertex vertex;

    // ���������� ��� Y
    for (int i = -525; i < 525; ++i) {
        vertex.position.x = WINDOW_WIDTH / 2; // ������������� �� X
        vertex.position.y = WINDOW_HEIGHT / 2 - i; // ������������� �� Y
        vertex.color = sfColor_fromRGB(255, 255, 255);
        sfVertexArray_append(Oy, vertex);
    }
// ���������� ��� X
    for (int i = -400; i < 400; ++i) {
        vertex.position.x = WINDOW_WIDTH / 2 + i; // ������������� �� X
        vertex.position.y = WINDOW_HEIGHT / 2; // ������������� �� Y
        vertex.color = sfColor_fromRGB(255, 255, 255); // ����� ����
        sfVertexArray_append(Ox, vertex);
    }

    // ����������� ������������ ����
    sfVertex xAxis[] = { {0, WINDOW_HEIGHT / 2}, {WINDOW_WIDTH, WINDOW_HEIGHT / 2} };
    sfVertex yAxis[] = { {WINDOW_WIDTH / 2, 0}, {WINDOW_WIDTH / 2, WINDOW_HEIGHT} };
    sfVertexArray *axes = sfVertexArray_create();
    sfVertexArray_append(axes, xAxis[0]);
    sfVertexArray_append(axes, xAxis[1]);
    sfVertexArray_append(axes, yAxis[0]);
    sfVertexArray_append(axes, yAxis[1]);
    sfVertexArray_setPrimitiveType(axes, sfLines);

    // ���������� ������������ �����
    sfVertexArray *verticalLine = sfVertexArray_create();
    sfVertex v1 = { {WINDOW_WIDTH / 2, 0}, sfRed }; // ��������� ����� ������������ �����
    sfVertex v2 = { {WINDOW_WIDTH / 2, WINDOW_HEIGHT}, sfRed }; // �������� ����� ������������ �����
    sfVertexArray_append(verticalLine, v1);
    sfVertexArray_append(verticalLine, v2);

    // ���������� �������������� �����
    sfVertexArray *horizontalLine = sfVertexArray_create();
    sfVertex h1 = { {0, WINDOW_HEIGHT / 2}, sfGreen }; // ��������� ����� �������������� �����
    sfVertex h2 = { {WINDOW_WIDTH, WINDOW_HEIGHT / 2}, sfGreen }; // �������� ����� �������������� �����
    sfVertexArray_append(horizontalLine, h1);
    sfVertexArray_append(horizontalLine, h2);

    // ����������� ���� ��������� ����� � ����
    sfRenderWindow_drawVertexArray(window, verticalLine, NULL);
    sfRenderWindow_drawVertexArray(window, horizontalLine, NULL);
    sfRenderWindow_drawVertexArray(window, axes, NULL);

    // ����������� ������� �������
    for (int i = 0; i < numPoints; ++i) {
        t = xmin + i * dx;
        g = (A / B) * t;

        x = (A - B) * cos(t) + D * cos(g);
        y = (A - B) * sin(t) - D * sin(g);

        sfVertex vertex = { { WINDOW_WIDTH / 2 + x * (*mx), WINDOW_HEIGHT / 2 - y * (*my) }, sfWhite };
        sfVertexArray_append(lines, vertex);
    }

    sfRenderWindow_drawVertexArray(window, lines, NULL);

    // ���������� ����� �� ������������ ���
    sfFont *font = sfFont_createFromFile("arial.ttf");
    // ���������� ����� �� ��� X
    for (int i = 0; i < 50; ++i) {
        sfText_setFont(textxVect[i], font);
        sfText_setCharacterSize(textxVect[i], 16);
        sfText_setFillColor(textxVect[i], sfWhite);
        char bufferx[16];
        snprintf(bufferx, sizeof(bufferx), "%d", -i);
        sfText_setString(textxVect[i], bufferx);
        sfText_setPosition(textxVect[i], (sfVector2f){385-i*scale, 300});
        sfRenderWindow_drawText(window, textxVect[i], NULL);
    }
    for (int i = 50; i < 100; ++i) {
        sfText_setFont(textxVect[i], font);
        sfText_setCharacterSize(textxVect[i], 16);
        sfText_setFillColor(textxVect[i], sfWhite);
        char bufferx[16];
        snprintf(bufferx, sizeof(bufferx), "%d", i-50);
        sfText_setString(textxVect[i], bufferx);
        sfText_setPosition(textxVect[i], (sfVector2f){385+(i-50)*scale, 300});
        sfRenderWindow_drawText(window, textxVect[i], NULL);
    }

    // ���������� ����� �� ��� Y
    for (int i = 0; i < 50; ++i) {
        sfText_setFont(textyVect[i], font);
        sfText_setCharacterSize(textyVect[i], 16);
        sfText_setFillColor(textyVect[i], sfWhite);
        char buffery[16];
        snprintf(buffery, sizeof(buffery), "%d", i);
        sfText_setString(textyVect[i], buffery);
        sfText_setPosition(textyVect[i], (sfVector2f){385, 300-i*scale});
        sfRenderWindow_drawText(window, textyVect[i], NULL);
    }
    for (int i = 50; i < 100; ++i) {
        sfText_setFont(textyVect[i], font);
        sfText_setCharacterSize(textyVect[i], 16);
        sfText_setFillColor(textyVect[i], sfWhite);
        char buffery[16];
        snprintf(buffery, sizeof(buffery), "%d", -1*(i-50));
        sfText_setString(textyVect[i], buffery);
        sfText_setPosition(textyVect[i], (sfVector2f){385, 300+(i-50)*scale});
        sfRenderWindow_drawText(window, textyVect[i], NULL);
    }

    sfFont_destroy(font);
}

int main() {
    // ���������� ����
    sfVideoMode mode = { WINDOW_WIDTH, WINDOW_HEIGHT, 32 };
    sfRenderWindow *window = sfRenderWindow_create(mode, "Graph", sfClose, NULL);

    sfVertexArray *lines = sfVertexArray_create();
    sfConvexShape* arrow = sfConvexShape_create();


    float mx = 1.0f;
    float my = 1.0f;
    float scale = 50.0f;

    // ���������� �������� ��� ������ �� ��� X � Y
    sfText *textxVect[100];
    sfText *textyVect[100];

    // ������������� �������� ������
    for (int i = 0; i < 100; ++i) {
        textxVect[i] = sfText_create();
        textyVect[i] = sfText_create();
    }



    while (sfRenderWindow_isOpen(window)) {
        // ��������� �������
        sfEvent event;
        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
            else if (event.type == sfEvtMouseWheelScrolled) {
                if (event.mouseWheelScroll.delta > 0 && scale < 100) {
                    // ���������� �������� ��� ��������� �����
                    scale *= 1.1f;
                    mx *= 1.1f; // ���������� �������� ��� ��������� �����
                    my *= 1.1f;
                } else if (event.mouseWheelScroll.delta < 0 && scale > 10) {
                    // ���������� �������� ��� ��������� ����
                    scale *= 0.9f;
                    mx *= 0.9f; // ���������� �������� ��� ��������� ����
                    my *= 0.9f;
                }
            }
        }

        // ������� � ���������� ����
        sfRenderWindow_clear(window, sfBlack);
        updateGraph(window, lines, &mx, &my, scale, textxVect, textyVect);
        drawCenterLines(window);
        drawArrow(window, arrow, (sfVector2f) {WINDOW_WIDTH-430, 30}, 270, sfWhite);
        drawArrow(window, arrow, (sfVector2f) {WINDOW_WIDTH-30, WINDOW_HEIGHT-330}, 0, sfWhite);

        sfRenderWindow_display(window);
    }

    // ������������ ��������
    sfVertexArray_destroy(lines);
    sfRenderWindow_destroy(window);

    // ������������ �������� ������
    for (int i = 0; i < 100; ++i) {
        sfText_destroy(textxVect[i]);
        sfText_destroy(textyVect[i]);
    }

    return 0;
}
