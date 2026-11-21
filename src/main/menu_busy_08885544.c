typedef struct Menu08885544 {
    int unk_00;
    void *list;                        /* +0x04 */
    void *pad;                         /* +0x08 */
    void *cursor;                      /* +0x0C */
    void *text;                        /* +0x10 */
} Menu08885544;

extern int func_08884EB8(void *list);
extern int func_0888641C(void *cursor);
extern int func_08886028(void *pad);
extern int func_088846AC(void *text);

/* 1 while any part of the menu is still busy. */
int func_08885544(Menu08885544 *self)
{
    if (self->list != 0 && func_08884EB8(self->list) != 0) {
        return 1;
    }
    if (self->cursor != 0 && func_0888641C(self->cursor) != 0) {
        return 1;
    }
    if (self->pad != 0 && func_08886028(self->pad) != 0) {
        return 1;
    }
    if (self->text != 0 && func_088846AC(self->text) != 0) {
        return 1;
    }
    return 0;
}
