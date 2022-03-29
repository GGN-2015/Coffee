STACKSG SEGMENT STACK
    GLOBAL_compact_buffer DW 129 DUP(0)
    DW 104, 101, 108, 108, 111, 32, 105, 110, 110, 101, 114, 32, 65, 83, 77, 33, 36, 0
    DW 16237 DUP(0)
STACKSG ENDS

CODESG SEGMENT
    ASSUME CS:CODESG, DS:STACKSG, SS:STACKSG

FUNC_compact: ; localVar:     1, arguCnt:     1
    PUSH BP
    MOV BP, SP
    SUB SP, 2
    MOV AX, [BP+4]
    MOV [BP-2], AX
    MOV SI, [BP-2]                ; code from inner asm
    LEA DI, GLOBAL_compact_buffer ; code from inner asm
    INNER_ASM_FLAG_1:             ; code from inner asm
    MOV AX, [SI]                  ; code from inner asm
    MOV BYTE PTR [DI], AL         ; code from inner asm
    ADD SI, 2                     ; code from inner asm
    INC DI                        ; code from inner asm
    AND AX, AX                    ; code from inner asm
    JNZ INNER_ASM_FLAG_1          ; code from inner asm
ENDFUNC_compact:
    ADD SP, 2
    POP BP
    RET

FUNC_main: ; localVar:     1, arguCnt:     0
    PUSH BP
    MOV BP, SP
    SUB SP, 2
    MOV AX, 258
    MOV [BP-2], AX
    PUSH WORD PTR [BP-2]
    CALL FUNC_output_hello
    ADD SP, 2
ENDFUNC_main:
    ADD SP, 2
    POP BP
    RET

FUNC_output_hello: ; localVar:     1, arguCnt:     1
    PUSH BP
    MOV BP, SP
    SUB SP, 2
    MOV AX, [BP+4]
    MOV [BP-2], AX
    PUSH WORD PTR [BP-2]
    CALL FUNC_compact
    ADD SP, 2
    MOV AH, 09H                   ; code from inner asm
    LEA DX, GLOBAL_compact_buffer ; code from inner asm
    INT 21H                       ; code from inner asm
ENDFUNC_output_hello:
    ADD SP, 2
    POP BP
    RET

ENTER_POINT:
    MOV AX, STACKSG
    MOV DS, AX
    MOV SS, AX
    MOV SP, 32768
    CALL FUNC_main
    MOV AH, 4CH
    INT 21H
CODESG ENDS
END ENTER_POINT
