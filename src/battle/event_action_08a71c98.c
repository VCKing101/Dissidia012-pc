typedef struct ActionRef {
    int action;
} ActionRef;

typedef struct EventUnit {
    char pad_000[0x3A4];
    ActionRef *action;
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
    void *view;
} EventCtrl;

extern EventSlot *func_08AB3264(int id);
extern void func_08AB3294(int id);
extern void func_08A97060(void *view, int mode);

/* Event: shows mode 0x15 during actions 0x13 and 0x19, otherwise marks the controller; -1 bad event, -2 no unit. */
int func_08A71C98(EventCtrl *self, void *event, void *a, void *b)
{
    EventUnit *body;
    int action;
    if (event == 0 || a == 0 || b == 0) {
        return -1;
    }
    body = func_08AB3264(self->id)->body;
    if (body == 0) {
        return -2;
    }
    func_08AB3294(self->id);
    action = body->action->action;
    if (action != 0x13 && action != 0x19) {
        self->inside = 1;
    } else {
        func_08A97060(self->view, 0x15);
    }
    return 0;
}
