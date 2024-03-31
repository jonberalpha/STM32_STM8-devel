/******************************************************************************/
/*   (C) Copyright - PRIVATE   All rights reserved. AUSTRIA                   */
/*                                                                            */
/* File Name:   timer.c                                                       */
/* Autor:       Berger Jonas                                                  */
/* Version:     V1.00                                                         */
/* Date:        05/04/2018                                                    */
/* Description: Uhr mit HSE 72MHz                                             */
/******************************************************************************/
/* History:   V1.00  creation                                                 */
/******************************************************************************/
#include "armv10_std.h"           

/*------------------------------ Function Prototypes -------------------------*/
static void TIM1_Config(void);
void clk_init(void);
static void NVIC_init(char position, char priority); 

/*------------------------------ Static Variables--------____-----------------*/
static int sekunden;
static int minuten;
static int stunden;
static int zehntel;

/******************************************************************************/
/*           Interrupt Service Routine  Timer1 (General Purpose Timer)        */
/******************************************************************************/
void TIM1_UP_IRQHandler(void)  //Timer 1, löst alle 1000ms aus
{
  TIM1->SR &=~0x01;   //Interrupt pending bit löschen (Verhinderung eines nochmaligen Interrupt-auslösens)
  if (zehntel == 10)
  {
    sekunden++;
    zehntel = 1;
  }
  else 
  {
    zehntel++;
  }
  if ((sekunden) == 60)
  {
    minuten++;
    sekunden = 0;
  }
  if (minuten == 60)
  {
    stunden++;
    minuten = 0;
  }
  if (stunden == 24)
  {
    stunden = 0;
  }
}

/******************************************************************************/
/*           Initialization Timer1 (General Purpose Timer)                    */
/******************************************************************************/
static void TIM1_Config(void)
{  
  /*---------------------- Timer 1 konfigurieren -----------------------*/
       
  RCC->APB2ENR |= 0x0800;  //Timer 1 Clock enable
  TIM1->SMCR = 0x0000;    //Timer 1 Clock Selection: CK_INT wird verwendet
  TIM1->CR1  = 0x0000;    // Auswahl des Timer Modus: Upcounter --> Zählt: von 0 bis zum Wert des Autoreload-Registers
  
  /* T_INT = 100ns, Annahme: Presc = 135 ---> Auto Reload Wert = 52801 (=0xCE41) --> 1s Update Event*/
  TIM1->PSC = 150;       //Wert des prescalers (Taktverminderung)  -- damit er genau 1 sekunde laueft -- 135 bei 8 MHz
  TIM1->ARR = 0xCE41;     //Auto-Reload Wert = Maximaler Zaehlerstand des Upcounters
  TIM1->RCR = 0;         //Repetition Counter deaktivieren

  /*--------------------- Interrupt Timer 1 konfigurieren ---------------------*/
  TIM1->DIER = 0x01;             //enable Interrupt bei einem UEV (Überlauf / Unterlauf)
  NVIC_init(TIM1_UP_IRQn,2);     //Enable Timer 1 Update Interrupt, Priority 2
  clk_init();                     //ruft clk_init auf

  /*-------------------------- Timer 1 Starten -------------------------------*/
  TIM1->CR1 |= 0x0001;   //Counter-Enable bit setzen
}
/******************************************************************************/
/*                   clk_init                                                   */
/* Funktion:                                                                  */
/*   Takt auf PLL setzen, um Programm mit 72MHz realisieren zu können         */
/******************************************************************************/
void clk_init(void)
{
  FLASH->ACR = 0x12; //Set Flash latency (2 waitstates)
  RCC->CR |= RCC_CR_HSEON; //HSE on
  
  while ((RCC->CR & RCC_CR_HSERDY) == 0); // Wait for HSERDY=1 (HSE is ready)
  
  RCC->CFGR |= RCC_CFGR_PLLMULL9; // 9 mal 8 = 72 MHz (SYSCLK)
  RCC->CFGR |= RCC_CFGR_ADCPRE_DIV6; //ADCCLK = SYSCLK/6 (APB2 PRESCALER=1)
  RCC->CFGR |= RCC_CFGR_PPRE1_DIV2; // PCLK1(APB1)=SYSCLK/2 (HCLK=SYSCLK)
  RCC->CFGR |= RCC_CFGR_PLLSRC; //PLL = SYSCLK,da AHB PRESCALER=1)
  RCC->CR |= RCC_CR_PLLON; //PLL on  
  //while ((RCC->CR & RCC_CR_PLLRDY) == 0); // Wait for PLLRDY=1 (PLL is ready)

  RCC->CFGR |= RCC_CFGR_SW_PLL; //PLL = Systemclock
  //while ((RCC->CFGR & RCC_CFGR_SWS_PLL) == 0);
  //Wait till SYSCLK is stabilized (depending on selected clock) */
  //while ((RCC->CFGR & RCC_CFGR_SWS) != ((RCC->CFGR<<2) & RCC_CFGR_SWS));
  // end of stm32_ClockSetup
  //RCC->BDCR |=RCC_BDCR_LSEON; //32kHz für RTC siehe AN2821 Reference Manual
  //448ff/1072 
    
  //RCC->CFGR |= 0x0241002;   //SW Register setzen fuer PLL = 2 //PLLXTPRE auf 0 setzen -- deaktivert divide by 2 // PLLMUL auf 9 setzen -- multipliziert HSE mit 9
  RCC->AHBENR |= 0x0000;    //AHB Prescaler setzen -- not divided
  RCC->APB1ENR |= 0x000;    // APB2 Prescaler setzen -- not divided*/
}
/******************************************************************************/
/*                   NVIC_init(char position, char priority)                  */ 
/* Funktion:                                                                  */    
/*   Übernimmt die vollständige initialisierung eines Interrupts  im Nested   */
/*   vectored Interrupt controller (Priorität setzen, Auslösen verhindern,    */
/*   Interrupt enablen)                                                       */
/* Übergabeparameter: "position" = 0-67 (Nummer des Interrupts)               */
/*                    "priority": 0-15 (Priorität des Interrupts)              */
/******************************************************************************/
static void NVIC_init(char position, char priority) 
{  
  NVIC->IP[position]=(priority<<4);  //Interrupt priority register: Setzen der Interrupt Priorität
  NVIC->ICPR[position >> 0x05] |= (0x01 << (position & 0x1F));//Interrupt Clear Pendig Register: Verhindert, dass der Interrupt auslöst sobald er enabled wird 
  NVIC->ISER[position >> 0x05] |= (0x01 << (position & 0x1F));//Interrupt Set Enable Register: Enable interrupt
} 

/******************************************************************************/
/*                                MAIN function                               */
/******************************************************************************/
int main (void) 
{
  set_clock_72MHz();
  
  int lcd_sekunden;     // aktueller Zählerstand auf LCD
  int lcd_minuten;
  int lcd_stunden;
  int lcd_zehntel;
  char einh[10];
  char ein[10];
  char zeh[10];
  char hour[10];
  char min[10];
  char eimin[10];
  char sek[10];

   lcd_init();           // LCD initialisieren
  lcd_clear();          // Lösche LCD Anzeige  
  sekunden=0;         // Sekundenzaehler initialisieren
  lcd_sekunden = 0;    // aktueller Anzeigewert
  minuten=0;
  lcd_minuten=0;
  stunden = 0;
  lcd_stunden=0;
  zehntel = 0;
  lcd_zehntel=0;
  TIM1_Config(); // Timer 1 starten: Upcounter --> loest alle 1s einen Update Interrupt
  do
  {
    if (lcd_zehntel != zehntel)          // 1 Zehntel vergangen
    {
      lcd_zehntel = zehntel;
    }
    if (lcd_sekunden != sekunden)        // 1 Sekunde vergangen ?
    { 
      lcd_sekunden = sekunden;
    }
    if (lcd_minuten != minuten)          // 1 Minute vergangen ?
    {
      lcd_minuten = minuten;
    } 
    if (lcd_stunden != stunden)          // 1 Stunde vergangen ?
    {
      lcd_stunden = stunden;
    }
    lcd_set_cursor(0,0);                 // Cursor auf Ursprung
    //Stundenanzeige
    int einer = lcd_stunden/10;
    sprintf(&einh[1],"%d", einer); // Stunden auf LCD aktualisieren
    lcd_put_string(&einh[1]);  
    //Einerstundenanzeige
    int hou = lcd_stunden%10;
    sprintf(&hour[1],"%d:", hou);     // Stunden auf LCD aktualisieren
    lcd_put_string(&hour[1]); 
    //Minutenanzeige Zehnerstelle
    int einermin = lcd_minuten/10;
    sprintf(&eimin[1],"%d", einermin);
    lcd_put_string(&eimin[1]);
    //Minutenanzeige Einerstelle
    int mins = lcd_minuten%10;
    sprintf(&min[1],"%d:", mins);
    lcd_put_string(&min[1]);
    //Sekundenanzeige - 
    sprintf(&sek[1],"%d", lcd_sekunden/10);
    lcd_put_string(&sek[1]);
    // - geteilt in einer und zehner stellen
    int einersek = lcd_sekunden%10;
    sprintf(&zeh[1],"%d:", einersek);
    lcd_put_string(&zeh[1]);
    //Anzeige der zehntel 
    int zehntel = lcd_zehntel%10;
    sprintf(&ein[1],"%d", zehntel);
    lcd_put_string(&ein[1]);
  }
  while (1);
}
