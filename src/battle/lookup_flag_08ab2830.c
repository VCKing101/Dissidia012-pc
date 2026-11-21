/* Resolves the object's current record and checks the (record slot, param) flag; 0 when anything is missing. */
typedef struct Rec38 { char pad[0x24]; short slot; } Rec38;
extern int func_08AAE300(void *self);
extern int func_08AAE84C(void *self);
extern void *func_08A80200(void *p);
extern Rec38 *func_08AACEE0(void *owner, int a, int b);
extern int func_08AAD228(void *owner, int slot, int param);
extern void *D_08A80218;
int func_08AB2830(unsigned char *self, int param)
{
    int a;
    void *owner;
    int b;
    Rec38 *rec;
    if (*self == 0) {
        return 0;
    } else if (param < 0) {
        return 0;
    } else {
        a = func_08AAE300((void *)self);
        if (a < 0) {
            return 0;
        } else {
            owner = func_08A80200(D_08A80218);
            func_08AAE300((void *)self);
            b = func_08AAE84C((void *)self);
            rec = func_08AACEE0(owner, a, b);
            if (rec == 0) {
                return 0;
            } else {
                return func_08AAD228(owner, rec->slot, param);
            }
        }
    }
}
