/* Forwards to func_0880EA20 with the object's model and the object. */
struct Object08B348FC {
    char pad_000[0x398];
    void *field_398;
};

extern void func_0880EA20(void *model, int arg, struct Object08B348FC *owner);

void func_08B348FC(struct Object08B348FC *self, int arg)
{
    func_0880EA20(self->field_398, arg, self);
}
