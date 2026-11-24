/* Forwards a float and two shorts from a message to func_08B1169C together with a handle; returns 0. */
extern void func_08B1169C(float a, float b, int h, int c);
typedef struct { float x; short y; short z; } Pt;
typedef struct { char pad[4]; Pt pt; } Msg;
typedef struct { char pad[0x30]; int h; } Owner;
int func_08B10F9C(Owner *self, Msg *msg) {
    Pt *p = &msg->pt;
    int h = self->h;
    int y = p->y;
    func_08B1169C(p->x, (float)y, h, p->z);
    return 0;
}
