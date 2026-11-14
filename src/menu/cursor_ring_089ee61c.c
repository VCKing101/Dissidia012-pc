typedef struct MenuCursorItem {
    char data[0x18];
} MenuCursorItem;

typedef struct MenuCursorRing {
    char pad_00[0x10];
    MenuCursorItem field_10[3];
    int field_58;
    int field_5C;
} MenuCursorRing;

extern void func_089EE1E0(MenuCursorItem *item, int selected);

int func_089EE61C(MenuCursorRing *self, int delta) {
    int previous = self->field_5C;
    int next = (previous + self->field_58 + delta) % self->field_58;

    self->field_5C = next;
    if (previous != next) {
        func_089EE1E0(&self->field_10[previous], 0);
        func_089EE1E0(&self->field_10[self->field_5C], 1);
    }
    return self->field_5C != previous;
}
