STACKSG SEGMENT STACK
    GLOBAL_BUILTIN_GETS_BUF DW 129 DUP(0)
    GLOBAL_BUILTIN_GETS_CHARCOUNT DW 1 DUP(0)
    GLOBAL_BUILTIN_COMPACT_BUF DW 129 DUP(0)
    DW 70,65,73,76,32,84,79,32,79,80
    DW 69,78,70,73,76,69,10,0
    DW 70,65,73,76,32,84,79,32,67,76
    DW 79,83,69,70,73,76,69,10,0
    DW 16088 DUP(0)
STACKSG ENDS

CODESG SEGMENT
    ASSUME CS:CODESG, DS:STACKSG, SS:STACKSG

FUNC_main: ; localVar:     1, arguCnt:     0
    PUSH BP
    MOV BP, SP
    SUB SP, 2
    MOV AX, 3
    MOV BX, 5 ;optimized by O1
    ADD AX, BX
    NEG AX
    PUSH AX
    MOV AX, 9
    MOV BX, -6 ;optimized by O1
    ADD AX, BX
    MOV BX, AX ;optimized by O1
    POP AX
    IMUL BX
    MOV BX, 60 ;optimized by O1
    SUB AX, BX
    MOV [BP-2], AX
    PUSH WORD PTR [BP-2]
    CALL FUNC_printInt
    ADD SP, 2
ENDFUNC_main:
    ADD SP, 2
    POP BP
    RET

FUNC_printInt: ; localVar:    12, arguCnt:     1
    PUSH BP
    MOV BP, SP
    SUB SP, 24
    MOV AX, [BP+4]
    MOV [BP-2], AX
    MOV AX, 0
    MOV [BP-24], AX
    MOV AX, WORD PTR [BP-2] ;optimized by O1
    MOV BX, 0 ;optimized by O1
    MOV CX, 1
    SUB AX, BX
    JL JUMP_26
    MOV CX, 0
JUMP_26:
    MOV AX, CX ;optimized by O1
    AND AX, AX
    JNZ JUMP_27
    JMP ENDIF_7
JUMP_27:
    MOV AX, WORD PTR [BP-2] ;optimized by O1
    NEG AX
    MOV [BP-2], AX
    MOV DX, 45 ;optimized by O1
    MOV AH, 2
    INT 21H
ENDIF_7:
    MOV AX, WORD PTR [BP-2] ;optimized by O1
    MOV BX, 0 ;optimized by O1
    MOV CX, 1
    SUB AX, BX
    JE JUMP_28
    MOV CX, 0
JUMP_28:
    MOV AX, CX ;optimized by O1
    AND AX, AX
    JNZ JUMP_29
    JMP ELSE_8
JUMP_29:
    MOV DX, 48 ;optimized by O1
    MOV AH, 2
    INT 21H
    JMP ENDIF_8
ELSE_8:
WHILE_5:
    MOV AX, WORD PTR [BP-2] ;optimized by O1
    MOV BX, 0 ;optimized by O1
    MOV CX, 1
    SUB AX, BX
    JNE JUMP_30
    MOV CX, 0
JUMP_30:
    MOV AX, CX ;optimized by O1
    AND AX, AX
    JNZ JUMP_31
    JMP ENDWHILE_5
JUMP_31:
    MOV AX, BP
    SUB AX, 22
    PUSH AX
    PUSH WORD PTR [BP-24]
    MOV AX, WORD PTR [BP-2] ;optimized by O1
    MOV BX, 10 ;optimized by O1
    MOV DX, 0
    CMP AX, 0
    JGE JUMP_32
    MOV DX, 0FFFFH
JUMP_32:
    IDIV BX
    MOV CX, DX ;optimized by O1
    POP BX
    POP AX
    ADD BX, BX
    ADD BX, AX
    MOV [BX], CX
    MOV AX, WORD PTR [BP-2] ;optimized by O1
    MOV BX, 10 ;optimized by O1
    MOV DX, 0
    CMP AX, 0
    JGE JUMP_33
    MOV DX, 0FFFFH
JUMP_33:
    IDIV BX
    MOV [BP-2], AX
    MOV AX, WORD PTR [BP-24] ;optimized by O1
    MOV BX, 1 ;optimized by O1
    ADD AX, BX
    MOV [BP-24], AX
    JMP WHILE_5
ENDWHILE_5:
WHILE_6:
    MOV AX, WORD PTR [BP-24] ;optimized by O1
    MOV BX, 0 ;optimized by O1
    MOV CX, 1
    SUB AX, BX
    JG JUMP_34
    MOV CX, 0
JUMP_34:
    MOV AX, CX ;optimized by O1
    AND AX, AX
    JNZ JUMP_35
    JMP ENDWHILE_6
JUMP_35:
    MOV AX, BP
    SUB AX, 22
    PUSH AX
    MOV AX, WORD PTR [BP-24] ;optimized by O1
    MOV BX, 1 ;optimized by O1
    SUB AX, BX
    MOV BX, AX ;optimized by O1
    POP AX
    ADD BX, BX
    ADD BX, AX
    MOV AX, WORD PTR [BX] ;optimized by O1
    MOV BX, 48 ;optimized by O1
    ADD AX, BX
    MOV DX, AX ;optimized by O1
    MOV AH, 2
    INT 21H
    MOV AX, WORD PTR [BP-24] ;optimized by O1
    MOV BX, 1 ;optimized by O1
    SUB AX, BX
    MOV [BP-24], AX
    JMP WHILE_6
ENDWHILE_6:
ENDIF_8:
ENDFUNC_printInt:
    ADD SP, 24
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
