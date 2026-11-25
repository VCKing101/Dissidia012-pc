// Scalar deleting wrapper: calls the cleanup function when asked to free.
extern "C" void func_089748D0(void *self);
extern "C" void func_089C2CE4(void *self, unsigned flags)
{
    if (self != 0 && (flags & 1)) {
        func_089748D0(self);
    }
}
