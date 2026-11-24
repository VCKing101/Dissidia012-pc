/* Queues a request on the task: -1 inactive, -2 bad kind, 0 unchanged or idle, -3 blocked, -4 failed to start. */
struct Owner { char d0[0x324]; int f324; };
struct Task {
    unsigned char active;
    unsigned char ready;
    char d2[2];
    struct Owner *owner;
    char d8[0xC];
    unsigned char f14;
    char d15[3];
    unsigned int kind;
    float v1C;
    unsigned char flag;
    char d21[3];
    float v24;
    char d28[0x7C];
    int cur;
    char dA8[0x88];
    char p130[1];
};
extern void func_08A87DCC(struct Task *t);
extern int func_08A95C10(void *p, int v);
extern int func_08A87E70(struct Task *t);

int func_08A87980(float a, float b, struct Task *t, int id, unsigned int kind, unsigned char flag)
{
    unsigned int m;
    int w;
    if (t->active == 0) {
        return -1;
    }
    if (!(kind < 8)) {
        return -2;
    }
    if (t->ready == 0) {
        return 0;
    }
    func_08A87DCC(t);
    w = t->owner->f324;
    if (w < 0) {
        m = -(w & 1);
    } else {
        m = w & 1;
    }
    if (m == 1) {
        if (func_08A95C10(t->p130, id) != 0) {
            return -3;
        }
    }
    if (t->cur == id) {
        return 0;
    }
    t->kind = kind;
    t->v1C = a;
    t->flag = flag;
    t->v24 = b;
    t->f14 = 1;
    if (func_08A87E70(t) != 0) {
        return -4;
    }
    return 0;
}
