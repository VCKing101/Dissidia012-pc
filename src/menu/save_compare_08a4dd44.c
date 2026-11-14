typedef struct MenuSaveHeader {
    char *field_00;
    char *field_04;
} MenuSaveHeader;

typedef struct MenuSaveState {
    int field_00;
    int field_04;
    char pad_0008[0x6B20 - 0x8];
    char *field_6B20;
    char pad_6B24[0x6FEC - 0x6B24];
    int field_6FEC;
} MenuSaveState;

typedef struct MenuSaveOwner {
    char pad_00[0x5C];
    MenuSaveState *field_5C;
} MenuSaveOwner;

extern MenuSaveHeader *func_08A66AA0(int handle);
extern int func_08935490(char *a, char *b, int size);
extern void func_08A4D07C(MenuSaveOwner *self, int mode, float delay);
extern void func_08A4D718(void);
extern void func_08A4D6F8(void);
extern void func_08A4E2E4(MenuSaveOwner *self, int mode, void (*yes)(void), void (*no)(void), void (*cancel)(void),
                          int text, int a, int b, int c);

void func_08A4DD44(MenuSaveOwner *self) {
    MenuSaveHeader *header = func_08A66AA0(self->field_5C->field_6FEC);
    int first = func_08935490(self->field_5C->field_6B20, header->field_00, 0x160);
    int second = func_08935490(self->field_5C->field_6B20 + 0x160, header->field_04, 0x3FC);

    if (first == 0 && second == 0) {
        func_08A4D07C(self, 0, 0.0f);
        self->field_5C->field_04 = 0x15;
    } else {
        func_08A4E2E4(self, 0, func_08A4D718, func_08A4D6F8, func_08A4D6F8, 0x29, -1, -1, -1);
    }
}
