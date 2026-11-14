/* Releases the unit's and its owner's effect filters, detaches the unit and
   resets the controller's cursor. */
struct Effect0884EF7C {
    char pad_00[0x28];
    void *handle;
};

struct Model0884EF7C {
    char pad_00[0x34];
    Effect0884EF7C effect;

    static Effect0884EF7C *effectOf(Model0884EF7C *m) { return &m->effect; }
};

struct UnitBase0884EF7C {
    char pad_000[0x390];
    Model0884EF7C *model;
    char pad_394[0x134];
    char filter[0x7C];
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
    virtual void detach();
};

struct Unit0884EF7C : UnitBase0884EF7C {
    char pad_548[0x8C];
    Unit0884EF7C *owner;
};

struct Ctl0884EF7C {
    int field_00;
    int field_04;
    Unit0884EF7C *unit;
};

extern "C" {

void func_088A9D7C(void *handle, void *filter);
void func_08AD0AF8(Unit0884EF7C *u);
void func_0884ECD4(Ctl0884EF7C *c);

void func_0884EF7C(Ctl0884EF7C *c)
{
    Unit0884EF7C *u = c->unit;
    void *handle = Model0884EF7C::effectOf(u->model)->handle;
    if (handle != 0) {
        func_088A9D7C(handle, u->filter);
        u = c->unit;
    }
    handle = Model0884EF7C::effectOf(u->owner->model)->handle;
    if (handle != 0) {
        func_088A9D7C(handle, u->filter);
        u = c->unit;
    }
    func_08AD0AF8(u);
    c->unit->detach();
    func_0884ECD4(c);
}

}
