typedef struct ChildForward {
    char pad_000[0x4];
    void *child;
} ChildForward;

extern int func_08A220F4(void *child);
extern int func_08A22294(void *child);
extern int func_08A223C0(void *child);

void func_08A22978(ChildForward *self)
{
    if (!self) {
        return;
    }
    if (!self->child) {
        return;
    }
    if (func_08A220F4(self->child)) {
        return;
    }
}

void func_08A229C8(ChildForward *self)
{
    if (!self) {
        return;
    }
    if (!self->child) {
        return;
    }
    if (func_08A22294(self->child)) {
        return;
    }
}

void func_08A22A18(ChildForward *self)
{
    if (!self) {
        return;
    }
    if (!self->child) {
        return;
    }
    if (func_08A223C0(self->child)) {
        return;
    }
}
