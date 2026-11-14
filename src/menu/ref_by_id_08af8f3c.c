/* Looks the reference up by id and passes it on. */
typedef struct IdRef08AF8F3C {
    int field_00;
    int field_04;
} IdRef08AF8F3C;

extern IdRef08AF8F3C func_088B97E8(unsigned short id);
extern int func_08AF8EF0(void *self, IdRef08AF8F3C *ref);

void func_08AF8F3C(void *self, unsigned short id)
{
    IdRef08AF8F3C ref;
    ref = func_088B97E8(id);
    func_08AF8EF0(self, &ref);
}
