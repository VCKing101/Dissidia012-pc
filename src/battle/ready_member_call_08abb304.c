typedef struct ReadyMemberDC {
    char data[1];
} ReadyMemberDC;

typedef struct ReadyOwnerDC {
    char pad_00[0xDC];
    ReadyMemberDC member;
} ReadyOwnerDC;

extern unsigned char func_08ABB474(ReadyOwnerDC *self);
extern void func_08ABE3F8(ReadyMemberDC *member);

/* Run func_08ABE3F8 on the member at 0xDC once the owner is ready. */
int func_08ABB304(ReadyOwnerDC *self)
{
    if (func_08ABB474(self) == 0) {
        return -1;
    }
    func_08ABE3F8(&self->member);
    return 0;
}
