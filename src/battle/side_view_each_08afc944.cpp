typedef struct SideView {
    char data[0x10];
} SideView;

extern "C" {

extern SideView D_09B6C288[];
extern void func_08AFC7F8(SideView *view);

void func_08AFC944(void)
{
    for (int i = 0; i < 2; i++) {
        SideView *view = &D_09B6C288[i];
        func_08AFC7F8(view);
    }
}

}
