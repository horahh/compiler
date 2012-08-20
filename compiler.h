#ifndef __COMPILER_H__
#define __COMPILER_H__

#include "compiler_pub.h"

extern unsigned freeAddr;
/* Instruction Codes */

#define MEM_SIZE 0x8000

typedef enum instruction_e
{
   NOP = 0, 
   ADD,     // SUMA
   SUB,     // RESTA
   AND,     // AND logico
   NOT,     // NOT logigo
   SR,      // SHIFT RIGTH
   SL,      // SHIFT LEFT
   MUL,     // MULTIPLICACION
   DIV,     // DIVISION
   SQR,     // RAIZ Cuadrada
   RD,      // Carga en registro
   WR,      // Escribe en memoria
   BR,      // Bifurca el flujo de ejecucion
   BRZ,     // Bifurca condicional a la bandera zero
   CMP0     // Compara el valor de un registro con zero
} intruction_t;

typedef enum register_e
{
   REG0 = 0,
   REG1 = 1,
   REG2 = 2,
   REG3 = 3
} register_t;









#endif /* __COMPILER_H__ */
