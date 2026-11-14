struct BytePair {
    unsigned char a;
    unsigned char b;
};

struct ByteTable {
    BytePair pairs[16];
    unsigned char rows[4][16];
};

extern "C" {

void func_088372B0(void *target, int id);
void func_088372D0(void *target, int id);
void func_0884722C(ByteTable *self);

/* Hands every used pair back to the target, then resets the table. */
void func_088473E8(ByteTable *self, void *target)
{
    for (int i = 0; i < 16; i++) {
        unsigned char id = self->pairs[i].a;
        if (id != 0xFF) {
            if (self->pairs[i].b != 0) {
                func_088372B0(target, id);
            } else {
                func_088372D0(target, id);
            }
        }
    }
    func_0884722C(self);
}

}
