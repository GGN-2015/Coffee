// ---------------------------------------------------------------------- //
// coffee.cpp - Version 0.0.1
//
// Author: GGN_2015 (JLU #21200612)
// Date  : 2022-03-11


// Comment: This is the first version of my compiler for my own language COFFEE,
// - The target language is intel ASM on 8086 machine.
// - Please don't laugh at me, COFFEE is now just a random name.


// ---------------------------------------------------------------------- //
// What can COFFEE Do?
//
// 1. Define global variables
// 2. Define (recusive) functions 
// 3. Define local variables
// 4. Branch structure 'IF'
// 5. Loop structure 'WHILE' and 'FOR'


// ---------------------------------------------------------------------- //
// Grammars of COFFEE:
// 
// 1. Define a function
//   FUNC funcName ( argName1, argName2, ... )
//       ...
//       [RETURN]
//       [RETURN expression]         ;ALL function without RETURN statements will return ZERO
//   ENDFUNC
// 
// 2. Define a variable 
//   VAR varName [[length]]          ;length here must be a positive constant integer
//                                   ;if length >= 2, varName will be regarded as an array
//                                   ;ALL basic variable is 16bit signed integer
// 3. Assignment statement
//   varName := expression
//   VARAT expression := expression  ;VARAT means "var at position ..."
//
// 4. IF statement
//   IF expression THEN
//       ...
//   [ELSE]
//       ...
//   ENDIF
//
// 5. WHILE statement
//   WHILE expression LOOP
//       ...
//       [BREAK]                     ;use BREAK to break the loop
//       [CONTINUE]                  ;use CONTINUE to end this turn of loop and begin next
//   ENDWHILE
//
// 6. FOR statement
//   FOR varName := expressionA TO expressionB [BY step] DO
//       ...
//   ENDFOR
