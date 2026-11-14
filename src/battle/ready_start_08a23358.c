typedef struct ReadyStart23358 {
    unsigned char active;
} ReadyStart23358;

extern unsigned char func_08A23454(ReadyStart23358 *self);
extern void func_08A239DC(ReadyStart23358 *self);
extern void func_08A23A7C(ReadyStart23358 *self);
extern void func_08A23DA8(ReadyStart23358 *self, int value);

int func_08A23358(ReadyStart23358 *self)
{
    if (!func_08A23454(self)) {
        return -1;
    }
    func_08A239DC(self);
    func_08A23A7C(self);
    func_08A23DA8(self, 0);
    return 0;
}
