typedef struct PairBinding {
    unsigned char active;
    void *first;
    void *second;
} PairBinding;

extern void func_08ABD670(PairBinding *self);

/* Reset, then bind both objects; -1 when either is missing. */
int func_08ABD68C(PairBinding *self, void *first, void *second)
{
    func_08ABD670(self);
    if (first == 0 || second == 0) {
        return -1;
    }
    self->first = first;
    self->second = second;
    self->active = 1;
    return 0;
}
