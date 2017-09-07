#include "system.h"
#include "screen.h"

  .globl set_color
  .globl clear_screen
  .globl get_color
  .globl set_cursor
  .globl put_char
  .globl puts
  .globl divide

  .data
chr_attr: .byte 0

  .text

# void set_color(uint8_t fgcolor, uint8_t bgcolor){
#   chr_attr = (bgcolor << 4) | (fgcolor & 0x0F);
# }

set_color:
  sll $a1, $a1, 4
  andi $a0, $a0, 0x0F
  or $t0, $a1, $a0
  sb $t0, chr_attr
  jr $ra

# void get_color(uint8_t fgcolor, uint8_t bgcolor){
#   *fgcolor = chr_attr
# }

get_color:

# void set_cursor(uint row, uint col){
#   if(row > (MAX_ROWS - 1) || col > (MAX_COLS - 1))
#     return;
#
#
# }
