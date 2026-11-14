typedef struct Port { unsigned char field_00; unsigned char open; } Port;
extern int func_08875978(Port *self);
int func_08875980(Port *self)
{
    if (func_08875978(self) == 0) {
        return -1;
    }
    self->open = 1;
    return 0;
}
int func_088759C0(Port *self)
{
    if (func_08875978(self) == 0) {
        return -1;
    }
    self->open = 0;
    return 0;
}
