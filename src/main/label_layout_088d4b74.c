/* Lays out the label's text and shifts it by the offset (horizontally or vertically). */
typedef struct Layout088D4B74 {
    char data[0x30];
    float x;
    float y;
    float z;
    float w;
} Layout088D4B74;

typedef struct Label088D4B74 {
    int field_00;
    int handle;
    int font;
    unsigned short text;
    char pad_0E[0xA];
    int offset;
} Label088D4B74;

void func_088DA0A8(int font, int text, Layout088D4B74 *layout, int *size);
void func_088D9B84(int handle, Layout088D4B74 *layout);
void func_088D4B74(Label088D4B74 *label, unsigned char vertical)
{
    if (label->handle != 0) {
        Layout088D4B74 layout;
        int size;
        func_088DA0A8(label->font, label->text, &layout, &size);
        if (vertical) {
            layout.x += label->offset;
        } else {
            layout.y += label->offset;
        }
        func_088D9B84(label->handle, &layout);
    }
}
