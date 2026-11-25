/* Moves the cursor to a clamped position (0 or more) and keeps the view window around it; returns -1 when the scroll top changed. */
typedef struct L088D484C { char pad[4]; int top; int pad2; int cur; int off; int pad3; int rows; } L088D484C;
void func_088D4744(L088D484C *l);
int func_088D484C(L088D484C *l, int pos, unsigned char force)
{
    int old = l->top;
    int ret = 0;
    if (l->cur != pos || force != 0) {
        int v;
        int c;
        if (pos >= 0) {
            v = pos;
        } else {
            v = 0;
        }
        l->cur = v;
        func_088D4744(l);
        int rows;
        int top;
        top = l->top;
        c = l->cur;
        rows = l->rows;
        if (!(top < c)) {
            l->top = c;
        }
        if (c < l->off + rows - 1) {
            int n = c - rows + 1;
            l->off = n;
            if (n < 0) {
                l->off = 0;
            }
        }
    }
    if (old != l->top) {
        ret = -1;
    }
    return ret;
}
