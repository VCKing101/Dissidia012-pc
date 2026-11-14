/* Queues the two confirm buttons that are down; returns whether any was. */
extern unsigned char D_089D5F20;
extern int func_088C0B10(int button);
extern void func_089E2DA4(void *self, int key, int value);

int func_089E3BFC(void *self)
{
    int pushed;
    if (D_089D5F20 != 0) {
        return 0;
    }
    pushed = 0;
    if (func_088C0B10(0) != 0) {
        void *target = self;
        func_089E2DA4(target, 0, -1);
        pushed = 1;
    }
    if (func_088C0B10(1) != 0) {
        func_089E2DA4(self, 1, -1);
        pushed = 1;
    }
    return pushed;
}
