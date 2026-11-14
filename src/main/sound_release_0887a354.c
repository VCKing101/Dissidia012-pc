typedef struct SoundHandle { int field_00; int handle; unsigned char streamed; } SoundHandle;
extern int func_0887CFE4(void);
extern void func_0887D45C(int mgr, int handle);
extern void func_0887BA5C(int handle);
extern void func_0887A344(SoundHandle *self);
void func_0887A354(SoundHandle *self)
{
    if (self->handle != 0) {
        if (self->streamed == 1) {
            func_0887D45C(func_0887CFE4(), self->handle);
        } else if (self->handle != 0) {
            func_0887BA5C(self->handle);
            self->handle = 0;
        }
    }
    func_0887A344(self);
}
