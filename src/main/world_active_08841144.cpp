/* World update lists. */
struct Actor08841144;

struct ActorBase08841144 {
    char data_000[0x388];
    unsigned long long state;
    char data_390[0x4A0 - 0x390];
    Actor08841144 *next;
    Actor08841144 *next_active;
    char data_4A8[0x544 - 0x4A8];
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void update();
    virtual void late_update();
};

struct Actor08841144 : ActorBase08841144 {
    static bool any(Actor08841144 *a, unsigned long long mask) { return (a->state & mask) != 0; }
    static Actor08841144 *&link(Actor08841144 *a) { return a->next_active; }
};

struct World08841144 {
    char pad_00[0xC];
    Actor08841144 *first;
    char pad_10[0x3C - 0x10];
    Actor08841144 *active_first;
    Actor08841144 *active_last;
    char pad_44[0x50 - 0x44];
    unsigned char running;

    static Actor08841144 *next(World08841144 *w, Actor08841144 *a) { return a == 0 ? w->first : a->next; }
    static Actor08841144 *next_active(World08841144 *w, Actor08841144 *a) { return a == 0 ? w->active_first : a->next_active; }

    void push_front(Actor08841144 *a)
    {
        if (this->active_first != 0) {
            a->next_active = this->active_first;
            this->active_first = a;
        } else {
            this->active_last = a;
            this->active_first = a;
            a->next_active = 0;
        }
    }

    void push_back(Actor08841144 *a)
    {
        if (this->active_last == 0) {
            this->active_last = a;
            this->active_first = a;
            Actor08841144::link(a) = 0;
        } else {
            Actor08841144::link(this->active_last) = a;
            Actor08841144::link(a) = 0;
            this->active_last = a;
        }
    }
};

extern "C" {

extern World08841144 *D_089D4958;
extern unsigned long long D_089C8698;

void func_0886D0BC(void);
int func_08837F6C(Actor08841144 *a);

/* Runs the update and late update of every active actor, then empties
   the active list. */
void func_08841144(void)
{
    World08841144 *w = D_089D4958;
    if (!w->running) {
        return;
    }
    func_0886D0BC();
    Actor08841144 *a = 0;
    while ((a = World08841144::next_active(w, a)) != 0) {
        a->update();
    }
    a = 0;
    while ((a = World08841144::next_active(w, a)) != 0) {
        a->late_update();
    }
    w->active_last = 0;
    w->active_first = 0;
}

}
