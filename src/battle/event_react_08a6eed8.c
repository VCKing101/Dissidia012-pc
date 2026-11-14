typedef struct EventUnit EventUnit;

typedef struct EventSlot {
    char pad_00[0x4];
    EventUnit *body;
} EventSlot;

typedef struct EventCtrl {
    char pad_00[0x64];
    int id;
} EventCtrl;

extern EventSlot *func_08AB3264(int id);
extern int func_08A84124(EventUnit *body);
extern int func_08A6C68C(EventCtrl *self, int kind);

/* Event: -1 bad event, -2 no unit, -3 when an airborne unit cannot take reaction 7. */
int func_08A6EED8(EventCtrl *self, void *event, void *a, void *b)
{
    EventUnit *body;
    if (event == 0 || a == 0 || b == 0) {
        return -1;
    }
    body = func_08AB3264(self->id)->body;
    if (body == 0) {
        return -2;
    }
    if (func_08A84124(body) == 1) {
        if (func_08A6C68C(self, 7) != 0) {
            return -3;
        }
    }
    return 0;
}
