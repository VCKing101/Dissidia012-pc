/* If the timer at +0x6C has reached 16, opens the menu: runs the enter hook, looks up the cursor object, sets two handlers and installs the controller from +0x6D0. */
typedef struct S { char p[4]; char q[0x68]; float t; char r[0x660]; int ctl; } S;
extern void func_08914B74(S *self);
extern int func_08911664(int x);
extern void func_08B48540(int o, int v);
extern void func_08B485A0(int slot, void *fn);
extern void func_08B485B8(int ctl);
extern void func_08865184(void *p);
extern int D_089D83A8;
extern void func_08917714(void);
extern void func_08917758(void);
void func_089148E8(S *self) {
    S *keep[1];
    if (self->t < 16.0f) return;
    keep[0] = self;
    func_08914B74(self);
    func_08B48540(func_08911664(D_089D83A8), -1);
    func_08B485A0(0, func_08917714);
    func_08B485A0(1, func_08917758);
    func_08B485B8(keep[0]->ctl);
    func_08865184((char *)keep[0] + 4);
}
