#include "menu/execution_state.h"
extern const char D_089E0704[];
extern const char D_089E070C[];
extern const char D_089E0718[];
extern const char D_089E0724[];
extern const unsigned char D_089E06F1[16];
extern const char *D_089D6010;
extern void *D_089D7280;
extern int func_0893476C(char *, const char *, ...);
extern void *func_08935BB4(void *, const void *, unsigned int);
extern void *func_08935828(void *, int, unsigned int);
extern void func_088B168C(void *, void *, unsigned int);
extern void *func_08935524(void *, const void *, unsigned int);
extern int func_089C5858(MenuSavedataRequest *);
void func_089DD760(MenuRuntimePrefix *self) {
    {
    char name[20];
    int i;
    for (i = 0; i < 20; ++i) {
        func_0893476C(name,D_089E0704,D_089E070C,i);
        func_08935BB4(self->field_01DC->names_0C28[i],name,20);
    }
    }
    func_08935828(&self->field_01DC->savedata_007C,0,0x600);
    func_088B168C(D_089D7280,0x7C + (unsigned char *)self->field_01DC,0x600);
    self->field_01DC->savedata_007C.mode = 11;
    self->field_01DC->savedata_007C.overwrite = 0;
    func_08935BB4(self->field_01DC->savedata_007C.game_name,D_089D6010,13);
    func_08935BB4(self->field_01DC->savedata_007C.save_name,D_089E0718,20);
    self->field_01DC->savedata_007C.name_list = self->field_01DC->names_0C28;
    func_08935BB4(self->field_01DC->savedata_007C.file_name,D_089E0724,13);
    self->field_01DC->savedata_007C.data = (unsigned char *)self->field_01DC + 0xDD0;
    self->field_01DC->savedata_007C.data_capacity = 0x3ECC8;
    self->field_01DC->savedata_007C.data_size = 0x3ECC8;
    self->field_01DC->savedata_007C.focus = 3;
    func_08935524(self->field_01DC->savedata_007C.key,D_089E06F1,16);
    self->field_01DC->savedata_007C.secure_version = 0;
    self->field_01DC->savedata_007C.multi_status = 1;
    self->field_01DC->savedata_007C.id_list = &self->field_01DC->slots_067C;
    self->field_01DC->slots_067C.capacity = 20;
    self->field_01DC->slots_067C.entries = self->field_01DC->entries_0688;
    int result = func_089C5858(&self->field_01DC->savedata_007C);
    self->field_01DC->field_0008 = !result ? self->field_01DC->field_0008 + 1 : 6;
}
