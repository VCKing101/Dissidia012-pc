/* If a sub-object exists, runs the updater and, when its state is 7, calls the follow-up routine. */
extern "C" void func_08B54DDC(void);
extern "C" void func_08B54200(void *);
struct Sub { char d[0x184]; int state; };
struct Self { char d[0x13C]; Sub *sub; };
struct H { static bool is7(Sub *s) { return s->state == 7; }
 static void run(Self *self) {
    Self *s = self;
    Sub *sub = s->sub;
    if (sub == 0) return;
    func_08B54DDC();
    if (is7(s->sub) != 1) return;
    func_08B54200(s);
 } };
extern "C" void func_08B5421C(Self *self) { H::run(self); }
