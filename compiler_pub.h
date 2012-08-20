
#ifndef __COMPILER_PUB_H__
#define __COMPILER_PUB_H__


#include <cstdio>
#include <iostream>
#include <vector>

using namespace std;

typedef int STATUS;
/***********************************************************************
 *
 * It is assumed that the fixed point arithmetic contains a signed integer
 * with a scaling factor of 1/2^6
 *
 * http://en.wikipedia.org/wiki/Fixed-point_arithmetic
 *
 ***********************************************************************/


typedef struct opcode_s
{
   unsigned dstReg     : 2;
   unsigned srcReg     : 2;
   unsigned opcode_gpr : 4;
} opcode_t;

typedef struct fpFields_s
{
   unsigned fraction : 5 ;
   unsigned integer  : 10;
   unsigned sign     : 1 ;
} fpFields_t;

// this can be an address or a opcode or a fixed point number (word_fields)
typedef union word_s 
{
   uint32_t address;
   opcode_t opcode;
   fpFields_t word_fields;
} word_t;

// this is to specify the meaning of each word in the memory
typedef enum type_e
{
   CONST,
   VAR,
   INSTRUCTION,
} type_t;

// this is the word in a memory location plus an indication of
// its meaning
typedef struct wordMeta_s
{
   word_t word;
   type_t type; 
} wordMeta_t;




/* Compiler output */

extern static word_t asm_bin[0x8000]; 

/***********************************************************************
 *
 * Compiler translation calls for binary implementation of processor
 * functionality
 *
 ***********************************************************************/

 // Explicacion: Nuestro traduccion de lenguaje de alto nivel por ejemplo, 
 // traducira el operador o token "+" a llamar a status add, el cual
 
// add
STATUS add( word_t op1, word_t op2 , word_t result );
// multiplication
STATUS mul( word_t op1, word_t op2 , word_t result );
// division
STATUS div( word_t op1, word_t op2 , word_t result );
// factorial
STATUS fac( word_t op1 , word_t result );

// loop para implementacion de un while
STATUS beg_loop(word_t op1);
STATUS end_loop(word_t op1);

// function calls to assign memory to Variables 
STATUS scalar( unsigned &var, unsigned size );
STATUS array( unsigned &var, unsigned size );  
/* Explicacion: Tal vez ocupemos un array para un lookup table para hacer
la potencia en SW. I.G*/


#endif /* __COMPILER_PUB_H__ */
