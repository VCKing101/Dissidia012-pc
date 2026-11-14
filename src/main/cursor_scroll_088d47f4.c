/* Scrolls the cursor window so that the index stays visible. */
typedef struct Cursor088D47F4 {
    int type;
    int index;
    int b;
    int c;
    int top;
    int d2;
    int span;
} Cursor088D47F4;

void func_088D47F4(Cursor088D47F4 *c)
{
    int span = c->span;
    if (span > 0) {
        int index = c->index;
        if (index < c->top) {
            c->top = index;
        } else if (index >= c->top + span) {
            int top = 0;
            if (index >= span) {
                top = index - span + 1;
            }
            c->top = top;
        }
    }
}
