typedef struct BodySlot {
    char pad_00[0x4];
    void *body;
} BodySlot;

typedef struct GuardWatcher {
    unsigned char active;
    unsigned char done;
    unsigned char blocked;
    char pad_03[0x61];
    int id;
    int handle;
} GuardWatcher;

extern BodySlot *func_08AB3264(int id);
extern int func_08AB344C(void *body);
extern void func_08A96BAC(int handle, int mode);

int func_08A70480(GuardWatcher *self, void *event, void *arg2, void *arg3)
{
    void *body;

    if (event == 0 || arg2 == 0 || arg3 == 0) {
        return -1;
    }
    body = func_08AB3264(self->id)->body;
    if (body == 0) {
        return -2;
    }
    if (func_08AB344C(body) == 0) {
        self->blocked = 1;
        return 0;
    }
    func_08A96BAC(self->handle, 2);
    self->done = 1;
    return 0;
}
