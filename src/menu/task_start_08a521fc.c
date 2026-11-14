typedef struct MenuTask {
    int field_00;
    void *field_04;
} MenuTask;

typedef struct MenuTaskOwner {
    char pad_00[0x1C];
    MenuTask *field_1C;
    int field_20;
} MenuTaskOwner;

extern char D_08A522F8[];
extern void func_08A51FD4(void);
extern MenuTask *func_088823C4(int parent, int a, int b, void (*update)(void), char *name);

void func_08A521FC(MenuTaskOwner *self, int parent, int a, int b) {
    MenuTask *task;

    self->field_20 = parent;
    task = func_088823C4(parent, a, b, func_08A51FD4, D_08A522F8);
    self->field_1C = task;
    task->field_04 = self;
}
