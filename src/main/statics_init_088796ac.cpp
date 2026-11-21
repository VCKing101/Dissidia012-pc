/* Two function-local static objects built on first use (guard word, constructor, then the
 * destructor record handed to func_08992F6C), whose addresses are published in two globals. */
extern "C" {
void func_08992F6C(void *dtor_record);   /* register a static object's destructor */
void func_089DE668(void *object);        /* constructor of the first object */
void func_089E0D94(void *object);        /* constructor of the second object */
extern int D_089D688C;                   /* destructor record of the second object */
extern int D_089D6898;                   /* destructor record of the first object */
extern void *D_089DE79C;
extern void *D_089E330C;
extern int D_09AEF640;                   /* guard of the second object */
extern int D_09AEF650;                   /* the second object */
extern int D_09AEF7A0;                   /* guard of the first object */
extern int D_09AEF7A4;                   /* the first object */
}

struct Statics {
    static void *first() {
        if (D_09AEF7A0 == 0) {
            D_09AEF7A0 = 1;
            func_089DE668(&D_09AEF7A4);
            func_08992F6C(&D_089D6898);
        }
        return &D_09AEF7A4;
    }
    static void *second() {
        if (D_09AEF640 == 0) {
            D_09AEF640 = 1;
            func_089E0D94(&D_09AEF650);
            func_08992F6C(&D_089D688C);
        }
        return &D_09AEF650;
    }
};

extern "C" void func_088796AC(void)
{
    D_089DE79C = Statics::first();
    D_089E330C = Statics::second();
}
