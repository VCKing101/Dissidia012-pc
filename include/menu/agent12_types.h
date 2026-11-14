#ifndef MENU_AGENT12_TYPES_H
#define MENU_AGENT12_TYPES_H

/* Menu part whose vtable pointer sits after 0x24 bytes of data
   (0x08A57xxx). Virtual N is at vtable offset 8 * (N + 1). */
class A12Part24 {
public:
    char data_000[0x24];
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void v9();
    virtual void v10();
    virtual void v11();
};

/* Menu part whose vtable pointer sits after 0xC bytes of data (0x08A65xxx-0x08A66xxx). */
class A12Part0C {
public:
    unsigned char field_00;
    unsigned char field_01;
    unsigned char field_02;
    char data_003[0x9];
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void v9();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
};

#endif
