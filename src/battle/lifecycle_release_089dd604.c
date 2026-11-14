/* Detaches the two linked owners and releases every subobject. */
typedef struct BattleLink {
    int field_00;
    int field_04;
} BattleLink;

typedef struct BattleRootLinks {
    unsigned char field_00;
    unsigned char padding_01[3];
    BattleLink *field_04;
    BattleLink *field_08;
} BattleRootLinks;

extern void func_089ED01C(void *);
extern void func_089E07AC(void *);
extern void func_089E69C0(void *);
extern void func_089E7200(void *);
extern void func_089E7AFC(void *);
extern void func_089E3300(void *);
extern void func_089DD5D4(void *);

void func_089DD604(BattleRootLinks *object)
{
    if (object->field_08) object->field_08->field_04 = 0;
    if (object->field_04) object->field_04->field_04 = 0;
    func_089ED01C((unsigned char *)object + 0x1D04);
    func_089E07AC((unsigned char *)object + 0x1C6C);
    func_089E69C0((unsigned char *)object + 0xA8C);
    func_089E7200((unsigned char *)object + 0xA44);
    func_089E7AFC((unsigned char *)object + 0x1DC);
    func_089E3300((unsigned char *)object + 0x18);
    func_089DD5D4(object);
}
