/* Runs the child's update when there is a child. */
struct Child08B3325C;

struct Owner08B3325C {
    int field_0;
    struct Child08B3325C *field_4;
};

extern void func_08B3329C(struct Child08B3325C *child);

void func_08B3325C(struct Owner08B3325C *self)
{
    if (self == 0) {
        return;
    }
    if (self->field_4 == 0) {
        return;
    }
    func_08B3329C(self->field_4);
}
