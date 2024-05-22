section .text
GLOBAL getHours, getMinutes, getSeconds, getDay, getMonth, getYear

config_rtc_output:
  mov al, 0x0B
  out 70h, al
  in al, 71h
  or al, 4
  out 71h, al
  ret

get_time:
  call config_rtc_output
  mov rax, rdi
  out 70h, al
  in al, 71h
  movzx rax, al
  ret

getYear:
  cli
  mov rdi, 0x09
  call get_time
  sti
  ret

getMonth:
  cli
  mov rdi, 0x08
  call get_time
  sti
  ret

getDay:
  cli
  mov rdi, 0x07
  call get_time
  sti
  ret

getHours:
  cli
  mov rdi, 0x04
  call get_time
  sti
  ret

getMinutes:
  cli
  mov rdi, 0x02
  call get_time
  sti
  ret

getSeconds:
  cli
  mov rdi, 0x00
  call get_time
  sti
  ret