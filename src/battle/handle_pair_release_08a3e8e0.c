/* Releases the object's extra part (when it has one) and its two sound
 * handles. */
typedef struct HandlePair {
    unsigned char pad_000[0x220];
    void *handles[2];
    unsigned char pad_228[0x50];
    void *extra;
} HandlePair;

extern void *D_089D7830;
extern void func_08A3E960(HandlePair *self);
extern void func_088CF314(void *mixer, void *handle, int flags);

void func_08A3E8E0(HandlePair *self)
{
    int i;
    if (self->extra != 0) {
        func_08A3E960(self);
    }
    for (i = 0; i < 2; i++) {
        if (self->handles[i] == 0) {
            continue;
        }
        {
            int flags = 0;
            func_088CF314(D_089D7830, self->handles[i], flags);
        }
        self->handles[i] = 0;
    }
}
