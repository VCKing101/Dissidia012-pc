typedef struct ReadyMember16C {
    char data[1];
} ReadyMember16C;

typedef struct ReadyOwner16C {
    char pad_00[0x16C];
    ReadyMember16C member;
} ReadyOwner16C;

extern unsigned char func_08ABB474(ReadyOwner16C *self);
extern void func_08AC2334(ReadyMember16C *member);

/* Run func_08AC2334 on the member at 0x16C once the owner is ready. */
int func_08ABB344(ReadyOwner16C *self)
{
    if (func_08ABB474(self) == 0) {
        return -1;
    }
    func_08AC2334(&self->member);
    return 0;
}
