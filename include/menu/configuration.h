#ifndef MENU_CONFIGURATION_H
#define MENU_CONFIGURATION_H

/* This models only the observed prefix and two float fields. Its sizeof is
 * not evidence of the original object's complete size or type. */
typedef struct MenuConfigurationPrefix {
    unsigned char unknown_0000[0x20];
    float field_0020;
    unsigned char unknown_0024[0x3C];
    float field_0060;
} MenuConfigurationPrefix;

extern MenuConfigurationPrefix D_08A94670;

#endif
