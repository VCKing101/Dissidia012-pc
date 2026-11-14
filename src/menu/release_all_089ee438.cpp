struct ReleaseEntry {
    int field_00[6];
};

struct ReleaseBlock {
    int field_00[3];
    int field_0C[1];
    ReleaseEntry field_10[1];
    char pad_28[0x58 - 0x28];
    int field_58;
};

extern "C" {

void func_0887BA5C(int handle);
void func_088DA060(int handle);
void func_089EE01C(ReleaseEntry *entry);

void func_089EE438(ReleaseBlock *self)
{
    for (int i = 0; i < self->field_58; i++) {
        func_089EE01C(&self->field_10[i]);
    }
    for (int i = 0; i < 3; i++) {
        if (self->field_00[i] != 0) {
            func_088DA060(self->field_00[i]);
            self->field_00[i] = 0;
        }
    }
    for (int i = 0; i < 1; i++) {
        if (self->field_0C[i] != 0) {
            func_0887BA5C(self->field_0C[i]);
            self->field_0C[i] = 0;
        }
    }
}

}
