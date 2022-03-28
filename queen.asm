STACKSG SEGMENT STACK
    GLOBAL_BUILTIN_GETS_BUF DW 129 DUP(0)
    GLOBAL_BUILTIN_GETS_CHARCOUNT DW 1 DUP(0)
    GLOBAL_BUILTIN_COMPACT_BUF DW 129 DUP(0)
    DW 70, 65, 73, 76, 32, 84, 79, 32, 79, 80, 69, 78, 70, 73, 76, 69, 10, 0
    DW 70, 65, 73, 76, 32, 84, 79, 32, 67, 76, 79, 83, 69, 70, 73, 76, 69, 10, 0
    GLOBAL_col DW 9 DUP(0)
    GLOBAL_add DW 16 DUP(0)
    GLOBAL_sub DW 16 DUP(0)
    GLOBAL_ans DW 1 DUP(0)
    DW 16046 DUP(0)
STACKSG ENDS

CODESG SEGMENT
    ASSUME CS:CODESG, DS:STACKSG, SS:STACKSG

FUNC_checkPos: ; localVar:     2, arguCnt:     2
    PUSH BP
    MOV BP, SP
    SUB SP, 4
    MOV AX, [BP+4]
    MOV [BP-2], AX
    MOV AX, [BP+6]
    MOV [BP-4], AX
    MOV AX, 592
    MOV BX, WORD PTR [BP-4] ;optimized by O1
    ADD BX, BX
    ADD BX, AX
    MOV AX, WORD PTR [BX] ;optimized by O1
    MOV CX, 0
    AND AX, AX
    JNZ JUMP_44
    MOV CX, 1
JUMP_44:
    PUSH CX
    MOV AX, 610
    PUSH AX
    MOV AX, WORD PTR [BP-2] ;optimized by O1
    MOV BX, WORD PTR [BP-4] ;optimized by O1
    ADD AX, BX
    PUSH AX
    MOV AX, 1
    MOV BX, AX ;optimized by O1
    POP AX
    SUB AX, BX
    MOV BX, AX ;optimized by O1
    POP AX
    ADD BX, BX
    ADD BX, AX
    MOV AX, WORD PTR [BX] ;optimized by O1
    MOV CX, 0
    AND AX, AX
    JNZ JUMP_45
    MOV CX, 1
JUMP_45:
    MOV BX, CX ;optimized by O1
    AND BX, BX
    JZ JUMP_46
    MOV BX, 1
JUMP_46:
    POP AX
    AND AX, AX
    JZ JUMP_47
    MOV AX, 1
JUMP_47:
    AND AX, BX
    PUSH AX
    MOV AX, 642
    PUSH AX
    MOV AX, WORD PTR [BP-2] ;optimized by O1
    MOV BX, WORD PTR [BP-4] ;optimized by O1
    SUB AX, BX
    PUSH AX
    MOV AX, 8
    MOV BX, AX ;optimized by O1
    POP AX
    ADD AX, BX
    MOV BX, AX ;optimized by O1
    POP AX
    ADD BX, BX
    ADD BX, AX
    MOV AX, WORD PTR [BX] ;optimized by O1
    MOV CX, 0
    AND AX, AX
    JNZ JUMP_48
    MOV CX, 1
JUMP_48:
    MOV BX, CX ;optimized by O1
    AND BX, BX
    JZ JUMP_49
    MOV BX, 1
JUMP_49:
    POP AX
    AND AX, AX
    JZ JUMP_50
    MOV AX, 1
JUMP_50:
    AND AX, BX
    JMP ENDFUNC_checkPos
ENDFUNC_checkPos:
    ADD SP, 4
    POP BP
    RET

FUNC_dfs: ; localVar:     2, arguCnt:     1
    PUSH BP
    MOV BP, SP
    SUB SP, 4
    MOV AX, [BP+4]
    MOV [BP-2], AX
    PUSH WORD PTR [BP-2]
    MOV AX, 9
    MOV BX, AX ;optimized by O1
    POP AX
    MOV CX, 1
    SUB AX, BX
    JGE JUMP_51
    MOV CX, 0
JUMP_51:
    MOV AX, CX ;optimized by O1
    AND AX, AX
    JNZ JUMP_52
    JMP ELSE_11
JUMP_52:
    MOV SI, 674
    PUSH WORD PTR [SI]
    MOV AX, 1
    MOV BX, AX ;optimized by O1
    POP AX
    ADD AX, BX
    MOV SI, 674
    MOV [SI], AX
    JMP ENDIF_11
ELSE_11:
    MOV AX, 1
    MOV [BP-4], AX
WHILE_9:
    MOV AX, 8
    MOV BX, WORD PTR [BP-4] ;optimized by O1
    MOV CX, 1
    SUB AX, BX
    JGE JUMP_53
    MOV CX, 0
JUMP_53:
    MOV AX, CX ;optimized by O1
    AND AX, AX
    JNZ JUMP_54
    JMP ENDWHILE_9
JUMP_54:
    PUSH WORD PTR [BP-2]
    PUSH WORD PTR [BP-4]
    MOV SI, SP
    MOV AX, [SI+0]
    MOV BX, [SI+2]
    MOV [SI+0], BX
    MOV [SI+2], AX
    CALL FUNC_checkPos
    ADD SP, 4
    AND AX, AX
    JNZ JUMP_55
    JMP ENDIF_12
JUMP_55:
    PUSH WORD PTR [BP-2]
    PUSH WORD PTR [BP-4]
    MOV AX, 1
    PUSH AX
    MOV SI, SP
    MOV AX, [SI+0]
    MOV BX, [SI+4]
    MOV [SI+0], BX
    MOV [SI+4], AX
    CALL FUNC_setQueen
    ADD SP, 6
    PUSH WORD PTR [BP-2]
    MOV AX, 1
    MOV BX, AX ;optimized by O1
    POP AX
    ADD AX, BX
    PUSH AX
    CALL FUNC_dfs
    ADD SP, 2
    PUSH WORD PTR [BP-2]
    PUSH WORD PTR [BP-4]
    MOV AX, 0
    PUSH AX
    MOV SI, SP
    MOV AX, [SI+0]
    MOV BX, [SI+4]
    MOV [SI+0], BX
    MOV [SI+4], AX
    CALL FUNC_setQueen
    ADD SP, 6
ENDIF_12:
    PUSH WORD PTR [BP-4]
    MOV AX, 1
    MOV BX, AX ;optimized by O1
    POP AX
    ADD AX, BX
    MOV [BP-4], AX
    JMP WHILE_9
ENDWHILE_9:
ENDIF_11:
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
    MOV SI, 674
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
    PUSH WORD PTR [BP-2]
    MOV AX, 0
    MOV BX, AX ;optimized by O1
    POP AX
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
    MOV AX, 45
    MOV DX, AX ;optimized by O1
    MOV AH, 2
    INT 21H
ENDIF_7:
    PUSH WORD PTR [BP-2]
    MOV AX, 0
    MOV BX, AX ;optimized by O1
    POP AX
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
    MOV AX, 48
    MOV DX, AX ;optimized by O1
    MOV AH, 2
    INT 21H
    JMP ENDIF_8
ELSE_8:
WHILE_5:
    PUSH WORD PTR [BP-2]
    MOV AX, 0
    MOV BX, AX ;optimized by O1
    POP AX
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
    PUSH WORD PTR [BP-2]
    MOV AX, 10
    MOV BX, AX ;optimized by O1
    POP AX
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
    PUSH WORD PTR [BP-2]
    MOV AX, 10
    MOV BX, AX ;optimized by O1
    POP AX
    MOV DX, 0
    CMP AX, 0
    JGE JUMP_33
    MOV DX, 0FFFFH
JUMP_33:
    IDIV BX
    MOV [BP-2], AX
    PUSH WORD PTR [BP-24]
    MOV AX, 1
    MOV BX, AX ;optimized by O1
    POP AX
    ADD AX, BX
    MOV [BP-24], AX
    JMP WHILE_5
ENDWHILE_5:
WHILE_6:
    PUSH WORD PTR [BP-24]
    MOV AX, 0
    MOV BX, AX ;optimized by O1
    POP AX
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
    PUSH WORD PTR [BP-24]
    MOV AX, 1
    MOV BX, AX ;optimized by O1
    POP AX
    SUB AX, BX
    MOV BX, AX ;optimized by O1
    POP AX
    ADD BX, BX
    ADD BX, AX
    PUSH WORD PTR [BX]
    MOV AX, 48
    MOV BX, AX ;optimized by O1
    POP AX
    ADD AX, BX
    MOV DX, AX ;optimized by O1
    MOV AH, 2
    INT 21H
    PUSH WORD PTR [BP-24]
    MOV AX, 1
    MOV BX, AX ;optimized by O1
    POP AX
    SUB AX, BX
    MOV [BP-24], AX
    JMP WHILE_6
ENDWHILE_6:
ENDIF_8:
ENDFUNC_printInt:
    ADD SP, 24
    POP BP
    RET

FUNC_setQueen: ; localVar:     3, arguCnt:     3
    PUSH BP
    MOV BP, SP
    SUB SP, 6
    MOV AX, [BP+4]
    MOV [BP-2], AX
    MOV AX, [BP+6]
    MOV [BP-4], AX
    MOV AX, [BP+8]
    MOV [BP-6], AX
    MOV AX, 592
    MOV BX, WORD PTR [BP-4] ;optimized by O1
    MOV CX, WORD PTR [BP-6] ;optimized by O1
    ADD BX, BX
    ADD BX, AX
    MOV [BX], CX
    MOV AX, 610
    PUSH AX
    MOV AX, WORD PTR [BP-2] ;optimized by O1
    MOV BX, WORD PTR [BP-4] ;optimized by O1
    ADD AX, BX
    PUSH AX
    MOV AX, 1
    MOV BX, AX ;optimized by O1
    POP AX
    SUB AX, BX
    MOV BX, AX ;optimized by O1
    MOV CX, WORD PTR [BP-6] ;optimized by O1
    POP AX
    ADD BX, BX
    ADD BX, AX
    MOV [BX], CX
    MOV AX, 642
    PUSH AX
    MOV AX, WORD PTR [BP-2] ;optimized by O1
    MOV BX, WORD PTR [BP-4] ;optimized by O1
    SUB AX, BX
    PUSH AX
    MOV AX, 8
    MOV BX, AX ;optimized by O1
    POP AX
    ADD AX, BX
    MOV BX, AX ;optimized by O1
    MOV CX, WORD PTR [BP-6] ;optimized by O1
    POP AX
    ADD BX, BX
    ADD BX, AX
    MOV [BX], CX
ENDFUNC_setQueen:
    ADD SP, 6
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
