/* Raises or lowers an actor's guard state from its stance flags and
   tells the actor. */
struct ActorBase088160D8 {
    char data_000[0x388];
    unsigned long long state;
    char data_390[0x544 - 0x390];
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
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void guard_raised();
    virtual void guard_lowered();
};

struct Actor088160D8 : ActorBase088160D8 {
    char data_548[0x560 - 0x548];
    unsigned int stance;
};

extern "C" {

extern unsigned long long D_089C7470;
extern unsigned long long D_089C7478;

void func_088160D8(Actor088160D8 *self)
{
    unsigned int stance = self->stance;
    if ((stance & 2) || (stance & 4)) {
        self->state |= D_089C7470;
        self->guard_raised();
    } else {
        self->state &= D_089C7478;
        self->guard_lowered();
    }
}

}
