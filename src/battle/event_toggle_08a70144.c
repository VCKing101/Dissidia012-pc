typedef struct ToggleEvent {
    char pad_00[0x8];
    int kind;
    int on;
} ToggleEvent;

extern int func_08A75D38(void *self, unsigned char on, int arg2, int arg3, int arg4);

/* Event kind 3: forwards the on/off value. */
int func_08A70144(void *self, ToggleEvent *event, void *arg2, void *arg3)
{
    if (event == 0 || arg2 == 0 || arg3 == 0) {
        return -1;
    }
    if (event->kind != 3) {
        return -2;
    }
    if (func_08A75D38(self, event->on != 0, 0, -1, 0) != 0) {
        return -3;
    }
    return 0;
}
