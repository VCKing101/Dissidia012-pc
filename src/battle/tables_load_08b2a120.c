#include "battle/agent27_types.h"

extern int func_08B2A1A8(ResTables *tables);
extern void func_08B2A2FC(ResTables *tables);
extern void func_08B2A8F4(ResTables *tables);
extern void func_08B2B350(ResTables *tables);
extern int func_08B2A3FC(ResTables *tables, int a, int b);

int func_08B2A120(ResTables *tables, int a, int b)
{
    if (func_08B2A1A8(tables) == 0) {
        return -1;
    }
    if (b == 0) {
        return -2;
    }
    func_08B2A2FC(tables);
    func_08B2A8F4(tables);
    func_08B2B350(tables);
    func_08B2A3FC(tables, a, b);
    return 0;
}
