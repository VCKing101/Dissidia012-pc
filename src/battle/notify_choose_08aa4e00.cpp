/* Chooses between two follow-up handlers depending on whether the unit differs from the current one. */
struct Ent38 { char pad[0x14]; void *key; };
struct Ref38 { int pad; int unit; };
struct Cur38 { char pad[0x38]; int unit; };
extern "C" {
Ref38 *func_08AB3264(void *key);
int func_08A22574(void *ui, int changed);
void func_08AA4E80(Ent38 *self, unsigned char flag);
void func_08AA4ED0(Ent38 *self, unsigned char flag);
extern Cur38 *D_089D6A10;
extern void *D_08A22AB0;
}
struct Ui38 {
    static int Check(int changed) { return func_08A22574(D_08A22AB0, changed); }
};
extern "C" void func_08AA4E00(Ent38 *self, unsigned char flag)
{
    if (Ui38::Check(func_08AB3264(self->key)->unit != D_089D6A10->unit) == 0) {
        func_08AA4E80(self, flag);
    } else {
        func_08AA4ED0(self, flag);
    }
}
