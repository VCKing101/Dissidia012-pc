/* Waits for the shared menu state to go idle, then opens the menu. */
typedef struct MenuShared088ACA20 {
    unsigned char field_00[3];
    unsigned char busy;
} MenuShared088ACA20;

typedef struct Menu088ACA20 {
    char pad_00[0xEF8];
    unsigned int flags;
    unsigned char opened;
} Menu088ACA20;

extern MenuShared088ACA20 *D_089D7280;
extern int D_089D69E0;

void func_08883F9C(int task, int count, int flags);
void func_088AD460(Menu088ACA20 *menu);
void func_088AC1AC(Menu088ACA20 *menu, int show, unsigned char opened);

void func_088ACA20(Menu088ACA20 *menu)
{
    while (D_089D7280->busy != 0) {
        func_08883F9C(D_089D69E0, 1, 0);
    }
    menu->flags |= 1;
    menu->flags |= 0x10000;
    menu->flags |= 2;
    func_088AD460(menu);
    menu->opened = 1;
    func_088AC1AC(menu, 1, menu->opened);
}
