/* Calls notify(arg) on every listener of the list at 0x194. */
struct Listener {
    Listener *next;
    virtual void notify(int arg);
};

struct ListenerOwner {
    char pad_000[0x194];
    Listener *head;

    static Listener *after(ListenerOwner *owner, Listener *l)
    {
        return l == 0 ? owner->head : l->next;
    }
};

extern "C" {
void func_089032D4(ListenerOwner *owner, int arg)
{
    Listener *l = 0;

    while ((l = ListenerOwner::after(owner, l)) != 0) {
        l->notify(arg);
    }
}
}
