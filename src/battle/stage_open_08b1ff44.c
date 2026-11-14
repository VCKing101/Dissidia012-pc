typedef struct StageLoad {
    unsigned char active;   /* 0x00 */
    char pad01[3];
    int state;              /* 0x04 */
    char pad08[8];
    unsigned int kind;      /* 0x10 */
    int field_14;
    int field_18;
    int field_1C;
    int field_20;
    int field_24;
} StageLoad;

extern void func_08B1FF28(StageLoad *load);

int func_08B1FF44(StageLoad *load, unsigned int kind, int a, int b, int c, int d, int e)
{
    func_08B1FF28(load);
    if (kind >= 2 || a == 0 || b == 0 || c == 0 || d == 0 || e == 0) {
        return -1;
    }
    load->kind = kind;
    load->field_14 = a;
    load->field_18 = b;
    load->field_1C = c;
    load->field_20 = d;
    load->field_24 = e;
    load->state = 0;
    load->active = 1;
    return 0;
}
