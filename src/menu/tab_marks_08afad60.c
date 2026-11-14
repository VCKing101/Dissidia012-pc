/* Marks the five tabs: current (frame 8, effect on), previous (10) and the rest (9). */
typedef struct MarkTabs {
    char pad_0000[0x63C];
    void *field_63C[5];
    char pad_0650[0x9FC - 0x650];
    void *field_9FC[5];
    char pad_0A10[0xB40 - 0xA10];
    unsigned int field_B40;
    char pad_0B44[0x2E3C - 0xB44];
    int field_2E3C;
    char pad_2E40[0x2E68 - 0x2E40];
    int field_2E68;
} MarkTabs;

typedef struct MarkTabsOwner {
    char pad_000[0x140];
    MarkTabs *field_140;
} MarkTabsOwner;

extern void func_088D9F5C(void *icon, int frame, int one);
extern void func_0887B924(void *effect);
extern void func_0887B944(void *effect);
extern void func_08AFAFC0(MarkTabsOwner *self);

void func_08AFAD60(MarkTabsOwner *self)
{
    MarkTabs *tabs = self->field_140;
    int current = tabs->field_2E3C;
    int previous;
    int i;
    if ((tabs->field_B40 & 0x200) != 0) {
        previous = current;
        current = tabs->field_2E68;
    } else {
        previous = -1;
    }
    for (i = 0; i < 5; i++) {
        void *icon = self->field_140->field_63C[i];
        if (current == i) {
            func_088D9F5C(icon, 8, 1);
            func_0887B924(self->field_140->field_9FC[i]);
        } else if (i == previous) {
            func_088D9F5C(icon, 10, 1);
            func_0887B944(self->field_140->field_9FC[i]);
        } else {
            func_088D9F5C(icon, 9, 1);
            func_0887B944(self->field_140->field_9FC[i]);
        }
    }
    func_08AFAFC0(self);
}
