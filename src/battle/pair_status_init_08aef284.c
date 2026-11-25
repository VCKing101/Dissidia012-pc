/* Initialises a pair-status record from two battle units: handles, remaining-percent values and cleared timers. */
typedef struct PsGauge { char pad0[8]; int cur; int max; } PsGauge;
typedef struct PsUnit {
    char pad0[0x30C];
    int h30C;
    char pad310[0x21C];
    PsGauge *g52C;
    char pad530[0xB84];
    int f10B4;
} PsUnit;
typedef struct PsRec {
    char pad0[8];
    PsUnit *a;
    int a_10B4;
    int a_h;
    int a_pct;
    PsUnit *b;
    int b_10B4;
    int b_h;
    int b_pct;
    char timer0[0x18];
    char timer1[0x18];
    char timer2[0x18];
    char timer3[0x18];
    char pad88[0x100];
    int f188, f18C, f190, f194;
    unsigned char f198;
} PsRec;
extern int func_088CBB3C(int h);
extern int func_088CBB58(int h);
extern void func_0885EB34(void *t, int a, int b, int c, int d);

void func_08AEF284(PsRec *self, PsUnit *a, PsUnit *b)
{
    int ha = a->h30C;
    PsGauge *ga = a->g52C;
    int hb = b->h30C;
    PsGauge *gb = b->g52C;
    int v;
    self->a = a;
    self->a_10B4 = a->f10B4;
    self->a_h = func_088CBB3C(ha);
    v = ga->max - ga->cur;
    v = v < 0 ? 0 : v;
    self->a_pct = v * 100 / ga->max;
    self->b = b;
    self->b_10B4 = b->f10B4;
    self->b_h = func_088CBB58(hb);
    v = gb->max - gb->cur;
    v = v < 0 ? 0 : v;
    self->b_pct = v * 100 / gb->max;
    func_0885EB34(self->timer0, 0, 0, 0, 0);
    func_0885EB34(self->timer1, 0, 0, 0, 0);
    func_0885EB34(self->timer2, 0, 0, 0, 0);
    func_0885EB34(self->timer3, 0, 0, 0, 0);
    self->f18C = 0;
    self->f190 = 0;
    self->f194 = 0;
    self->f198 = 0;
    self->f188 = 0;
}
