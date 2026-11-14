#include "main/auto_region1.h"

/* Releases menu-overlay state and drops entries 0x7F, 0x80, 0x81 and 0xC1 of
 * the table D_089D4960. */
void func_088138AC(void)
{
    func_08ABAE78(D_08ABBDF8);
    func_08ABAE5C(D_08ABBDF8);
    func_08A801E4(D_08A80218);
    func_08844520(D_089D4960, 0x7f);
    func_08844520(D_089D4960, 0x80);
    func_08844520(D_089D4960, 0x81);
    func_08844520(D_089D4960, 0xc1);
}
