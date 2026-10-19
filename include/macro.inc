# Basic GNU assembler labels used by the generated disassembly.
# No local include-guard symbol: the community assembler mishandles its sh_info.
.macro glabel name
    .globl \name
    .type \name, @function
\name:
.endm
.macro alabel name
    glabel \name
.endm
.macro dlabel name
    .globl \name
    .type \name, @object
\name:
.endm
.macro jlabel name
    .globl \name
\name:
.endm
.macro endlabel name
    .size \name, . - \name
.endm
.macro enddlabel name
    .size \name, . - \name
.endm
.macro nonmatching name, size=0
.endm
