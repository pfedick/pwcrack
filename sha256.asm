BITS 64
;%include "x86_64.inc"

section .data
    input_msg db "Hello, world!", 0   ; Message to hash
    input_len equ $ - input_msg       ; Length of input
    pad_msg resb 64                   ; 512-bit padded message block
    initial_hash dq 0x6a09e667f3bcc908, 0xbb67ae8584caa73b, 0x3c6ef372fe94f82b, 0xa54ff53a5f1d36f1
                 dq 0x510e527fade682d1, 0x9b05688c2b3e6c1f, 0x1f83d9abfb41bd6b, 0x5be0cd19137e2179
    round_constants dq 0x428a2f98d728ae22, 0x7137449123ef65cd, 0xb5c0fbcfec4d3b2f, 0xe9b5dba58189dbbc
    hex_chars db "0123456789abcdef", 0
    newline db 10, 0


section .bss
    digest resb 32  ; Store the SHA-256 hash output
    W resb 256      ; Message schedule (W0-W63)

section .text
    global _start
    extern printf

    ; Step 1: Check for SHA-NI Support
    mov eax, 7
    mov ecx, 0
    cpuid
    test ebx, (1 << 29)  ; Check if SHA bit is set
    jz no_sha_support

    ; Step 2: Prepare Padded Message Block (64 bytes)
    call pad_message

    ; Step 3: Initialize Hash Values
    movdqu xmm0, [initial_hash]    ; Load H0-H1
    movdqu xmm1, [initial_hash+16] ; Load H2-H3
    movdqu xmm2, [initial_hash+32] ; Load H4-H5
    movdqu xmm3, [initial_hash+48] ; Load H6-H7

    ; Step 4: Load Constants
    movdqu xmm4, [round_constants]  ; Load K0-K1

    ; Step 5: Process the Message Block (W0-W15)
    movdqu xmm5, [pad_msg]
    movdqu xmm6, [pad_msg+16]
    movdqu xmm7, [pad_msg+32]
    movdqu xmm8, [pad_msg+48]

    ; Step 6: SHA256 Compression (64 Rounds)
    sha256rnds2 xmm0, xmm4, xmm5
    sha256rnds2 xmm1, xmm4, xmm6
    sha256rnds2 xmm2, xmm4, xmm7
    sha256rnds2 xmm3, xmm4, xmm8

    ; Step 7: Store the Final Hash
    movdqu [digest], xmm0
    movdqu [digest+16], xmm1

    ; Step 8: Print the Hash in Hex
    call print_hash

    ; Exit
    mov eax, 60
    xor edi, edi
    syscall


no_sha_support:
    ; Print "SHA not supported"
    mov rdi, sha_error_msg
    call printf
    jmp exit

exit:
    mov eax, 60
    xor edi, edi
    syscall


; -----------------------------
; Pad the Input Message to 64 bytes
; -----------------------------
pad_message:
    mov rsi, input_msg
    mov rdi, pad_msg
    mov rcx, input_len
    rep movsb             ; Copy input message

    mov byte [rdi], 0x80  ; Append '1' bit

    ; Zero-pad remaining bytes until last 8 bytes
    mov rcx, 55 - input_len
    xor rax, rax
    rep stosb

    ; Append message length (in bits)
    mov rax, input_len
    shl rax, 3            ; Convert length to bits
    mov [pad_msg+56], rax
    ret

; -----------------------------
; Print Hash in Hex Format
; -----------------------------
print_hash:
    mov rsi, digest
    mov rcx, 32           ; Process 32 bytes
print_loop:
    movzx rbx, byte [rsi] ; Load byte
    shr rbx, 4            ; Extract upper 4 bits
    mov rdi, hex_chars
    movzx rdx, byte [rdi+rbx]
    movzx rdi, rdx
    call putchar

    movzx rbx, byte [rsi] ; Reload byte
    and rbx, 0x0F         ; Extract lower 4 bits
    mov rdi, hex_chars
    movzx rdx, byte [rdi+rbx]
    movzx rdi, rdx
    call putchar

    inc rsi
    loop print_loop

    mov rdi, newline
    call puts
    ret

; -----------------------------
; Print a Single Character
; -----------------------------
putchar:
    mov rdx, 1
    mov rsi, rdi
    mov rdi, 1
    mov eax, 1
    syscall
    ret