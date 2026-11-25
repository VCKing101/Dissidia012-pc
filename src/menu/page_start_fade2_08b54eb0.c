/* Menu page start: fades kind 2, schedules callback func_08B55074 and sets state 1. */
extern int func_088D4C5C(void);
extern void func_08873DEC(int kind, int mode);
extern void func_08873D34(float time, int kind, void (*done)(void), int arg, unsigned char one, int two);
extern void func_088D53BC(void *);
extern void func_08B55074(void);
typedef struct Page77 { char p[0x174]; char snd[0x10]; int state; } Page77;
void func_08B54EB0(Page77 *self) {
    int t = func_088D4C5C();
    func_08873DEC(2, 2);
    func_08873D34((float)t, 2, func_08B55074, (int)self, 1, 2);
    func_088D53BC(self->snd);
    self->state = 1;
}
