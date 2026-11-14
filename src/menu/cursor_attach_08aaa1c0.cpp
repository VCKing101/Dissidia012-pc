/* Shows the board cursor and moves it onto node 1 of piece `index` while the cursor is enabled. */
struct AttachMatrix {
    float m[16];
};

struct AttachBoard {
    char pad_0000[0x10];
    void *field_10[(0x9C - 0x10) / 4];
    void *field_9C;
    char pad_00A0[0x7CC0 - 0xA0];
    unsigned int field_7CC0;
    int field_7CC4;

    /* Five pointers per piece; the piece models start at index 0x2E. */
    static void *slot(AttachBoard *board, int index) { return board->field_10[index]; }
};

struct AttachOwner {
    char pad_0000[0x7CFC];
    AttachBoard *field_7CFC;
};

extern "C" void func_088D9CFC(void *icon);
extern "C" void func_088D9CD8(void *icon);
extern "C" void func_088DA0A8(void *model, int node, AttachMatrix *matrix, int *extra);
extern "C" void func_088D9B84(void *icon, AttachMatrix *matrix);
extern "C" void func_088D9AF4(void *icon, int *extra);

extern "C" {
void func_08AAA1C0(AttachOwner *self, int unused, int index)
{
    AttachMatrix matrix;
    int extra;
    if ((self->field_7CFC->field_7CC0 & 2) != 0) {
        func_088D9CFC(self->field_7CFC->field_9C);
        if (self->field_7CFC->field_7CC4 == 0) {
            func_088D9CD8(self->field_7CFC->field_9C);
            int slot = index * 5 + 0x2E;
            func_088DA0A8(AttachBoard::slot(self->field_7CFC, slot), 1, &matrix, &extra);
            func_088D9B84(self->field_7CFC->field_9C, &matrix);
            func_088D9AF4(self->field_7CFC->field_9C, &extra);
        }
    }
}
}
