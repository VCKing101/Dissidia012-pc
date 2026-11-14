typedef struct Header { char data_00[0xC]; unsigned short size; char data_0E[0x58 - 0xE]; } Header;
typedef struct Body { char data_00[0x3D]; signed char state; char data_3E[2]; } Body;
extern int func_088916A4(unsigned char *path, Header *header);
extern int func_08891894(unsigned char *path, Body *body, unsigned short size);

int func_088912D0(unsigned char *path)
{
    Header header;
    Body body;
    if (*path == 0) {
        return 0;
    }
    if (func_088916A4(path, &header) != 0) {
        return 0;
    }
    if (func_08891894(path, &body, header.size) != 0) {
        return 0;
    }
    return body.state >= 0;
}
