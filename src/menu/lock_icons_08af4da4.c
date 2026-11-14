/* Sets the lock icon (0x24 locked, 0x25 open) and the lock state icon. */
typedef struct LockIconState {
    char pad_000[0x56C];
    void *field_56C;
    void *field_570;
    char pad_574[0xBC4 - 0x574];
    int field_BC4;
} LockIconState;

typedef struct LockIconOwner {
    char pad_000[0x140];
    LockIconState *field_140;
} LockIconOwner;

extern void func_088D9F5C(void *icon, int frame, int one);

void func_08AF4DA4(LockIconOwner *self)
{
    int frame = 0x25;
    int locked = 0;
    if (self->field_140->field_BC4 != 0) {
        frame = 0x24;
        locked = 1;
    }
    func_088D9F5C(self->field_140->field_56C, frame, 1);
    func_088D9F5C(self->field_140->field_570, locked, 1);
}
