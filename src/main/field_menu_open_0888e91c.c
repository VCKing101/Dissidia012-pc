/* Opens the field menu page for the current field mode. */
typedef struct FieldCtl {
    char pad_000[0x134];
    int page;
    int last_page;
} FieldCtl;

extern int D_089D7F48;
extern int D_08A23FF0;
extern int D_08A6D710;

extern void func_08A1B278(int menu, int page, int flag);
extern void func_08A6CF2C(int menu);

void func_0888E91C(FieldCtl *self)
{
    switch (D_089D7F48) {
    case 0:
        if (self->last_page != self->page) {
            func_08A1B278(D_08A23FF0, 8, 0);
        } else {
            func_08A1B278(D_08A23FF0, 0, 0);
        }
        break;
    case 1:
        func_08A6CF2C(D_08A6D710);
        break;
    }
}
