/* Resets the profile and copies the 0x98-byte default record into it. */
typedef struct Profile {
    char pad_00[0x4A];
    char record[0x98];
} Profile;

extern char D_09B68B48[];
extern void func_088F5844(Profile *p);
extern void func_088F5804(Profile *p);
extern void *func_0887AFBC(void *table, int index, int arg);
extern void func_08935524(void *dst, const void *src, int size);

void func_088F5870(Profile *p)
{
    func_088F5844(p);
    func_088F5804(p);
    func_08935524(p->record, func_0887AFBC(D_09B68B48, 2, 0), 0x98);
}
