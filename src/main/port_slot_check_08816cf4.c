/* 1 when the port is open and the slot's state word is 1. */
typedef struct Port08816CF4 {
    unsigned char active;
    unsigned char unknown_01[3];
    unsigned char pool[4];
} Port08816CF4;

int *func_0881B35C(void *pool, unsigned int index);

int func_08816CF4(Port08816CF4 *self, unsigned int index)
{
    int *slot;
    if (self->active == 0) {
        return 0;
    }
    slot = func_0881B35C(self->pool, index);
    if (slot == 0) {
        return 0;
    }
    return *slot == 1;
}
