typedef struct LinkTask {
    unsigned char active;
    char pad_01[0x3];
    int count;
    char link[0x10];
    void *first;
    void *second;
    void *third;
    void *fourth;
    void *fifth;
} LinkTask;

extern void func_08AA2AB8(LinkTask *self);
extern int func_08A7E7FC(void *link);

/* Records the five objects and clears the count; -1 when one is missing, -2 when the link fails. */
int func_08AA2AE8(LinkTask *self, void *first, void *second, void *third, void *fourth, void *fifth)
{
    func_08AA2AB8(self);
    if (first == 0 || second == 0 || third == 0 || fourth == 0 || fifth == 0) {
        return -1;
    }
    if (func_08A7E7FC(self->link) != 0) {
        return -2;
    }
    self->first = first;
    self->second = second;
    self->third = third;
    self->fourth = fourth;
    self->fifth = fifth;
    self->count = 0;
    self->active = 1;
    return 0;
}
