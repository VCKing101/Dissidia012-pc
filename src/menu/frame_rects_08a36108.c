/* Fills two frame rectangles at (x, y) and registers the third with the drawer. */
typedef struct FrameRect {
    short field_00;
    short field_02;
    short field_04;
    short field_06;
} FrameRect;

extern void *D_08A457EC;
extern void func_08A41454(void *drawer, FrameRect *rect);

void func_08A36108(void *self, FrameRect *rects, int x, int y)
{
    rects[0].field_00 = y + 5;
    rects[0].field_02 = 9;
    rects[0].field_04 = x + 9;
    rects[0].field_06 = 8;
    rects[1].field_00 = y + 4;
    rects[1].field_02 = 9;
    rects[1].field_04 = x + 9;
    rects[1].field_06 = 9;
    func_08A41454(D_08A457EC, &rects[2]);
}
