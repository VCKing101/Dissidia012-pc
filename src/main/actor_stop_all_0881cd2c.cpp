/* Stops the effect handle of every node that has one. */
struct Effect0881CD2C {
    char pad_00[0x28];
    void *handle;
};

struct Model0881CD2C {
    char pad_00[0x34];
    Effect0881CD2C effect;

    static Effect0881CD2C *effectOf(Model0881CD2C *m) { return &m->effect; }
};

struct Actor0881CD2C {
    char pad_000[0x390];
    Model0881CD2C *model;
};

struct Node0881CD2C {
    char pad_00[0x14];
    Actor0881CD2C *actor;
};

extern "C" {

Node0881CD2C *func_0881CB80(Node0881CD2C *node);
void func_088A9D28(void *handle);

void func_0881CD2C(void)
{
    Node0881CD2C *node = 0;
    while ((node = func_0881CB80(node)) != 0) {
        if (node->actor == 0) {
            continue;
        }
        void *handle = Model0881CD2C::effectOf(node->actor->model)->handle;
        if (handle == 0) {
            continue;
        }
        func_088A9D28(handle);
    }
}

}
