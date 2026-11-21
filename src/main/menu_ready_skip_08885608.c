typedef struct Menu08885608 {
    int unk_00;
    void *list;                        /* +0x04 */
    void *pad;                         /* +0x08 */
    void *cursor;                      /* +0x0C */
    void *text;                        /* +0x10 */
} Menu08885608;

extern int func_08884F24(void *list);
extern int func_08886478(void *cursor);
extern int func_08886094(void *pad);
extern int func_0888473C(void *text);

/* 1 when every part of the menu is ready (the text only when `skip_text` is 0). */
int func_08885608(Menu08885608 *self, unsigned char skip_text)
{
    if (self->list != 0 && func_08884F24(self->list) == 0) {
        return 0;
    }
    if (self->cursor != 0 && func_08886478(self->cursor) == 0) {
        return 0;
    }
    if (self->pad != 0 && func_08886094(self->pad) == 0) {
        return 0;
    }
    if (skip_text == 0 && self->text != 0 && func_0888473C(self->text) == 0) {
        return 0;
    }
    return 1;
}
