/* Marks the caller's flag (and the event, when it may finish) once the
 * event's scaled condition holds (-1 bad arguments, -2 no timing data). */
typedef struct ScaleInfo {
    unsigned char pad_00[0xC];
    float scale;
} ScaleInfo;

typedef struct ScaleEvent {
    unsigned char open;
    unsigned char marked;
    unsigned char done;
} ScaleEvent;

extern void *D_08A80218;
extern void *func_08A80200(void *battle);
extern ScaleInfo *func_08AACB10(void *stage);
extern int func_08A6CEE0(ScaleEvent *self, int mode, float scale);
extern int func_08A67614(ScaleEvent *self);

int func_08A6F2E4(ScaleEvent *self, void *key, void *context, unsigned char *flag)
{
    ScaleInfo *info;
    if (key == 0 || context == 0 || flag == 0) {
        return -1;
    }
    info = func_08AACB10(func_08A80200(D_08A80218));
    if (info == 0) {
        return -2;
    }
    if (func_08A6CEE0(self, 0, info->scale) != 1) {
        return 0;
    }
    if (func_08A67614(self)) {
        self->done = 1;
    }
    *flag = 1;
    return 0;
}
