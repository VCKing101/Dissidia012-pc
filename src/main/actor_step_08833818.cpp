/* Per-frame actor step: makes sure the actor has a state, runs its
   controller, then advances its motion by the time since the last frame. */
struct Actor08833818;

struct Controller08833818 {
    int pad_0;
    int pad_4;
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void update(Actor08833818 *actor);
};

struct Actor08833818 {
    char data_000[0x84];
    float last_time;
    char data_088[0x3A4 - 0x88];
    Controller08833818 *controller;
    char data_3A8[0x544 - 0x3A8];
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
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void advance(float dt);
};

extern "C" {

int func_08837C44(Actor08833818 *self);
void func_08837A5C(Actor08833818 *self);

void func_08833818(Actor08833818 *self, float time)
{
    if (!func_08837C44(self)) {
        func_08837A5C(self);
    }
    self->controller->update(self);
    self->advance(time - self->last_time);
}

}
