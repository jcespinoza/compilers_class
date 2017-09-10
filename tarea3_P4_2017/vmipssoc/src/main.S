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
    la $a0, msg
    #move $a0, msg
    jal puts

    jr $ra
