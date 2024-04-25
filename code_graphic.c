#include <SFML/Graphics.h>
#include <math.h>

#define PI 3.14159265

void drawArrow(sfRenderTexture* texture, sfVector2f start, sfVector2f end, sfBool horizontal) {
    // ���������� ���� ����� ���� X � ������, ����������� start � end
    float angle = atan2f(end.y - start.y, end.x - start.x) * 180 / PI;

    // ���������� �� ��������� ����� �� �������� �����
    float length = sqrtf(powf(end.x - start.x, 2) + powf(end.y - start.y, 2));

    // �������� ������������
    sfConvexShape* arrow = sfConvexShape_create();
    sfConvexShape_setPointCount(arrow, 3);
    sfConvexShape_setPoint(arrow, 0, (sfVector2f){0, 0});
    sfConvexShape_setPoint(arrow, 1, (horizontal ? (sfVector2f){-10, -5} : (sfVector2f){10, -5}));
    sfConvexShape_setPoint(arrow, 2, (horizontal ? (sfVector2f){-10, 5} : (sfVector2f){10, 5}));
    sfConvexShape_setFillColor(arrow, sfWhite);
    sfConvexShape_setRotation(arrow, angle);
    sfConvexShape_setPosition(arrow, end);

    // ��������� ���������
    sfRenderTexture_drawConvexShape(texture, arrow, NULL);
}

int main() {
    // �������� ���� �������� 800x600
    sfVideoMode mode = {800, 600, 32};
    sfRenderWindow* window = sfRenderWindow_create(mode, "Coordinate Axes", sfResize | sfClose, NULL);
    if (!window)
        return 1;

    // �������� ������������ ������� ��� ���������
    sfRenderTexture* texture = sfRenderTexture_create(800, 600, 0);
    sfRenderTexture_clear(texture, sfBlack);

    // ��������� �������������� ���
    sfVertex horizontalLine[] = {
        {{100, 300}, sfWhite},
        {{700, 300}, sfWhite}
    };
    sfRenderTexture_drawPrimitives(texture, horizontalLine, 2, sfLines, NULL);
    drawArrow(texture, (sfVector2f){100, 300}, (sfVector2f){700, 300}, sfTrue);

    // ��������� ������� �� �������������� ���
    sfFont* font = sfFont_createFromFile("arial.ttf");
    sfText* textX = sfText_create();
    sfText_setFont(textX, font);
    sfText_setCharacterSize(textX, 20);
    sfText_setString(textX, "x");
    sfText_setPosition(textX, (sfVector2f){690, 300});
    sfText_setFillColor(textX, sfWhite);
    sfRenderTexture_drawText(texture, textX, NULL);

    // ��������� ������� �� �������������� ���
    for (int i = 1; i < 30; ++i) {
        sfVertex tick[] = {
            {{100 + i * 20, 295}, sfWhite},
            {{100 + i * 20, 305}, sfWhite}
        };
        sfRenderTexture_drawPrimitives(texture, tick, 2, sfLines, NULL);
    }

    // ��������� ������������ ���
    sfVertex verticalLine[] = {
        {{400, 100}, sfWhite},
        {{400, 500}, sfWhite}
    };
    sfRenderTexture_drawPrimitives(texture, verticalLine, 2, sfLines, NULL);
    drawArrow(texture, (sfVector2f){400, 99}, (sfVector2f){400, 100}, sfFalse);

    // ��������� ������� �� ������������ ���
    sfText* textY = sfText_create();
    sfText_setFont(textY, font);
    sfText_setCharacterSize(textY, 20);
    sfText_setString(textY, "y");
    sfText_setPosition(textY, (sfVector2f){380, 90});
    sfText_setFillColor(textY, sfWhite);
    sfRenderTexture_drawText(texture, textY, NULL);

    // ��������� ������� �� ������������ ���
    for (int i = 1; i < 20; ++i) {
        sfVertex tick[] = {
            {{395, 100 + i * 20}, sfWhite},
            {{405, 100 + i * 20}, sfWhite}
        };
        sfRenderTexture_drawPrimitives(texture, tick, 2, sfLines, NULL);
    }

    // ��������� ����� �� ����
    sfRenderTexture_display(texture);
    sfSprite* sprite = sfSprite_create();
    sfSprite_setTexture(sprite, sfRenderTexture_getTexture(texture), sfTrue);

    // �������� ���� ����������
    while (sfRenderWindow_isOpen(window)) {
        sfEvent event;
        while (sfRenderWindow_pollEvent(window, &event)) {
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
        }
        sfRenderWindow_clear(window, sfBlack);
        sfRenderWindow_drawSprite(window, sprite, NULL);
        sfRenderWindow_display(window);
    }
	// ������� ��������
    sfSprite_destroy(sprite);
    sfRenderTexture_destroy(texture);
    sfRenderWindow_destroy(window);
    sfFont_destroy(font);
    sfText_destroy(textX);
    sfText_destroy(textY);

    return 0;
}
