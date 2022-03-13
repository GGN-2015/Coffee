STACKSG SEGMENT STACK
    DW 16384 DUP(0)
STACKSG ENDS

CODESG SEGMENT
    ASSUME CS:CODESG, DS:STACKSG, SS:STACKSG

FUNC_dfs: ; localVar:     3, arguCnt:     1
    PUSH BP
    MOV BP, SP
    SUB SP, 6
    MOV AX, [BP+4]
    MOV [BP-2], AX
    PUSH WORD PTR [BP-2]
    MOV AX, 9
    PUSH AX
    POP BX
    POP AX
    MOV CX, 1
    SUB AX, BX
    JGE JUMP_7
    MOV CX, 0
JUMP_7:
    PUSH CX
    POP AX
    AND AX, AX
    JNZ JUMP_8
    JMP ELSE_2
JUMP_8:
    MOV SI, 82
    PUSH WORD PTR [SI]
    MOV AX, 1
    PUSH AX
    POP BX
    POP AX
    ADD AX, BX
    PUSH AX
    POP AX
    MOV SI, 82
    MOV [SI], AX
    JMP ENDIF_2
ELSE_2:
    MOV AX, 1
    PUSH AX
    POP AX
    MOV [BP-4], AX
WHILE_3:
    PUSH WORD PTR [BP-4]
    MOV AX, 8
    PUSH AX
    POP BX
    POP AX
    MOV CX, 1
    SUB AX, BX
    JLE JUMP_9
    MOV CX, 0
JUMP_9:
    PUSH CX
    POP AX
    AND AX, AX
    JNZ JUMP_10
    JMP ENDWHILE_3
JUMP_10:
    MOV AX, 0
    PUSH AX
    PUSH WORD PTR [BP-4]
    POP BX
    POP AX
    ADD BX, BX
    ADD BX, AX
    PUSH WORD PTR [BX]
    MOV AX, 18
    PUSH AX
    PUSH WORD PTR [BP-2]
    PUSH WORD PTR [BP-4]
    POP BX
    POP AX
    ADD AX, BX
    PUSH AX
    MOV AX, 1
    PUSH AX
    POP BX
    POP AX
    SUB AX, BX
    PUSH AX
    POP BX
    POP AX
    ADD BX, BX
    ADD BX, AX
    PUSH WORD PTR [BX]
    POP BX
    POP AX
    OR AX, BX
    PUSH AX
    MOV AX, 50
    PUSH AX
    PUSH WORD PTR [BP-2]
    PUSH WORD PTR [BP-4]
    POP BX
    POP AX
    SUB AX, BX
    PUSH AX
    MOV AX, 8
    PUSH AX
    POP BX
    POP AX
    ADD AX, BX
    PUSH AX
    POP BX
    POP AX
    ADD BX, BX
    ADD BX, AX
    PUSH WORD PTR [BX]
    POP BX
    POP AX
    OR AX, BX
    PUSH AX
    POP AX
    MOV [BP-6], AX
    PUSH WORD PTR [BP-6]
    MOV AX, 0
    PUSH AX
    POP BX
    POP AX
    MOV CX, 1
    SUB AX, BX
    JE JUMP_11
    MOV CX, 0
JUMP_11:
    PUSH CX
    POP AX
    AND AX, AX
    JNZ JUMP_12
    JMP ENDIF_3
JUMP_12:
    MOV AX, 0
    PUSH AX
    PUSH WORD PTR [BP-4]
    MOV AX, 1
    PUSH AX
    POP CX
    POP BX
    POP AX
    ADD BX, BX
    ADD BX, AX
    MOV [BX], CX
    MOV AX, 18
    PUSH AX
    PUSH WORD PTR [BP-2]
    PUSH WORD PTR [BP-4]
    POP BX
    POP AX
    ADD AX, BX
    PUSH AX
    MOV AX, 1
    PUSH AX
    POP BX
    POP AX
    SUB AX, BX
    PUSH AX
    MOV AX, 1
    PUSH AX
    POP CX
    POP BX
    POP AX
    ADD BX, BX
    ADD BX, AX
    MOV [BX], CX
    MOV AX, 50
    PUSH AX
    PUSH WORD PTR [BP-2]
    PUSH WORD PTR [BP-4]
    POP BX
    POP AX
    SUB AX, BX
    PUSH AX
    MOV AX, 8
    PUSH AX
    POP BX
    POP AX
    ADD AX, BX
    PUSH AX
    MOV AX, 1
    PUSH AX
    POP CX
    POP BX
    POP AX
    ADD BX, BX
    ADD BX, AX
    MOV [BX], CX
    PUSH WORD PTR [BP-2]
    MOV AX, 1
    PUSH AX
    POP BX
    POP AX
    ADD AX, BX
    PUSH AX
    CALL FUNC_dfs
    ADD SP, 2
    MOV AX, 0
    PUSH AX
    PUSH WORD PTR [BP-4]
    MOV AX, 0
    PUSH AX
    POP CX
    POP BX
    POP AX
    ADD BX, BX
    ADD BX, AX
    MOV [BX], CX
    MOV AX, 18
    PUSH AX
    PUSH WORD PTR [BP-2]
    PUSH WORD PTR [BP-4]
    POP BX
    POP AX
    ADD AX, BX
    PUSH AX
    MOV AX, 1
    PUSH AX
    POP BX
    POP AX
    SUB AX, BX
    PUSH AX
    MOV AX, 0
    PUSH AX
    POP CX
    POP BX
    POP AX
    ADD BX, BX
    ADD BX, AX
    MOV [BX], CX
    MOV AX, 50
    PUSH AX
    PUSH WORD PTR [BP-2]
    PUSH WORD PTR [BP-4]
    POP BX
    POP AX
    SUB AX, BX
    PUSH AX
    MOV AX, 8
    PUSH AX
    POP BX
    POP AX
    ADD AX, BX
    PUSH AX
    MOV AX, 0
    PUSH AX
    POP CX
    POP BX
    POP AX
    ADD BX, BX
    ADD BX, AX
    MOV [BX], CX
ENDIF_3:
    PUSH WORD PTR [BP-4]
    MOV AX, 1
    PUSH AX
    POP BX
    POP AX
    ADD AX, BX
    PUSH AX
    POP AX
    MOV [BP-4], AX
    JMP WHILE_3
ENDWHILE_3:
ENDIF_2:
ENDFUNC_dfs:
    ADD SP, 6
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
    MOV SI, 82
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
    PUSH AX
    POP AX
    MOV [BP-24], AX
    PUSH WORD PTR [BP-2]
    MOV AX, 0
    PUSH AX
    POP BX
    POP AX
    MOV CX, 1
    SUB AX, BX
    JE JUMP_1
    MOV CX, 0
JUMP_1:
    PUSH CX
    POP AX
    AND AX, AX
    JNZ JUMP_2
    JMP ELSE_1
JUMP_2:
    MOV AX, 48
    PUSH AX
    POP DX
    MOV AH, 2
    INT 21H
    JMP ENDIF_1
ELSE_1:
WHILE_1:
    PUSH WORD PTR [BP-2]
    MOV AX, 0
    PUSH AX
    POP BX
    POP AX
    MOV CX, 1
    SUB AX, BX
    JNE JUMP_3
    MOV CX, 0
JUMP_3:
    PUSH CX
    POP AX
    AND AX, AX
    JNZ JUMP_4
    JMP ENDWHILE_1
JUMP_4:
    MOV AX, BP
    SUB AX, 22
    PUSH AX
    PUSH WORD PTR [BP-24]
    PUSH WORD PTR [BP-2]
    MOV AX, 10
    PUSH AX
    POP BX
    POP AX
    MOV DX, 0
    DIV BX
    PUSH DX
    POP CX
    POP BX
    POP AX
    ADD BX, BX
    ADD BX, AX
    MOV [BX], CX
    PUSH WORD PTR [BP-2]
    MOV AX, 10
    PUSH AX
    POP BX
    POP AX
    MOV DX, 0
    DIV BX
    PUSH AX
    POP AX
    MOV [BP-2], AX
    PUSH WORD PTR [BP-24]
    MOV AX, 1
    PUSH AX
    POP BX
    POP AX
    ADD AX, BX
    PUSH AX
    POP AX
    MOV [BP-24], AX
    JMP WHILE_1
ENDWHILE_1:
WHILE_2:
    PUSH WORD PTR [BP-24]
    MOV AX, 0
    PUSH AX
    POP BX
    POP AX
    MOV CX, 1
    SUB AX, BX
    JG JUMP_5
    MOV CX, 0
JUMP_5:
    PUSH CX
    POP AX
    AND AX, AX
    JNZ JUMP_6
    JMP ENDWHILE_2
JUMP_6:
    MOV AX, BP
    SUB AX, 22
    PUSH AX
    PUSH WORD PTR [BP-24]
    MOV AX, 1
    PUSH AX
    POP BX
    POP AX
    SUB AX, BX
    PUSH AX
    POP BX
    POP AX
    ADD BX, BX
    ADD BX, AX
    PUSH WORD PTR [BX]
    MOV AX, 48
    PUSH AX
    POP BX
    POP AX
    ADD AX, BX
    PUSH AX
    POP DX
    MOV AH, 2
    INT 21H
    PUSH WORD PTR [BP-24]
    MOV AX, 1
    PUSH AX
    POP BX
    POP AX
    SUB AX, BX
    PUSH AX
    POP AX
    MOV [BP-24], AX
    JMP WHILE_2
ENDWHILE_2:
ENDIF_1:
    MOV AX, 32
    PUSH AX
    POP DX
    MOV AH, 2
    INT 21H
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
