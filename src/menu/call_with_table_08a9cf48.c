/* Calls the follow-up routine with the table entry selected by the global index. */
extern int D_09B6EFB0;
extern int D_08AA3F30[];
extern int D_08AA3F44;
extern void func_08A97FA4(int a, int b, int c, int d, int e);
void func_08A9CF48(void) {
    func_08A97FA4(D_08AA3F44, D_08AA3F30[D_09B6EFB0], -1, D_09B6EFB0, 1);
}
