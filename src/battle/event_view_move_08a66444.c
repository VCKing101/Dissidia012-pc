/* Starts a camera move on the stage object's view point at the given
 * level (-1 closed, -2 no fighter, -3 no stage object, -4 not ready, -5 no
 * view, -6 the move was refused). */
typedef struct ViewTarget {
    unsigned char pad_00[0x60];
    unsigned char point[4];
} ViewTarget;

typedef struct ViewInfo {
    unsigned char pad_00[4];
    void *fighter;
} ViewInfo;

typedef struct ViewEvent {
    unsigned char open;
    unsigned char pad_01[0x63];
    void *owner;
} ViewEvent;

extern ViewInfo *func_08AB3264(void *owner);
extern void *func_08A973A8(ViewInfo *info);
extern int func_088377C0(void *body);
extern ViewTarget *func_08B30898(void *body);
extern void *func_08AB3274(void *owner);
extern float func_08A82764(void *settings, int id);
extern int func_08A72B68(ViewEvent *self, void *point, int mode, unsigned char a, float time, unsigned char b, unsigned char c,
                         int d);

int func_08A66444(ViewEvent *self, unsigned char a, unsigned char b, unsigned char c, int d)
{
    void *stage;
    ViewTarget *view;
    float time;
    if (!self->open) {
        return -1;
    }
    if (func_08AB3264(self->owner)->fighter == 0) {
        return -2;
    }
    stage = func_08A973A8(func_08AB3264(self->owner));
    if (stage == 0) {
        return -3;
    }
    if (func_088377C0(stage) == 0) {
        return -4;
    }
    view = func_08B30898(stage);
    if (view == 0) {
        return -5;
    }
    time = func_08A82764(func_08AB3274(self->owner), 6);
    if (func_08A72B68(self, view->point, 4, a, time, b, c, d) != 0) {
        return -6;
    }
    return 0;
}
