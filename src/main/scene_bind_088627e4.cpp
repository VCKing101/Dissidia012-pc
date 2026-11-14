/* Binds a scene object by id, activates it and registers it with the renderer. */
struct Obj088627E4 {
    char pad_000[0x154];
    virtual void activate();
};

struct Binding088627E4 {
    int field_00;
    Obj088627E4 *obj;
    int renderer;
    int camera;
};

extern "C" {

extern int D_089D5FF0;
extern int D_089D5FB4;

Obj088627E4 *func_0886362C(int id);
void func_08871394(int renderer, Obj088627E4 *obj);

void func_088627E4(Binding088627E4 *b, int id)
{
    Obj088627E4 *obj = func_0886362C(id);
    b->obj = obj;
    obj->activate();
    func_08871394(D_089D5FF0, b->obj);
    b->renderer = D_089D5FF0;
    b->camera = D_089D5FB4;
}

}
