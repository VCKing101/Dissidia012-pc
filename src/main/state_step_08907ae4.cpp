/* State step on the field at 0x1658: state 7 releases the child (virtual 1), state 2 either advances to 3 or finishes the child (virtual 2 with 1), anything else releases it; all but the advance reset the state. */
struct Child08907AE4 { virtual void v0(); virtual void v1(); virtual void v2(int a); };
struct Obj08907AE4 {
    char pad0[0x1658]; int state;
    char pad1[4]; Child08907AE4 *child;
    char pad2[0x16DC - 0x1664]; int count;
    char pad3[0x16E5 - 0x16E0]; unsigned char flag;
};
extern "C" {
void func_08907AE4(Obj08907AE4 *self)
{
    switch (self->state) {
    case 7:
        if (self->child != 0) {
            self->child->v1();
        }
        self->state = 0;
        break;
    case 2:
        if (self->count < 5 && self->flag == 0) {
            self->state = 3;
        } else {
            if (self->child != 0) {
                self->child->v2(1);
            }
            self->count = 0;
            self->flag = 0;
            self->state = 0;
        }
        break;
    default:
        if (self->child != 0) {
            self->child->v1();
        }
        self->state = 0;
        break;
    }
}
}
