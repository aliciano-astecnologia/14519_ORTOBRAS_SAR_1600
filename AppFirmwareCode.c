
#include "AppFirmwareCode.h"
#include "Entradas.h"
#include "TickTimer.h"
#include "AppDisp7s.h"

#define NUMBER_OF_DIGITS      4
#define FIRMWARE_CODE_LENGTH  9

extern U8 FlagTecla;
extern U8 FlagStart;
static TickTimer TUpdateScroll;
const char StrCodigoVersaoFirmware[] = FIRMWARE_CODE "    ";   /* Deixar 4 espacos para nao ficar dificil a visualizacao */

static void ShowFirmwareCode(void);
static void PrintDigitsStringScroll(char *Buffer);
static void AppFirmwareCodeTask (void);

void AppFirmwareCodeCheckEnter (void)
{
   static U8 i = 0;
	for(i=0; i<110; i++)
	{
		EntradasTask();
	}

   if(FlagTecla)
   {
      AppFirmwareCodeTask();
   }
}

static void AppFirmwareCodeTask (void)
{
   TickTimerSet(&TUpdateScroll, 333);
   FlagStart = 0; /* habilita o update do display */

   while (FlagTecla)
   {
      EntradasTask();
      IWDG_ReloadCounter();
      if(TickTimerExpired(&TUpdateScroll))
      {
         TickTimerRestart(&TUpdateScroll);
         ShowFirmwareCode();
      }
   }
}

static void ShowFirmwareCode(void)
{
   PrintDigitsStringScroll((char *)StrCodigoVersaoFirmware);
}

static void PrintDigitsStringScroll(char *Buffer)
{
   static U8 ScrollIndex = 0;
   U8 Index0 = ScrollIndex;
   U8 Index1 = ScrollIndex + 1;
   U8 Index2 = ScrollIndex + 2;
   U8 Index3 = ScrollIndex + 3;

   if (Index1 >= FIRMWARE_CODE_LENGTH)
   {
      Index1 -= FIRMWARE_CODE_LENGTH;
   }
   if (Index2 >= FIRMWARE_CODE_LENGTH)
   {
      Index2 -= FIRMWARE_CODE_LENGTH;
   }
   if (Index3 >= FIRMWARE_CODE_LENGTH)
   {
      Index3 -= FIRMWARE_CODE_LENGTH;
   }

   AppDisp7sSetDigito(Buffer[Index0] - '0',
                      Buffer[Index1] - '0',
                      Buffer[Index2] - '0',
                      Buffer[Index3] - '0');

   ScrollIndex ++;
   if (ScrollIndex >= FIRMWARE_CODE_LENGTH)
   {
      ScrollIndex = 0;
   }
}








