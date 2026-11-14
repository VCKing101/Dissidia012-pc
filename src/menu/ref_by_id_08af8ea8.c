/* Looks the reference up by id and passes it on. */
typedef struct IdRef08AF8EA8 {
    int field_00;
    int field_04;
} IdRef08AF8EA8;

extern IdRef08AF8EA8 func_088B97E8(unsigned short id);
extern int func_08AF8E14(void *self, IdRef08AF8EA8 *ref);

void func_08AF8EA8(void *self, unsigned short id)
{
    IdRef08AF8EA8 ref;
    ref = func_088B97E8(id);
    func_08AF8E14(self, &ref);
}
