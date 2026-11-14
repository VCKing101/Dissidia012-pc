typedef struct MenuTableRow {
    unsigned short field_00;
    unsigned short field_02;
    unsigned short field_04;
    unsigned short field_06;
    unsigned short field_08;
    unsigned short field_0A;
    unsigned short field_0C;
} MenuTableRow;

extern MenuTableRow D_08A0AF92[];
extern int func_089EF420(void);

unsigned short func_089EF4EC(void) {
    return D_08A0AF92[func_089EF420()].field_06;
}

unsigned short func_089EF524(void) {
    return D_08A0AF92[func_089EF420()].field_08;
}

unsigned short func_089EF55C(void) {
    return D_08A0AF92[func_089EF420()].field_0C;
}

unsigned short func_089EF594(void) {
    return D_08A0AF92[func_089EF420()].field_0A;
}

unsigned short func_089EF5CC(void) {
    return D_08A0AF92[func_089EF420()].field_08;
}
