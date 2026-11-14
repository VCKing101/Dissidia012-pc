typedef struct EventUnit {
    char pad_00[0x80];
    char pos[0x30];
    char dir[0x10];
} EventUnit;

typedef struct EventSlot {
    char pad_00[0x4];
    EventUnit *body;
} EventSlot;

typedef struct EventCtrl {
    char pad_00[0x1];
    unsigned char inside;
    char pad_02[0x62];
    int id;
} EventCtrl;

extern int D_08ABBDF8;
extern EventSlot *func_08AB3264(int id);
extern void *func_08ABB4B4(int key);
extern int func_08AB7CB4(void *area, void *pos);
extern int func_088377C0(EventUnit *body);
extern int func_08A6D134(EventCtrl *self);
extern float func_089A3BF8(void *vec);

/* Event: -1 bad event, -2 no unit or busy. */
int func_08A70A3C(EventCtrl *self, void *event, void *a, void *b)
{
    EventUnit *body;
    if (event == 0 || a == 0 || b == 0) {
        return -1;
    }
    body = func_08AB3264(self->id)->body;
    if (body == 0) {
        return -2;
    }
    if (func_08A6D134(self) != 0) {
        return -2;
    }
    if (func_089A3BF8(body->dir) == 0.0f) {
        return 0;
    }
    return 0;
}
