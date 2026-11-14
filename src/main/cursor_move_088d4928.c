/* Moves the cursor to the index and scrolls; 1 when it moved. */
typedef struct Cursor088D4928 {
    int type;
    int index;
} Cursor088D4928;

void func_088D47F4(Cursor088D4928 *c);
int func_088D4928(Cursor088D4928 *c, unsigned int index)
{
    int changed = 0;
    unsigned int old = c->index;
    c->index = index;
    if (old != index) {
        func_088D47F4(c);
        changed = 1;
    }
    return changed;
}
