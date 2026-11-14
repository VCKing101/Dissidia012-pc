struct Header { short field_00; unsigned short count; };
struct Entry { int field_00; unsigned short id; short field_06; };
extern "C" {
extern Header *D_089D7808;
extern Entry *D_089D780C;

int func_088CC22C(void)
{
    if (D_089D7808 != 0) {
        int n = 0;
        for (int i = 0; i < D_089D7808->count; i++, n++) {
            if (D_089D780C[i].id == 0xFFFF) {
                break;
            }
        }
        return n;
    }
    return 0;
}
}
