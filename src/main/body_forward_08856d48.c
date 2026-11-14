typedef struct Model { char pad_00[0xCC]; int field_CC; } Model;
typedef struct Body { char pad_00[0x390]; Model *model; } Body;
extern void func_08856D70(int a, Body *body, int c, int d, int e);
void func_08856D48(int a, Body *body, int d, int e)
{
    func_08856D70(a, body, body->model->field_CC, d, e);
}
