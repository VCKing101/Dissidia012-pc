/* Sets up an actor's status: copies the status block, then creates the
   ability set and the equipment list built from it. */
struct Status08835468 {
    int data[0x778 / 4];
};

struct Abilities08835468 {
    char data[0x634];
};

struct Equipment08835468 {
    char data[0x2C];
};

struct Actor08835468;

extern "C" {

void *func_08842100(int size);
void func_088CB9E8(Status08835468 *status);
void *func_088312D0(Actor08835468 *self, int size);
void func_0883F014(Abilities08835468 *abilities);
void func_088BF238(Abilities08835468 *abilities, Status08835468 *status, void *source);
void func_08842954(Equipment08835468 *equipment);
void func_088429A4(Equipment08835468 *equipment, Status08835468 *status);
void func_08842A5C(Equipment08835468 *equipment, Status08835468 *status,
                   Abilities08835468 *abilities);

}

struct Actor08835468 {
    char pad_000[0x30C];
    Status08835468 *status;
    Abilities08835468 *abilities;
    char pad_314[0x52C - 0x314];
    Equipment08835468 *equipment;
    char pad_530[0x544 - 0x530];
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
    virtual void status_changed();
    virtual void equipment_changed();

    static Status08835468 *new_status()
    {
        Status08835468 *p = (Status08835468 *)func_08842100(sizeof(Status08835468));
        Status08835468 *r = 0;
        if (p != 0) {
            func_088CB9E8(p);
            r = p;
        }
        return r;
    }
};

extern "C" {

void func_08835468(Actor08835468 *self, Status08835468 *src, void *source)
{
    Equipment08835468 *result2[1];
    Abilities08835468 *result1[1];
    self->status = Actor08835468::new_status();
    *self->status = *src;
    Abilities08835468 *none1 = 0;
    result1[0] = none1;
    Abilities08835468 *abilities =
        (Abilities08835468 *)func_088312D0(self, sizeof(Abilities08835468));
    Abilities08835468 *made1 = result1[0];
    if (abilities != 0) {
        func_0883F014(abilities);
        made1 = abilities;
    }
    self->abilities = made1;
    func_088BF238(self->abilities, self->status, source);
    Equipment08835468 *none2 = 0;
    result2[0] = none2;
    Equipment08835468 *equipment =
        (Equipment08835468 *)func_088312D0(self, sizeof(Equipment08835468));
    Equipment08835468 *made2 = result2[0];
    if (equipment != 0) {
        func_08842954(equipment);
        made2 = equipment;
    }
    self->equipment = made2;
    func_088429A4(self->equipment, self->status);
    self->status_changed();
    func_08842A5C(self->equipment, self->status, self->abilities);
    self->equipment_changed();
}

}
