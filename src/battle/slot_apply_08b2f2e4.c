/* For a kind below 6, calls func_08A64078 on the object held at +0x5C4; -1 bad kind, -2 nothing held, -3 call failed. */
typedef struct HeldSelf { char pad[0x5C4]; void *held; } HeldSelf;
extern int func_08A64078(void *held, unsigned kind);
int func_08B2F2E4(HeldSelf *self, unsigned kind)
{
    if (kind >= 6) {
        return -1;
    }
    if (self->held == 0) {
        return -2;
    }
    if (func_08A64078(self->held, kind) != 0) {
        return -3;
    }
    return 0;
}
