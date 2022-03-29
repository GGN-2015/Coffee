STACKSG SEGMENT STACK
    GLOBAL_BUILTIN_GETS_BUF DW 129 DUP(0)
    GLOBAL_BUILTIN_GETS_CHARCOUNT DW 1 DUP(0)
    GLOBAL_BUILTIN_COMPACT_BUF DW 129 DUP(0)
    DW 70, 65, 73, 76, 32, 84, 79, 32, 79, 80, 69, 78, 70, 73, 76, 69, 10, 0
    DW 70, 65, 73, 76, 32, 84, 79, 32, 67, 76, 79, 83, 69, 70, 73, 76, 69, 10, 0
    DW 70, 73, 76, 69, 46, 84, 88, 84, 0
    DW 72, 101, 108, 108, 111, 32, 70, 73, 76, 69, 33, 0
    DW 70, 73, 76, 69, 46, 84, 88, 84, 0
    DW 70, 73, 76, 69, 46, 84, 88, 84, 0
    DW 72, 101, 108, 108, 111, 32, 73, 110, 110, 101, 114, 32, 65, 83, 77, 33, 0
    DW 70, 73, 76, 69, 46, 84, 88, 84, 0
    DW 16023 DUP(0)
STACKSG ENDS

CODESG SEGMENT
    ASSUME CS:CODESG, DS:STACKSG, SS:STACKSG

FUNC_closeFile: ; localVar:     1, arguCnt:     1
    PUSH BP
    MOV BP, SP
    SUB SP, 2
    MOV AX, [BP+4]
    MOV [BP-2], AX
    MOV AH, 3EH                   ; code from inner asm
    MOV BX, [BP-2]                ; code from inner asm
    INT 21H                       ; code from inner asm
ENDFUNC_closeFile:
    ADD SP, 2
    POP BP
    RET

FUNC_compact: ; localVar:     1, arguCnt:     1
    PUSH BP
    MOV BP, SP
    SUB SP, 2
    MOV AX, [BP+4]
    MOV [BP-2], AX
    MOV SI, [BP-2]                ; code from inner asm
    LEA DI, GLOBAL_BUILTIN_COMPACT_BUF; code from inner asm
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

FUNC_createFile: ; localVar:     2, arguCnt:     2
    PUSH BP
    MOV BP, SP
    SUB SP, 4
    MOV AX, [BP+4]
    MOV [BP-2], AX
    MOV AX, [BP+6]
    MOV [BP-4], AX
    PUSH WORD PTR [BP-2]
    CALL FUNC_compact
    ADD SP, 2
    MOV AH, 3CH                   ; code from inner asm
    LEA DX, GLOBAL_BUILTIN_COMPACT_BUF; code from inner asm
    MOV CX, [BP-4]                ; code from inner asm
    INT 21H                       ; code from inner asm
ENDFUNC_createFile:
    ADD SP, 4
    POP BP
    RET

FUNC_fgetc: ; localVar:     2, arguCnt:     1
    PUSH BP
    MOV BP, SP
    SUB SP, 4
    MOV AX, [BP+4]
    MOV [BP-2], AX
    MOV AX, 0
    MOV [BP-4], AX
    MOV AH, 3FH                   ; code from inner asm
    MOV BX, [BP-2]                ; code from inner asm
    LEA DX, [BP-4]                ; code from inner asm
    MOV CX, 1                     ; code from inner asm
    INT 21H                       ; code from inner asm
    AND AX, AX                    ; code from inner asm
    JNZ FGETC_FILE_NOT_END        ; code from inner asm
    MOV AX, -1                    ; code from inner asm
    JMP ENDFUNC_fgetc             ; code from inner asm
    FGETC_FILE_NOT_END:           ; code from inner asm
    MOV AX, WORD PTR [BP-4] ;optimized by O1
    JMP ENDFUNC_fgetc
ENDFUNC_fgetc:
    ADD SP, 4
    POP BP
    RET

FUNC_fputc: ; localVar:     2, arguCnt:     2
    PUSH BP
    MOV BP, SP
    SUB SP, 4
    MOV AX, [BP+4]
    MOV [BP-2], AX
    MOV AX, [BP+6]
    MOV [BP-4], AX
    MOV AH, 40H                   ; code from inner asm
    MOV BX, [BP-2]                ; code from inner asm
    LEA DX, [BP-4]                ; code from inner asm
    MOV CX, 1                     ; code from inner asm
    INT 21H                       ; code from inner asm
    AND AX, AX                    ; code from inner asm
    JNZ FPUTC_FILE_NOT_END        ; code from inner asm
    MOV AX, -1                    ; code from inner asm
    JMP ENDFUNC_fgetc             ; code from inner asm
    FPUTC_FILE_NOT_END:           ; code from inner asm
    MOV AX, WORD PTR [BP-4] ;optimized by O1
    JMP ENDFUNC_fputc
ENDFUNC_fputc:
    ADD SP, 4
    POP BP
    RET

FUNC_main: ; localVar:     0, arguCnt:     0
    PUSH BP
    MOV BP, SP
    SUB SP, 0
    MOV AX, 592
    PUSH AX
    MOV AX, 610
    PUSH AX
    MOV SI, SP
    MOV AX, [SI+0]
    MOV BX, [SI+2]
    MOV [SI+0], BX
    MOV [SI+2], AX
    CALL FUNC_setFile
    ADD SP, 4
    MOV AX, 634
    PUSH AX
    CALL FUNC_showFile
    ADD SP, 2
    MOV DX, 10 ;optimized by O1
    MOV AH, 2
    INT 21H
    MOV AX, 652
    PUSH AX
    MOV AX, 670
    PUSH AX
    MOV SI, SP
    MOV AX, [SI+0]
    MOV BX, [SI+2]
    MOV [SI+0], BX
    MOV [SI+2], AX
    CALL FUNC_setFile
    ADD SP, 4
    MOV AX, 704
    PUSH AX
    CALL FUNC_showFile
    ADD SP, 2
    MOV DX, 10 ;optimized by O1
    MOV AH, 2
    INT 21H
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

FUNC_readFile: ; localVar:     1, arguCnt:     1
    PUSH BP
    MOV BP, SP
    SUB SP, 2
    MOV AX, [BP+4]
    MOV [BP-2], AX
    PUSH WORD PTR [BP-2]
    CALL FUNC_compact
    ADD SP, 2
    MOV AH, 3DH                   ; code from inner asm
    MOV AL, 0                     ; code from inner asm
    LEA DX, GLOBAL_BUILTIN_COMPACT_BUF; code from inner asm
    INT 21H                       ; code from inner asm
    JC READ_FILE_ERR              ; code from inner asm
    JMP ENDFUNC_readFile          ; code from inner asm
    READ_FILE_ERR:                ; code from inner asm
    MOV AX, -1                    ; code from inner asm
ENDFUNC_readFile:
    ADD SP, 2
    POP BP
    RET

FUNC_setFile: ; localVar:     5, arguCnt:     2
    PUSH BP
    MOV BP, SP
    SUB SP, 10
    MOV AX, [BP+4]
    MOV [BP-2], AX
    MOV AX, [BP+6]
    MOV [BP-4], AX
    PUSH WORD PTR [BP-2]
    CALL FUNC_writeFile
    ADD SP, 2
    MOV [BP-6], AX
    MOV AX, WORD PTR [BP-6] ;optimized by O1
    MOV BX, -1 ;optimized by O1
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
    MOV AX, 554
    PUSH AX
    CALL FUNC_print
    ADD SP, 2
    MOV AX, 0
    JMP ENDFUNC_setFile
ENDIF_10:
    MOV AX, 0
    MOV [BP-10], AX
WHILE_8:
    PUSH WORD PTR [BP-4]
    MOV AX, 2
    MOV BX, WORD PTR [BP-10] ;optimized by O1
    IMUL BX
    MOV BX, AX ;optimized by O1
    POP AX
    ADD AX, BX
    MOV BX, AX ;optimized by O1
    MOV AX, WORD PTR [BX] ;optimized by O1
    MOV BX, 0 ;optimized by O1
    MOV CX, 1
    SUB AX, BX
    JNE JUMP_42
    MOV CX, 0
JUMP_42:
    MOV AX, CX ;optimized by O1
    AND AX, AX
    JNZ JUMP_43
    JMP ENDWHILE_8
JUMP_43:
    PUSH WORD PTR [BP-6]
    PUSH WORD PTR [BP-4]
    MOV AX, 2
    MOV BX, WORD PTR [BP-10] ;optimized by O1
    IMUL BX
    MOV BX, AX ;optimized by O1
    POP AX
    ADD AX, BX
    MOV BX, AX ;optimized by O1
    PUSH WORD PTR [BX]
    MOV SI, SP
    MOV AX, [SI+0]
    MOV BX, [SI+2]
    MOV [SI+0], BX
    MOV [SI+2], AX
    CALL FUNC_fputc
    ADD SP, 4
    MOV AX, WORD PTR [BP-10] ;optimized by O1
    MOV BX, 1 ;optimized by O1
    ADD AX, BX
    MOV [BP-10], AX
    JMP WHILE_8
ENDWHILE_8:
    PUSH WORD PTR [BP-6]
    CALL FUNC_closeFile
    ADD SP, 2
ENDFUNC_setFile:
    ADD SP, 10
    POP BP
    RET

FUNC_showFile: ; localVar:     3, arguCnt:     1
    PUSH BP
    MOV BP, SP
    SUB SP, 6
    MOV AX, [BP+4]
    MOV [BP-2], AX
    PUSH WORD PTR [BP-2]
    CALL FUNC_readFile
    ADD SP, 2
    MOV [BP-4], AX
    MOV AX, WORD PTR [BP-4] ;optimized by O1
    MOV BX, -1 ;optimized by O1
    MOV CX, 1
    SUB AX, BX
    JE JUMP_36
    MOV CX, 0
JUMP_36:
    MOV AX, CX ;optimized by O1
    AND AX, AX
    JNZ JUMP_37
    JMP ENDIF_9
JUMP_37:
    MOV AX, 518
    PUSH AX
    CALL FUNC_print
    ADD SP, 2
    MOV AX, 0
    JMP ENDFUNC_showFile
ENDIF_9:
    PUSH WORD PTR [BP-4]
    CALL FUNC_fgetc
    ADD SP, 2
    MOV [BP-6], AX
WHILE_7:
    MOV AX, WORD PTR [BP-6] ;optimized by O1
    MOV BX, -1 ;optimized by O1
    MOV CX, 1
    SUB AX, BX
    JNE JUMP_38
    MOV CX, 0
JUMP_38:
    MOV AX, CX ;optimized by O1
    AND AX, AX
    JNZ JUMP_39
    JMP ENDWHILE_7
JUMP_39:
    MOV DX, WORD PTR [BP-6] ;optimized by O1
    MOV AH, 2
    INT 21H
    PUSH WORD PTR [BP-4]
    CALL FUNC_fgetc
    ADD SP, 2
    MOV [BP-6], AX
    JMP WHILE_7
ENDWHILE_7:
    PUSH WORD PTR [BP-4]
    CALL FUNC_closeFile
    ADD SP, 2
ENDFUNC_showFile:
    ADD SP, 6
    POP BP
    RET

FUNC_writeFile: ; localVar:     1, arguCnt:     1
    PUSH BP
    MOV BP, SP
    SUB SP, 2
    MOV AX, [BP+4]
    MOV [BP-2], AX
    PUSH WORD PTR [BP-2]
    MOV AX, 0
    PUSH AX
    MOV SI, SP
    MOV AX, [SI+0]
    MOV BX, [SI+2]
    MOV [SI+0], BX
    MOV [SI+2], AX
    CALL FUNC_createFile
    ADD SP, 4
    MOV AH, 3DH                   ; code from inner asm
    MOV AL, 1                     ; code from inner asm
    LEA DX, GLOBAL_BUILTIN_COMPACT_BUF; code from inner asm
    INT 21H                       ; code from inner asm
    JC WRITE_FILE_ERR             ; code from inner asm
    JMP ENDFUNC_writeFile         ; code from inner asm
    WRITE_FILE_ERR:               ; code from inner asm
    MOV AX, -1                    ; code from inner asm
ENDFUNC_writeFile:
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
