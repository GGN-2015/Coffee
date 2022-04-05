STACKSG SEGMENT STACK
    GLOBAL_BUILTIN_GETS_BUF DW 129 DUP(0)
    GLOBAL_BUILTIN_GETS_CHARCOUNT DW 1 DUP(0)
    GLOBAL_BUILTIN_COMPACT_BUF DW 129 DUP(0)
    DW 70,65,73,76,32,84,79,32,79,80
    DW 69,78,70,73,76,69,10,0
    DW 70,65,73,76,32,84,79,32,67,76
    DW 79,83,69,70,73,76,69,10,0
    DW 32,97,32,61,32,0
    DW 32,98,32,61,32,0
    DW 73,110,112,117,116,32,120,58,0
    DW 73,110,112,117,116,32,121,58,0
    DW 32,97,32,61,32,0
    DW 32,98,32,61,32,0
    DW 32,103,99,100,32,61,32,0
    DW 16038 DUP(0)
STACKSG ENDS

CODESG SEGMENT
    ASSUME CS:CODESG, DS:STACKSG, SS:STACKSG

FUNC_exgcd: ; localVar:     7, arguCnt:     4
    PUSH BP
    MOV BP, SP
    SUB SP, 14
    MOV AX, [BP+4]
    MOV [BP-2], AX
    MOV AX, [BP+6]
    MOV [BP-4], AX
    MOV AX, [BP+8]
    MOV [BP-6], AX
    MOV AX, [BP+10]
    MOV [BP-8], AX
    MOV AX, 592
    PUSH AX
    CALL FUNC_print
    ADD SP, 2
    PUSH WORD PTR [BP-2]
    CALL FUNC_printInt
    ADD SP, 2
    MOV DX, 44 ;optimized by O1
    MOV AH, 2
    INT 21H
    MOV AX, 604
    PUSH AX
    CALL FUNC_print
    ADD SP, 2
    PUSH WORD PTR [BP-4]
    CALL FUNC_printInt
    ADD SP, 2
    MOV DX, 10 ;optimized by O1
    MOV AH, 2
    INT 21H
    MOV AX, WORD PTR [BP-4] ;optimized by O1
    MOV BX, 0 ;optimized by O1
    MOV CX, 1
    SUB AX, BX
    JE JUMP_44
    MOV CX, 0
JUMP_44:
    MOV AX, CX ;optimized by O1
    AND AX, AX
    JNZ JUMP_45
    JMP ELSE_11
JUMP_45:
    MOV SI, WORD PTR [BP-6] ;optimized by O1
    MOV AX, 1
    MOV [SI], AX
    MOV SI, WORD PTR [BP-8] ;optimized by O1
    MOV AX, 0
    MOV [SI], AX
    MOV AX, WORD PTR [BP-2] ;optimized by O1
    JMP ENDFUNC_exgcd
    JMP ENDIF_11
ELSE_11:
    PUSH WORD PTR [BP-4]
    MOV AX, WORD PTR [BP-2] ;optimized by O1
    MOV BX, WORD PTR [BP-4] ;optimized by O1
    MOV DX, 0
    CMP AX, 0
    JGE JUMP_46
    MOV DX, 0FFFFH
JUMP_46:
    IDIV BX
    PUSH DX
    MOV AX, BP
    SUB AX, 10
    PUSH AX
    MOV AX, BP
    SUB AX, 12
    PUSH AX
    MOV SI, SP
    MOV AX, [SI+0]
    MOV BX, [SI+6]
    MOV [SI+0], BX
    MOV [SI+6], AX
    MOV SI, SP
    MOV AX, [SI+2]
    MOV BX, [SI+4]
    MOV [SI+2], BX
    MOV [SI+4], AX
    CALL FUNC_exgcd
    ADD SP, 8
    MOV [BP-14], AX
    MOV SI, WORD PTR [BP-6] ;optimized by O1
    MOV AX, WORD PTR [BP-12] ;optimized by O1
    MOV [SI], AX
    PUSH WORD PTR [BP-8]
    PUSH WORD PTR [BP-10]
    PUSH WORD PTR [BP-12]
    MOV AX, WORD PTR [BP-2] ;optimized by O1
    MOV BX, WORD PTR [BP-4] ;optimized by O1
    MOV DX, 0
    CMP AX, 0
    JGE JUMP_47
    MOV DX, 0FFFFH
JUMP_47:
    IDIV BX
    MOV BX, AX ;optimized by O1
    POP AX
    IMUL BX
    MOV BX, AX ;optimized by O1
    POP AX
    SUB AX, BX
    POP SI
    MOV [SI], AX
    MOV AX, WORD PTR [BP-14] ;optimized by O1
    JMP ENDFUNC_exgcd
ENDIF_11:
ENDFUNC_exgcd:
    ADD SP, 14
    POP BP
    RET

FUNC_geti: ; localVar:     3, arguCnt:     0
    PUSH BP
    MOV BP, SP
    SUB SP, 6
    CALL FUNC_gets
    MOV AX, 0
    MOV [BP-2], AX
    MOV AX, 0
    MOV [BP-4], AX
    MOV AX, 0
    MOV [BP-6], AX
WHILE_3:
    MOV AX, 48
    PUSH AX
    MOV AX, 0
    MOV BX, WORD PTR [BP-2] ;optimized by O1
    ADD BX, BX
    ADD BX, AX
    MOV BX, WORD PTR [BX] ;optimized by O1
    POP AX
    MOV CX, 1
    SUB AX, BX
    JG JUMP_14
    MOV CX, 0
JUMP_14:
    PUSH CX
    MOV AX, 0
    MOV BX, WORD PTR [BP-2] ;optimized by O1
    ADD BX, BX
    ADD BX, AX
    MOV AX, WORD PTR [BX] ;optimized by O1
    MOV BX, 57 ;optimized by O1
    MOV CX, 1
    SUB AX, BX
    JG JUMP_15
    MOV CX, 0
JUMP_15:
    MOV BX, CX ;optimized by O1
    POP AX
    OR AX, BX
    MOV CX, 0
    AND AX, AX
    JZ JUMP_16
    MOV CX, 1
JUMP_16:
    MOV AX, CX ;optimized by O1
    AND AX, AX
    JNZ JUMP_17
    JMP ENDWHILE_3
JUMP_17:
    MOV AX, 0
    MOV BX, WORD PTR [BP-2] ;optimized by O1
    ADD BX, BX
    ADD BX, AX
    MOV AX, WORD PTR [BX] ;optimized by O1
    MOV BX, 45 ;optimized by O1
    MOV CX, 1
    SUB AX, BX
    JE JUMP_18
    MOV CX, 0
JUMP_18:
    MOV AX, CX ;optimized by O1
    AND AX, AX
    JNZ JUMP_19
    JMP ENDIF_5
JUMP_19:
    MOV AX, 1
    MOV [BP-6], AX
ENDIF_5:
    MOV AX, WORD PTR [BP-2] ;optimized by O1
    MOV BX, 1 ;optimized by O1
    ADD AX, BX
    MOV [BP-2], AX
    JMP WHILE_3
ENDWHILE_3:
WHILE_4:
    MOV AX, 48
    PUSH AX
    MOV AX, 0
    MOV BX, WORD PTR [BP-2] ;optimized by O1
    ADD BX, BX
    ADD BX, AX
    MOV BX, WORD PTR [BX] ;optimized by O1
    POP AX
    MOV CX, 1
    SUB AX, BX
    JLE JUMP_20
    MOV CX, 0
JUMP_20:
    PUSH CX
    MOV AX, 0
    MOV BX, WORD PTR [BP-2] ;optimized by O1
    ADD BX, BX
    ADD BX, AX
    MOV AX, WORD PTR [BX] ;optimized by O1
    MOV BX, 57 ;optimized by O1
    MOV CX, 1
    SUB AX, BX
    JLE JUMP_21
    MOV CX, 0
JUMP_21:
    MOV BX, CX ;optimized by O1
    AND BX, BX
    JZ JUMP_22
    MOV BX, 1
JUMP_22:
    POP AX
    AND AX, AX
    JZ JUMP_23
    MOV AX, 1
JUMP_23:
    AND AX, BX
    AND AX, AX
    JNZ JUMP_24
    JMP ENDWHILE_4
JUMP_24:
    MOV AX, WORD PTR [BP-4] ;optimized by O1
    MOV BX, 10 ;optimized by O1
    IMUL BX
    PUSH AX
    MOV AX, 0
    MOV BX, WORD PTR [BP-2] ;optimized by O1
    ADD BX, BX
    ADD BX, AX
    MOV BX, WORD PTR [BX] ;optimized by O1
    POP AX
    ADD AX, BX
    MOV BX, 48 ;optimized by O1
    SUB AX, BX
    MOV [BP-4], AX
    MOV AX, WORD PTR [BP-2] ;optimized by O1
    MOV BX, 1 ;optimized by O1
    ADD AX, BX
    MOV [BP-2], AX
    JMP WHILE_4
ENDWHILE_4:
    MOV AX, WORD PTR [BP-6] ;optimized by O1
    AND AX, AX
    JNZ JUMP_25
    JMP ELSE_6
JUMP_25:
    MOV AX, 0
    MOV BX, WORD PTR [BP-4] ;optimized by O1
    SUB AX, BX
    JMP ENDFUNC_geti
    JMP ENDIF_6
ELSE_6:
    MOV AX, WORD PTR [BP-4] ;optimized by O1
    JMP ENDFUNC_geti
ENDIF_6:
ENDFUNC_geti:
    ADD SP, 6
    POP BP
    RET

FUNC_gets: ; localVar:     1, arguCnt:     0
    PUSH BP
    MOV BP, SP
    SUB SP, 2
    MOV AH, 1
    INT 21H
    MOV AH, 0
    MOV [BP-2], AX
    MOV AX, 0
    MOV SI, 258
    MOV [SI], AX
WHILE_1:
    MOV AX, 1
    AND AX, AX
    JNZ JUMP_1
    JMP ENDWHILE_1
JUMP_1:
    MOV AX, WORD PTR [BP-2] ;optimized by O1
    MOV BX, 10 ;optimized by O1
    MOV CX, 1
    SUB AX, BX
    JE JUMP_2
    MOV CX, 0
JUMP_2:
    PUSH CX
    MOV AX, WORD PTR [BP-2] ;optimized by O1
    MOV BX, 13 ;optimized by O1
    MOV CX, 1
    SUB AX, BX
    JE JUMP_3
    MOV CX, 0
JUMP_3:
    MOV BX, CX ;optimized by O1
    POP AX
    OR AX, BX
    MOV CX, 0
    AND AX, AX
    JZ JUMP_4
    MOV CX, 1
JUMP_4:
    MOV AX, CX ;optimized by O1
    AND AX, AX
    JNZ JUMP_5
    JMP ELSE_1
JUMP_5:
    MOV AX, 0
    MOV SI, 258
    MOV BX, WORD PTR [SI] ;optimized by O1
    MOV CX, 0 ;optimized by O1
    ADD BX, BX
    ADD BX, AX
    MOV [BX], CX
    MOV AX, 0
    JMP ENDFUNC_gets
    JMP ENDIF_1
ELSE_1:
    MOV AX, WORD PTR [BP-2] ;optimized by O1
    MOV BX, 8 ;optimized by O1
    MOV CX, 1
    SUB AX, BX
    JNE JUMP_6
    MOV CX, 0
JUMP_6:
    MOV AX, CX ;optimized by O1
    AND AX, AX
    JNZ JUMP_7
    JMP ELSE_2
JUMP_7:
    MOV SI, 258
    MOV AX, WORD PTR [SI] ;optimized by O1
    MOV BX, 128 ;optimized by O1
    MOV CX, 1
    SUB AX, BX
    JL JUMP_8
    MOV CX, 0
JUMP_8:
    MOV AX, CX ;optimized by O1
    AND AX, AX
    JNZ JUMP_9
    JMP ELSE_3
JUMP_9:
    MOV AX, 0
    MOV SI, 258
    MOV BX, WORD PTR [SI] ;optimized by O1
    MOV CX, WORD PTR [BP-2] ;optimized by O1
    ADD BX, BX
    ADD BX, AX
    MOV [BX], CX
    MOV SI, 258
    MOV AX, WORD PTR [SI] ;optimized by O1
    MOV BX, 1 ;optimized by O1
    ADD AX, BX
    MOV SI, 258
    MOV [SI], AX
    JMP ENDIF_3
ELSE_3:
    MOV DX, 8 ;optimized by O1
    MOV AH, 2
    INT 21H
    MOV DX, 32 ;optimized by O1
    MOV AH, 2
    INT 21H
    MOV DX, 8 ;optimized by O1
    MOV AH, 2
    INT 21H
ENDIF_3:
    JMP ENDIF_2
ELSE_2:
    MOV SI, 258
    MOV AX, WORD PTR [SI] ;optimized by O1
    MOV BX, 0 ;optimized by O1
    MOV CX, 1
    SUB AX, BX
    JG JUMP_10
    MOV CX, 0
JUMP_10:
    MOV AX, CX ;optimized by O1
    AND AX, AX
    JNZ JUMP_11
    JMP ENDIF_4
JUMP_11:
    MOV SI, 258
    MOV AX, WORD PTR [SI] ;optimized by O1
    MOV BX, 1 ;optimized by O1
    SUB AX, BX
    MOV SI, 258
    MOV [SI], AX
    MOV DX, 32 ;optimized by O1
    MOV AH, 2
    INT 21H
    MOV DX, 8 ;optimized by O1
    MOV AH, 2
    INT 21H
ENDIF_4:
ENDIF_2:
ENDIF_1:
    MOV AH, 1
    INT 21H
    MOV AH, 0
    MOV [BP-2], AX
    JMP WHILE_1
ENDWHILE_1:
ENDFUNC_gets:
    ADD SP, 2
    POP BP
    RET

FUNC_main: ; localVar:     5, arguCnt:     0
    PUSH BP
    MOV BP, SP
    SUB SP, 10
    MOV AX, 616
    PUSH AX
    CALL FUNC_print
    ADD SP, 2
    CALL FUNC_geti
    MOV [BP-2], AX
    MOV AX, 634
    PUSH AX
    CALL FUNC_print
    ADD SP, 2
    CALL FUNC_geti
    MOV [BP-4], AX
    PUSH WORD PTR [BP-2]
    PUSH WORD PTR [BP-4]
    MOV AX, BP
    SUB AX, 6
    PUSH AX
    MOV AX, BP
    SUB AX, 8
    PUSH AX
    MOV SI, SP
    MOV AX, [SI+0]
    MOV BX, [SI+6]
    MOV [SI+0], BX
    MOV [SI+6], AX
    MOV SI, SP
    MOV AX, [SI+2]
    MOV BX, [SI+4]
    MOV [SI+2], BX
    MOV [SI+4], AX
    CALL FUNC_exgcd
    ADD SP, 8
    MOV [BP-10], AX
    MOV AX, 652
    PUSH AX
    CALL FUNC_print
    ADD SP, 2
    PUSH WORD PTR [BP-6]
    CALL FUNC_printInt
    ADD SP, 2
    MOV DX, 10 ;optimized by O1
    MOV AH, 2
    INT 21H
    MOV AX, 664
    PUSH AX
    CALL FUNC_print
    ADD SP, 2
    PUSH WORD PTR [BP-8]
    CALL FUNC_printInt
    ADD SP, 2
    MOV DX, 10 ;optimized by O1
    MOV AH, 2
    INT 21H
    MOV AX, 676
    PUSH AX
    CALL FUNC_print
    ADD SP, 2
    PUSH WORD PTR [BP-10]
    CALL FUNC_printInt
    ADD SP, 2
    MOV DX, 10 ;optimized by O1
    MOV AH, 2
    INT 21H
ENDFUNC_main:
    ADD SP, 10
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
