typedef struct Menu088856DC {
    int unk_00;
    void *list;                        /* +0x04 */
    void *pad;                         /* +0x08 */
    void *cursor;                      /* +0x0C */
    void *text;                        /* +0x10 */
} Menu088856DC;

extern int func_08884F58(void *list);
extern int func_088864AC(void *cursor);
extern int func_088860C8(void *pad);
extern int func_08884770(void *text);

/* 1 when every part of the menu is ready. */
int func_088856DC(Menu088856DC *self)
{
    if (self->list != 0 && func_08884F58(self->list) == 0) {
        return 0;
    }
    if (self->cursor != 0 && func_088864AC(self->cursor) == 0) {
        return 0;
    }
    if (self->pad != 0 && func_088860C8(self->pad) == 0) {
        return 0;
    }
    if (self->text != 0 && func_08884770(self->text) == 0) {
        return 0;
    }
    return 1;
}
