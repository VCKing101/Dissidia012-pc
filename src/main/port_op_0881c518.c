/* Runs an operation on the device's port unless the device is idle (state 1):
   -1 when inactive, -2 when the port call fails. */
typedef struct Device0881C518 {
    unsigned char active;
    char pad_01[7];
    unsigned char state;
    char pad_09[0xB];
    unsigned char port[4];
} Device0881C518;

int func_08816A00(void *port, int arg);

int func_0881C518(Device0881C518 *self, int arg)
{
    if (self->active == 0) {
        return -1;
    }
    if (self->state == 1) {
        return 0;
    }
    if (func_08816A00(self->port, arg) != 0) {
        return -2;
    }
    return 0;
}
