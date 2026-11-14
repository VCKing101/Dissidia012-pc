typedef struct ModelHolder { char pad_00[0x40]; void *model; } ModelHolder;
extern void func_088D9AEC(void *model);
extern void func_088D9F08(void *model, float value);
void func_0892BD70(ModelHolder *h, float value)
{
    func_088D9AEC(h->model);
    func_088D9F08(h->model, value);
}
