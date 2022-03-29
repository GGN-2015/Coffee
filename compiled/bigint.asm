STACKSG SEGMENT STACK
    DW 16384 DUP(0)
STACKSG ENDS

CODESG SEGMENT
    ASSUME CS:CODESG, DS:STACKSG, SS:STACKSG

FUNC_main: ; localVar:   105, arguCnt:     0
    PUSH BP
    MOV BP, SP
    SUB SP, 210
    MOV AX, BP
    SUB AX, 200
    MOV BX, AX ;optimized by O1
    MOV AX, 100
    MOV CX, AX ;optimized by O1
JUMP_1:
    MOV WORD PTR [BX], 0
    ADD BX, 2
    LOOP JUMP_1
    MOV AX, BP
    SUB AX, 200
    PUSH AX
    MOV AX, 0
    MOV BX, AX ;optimized by O1
    MOV AX, 1
    MOV CX, AX ;optimized by O1
    POP AX
    ADD BX, BX
    ADD BX, AX
    MOV [BX], CX
    MOV AX, BP
    SUB AX, 200
    PUSH AX
    MOV AX, 1
    MOV BX, AX ;optimized by O1
    MOV AX, 0
    MOV CX, AX ;optimized by O1
    POP AX
    ADD BX, BX
    ADD BX, AX
    MOV [BX], CX
    MOV AX, BP
    SUB AX, 200
    PUSH AX
    MOV AX, 2
    MOV BX, AX ;optimized by O1
    MOV AX, 0
    MOV CX, AX ;optimized by O1
    POP AX
    ADD BX, BX
    ADD BX, AX
    MOV [BX], CX
    MOV AX, 1
    MOV [BP-202], AX
    MOV AX, 2
    MOV [BP-204], AX
    MOV AX, 100
    MOV [BP-206], AX
    MOV AX, 1
    MOV [BP-208], AX
WHILE_1:
    MOV AX, WORD PTR [BP-206] ;optimized by O1
    MOV BX, WORD PTR [BP-208] ;optimized by O1
    MOV CX, 1
    SUB AX, BX
    JGE JUMP_2
    MOV CX, 0
JUMP_2:
    MOV AX, CX ;optimized by O1
    AND AX, AX
    JNZ JUMP_3
    JMP ENDWHILE_1
JUMP_3:
    MOV AX, 0
    MOV [BP-210], AX
WHILE_2:
    MOV AX, WORD PTR [BP-202] ;optimized by O1
MOV BX, 1
 ;optimized by O1
    SUB AX, BX
    MOV BX, WORD PTR [BP-210] ;optimized by O1
    MOV CX, 1
    SUB AX, BX
    JGE JUMP_4
    MOV CX, 0
JUMP_4:
    MOV AX, CX ;optimized by O1
    AND AX, AX
    JNZ JUMP_5
    JMP ENDWHILE_2
JUMP_5:
    MOV AX, BP
    SUB AX, 200
    PUSH AX
    PUSH WORD PTR [BP-210]
    MOV AX, BP
    SUB AX, 200
    MOV BX, WORD PTR [BP-210] ;optimized by O1
    ADD BX, BX
    ADD BX, AX
    MOV AX, WORD PTR [BX] ;optimized by O1
    MOV BX, WORD PTR [BP-204] ;optimized by O1
    MUL BX
    MOV CX, AX ;optimized by O1
    POP BX
    POP AX
    ADD BX, BX
    ADD BX, AX
    MOV [BX], CX
    MOV AX, WORD PTR [BP-210] ;optimized by O1
MOV BX, 1
 ;optimized by O1
    ADD AX, BX
    MOV [BP-210], AX
    JMP WHILE_2
ENDWHILE_2:
    MOV AX, 0
    MOV [BP-210], AX
WHILE_3:
    MOV AX, WORD PTR [BP-202] ;optimized by O1
MOV BX, 1
 ;optimized by O1
    SUB AX, BX
    MOV BX, WORD PTR [BP-210] ;optimized by O1
    MOV CX, 1
    SUB AX, BX
    JGE JUMP_6
    MOV CX, 0
JUMP_6:
    MOV AX, CX ;optimized by O1
    AND AX, AX
    JNZ JUMP_7
    JMP ENDWHILE_3
JUMP_7:
    MOV AX, BP
    SUB AX, 200
    PUSH AX
    MOV AX, WORD PTR [BP-210] ;optimized by O1
MOV BX, 1
 ;optimized by O1
    ADD AX, BX
    PUSH AX
    MOV AX, BP
    SUB AX, 200
    PUSH AX
    MOV AX, WORD PTR [BP-210] ;optimized by O1
MOV BX, 1
 ;optimized by O1
    ADD AX, BX
    MOV BX, AX ;optimized by O1
    POP AX
    ADD BX, BX
    ADD BX, AX
    PUSH WORD PTR [BX]
    MOV AX, BP
    SUB AX, 200
    MOV BX, WORD PTR [BP-210] ;optimized by O1
    ADD BX, BX
    ADD BX, AX
    MOV AX, WORD PTR [BX] ;optimized by O1
MOV BX, 10
 ;optimized by O1
    MOV DX, 0
    CMP AX, 0
    JGE JUMP_8
    MOV DX, 0FFFFH
JUMP_8:
    IDIV BX
    MOV BX, AX ;optimized by O1
    POP AX
    ADD AX, BX
    MOV CX, AX ;optimized by O1
    POP BX
    POP AX
    ADD BX, BX
    ADD BX, AX
    MOV [BX], CX
    MOV AX, BP
    SUB AX, 200
    PUSH AX
    PUSH WORD PTR [BP-210]
    MOV AX, BP
    SUB AX, 200
    MOV BX, WORD PTR [BP-210] ;optimized by O1
    ADD BX, BX
    ADD BX, AX
    MOV AX, WORD PTR [BX] ;optimized by O1
MOV BX, 10
 ;optimized by O1
    MOV DX, 0
    CMP AX, 0
    JGE JUMP_9
    MOV DX, 0FFFFH
JUMP_9:
    IDIV BX
    MOV CX, DX ;optimized by O1
    POP BX
    POP AX
    ADD BX, BX
    ADD BX, AX
    MOV [BX], CX
    MOV AX, WORD PTR [BP-210] ;optimized by O1
MOV BX, 1
 ;optimized by O1
    ADD AX, BX
    MOV [BP-210], AX
    JMP WHILE_3
ENDWHILE_3:
    MOV AX, BP
    SUB AX, 200
    MOV BX, WORD PTR [BP-202] ;optimized by O1
    ADD BX, BX
    ADD BX, AX
    MOV AX, WORD PTR [BX] ;optimized by O1
MOV BX, 0
 ;optimized by O1
    MOV CX, 1
    SUB AX, BX
    JNE JUMP_10
    MOV CX, 0
JUMP_10:
    MOV AX, CX ;optimized by O1
    AND AX, AX
    JNZ JUMP_11
    JMP ENDIF_1
JUMP_11:
    MOV AX, WORD PTR [BP-202] ;optimized by O1
MOV BX, 1
 ;optimized by O1
    ADD AX, BX
    MOV [BP-202], AX
ENDIF_1:
    MOV AX, WORD PTR [BP-208] ;optimized by O1
MOV BX, 1
 ;optimized by O1
    ADD AX, BX
    MOV [BP-208], AX
    JMP WHILE_1
ENDWHILE_1:
    MOV AX, WORD PTR [BP-202] ;optimized by O1
MOV BX, 1
 ;optimized by O1
    SUB AX, BX
    MOV [BP-208], AX
WHILE_4:
    MOV AX, 0
    MOV BX, WORD PTR [BP-208] ;optimized by O1
    MOV CX, 1
    SUB AX, BX
    JLE JUMP_12
    MOV CX, 0
JUMP_12:
    MOV AX, CX ;optimized by O1
    AND AX, AX
    JNZ JUMP_13
    JMP ENDWHILE_4
JUMP_13:
    MOV AX, BP
    SUB AX, 200
    MOV BX, WORD PTR [BP-208] ;optimized by O1
    ADD BX, BX
    ADD BX, AX
    MOV AX, WORD PTR [BX] ;optimized by O1
MOV BX, 48
 ;optimized by O1
    ADD AX, BX
    MOV DX, AX ;optimized by O1
    MOV AH, 2
    INT 21H
    MOV AX, WORD PTR [BP-208] ;optimized by O1
MOV BX, -1
 ;optimized by O1
    ADD AX, BX
    MOV [BP-208], AX
    JMP WHILE_4
ENDWHILE_4:
ENDFUNC_main:
    ADD SP, 210
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
