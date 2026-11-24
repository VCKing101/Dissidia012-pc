/* Calls one of two functions on the object depending on the byte at 0x59. */
typedef struct { char pad[0x59]; unsigned char flag; } Obj08983FB8;
extern void func_08983DF0(Obj08983FB8 *);
extern void func_08983F70(Obj08983FB8 *);
void func_08983FB8(Obj08983FB8 *self) {
    if (self->flag) {
        func_08983DF0(self);
    } else {
        func_08983F70(self);
    }
}
