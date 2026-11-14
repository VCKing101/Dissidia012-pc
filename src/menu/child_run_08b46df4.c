/* Runs the child's update when there is a child. */
struct Child08B46DF4;

struct Owner08B46DF4 {
    int field_0;
    struct Child08B46DF4 *field_4;
};

extern void func_08B46E34(struct Child08B46DF4 *child);

void func_08B46DF4(struct Owner08B46DF4 *self)
{
    if (self == 0) {
        return;
    }
    if (self->field_4 == 0) {
        return;
    }
    func_08B46E34(self->field_4);
}
