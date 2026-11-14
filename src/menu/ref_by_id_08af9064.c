/* Looks the reference up by id and passes it on. */
typedef struct IdRef08AF9064 {
    int field_00;
    int field_04;
} IdRef08AF9064;

extern IdRef08AF9064 func_088B97E8(unsigned short id);
extern void func_08AF8F84(void *self, IdRef08AF9064 *ref, unsigned char flag);

void func_08AF9064(void *self, unsigned short id, unsigned char flag)
{
    IdRef08AF9064 ref;
    ref = func_088B97E8(id);
    func_08AF8F84(self, &ref, flag);
}
