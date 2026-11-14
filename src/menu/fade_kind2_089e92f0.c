/* Fade helper of the arcade menu: sets fade mode 2 and starts a fade of
   `time` frames that calls `done` when it ends. */
extern void func_08873DEC(int kind, int mode);
extern void func_08873D34(float time, int kind, void (*done)(void), int arg, int one, int two);

void func_089E92F0(void *self, int time, void (*done)(void))
{
    func_08873DEC(2, 2);
    func_08873D34((float)time, 2, done, 0, 1, 2);
}
