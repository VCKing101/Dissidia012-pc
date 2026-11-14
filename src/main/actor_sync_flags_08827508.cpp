extern "C" {
struct ActorBase08827508 {
    char pad_000[0x374];
    int f374;
    char pad_378[0x544 - 0x378];
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void onFlags();
};
typedef struct OwnerSub { char pad_00[0x44]; int f44; } OwnerSub;
typedef struct ActorOwner { char pad_000[0x330]; OwnerSub sub[1]; char pad_378[0x388 - 0x378]; unsigned long long flags; } ActorOwner;
struct Actor08827508 : ActorBase08827508 {
    char pad_548[8];
    ActorOwner *owner;          /* 0x550 */
};
extern unsigned long long D_089C78B8;
extern void func_0882773C(Actor08827508 *a);
extern void func_08831360(Actor08827508 *a);
struct OwnerOps {
    static bool has(ActorOwner *o) { return (o->flags & D_089C78B8) != 0; }
    static bool none(ActorOwner *o) { return !has(o); }
};
void func_08827508(Actor08827508 *a)
{
    func_0882773C(a);
    func_08831360(a);
    ActorOwner *o = a->owner;
    OwnerSub *sub = o->sub;
    a->f374 = sub->f44;
    if (!OwnerOps::none(o)) {
        a->onFlags();
    }
}
}
