/* Polls the child at 4 when there is one. */
typedef struct ChildOwner {
    int field_00;
    void *child;
} ChildOwner;

extern int func_088DE5FC(void *child);

void func_088DE5AC(ChildOwner *owner)
{
    if (owner == 0) {
        return;
    }
    if (owner->child == 0) {
        return;
    }
    if (func_088DE5FC(owner->child) != 0) {
        return;
    }
}
