typedef struct EventUnit EventUnit;

typedef struct EventSlot {
    char pad_00[0x4];
    EventUnit *body;
} EventSlot;

typedef struct EventCtrl {
    char pad_00[0x4E];
    unsigned char force;
    char pad_4F[0x15];
    int id;
    void *view;
} EventCtrl;

extern EventSlot *func_08AB3264(int id);
extern char *func_08A973A8(EventSlot *slot);
extern int func_08A75A14(EventCtrl *self, void *target, int a, int b, int c, int d);
extern int func_08A6CCB4(EventCtrl *self, int a, int b, int c, int d, int e);
extern int func_08A6B904(EventCtrl *self, int a);
extern unsigned char func_08A6B9D8(EventCtrl *self, unsigned char force);
extern void func_08A969BC(void *view, int on);
extern void func_08A97098(void *view, int mode);
extern int func_08A84124(EventUnit *body);
extern int func_08A6C68C(EventCtrl *self, int kind);

/* Event: locks on to the unit's target; -1 bad event, -2 no unit, -3 no target, -4 lock failed. */
int func_08A719EC(EventCtrl *self, void *event, void *a, void *b)
{
    char *target;
    if (event == 0 || a == 0 || b == 0) {
        return -1;
    }
    if (func_08AB3264(self->id)->body == 0) {
        return -2;
    }
    target = func_08A973A8(func_08AB3264(self->id));
    if (target == 0) {
        return -3;
    }
    if (func_08A75A14(self, target + 0x80, 1, 0, 0, 0) != 0) {
        return -4;
    }
    return 0;
}
