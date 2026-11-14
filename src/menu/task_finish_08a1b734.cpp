/* When the running task reports done, deletes it and moves the page to state 1. */
class PageTask {
public:
    virtual ~PageTask();
    virtual int isDone();
};

struct TaskPage {
    char pad_0000[0xD7C];
    int field_D7C;
    char pad_0D80[0x67B0 - 0xD80];
    PageTask *field_67B0;
};

struct TaskOwner {
    char pad_000[0x154];
    TaskPage *field_154;
};

extern "C" {
void func_08A1B734(TaskOwner *self)
{
    if (self->field_154->field_67B0->isDone() != 0) {
        if (self->field_154->field_67B0 != 0) {
            delete self->field_154->field_67B0;
            self->field_154->field_67B0 = 0;
        }
        self->field_154->field_D7C = 1;
    }
}
}
