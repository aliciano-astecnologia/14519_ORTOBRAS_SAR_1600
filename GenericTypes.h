//Definição dos tipos de variáveis para o microcontrolador
//Versão 3
//Joel Langer
#ifndef GENERICTYPES_H
#define GENERICTYPES_H

//Redefinition of types
//typedef unsigned long long U64;
//typedef signed long long S64;
typedef unsigned long DWORD, ULONG, U32; //32 bits sem sinal
typedef signed long S32; //, LONG
typedef unsigned short WORD, U16; //16 bits sem sinal (32bits Coldfyre, ARM)
//typedef unsigned int        U16;//16 bits sem sinal (8bits HCS08)
typedef signed short S16; //, SHORT;
typedef unsigned char UCHAR, BOOL, U8; //BYTE, //8 bits sem sinal
typedef signed char S8;
typedef const char U8FLASH; //8 bits constante em FLASH
typedef const char CAR8FLASH;//caracteres ASCII constante em FLASH
typedef char PTR; //ponteiro para String
typedef char CAR8; //, CHAR; //caracteres ASCII
typedef char INT; //inteiro
typedef float F32;

/*
#ifndef TRUE 
#define TRUE  (1)
#endif
#ifndef FALSE
#define FALSE (0)
#endif
*/

//Bits handling
typedef union
{
    U8 u8Bits;
    struct
    {
        U8 b0 :1;
        U8 b1 :1;
        U8 b2 :1;
        U8 b3 :1;
        U8 b4 :1;
        U8 b5 :1;
        U8 b6 :1;
        U8 b7 :1;
    } sBits;
} uBits;

//Bytes handling
typedef union
{
    U32 u32ULong;
    U16 u16Word[2];
    U8 u8Byte[4];
} uBytes;

#ifndef NOK
#define NOK  (1)
#endif
#ifndef OK
#define OK (0)
#endif

//Function pointer
typedef void (*pFunc_t)(void);

//Manipulation of bits
#define BSET(bit, Register) ((Register) |=  (1<<(bit))) //set
#define BCLR(bit, Register) ((Register) &= ~(1<<(bit))) //clear
#define BTOG(bit, Register) ((Register) ^=  (1<<(bit))) //toggle
#define BGET(bit, Register) (((Register) >> (bit)) & 1) //test
#define RSET(bits, Register)  ((Register) |= (bits)) //set bits from Register
#define RCLR(bits, Register)  ((Register) &= ~(bits)) //clear bits from Register

//fim do arquivo
#endif
