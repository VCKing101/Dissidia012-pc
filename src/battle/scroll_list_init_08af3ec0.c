typedef struct ScrollList {
    int visible;
    int count;
    int cursor;
    int top;
} ScrollList;

void func_08AF3EC0(ScrollList *self, int visible, int count)
{
    self->visible = visible;
    self->count = count;
    self->cursor = count != 0 ? 0 : -1;
    self->top = 0;
}
