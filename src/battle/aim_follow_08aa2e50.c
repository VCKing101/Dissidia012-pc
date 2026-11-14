typedef struct AimSub {
    unsigned int flags;
    char pad_04[0x10];
    float a;
    float b;
    float c;
} AimSub;

typedef struct AimData {
    float x;
    float y;
    AimSub sub;
} AimData;

typedef struct AimFollow {
    char pad_00[0x4];
    int done;
    char pad_08[0x10];
    int unit;
} AimFollow;

extern int func_08AA3488(AimFollow *self);
extern int func_08AB3264(int unit);
extern AimData *func_08A978EC(int info);
extern void func_08AA3E44(AimFollow *self, int mode, float x, float y, float a, float b, float c);

int func_08AA2E50(AimFollow *self)
{
    AimData *data;
    AimSub *sub;
    if (func_08AA3488(self) == 1) {
        self->done = 1;
        return 0;
    }
    data = func_08A978EC(func_08AB3264(self->unit));
    sub = &data->sub;
    if (sub->flags & 2) {
        func_08AA3E44(self, 0, data->x, data->y, sub->a, sub->b, sub->c);
    }
    return 0;
}
