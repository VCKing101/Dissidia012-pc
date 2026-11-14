typedef struct CommandCheck69254 {
    unsigned char active;
} CommandCheck69254;

extern int func_08A67CBC(CommandCheck69254 *self);

int func_08A69254(CommandCheck69254 *self)
{
    if (!self->active) {
        return -1;
    }
    if (func_08A67CBC(self)) {
        return -2;
    }
    return 0;
}
