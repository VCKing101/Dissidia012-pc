extern "C" {
/* Closes the menu unless it is locked (state 1): 1 when closed. */
typedef struct Lock0888B014 {
    unsigned char state;
} Lock0888B014;

typedef struct Game0888B014 {
    char pad_000[0x7CC];
    void *menu;
} Game0888B014;

extern Lock0888B014 *D_089D7280;
extern void *D_089D7260;
extern Game0888B014 *D_089D6A10;

void func_088AF044(void *ui);
void func_088AEF98(void *ui);
void func_08890A00(void *menu);

bool func_0888B014(void)
{
    if (D_089D7280->state == 1) {
        return false;
    }
    func_088AF044(D_089D7260);
    func_088AEF98(D_089D7260);
    func_08890A00(D_089D6A10->menu);
    D_089D6A10->menu = 0;
    return true;
}
}
