typedef struct Child088195C8 {
    unsigned char unknown_000[0x570];
    unsigned char field_570[4];
} Child088195C8;

typedef struct Owner088195C8 {
    unsigned char unknown_00[0x10];
    int state;
    Child088195C8 *child;
} Owner088195C8;

int func_08822DA8(void *p);
void func_0881559C(Child088195C8 *child);

void func_088195C8(Owner088195C8 *self) {
    if (self->child == 0) {
        return;
    }
    if (func_08822DA8(self->child->field_570) == 0) {
        return;
    }
    func_0881559C(self->child);
    self->state = 3;
}
