typedef struct Header { char data_00[0xC]; unsigned short size; char data_0E[0x58 - 0xE]; } Header;
typedef struct Body { char data_00[0x2C]; short state; char data_2E[0x40 - 0x2E]; } Body;
extern int func_08891C18(void *store, Header *header, int slot);
extern int func_08891CA4(void *store, Body *body, int arg, unsigned short size);

int func_08891B8C(void *store, int arg, int slot)
{
    Header header;
    Body body;
    if (slot < 0) {
        return 0;
    }
    if (func_08891C18(store, &header, slot) != 0) {
        return 0;
    }
    if (func_08891CA4(store, &body, arg, header.size) != 0) {
        return 0;
    }
    return body.state == 6;
}
