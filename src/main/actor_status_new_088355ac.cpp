/* Sets up an actor's status from a character id, then creates the ability
   set and the equipment list built from it. */
struct Status088355AC {
    int data[0x778 / 4];
};

struct Abilities088355AC {
    char data[0x634];
};

struct Equipment088355AC {
    char data[0x2C];
};

struct Actor088355AC;

extern "C" {

void *func_08842100(int size);
void func_088CB9E8(Status088355AC *status);
void *func_088312D0(Actor088355AC *self, int size);
void func_0883F014(Abilities088355AC *abilities);
void func_088BF238(Abilities088355AC *abilities, Status088355AC *status, void *source);
void func_08842954(Equipment088355AC *equipment);
void func_088429A4(Equipment088355AC *equipment, Status088355AC *status);
void func_088BBAB4(Status088355AC *status, int id, int costume, int level);
void func_08842A5C(Equipment088355AC *equipment, Status088355AC *status,
                   Abilities088355AC *abilities);

}

struct Actor088355AC {
    char pad_000[0x30C];
    Status088355AC *status;
    Abilities088355AC *abilities;
    char pad_314[0x52C - 0x314];
    Equipment088355AC *equipment;
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

    static Status088355AC *new_status()
    {
        Status088355AC *p = (Status088355AC *)func_08842100(sizeof(Status088355AC));
        Status088355AC *r = 0;
        if (p != 0) {
            func_088CB9E8(p);
            r = p;
        }
        return r;
    }
};

extern "C" {

void func_088355AC(Actor088355AC *self, int id)
{
    Equipment088355AC *result2[1];
    Abilities088355AC *result1[1];
    self->status = Actor088355AC::new_status();
    func_088BBAB4(self->status, id, -1, -1);
    Abilities088355AC *none1 = 0;
    result1[0] = none1;
    Abilities088355AC *abilities =
        (Abilities088355AC *)func_088312D0(self, sizeof(Abilities088355AC));
    Abilities088355AC *made1 = result1[0];
    if (abilities != 0) {
        func_0883F014(abilities);
        made1 = abilities;
    }
    self->abilities = made1;
    func_088BF238(self->abilities, self->status, 0);
    Equipment088355AC *none2 = 0;
    result2[0] = none2;
    Equipment088355AC *equipment =
        (Equipment088355AC *)func_088312D0(self, sizeof(Equipment088355AC));
    Equipment088355AC *made2 = result2[0];
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
