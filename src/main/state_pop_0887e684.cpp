/* Leaves the current state and makes the pending one current. */
struct State0887E684 {
    char pad_00[0x24];
    virtual void v0();
    virtual void v1();
    virtual void leave();
};

struct Slot0887E684 {
    int id;
    State0887E684 *state;
};

struct Machine0887E684 {
    char pad_00[0x10];
    Slot0887E684 current;
    Slot0887E684 pending;
};

extern "C" {

void func_0885C488(State0887E684 *state);

void func_0887E684(Machine0887E684 *m)
{
    m->current.state->leave();
    func_0885C488(m->current.state);
    int id = m->pending.id;
    State0887E684 *state = m->pending.state;
    m->current.id = id;
    m->current.state = state;
    m->pending.id = 0;
    m->pending.state = 0;
}

}
