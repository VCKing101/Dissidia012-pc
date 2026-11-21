/* Tells the battle UI whether the entry's unit differs from the current one. */
struct Ent38 { char pad[0x14]; void *key; };
struct Ref38 { int pad; int unit; };
struct Cur38 { char pad[0x38]; int unit; };
extern "C" {
Ref38 *func_08AB3264(void *key);
void func_08A22534(void *ui, int changed, int flag);
extern Cur38 *D_089D6A10;
extern void *D_08A22AB0;
}
struct Ui38 {
    static void Notify(int changed, int flag) { func_08A22534(D_08A22AB0, changed, flag); }
};
extern "C" void func_08AA4E80(Ent38 *self, unsigned char flag)
{
    Ui38::Notify(func_08AB3264(self->key)->unit != D_089D6A10->unit, flag);
}
