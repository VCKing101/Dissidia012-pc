/* Moves the tutorial to step 4 (closing the open message when `close`) and shows its messages. */
extern int D_089D7F98;
extern int D_089D7FA0[];
extern void func_088FE9AC(void);
extern void func_088FE6A0(int *ids);

void func_088FEA84(unsigned char close)
{
    if (D_089D7F98 == 4) {
        return;
    }
    if (close) {
        func_088FE9AC();
    }
    D_089D7F98 = 4;
    func_088FE6A0(D_089D7FA0);
}
