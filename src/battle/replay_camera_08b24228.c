extern char D_08B278E0[];
extern void func_0893476C(char *out, const char *format, int index);
extern void *func_08899300(void);
extern int func_08897D50(void *table, const char *name);
extern int func_08897998(void *table, int id);
extern int func_08897DEC(void *table, int id);
extern int func_08897E48(void *table, int id);

void func_08B24228(int index, int *first, int *second)
{
    char name[0x100];
    void *table;
    int id;

    func_0893476C(name, D_08B278E0, index);
    table = func_08899300();
    id = func_08897D50(table, name);
    if (id < 0 || func_08897998(table, id) == 0) {
        *first = 0;
        *second = 0;
    } else {
        *first = func_08897DEC(table, id);
        *second = func_08897E48(table, id);
    }
}
