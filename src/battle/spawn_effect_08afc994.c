/* Spawns the effect 0x18701 timed to the frame step, plays sound 1, then forwards the slot and argument to func_08AFC870. */
typedef struct FrameClock { char pad[0x18]; float step; } FrameClock;
typedef struct EffectRec { char pad[0x38]; float time; } EffectRec;
struct SlotRec { int a, b, c, d; };
extern FrameClock *D_089D69E0;
extern struct SlotRec D_09B6C288[];
extern EffectRec *func_088A24D4(int a, int id, int b, int c);
extern void func_088A1FBC(EffectRec *e, int a, int b);
extern void func_088A28E8(int a, int b);
extern void func_08AFC870(struct SlotRec *slot, int arg);

void func_08AFC994(int slot, int arg)
{
    EffectRec *e = func_088A24D4(0, 0x18701, 1, 0);
    if (e != 0) {
        e->time = D_089D69E0->step;
        func_088A1FBC(e, 1, 0);
    }
    func_088A28E8(1, 1);
    func_08AFC870(&D_09B6C288[slot], arg);
}
