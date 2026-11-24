/* Calls one of two functions with the word at 0x1C depending on the byte at 0x34. */
typedef struct { char pad[0x1C]; int h; char pad2[0x34 - 0x20]; unsigned char flag; } Obj089408DC;
extern void func_0894ABA4(int);
extern void func_0894A824(int);
void func_089408DC(Obj089408DC *self) {
    if (self->flag) {
        func_0894ABA4(self->h);
    } else {
        func_0894A824(self->h);
    }
}
