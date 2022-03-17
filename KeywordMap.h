#ifndef KEYWORDMAP_H
#define KEYWORDMAP_H


#include <string>
#include <map>


// ---------------------------------------------------------------------- //
// ALL the KEYWORDs in COFFEE Ver0.0.1
#define TOKEN_NOT_KEYWORD (-1)


#define TOKEN_ENDOFLINE  (0) // there is an EOL on every line
#define TOKEN_VAR        (1)
#define TOKEN_FUNC       (2)
#define TOKEN_ENDFUNC    (3)
#define TOKEN_RETURN     (4)
#define TOKEN_VARAT      (5) 
#define TOKEN_OFFSET     (6) // get the position of a global/local var
#define TOKEN_IF         (7)
#define TOKEN_THEN       (8)
#define TOKEN_ELSE       (9)
#define TOKEN_ENDIF      (10)
#define TOKEN_WHILE      (11)
#define TOKEN_LOOP       (12)
#define TOKEN_BREAK      (13)
#define TOKEN_CONTINUE   (14)
#define TOKEN_ENDWHILE   (15)
#define TOKEN_FOR        (16)
#define TOKEN_TO         (17)
#define TOKEN_BY         (18)
#define TOKEN_DO         (19)
#define TOKEN_ENDFOR     (20)


#define TOKEN_IDENTIFIER (21)
#define TOKEN_NUMBER     (22)
#define TOKEN_STRING     (23)


#define TOKEN_ASS   (24) //  :=
#define TOKEN_ADD   (25) //   +
#define TOKEN_SUB   (26) //   -
#define TOKEN_MUL   (27) //   *
#define TOKEN_DIV   (28) //   /
#define TOKEN_MOD   (29) //   %
#define TOKEN_COMMA (30) //   ,


#define TOKEN_LSS (31) //   <
#define TOKEN_LEQ (32) //  <=
#define TOKEN_GTR (33) //   >
#define TOKEN_GEQ (34) //  >=
#define TOKEN_EQU (35) //   =
#define TOKEN_NEQ (36) //  <>


#define TOKEN_OPEN        (37) // (
#define TOKEN_CLOSE       (38) // )
#define TOKEN_INDEXOPEN   (39) // [
#define TOKEN_INDEXCLOSE  (40) // ]


#define TOKEN_CALL        (41) // use CALL to call a function and ignore its return value
#define TOKEN_GETCHAR     (42) // getchar from keyboard
#define TOKEN_PUTCHAR     (43) // putchar from keyboard


#define TOKEN_AND         (44)
#define TOKEN_OR          (45)
#define TOKEN_NOT         (46)


#define TOKEN_ASM         (47) // ASM "MOV AX, BX"


#define TOKEN_INT         (48) // INT
#define TOKEN_POINTER     (49) // POINTER
#define TOKEN_ARRAY       (50) // ARRAY
#define TOKEN_STRUCT      (51) // STRUCT


class KeywordMap
{
public:
    static KeywordMap& getInstance();
    int getKeywordId(std::string keywordName);
    
protected:
    KeywordMap() {
        mKeywordId["VAR"     ] = TOKEN_VAR     ;
        mKeywordId["FUNC"    ] = TOKEN_FUNC    ;
        mKeywordId["ENDFUNC" ] = TOKEN_ENDFUNC ;
        mKeywordId["RETURN"  ] = TOKEN_RETURN  ;
        mKeywordId["VARAT"   ] = TOKEN_VARAT   ;
        mKeywordId["OFFSET"  ] = TOKEN_OFFSET  ;
        mKeywordId["IF"      ] = TOKEN_IF      ;
        mKeywordId["THEN"    ] = TOKEN_THEN    ;
        mKeywordId["ELSE"    ] = TOKEN_ELSE    ;
        mKeywordId["ENDIF"   ] = TOKEN_ENDIF   ;
        mKeywordId["WHILE"   ] = TOKEN_WHILE   ;
        mKeywordId["LOOP"    ] = TOKEN_LOOP    ;
        mKeywordId["BREAK"   ] = TOKEN_BREAK   ;
        mKeywordId["CONTINUE"] = TOKEN_CONTINUE;
        mKeywordId["ENDWHILE"] = TOKEN_ENDWHILE;
        mKeywordId["FOR"     ] = TOKEN_FOR     ;
        mKeywordId["TO"      ] = TOKEN_TO      ;
        mKeywordId["BY"      ] = TOKEN_BY      ;
        mKeywordId["DO"      ] = TOKEN_DO      ;
        mKeywordId["ENDFOR"  ] = TOKEN_ENDFOR  ;
        mKeywordId["CALL"    ] = TOKEN_CALL    ;
        mKeywordId["GETCHAR" ] = TOKEN_GETCHAR ;
        mKeywordId["PUTCHAR" ] = TOKEN_PUTCHAR ;
        mKeywordId["AND"     ] = TOKEN_AND     ;
        mKeywordId["OR"      ] = TOKEN_OR      ;
        mKeywordId["NOT"     ] = TOKEN_NOT     ;
        mKeywordId["ASM"     ] = TOKEN_ASM     ;
        
        // type relevant
        mKeywordId["INT"     ] = TOKEN_INT     ;
        mKeywordId["POINTER" ] = TOKEN_POINTER ;
        mKeywordId["ARRAY"   ] = TOKEN_ARRAY   ;
        mKeywordId["STRUCT"  ] = TOKEN_STRUCT  ;
    }
    
    std::map<std::string, int> mKeywordId;
};

#endif

