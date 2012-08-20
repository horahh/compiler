#include "compiler.h"

/* Variable para la asignacion de memoria para
 * variables del programa
 * */
// need to init at 0x4000
unsigned freeAddr;
unsigned loopStartAddr;

/* Compiler output */
vector< wordMeta_t > asm_bin; 

static STATUS loadReg( wordMeta_t op1, register_t reg )
{
   wordMeta_t inst;
   inst.word.opcode.opcode_gpr = RD;
   inst.word.opcode.dstReg = reg;
   inst.type = INSTRUCTION;
   asm_bin.push_back( inst );
   asm_bin.push_back( op1 );

   return 0;
}

static STATUS unloadReg( wordMeta_t op1, register_t reg )
{
   wordMeta_t inst;
   inst.word.opcode.opcode_gpr = WR;
   inst.word.opcode.srcReg = reg;
   asm_bin.push_back( inst );
   asm_bin.push_back( op1 );

   return 0;
}



// add
STATUS add( wordMeta_t op1, wordMeta_t op2, wordMeta_t result )
{
   wordMeta_t inst;
   // load op1 into REG0
   loadReg(op1, REG0);


   // load op2 into REG1
   loadReg(op1, REG1);

   // add instruction
   // add operands
   inst.word.opcode.opcode_gpr = ADD;
   inst.word.opcode.dstReg     = REG1;
   inst.word.opcode.srcReg     = REG0;
   asm_bin.push_back( inst );
 
   // guardar el resultado en la variable 
   unloadReg( result, REG1 );

   return 0;
}
// multiplication
STATUS mul( wordMeta_t op1, wordMeta_t op2, wordMeta_t result )
{
   return 0;
}
// division
STATUS div( wordMeta_t op1, wordMeta_t op2, wordMeta_t result )
{
   return 0;
}
// factorial
STATUS fac( wordMeta_t op1, wordMeta_t result )
{
   return 0;
}
STATUS power( wordMeta_t op1, wordMeta_t result )
{
   return 0;
}
//
// loop para implementacion de un while
STATUS begin_loop(wordMeta_t op1)
{
   wordMeta_t inst;
   // Se almacena la direccion de regreso del loop
   loopStartAddr = asm_bin.size();

   // se carga el operando 1 en el registro 0
   loadReg( op1, REG0 );

   // direccion del registro
   asm_bin.push_back( inst );

   // se compara REG0 con 0 para determinar el salto por activacion
   // del flag Z
   inst.word.opcode.opcode_gpr = CMP0;
   inst.word.opcode.srcReg     = REG0;
   asm_bin.push_back( CMP0);
   
   // instruccion de salto condicional
   inst.word.opcode.opcode_gpr = BRZ;  // salta fuera del loop
   asm_bin.push_back(inst);
   inst.address = 0;

   // comparacion con cero para el branch condicional
   asm_bin.push_back(op1);

   // direccion de salto para fin del loop, de momento desconocida
   loopAddrBegBranch = asm_bin.size();
   asm_bin.push_back( 0 );

   return 0;
}
STATUS end_loop(wordMeta_t op1)
{
   wordMeta_t inst;
   // ejecucion se devuelve al punto de inicio del loop
   inst.word.opcode.opcode_gpr = BR;
   asm_bin.push_back(inst);
   // direccion de inicio del loop;
   if( inicio != 0 )
   {
      inst.address = asm_bin[inicio];
   }
   else
   {
      // error: para evitar que se trate de instanciar un fin de loop
      // cuando no existe un inicio
      return -1;
   }
   asm_bin.push_back( inst );
   // ahora se conoce la direccion de terminacion del loop
   // se coloca la direccion de fin del loop
   // se utiliza size porque es la ultima direccion del programa mas 1
   // es decir que apunta a la siguiente instruccion despues del loop
   asm_bin[ loopAddrBegBranch ] = asm_bin.size();
   loopStartAddr = 0;
   return 0;
}


// function calls to assign memory to Variables 
STATUS scalar( unsigned &var)
{
  var = freeAddr;
  asm_bin[freeAddr].type=VAR;
  freeAddr++;
  return 0;
}
STATUS array( unsigned &var, unsigned size )
{
  unsigned tmp;
  unsigned i;
  var = freeAddr;
  for( i =0 ; i < size ; i++ )
  {
     scalar(&tmp);
  }  
  freeAddr += size;
  return 0;
}
STATUS scalar_const( unsigned &var, word_t value)
{
  var = freeAddr;
  asm_bin[freeAddr].type   = CONST;
  asm_bin[freeAddr].word_t = value;
  freeAddr++;
  return 0;
}

STATUS subroutine( ... )
{
}
// este codigo deberia estar aparte en otro archivo
STATUS opti_division( )
{
    /**Este código posee las siguiente suposiciones:
   * -> Toda división posee dos operando explícitos
   * -> La carga de dichos operandos a regístros se realizan justo antes de la 
        operación.
   * -> Es la primera optimización a correr.
   * -> El divisor se coloca REG1, el cual es también donde se guarda el 
   *    resultado.
   * -> El dividendo está en REG0. Este es el primero en guardarse.
   * */
  //Esta variable nos va a servir para identificar el tipo de operación
  int op_type = 0;
  int denominador = 0;  //Valor del divisor
  int numerador = 0;//Valor del dividendo
  int op_cnt = 0;
  vector< wordMeta_t > asm_bin_tmp; //Donde se va a generar el código optimizado.
  
  //Recorrer el arreglo de elementos
  for (vector<wordMeta_t>::iterator asm_itr = asm_bin.begin(); 
       asm_itr != asm_bin.end(); asm_itr++) {
    switch ((*asm_iter).opcode.opcode_gpr) {
      case MUL: op_type = MUL; break;
      case DIV: op_type = DIV; break;
      default: continue;
    }
    numerador = *(asm_iter-3);
    denominador = *(asm_iter-3);
    op_cnt = 0;
    while (denominador%2 != 0) { 
      denominador = denominador/2;
      op_cnt++;
    }
    if (op_type == DIV){
      shift_right (numerador, cnt, &asm_bin_tmp); 
      if (denominador != 1){
        div(numerador,denominador,REG1,&asm_bin_tmp);
      }
    }else{
      shift_left (numerador, cnt, &asm_bin_tmp); 
      if (denominador != 1){
        mul(numerador,denominador,REG1,&asm_bin_tmp);
      }
    }
   if (op_type == DIV){
      div(numerador,denominador,REG1,&asm_bin_tmp);
    } else {
      mul(numerador,denominador,REG1,&asm_bin_tmp);
    } 
  }
}

