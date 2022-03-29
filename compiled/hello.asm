STACKSG SEGMENT STACK
    GLOBAL_BUILTIN_GETS_BUF DW 129 DUP(0)
    GLOBAL_BUILTIN_GETS_CHARCOUNT DW 1 DUP(0)
    DW 72, 101, 108, 108, 111, 32, 81, 117, 111, 116, 101, 34, 32, 10, 32, 108, 105, 107, 101, 32, 116, 104, 105, 115, 33, 0
    DW 16228 DUP(0)
STACKSG ENDS

CODESG SEGMENT
    ASSUME CS:CODESG, DS:STACKSG, SS:STACKSG

FUNC_main: ; localVar:     0, arguCnt:     0
    PUSH BP
    MOV BP, SP
    SUB SP, 0
    MOV AX, 260
    PUSH AX
    CALL FUNC_print
    ADD SP, 2
ENDFUNC_main:
    ADD SP, 0
    POP BP
    RET

FUNC_print: ; localVar:     2, arguCnt:     1
    PUSH BP
    MOV BP, SP
    SUB SP, 4
    MOV AX, [BP+4]
    MOV [BP-2], AX
    MOV AX, 0
    MOV [BP-4], AX
WHILE_2:
    PUSH WORD PTR [BP-2]
    MOV AX, 2
    MOV BX, WORD PTR [BP-4] ;optimized by O1
    IMUL BX
    MOV BX, AX ;optimized by O1
    POP AX
    ADD AX, BX
    MOV BX, AX ;optimized by O1
    MOV AX, WORD PTR [BX] ;optimized by O1
    MOV BX, 0 ;optimized by O1
    MOV CX, 1
    SUB AX, BX
    JNE JUMP_12
    MOV CX, 0
JUMP_12:
    MOV AX, CX ;optimized by O1
    AND AX, AX
    JNZ JUMP_13
    JMP ENDWHILE_2
JUMP_13:
    PUSH WORD PTR [BP-2]
    MOV AX, 2
    MOV BX, WORD PTR [BP-4] ;optimized by O1
    IMUL BX
    MOV BX, AX ;optimized by O1
    POP AX
    ADD AX, BX
    MOV BX, AX ;optimized by O1
    MOV DX, WORD PTR [BX] ;optimized by O1
    MOV AH, 2
    INT 21H
    MOV AX, WORD PTR [BP-4] ;optimized by O1
    MOV BX, 1 ;optimized by O1
    ADD AX, BX
    MOV [BP-4], AX
    JMP WHILE_2
ENDWHILE_2:
ENDFUNC_print:
    ADD SP, 4
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
