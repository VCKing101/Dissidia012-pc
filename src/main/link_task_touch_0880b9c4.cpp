/* A link task touching an actor: a fighter (kind 0) gets the task's
   linked reaction and, when the owner is the player, its linked sound;
   then the normal attack hit runs. */
struct Fighter0880B9C4 {
    char data[0x544];
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void react(int a, int b);
};

struct Actor0880B9C4 {
    char pad_000[0x542];
    unsigned char kind;

    static bool is_kind(Actor0880B9C4 *a, int kind) { return a->kind == kind; }
};

struct LinkTask0880B9C4 {
    char pad_000[0x18];
    Fighter0880B9C4 *owner;
    char pad_01C[0x20C - 0x1C];
    int links[4];
};

extern "C" {

extern char *D_089D4DC8;

int func_0883739C(Fighter0880B9C4 *owner);
void func_08856D48(char *sounds, Fighter0880B9C4 *owner, int a, int b);
void *func_088080C4(LinkTask0880B9C4 *self, Actor0880B9C4 *actor, unsigned char kind);

void *func_0880B9C4(LinkTask0880B9C4 *self, Actor0880B9C4 *actor, unsigned char kind)
{
    if (self->links[0] >= 0 && Actor0880B9C4::is_kind(actor, 0) == true) {
        self->owner->react(self->links[0], self->links[1]);
    }
    if (self->links[2] >= 0 && Actor0880B9C4::is_kind(actor, 0) == true && func_0883739C(self->owner) == 1) {
        func_08856D48(D_089D4DC8 + 0x69C0, self->owner, self->links[2], self->links[3]);
    }
    return func_088080C4(self, actor, kind);
}

}
