typedef struct Port { unsigned char field_00; unsigned char open; } Port;
extern int func_08861050(Port *self);
int func_08861058(Port *self)
{
    if (func_08861050(self) == 0) {
        return -1;
    }
    self->open = 1;
    return 0;
}
int func_08861098(Port *self)
{
    if (func_08861050(self) == 0) {
        return -1;
    }
    self->open = 0;
    return 0;
}
