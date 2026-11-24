/* Runs the handler for the current phase value; phase 0 advances to phase 100. */
extern void func_08A89480(void *self);
extern void func_08A8A548(void *self);
extern void func_08A8936C(void *self);
extern void func_08A89624(void *self);
extern void func_08A89574(void *self);

typedef struct P89288 { char pad[0x420]; int phase; } P89288;
typedef struct S89288 { char pad[0x30C]; P89288 *p; } S89288;

void func_08A89288(S89288 *self)
{
    switch (self->p->phase) {
    case 0:
        self->p->phase = 100;
        break;
    case 100:
        func_08A8936C(self);
        break;
    case 200:
        func_08A8A548(self);
        break;
    case 300:
        func_08A89480(self);
        break;
    case 400:
        func_08A89574(self);
        break;
    case 0x19A:
        func_08A89624(self);
        break;
    case 500:
        break;
    case 600:
        break;
    case 10000:
        break;
    default:
        break;
    }
}
