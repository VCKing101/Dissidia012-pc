struct Event1FBE0 {
    short id;
    int a;
    int b;
};

struct Work1FBE0 {
    char pad_00[0x25C88];
    int count;
    int pad_8C;
    int head;
    int pad_94[2];
    Event1FBE0 events[4];
};

struct Screen1FBE0 {
    char pad_00[0x154];
    Work1FBE0 *work;
};

extern "C" void func_08A1FBE0(Screen1FBE0 *self, int id, int a, int b)
{
    if (self->work->count < 4) {
        self->work->events[self->work->head].id = id;
        self->work->events[self->work->head].a = a;
        self->work->events[self->work->head].b = b;
        self->work->head++;
        if (self->work->head >= 4) {
            self->work->head = 0;
        }
        self->work->count++;
    }
}
