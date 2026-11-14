typedef struct ScrollList {
    int visible;
    int count;
    int cursor;
    int top;
} ScrollList;

/* Move the cursor by delta with wrap-around and keep it in view. */
void func_08AF3E60(ScrollList *self, int delta)
{
    if (self->count != 0) {
        int cursor = (self->cursor + self->count + delta) % self->count;
        self->cursor = cursor;
        if (cursor - self->top >= self->visible) {
            self->top = cursor - self->visible + 1;
        } else if (cursor < self->top) {
            self->top = cursor;
        }
    }
}
