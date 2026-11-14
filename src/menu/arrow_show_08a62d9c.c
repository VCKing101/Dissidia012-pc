/* Shows or hides the left arrow (not on the first page) and the right arrow (not on the last). */
typedef struct ArrowPage {
    char pad_000[0xDC];
    void *field_DC;
    void *field_E0;
    char pad_0E4[0xDBC - 0xE4];
    int field_DBC;
    char pad_DC0[0x4];
    int field_DC4;
} ArrowPage;

extern void func_088D9CFC(void *icon);
extern void func_088D9CD8(void *icon);

void func_08A62D9C(ArrowPage *self)
{
    if (self->field_DBC == 0) {
        func_088D9CFC(self->field_DC);
    } else {
        func_088D9CD8(self->field_DC);
    }
    if (self->field_DBC == self->field_DC4) {
        func_088D9CFC(self->field_E0);
    } else {
        func_088D9CD8(self->field_E0);
    }
}
