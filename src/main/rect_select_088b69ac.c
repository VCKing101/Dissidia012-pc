typedef struct Rect { short x; short y; short w; short h; } Rect;
typedef struct RectTable { unsigned short count; Rect rects[1]; } RectTable;
extern Rect D_09B4D488;
extern RectTable *D_089D733C;

Rect *func_088B69AC(int index)
{
    D_09B4D488.x = 0;
    D_09B4D488.y = 0;
    D_09B4D488.h = 100;
    D_09B4D488.w = 100;
    if (D_089D733C != 0) {
        if (index <= 0 || index >= D_089D733C->count) {
            index = 1;
        }
        index--;
        D_09B4D488 = D_089D733C->rects[index];
    }
    return &D_09B4D488;
}
