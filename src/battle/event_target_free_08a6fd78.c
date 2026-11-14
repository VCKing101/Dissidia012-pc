typedef struct BodySlot {
    char pad_00[0x4];
    void *body;
} BodySlot;

typedef struct TargetWatcher {
    unsigned char active;
    unsigned char triggered;
    char pad_02[0x62];
    int id;
} TargetWatcher;

extern BodySlot *func_08AB3264(int id);
extern int func_088377C0(void *body);

int func_08A6FD78(TargetWatcher *self, void *event, void *arg2, void *arg3)
{
    void *body;

    if (event == 0 || arg2 == 0 || arg3 == 0) {
        return -1;
    }
    body = func_08AB3264(self->id)->body;
    if (body == 0) {
        return -2;
    }
    if (func_088377C0(body) == 0) {
        unsigned char one = 1;
        self->triggered = one;
    }
    return 0;
}
