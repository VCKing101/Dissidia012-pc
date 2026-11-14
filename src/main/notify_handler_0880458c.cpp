/* Builds a (key, value) pair for an actor, marks the actor and passes the
   pair to the listener's fifth virtual. */
struct Pair {
    int key;
    float value;
    int field_08;
    int field_0C;
};

/* The flag word at 0x384 is written through a byte offset: the typed member
   lets SNC move the load above the float store. */
struct Actor {
    char pad_000[0x384];
    unsigned int flags;
};

struct Listener {
    char pad_00[8];
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void notify(Actor *actor, Pair *pair);
};

extern "C" {

void func_089A3868(Pair *out, void *src);
float func_08834840(Actor *actor);

void func_0880458C(Listener *self, Actor *actor)
{
    Pair pair;

    func_089A3868(&pair, (char *)actor + 0xB0);
    pair.value = func_08834840(actor);
    *(unsigned int *)((char *)actor + 0x384) |= 2;
    self->notify(actor, &pair);
}

}
