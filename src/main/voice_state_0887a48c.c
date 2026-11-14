typedef struct Voice { int field_00; int handle; unsigned char state; } Voice;
extern int func_0887A484(Voice *self);
unsigned char func_0887A48C(Voice *self)
{
    if (func_0887A484(self) == 0) {
        return 0;
    }
    return self->state;
}
int func_0887A4C4(Voice *self)
{
    if (func_0887A484(self) == 0) {
        return 0;
    }
    return self->handle;
}
