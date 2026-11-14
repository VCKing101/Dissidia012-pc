typedef struct EventUnit EventUnit;

typedef struct EventSlot {
    char pad_00[0x4];
    EventUnit *body;
} EventSlot;

typedef struct EventCtrl {
    char pad_00[0x64];
    int id;
} EventCtrl;

typedef struct HitEvent {
    char pad_00[0x8];
    int kind;
    int reaction;
} HitEvent;

extern EventSlot *func_08AB3264(int id);
extern int func_088377C0(EventUnit *body);
extern int func_08A6C68C(EventCtrl *self, int kind);

/* Hit event: -1 bad event, -2 not a hit, -3 no unit, -4 when a free unit cannot take the reaction. */
int func_08A6EF64(EventCtrl *self, HitEvent *event, void *a, void *b)
{
    EventUnit *body;
    if (event == 0 || a == 0 || b == 0) {
        return -1;
    }
    if (event->kind != 3) {
        return -2;
    }
    body = func_08AB3264(self->id)->body;
    if (body == 0) {
        return -3;
    }
    if (func_088377C0(body) == 0) {
        if (func_08A6C68C(self, event->reaction) != 0) {
            return -4;
        }
    }
    return 0;
}
