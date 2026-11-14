/* Id of the referenced record when its definition loads without error; -1 otherwise. */
struct Info088C1678 { char data[0x30]; int error; };
struct Rec088C1678 { char pad[8]; short id; };
struct Ref088C1678 { Rec088C1678 *rec; void *def; };
extern "C" {
void func_088C014C(Info088C1678 *info, void *def);
int func_088C1678(Ref088C1678 *ref)
{
    bool ok = true;
    if (ref->def != 0) {
        Info088C1678 info;
        func_088C014C(&info, ref->def);
        ok = info.error == 0;
    }
    if (ok == true && ref->rec != 0) {
        return ref->rec->id;
    }
    return -1;
}
}
