#include "screen.h"
#include "system.h"
.global main

.data

msg: .asciz "Hello World\n"
.text
main:
    li $a0, BRIGHT_WHITE
    li $a1, BLACK
    jal set_color
    jal clear_screen

    li $a0, 2
    li $a1, 2
    jal power

    move $a0, $v0
    jal put_udecimal

    jr $ra
