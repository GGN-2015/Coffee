STACKSG SEGMENT STACK
    GLOBAL_BUILTIN_GETS_BUF DW 129 DUP(0)
    GLOBAL_BUILTIN_GETS_CHARCOUNT DW 1 DUP(0)
    GLOBAL_col DW 9 DUP(0)
    GLOBAL_ans DW 1 DUP(0)
    DW 16244 DUP(0)
STACKSG ENDS

CODESG SEGMENT
    ASSUME CS:CODESG, DS:STACKSG, SS:STACKSG

FUNC_dfs: ; localVar:     2, arguCnt:     1
    PUSH BP
    MOV BP, SP
    SUB SP, 4
    MOV AX, [BP+4]
    MOV [BP-2], AX
    MOV AX, WORD PTR [BP-2] ;optimized by O1
    MOV BX, 9 ;optimized by O1
    MOV CX, 1
    SUB AX, BX
    JGE JUMP_36
    MOV CX, 0
JUMP_36:
    MOV AX, CX ;optimized by O1
    AND AX, AX
    JNZ JUMP_37
    JMP ELSE_9
JUMP_37:
    MOV SI, 278
    MOV AX, WORD PTR [SI] ;optimized by O1
    MOV BX, 1 ;optimized by O1
    ADD AX, BX
    MOV SI, 278
    MOV [SI], AX
    JMP ENDIF_9
ELSE_9:
    MOV AX, 1
    MOV [BP-4], AX
WHILE_7:
    MOV AX, WORD PTR [BP-4] ;optimized by O1
    MOV BX, 8 ;optimized by O1
    MOV CX, 1
    SUB AX, BX
    JLE JUMP_38
    MOV CX, 0
JUMP_38:
    MOV AX, CX ;optimized by O1
    AND AX, AX
    JNZ JUMP_39
    JMP ENDWHILE_7
JUMP_39:
    MOV AX, 260
    MOV BX, WORD PTR [BP-4] ;optimized by O1
    ADD BX, BX
    ADD BX, AX
    MOV AX, WORD PTR [BX] ;optimized by O1
    MOV BX, 0 ;optimized by O1
    MOV CX, 1
    SUB AX, BX
    JE JUMP_40
    MOV CX, 0
JUMP_40:
    MOV AX, CX ;optimized by O1
    AND AX, AX
    JNZ JUMP_41
    JMP ENDIF_10
JUMP_41:
    MOV AX, 260
    MOV BX, WORD PTR [BP-4] ;optimized by O1
    MOV CX, 1 ;optimized by O1
    ADD BX, BX
    ADD BX, AX
    MOV [BX], CX
    MOV AX, WORD PTR [BP-2] ;optimized by O1
    MOV BX, 1 ;optimized by O1
    ADD AX, BX
    PUSH AX
    CALL FUNC_dfs
    ADD SP, 2
    MOV AX, 260
    MOV BX, WORD PTR [BP-4] ;optimized by O1
    MOV CX, 0 ;optimized by O1
    ADD BX, BX
    ADD BX, AX
    MOV [BX], CX
ENDIF_10:
    MOV AX, WORD PTR [BP-4] ;optimized by O1
    MOV BX, 1 ;optimized by O1
    ADD AX, BX
    MOV [BP-4], AX
    JMP WHILE_7
ENDWHILE_7:
ENDIF_9:
ENDFUNC_dfs:
    ADD SP, 4
    POP BP
    RET

FUNC_main: ; localVar:     0, arguCnt:     0
    PUSH BP
    MOV BP, SP
    SUB SP, 0
    MOV AX, 1
    PUSH AX
    CALL FUNC_dfs
    ADD SP, 2
    MOV SI, 278
    PUSH WORD PTR [SI]
    CALL FUNC_printInt
    ADD SP, 2
ENDFUNC_main:
    ADD SP, 0
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
