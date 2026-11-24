/* For each of the two battle sides whose slot is the given object, sends event 0xCC through a stack message. */
typedef struct Msg { int side; int id; char pad_08[0x20]; int flag; char pad_2c[4]; } Msg;
typedef struct SideRoot { char pad_00[0x38]; void *slot[2]; } SideRoot;
extern "C" SideRoot *D_089D6A10;
extern "C" void func_089EB798(Msg *m);
extern "C" void func_0888ACE4(Msg *m);
extern "C" void func_089EB7D0(Msg *m, int v);
extern "C" int func_08B15A34(void *obj)
{

    for (int i = 0; i < 2; i++) {
        if (D_089D6A10->slot[i] != obj) {
            continue;
        } else {
            Msg m;
            func_089EB798(&m);
            m.side = i;
            m.id = 0xCC;
            m.flag = 1;
            func_0888ACE4(&m);
            func_089EB7D0(&m, 2);
        }
    }
    return 0;
}
