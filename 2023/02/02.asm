        %define MAX_RED 12
        %define MAX_GREEN 13
        %define MAX_BLUE 14

        %define MIN_RED 0
        %define MIN_GREEN 8
        %define MIN_BLUE 16

section .bss
buf:
        resb 20480
len: equ $ - buf

section .data
delim: db `\n :,`, 0
format: db `%d\n%d\n`, 0

section .text
global main
extern puts
extern strtok
extern atoi
extern printf

main:
        mov rax, 2              ; open
        mov rdi, [rsi + 8]
        mov rsi, 0              ; O_RDONLY
        syscall

        mov rdi, rax            ; fd
        mov rax, 0              ; read
        mov rsi, buf
        mov rdx, len
        syscall

        mov rax, 3              ; close
        syscall

        mov [buf + len], byte 0

        mov rbx, 0              ; Game ID.
        mov r12, 0              ; Cube count.
        mov r13, 0              ; Possible.
        mov r14, 0              ; Result.
        mov r15, 0              ; Power sum.

        sub rsp, 24             ; Minimum counts.

        mov rdi, buf
        mov rsi, delim
        call strtok
        jmp .strtok

.loop:
        mov rdi, 0
        mov rsi, delim
        call strtok
.strtok:
        cmp rax, 0
        je .done

        cmp [rax], byte 'G'     ; "Game N"
        jl .number

.done:
        cmp r13, 1
        jne .next

        add r14, rbx

.next:
        mov r13, 1              ; Possible.
        mov rdi, 0
        mov rsi, delim
        call strtok
        mov r11, rax

        mov rax, [rsp + MIN_RED]
        mul qword [rsp + MIN_GREEN]
        mul qword [rsp + MIN_BLUE]
        add r15, rax

        mov [rsp + MIN_RED], dword 1
        mov [rsp + MIN_RED + 4], dword 0
        mov [rsp + MIN_GREEN], dword 1
        mov [rsp + MIN_GREEN + 4], dword 0
        mov [rsp + MIN_BLUE], dword 1
        mov [rsp + MIN_BLUE + 4], dword 0

        cmp r11, 0
        je .exit

        mov rdi, r11
        call atoi
        mov rbx, rax

        jmp .loop

.number:
        mov rdi, rax
        call atoi
        mov r12, rax

        mov rdi, 0
        mov rsi, delim
        call strtok

        cmp [rax], byte 'g'
        jl .blue
        jg .red

        ; green
        mov rax, [rsp + MIN_GREEN]
        cmp r12, rax
        cmovg rax, r12
        mov [rsp + MIN_GREEN], rax
        cmp r12, MAX_GREEN
        jg .impossible
        jmp .loop

.blue:
        mov rax, [rsp + MIN_BLUE]
        cmp r12, rax
        cmovg rax, r12
        mov [rsp + MIN_BLUE], rax
        cmp r12, MAX_BLUE
        jg .impossible
        jmp .loop

.red:
        mov rax, [rsp + MIN_RED]
        cmp r12, rax
        cmovg rax, r12
        mov [rsp + MIN_RED], rax
        cmp r12, MAX_RED
        jg .impossible
        jmp .loop

.impossible:
        mov r13, 0              ; Impossible.
        jmp .loop

.exit:
        mov rdi, format
        mov rsi, r14
        mov rdx, r15
        call printf

        add rsp, 24

        ret
