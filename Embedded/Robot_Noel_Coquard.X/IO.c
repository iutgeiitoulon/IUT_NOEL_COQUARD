/*
 * File:   IO.c
 */

#include <xc.h>
#include "IO.h"
#include "main.h"

void InitIO()
{
    // IMPORTANT : d�sactiver les entr�es analogiques, sinon on perd les entr�es num�riques
    ANSELA = 0; // 0 desactive
    ANSELB = 0;
    ANSELC = 0;
    ANSELD = 0;
    ANSELE = 0;
    ANSELF = 0;
    ANSELG = 0;

    // Configuration des sorties

    //******* LED ***************************
    _TRISC10 = 0;  // LED Orange
    _TRISG6  = 0; //LED Blanche
    _TRISG7  = 0; // LED Bleue
    
    //****** Moteurs ************************
    _TRISB14 = 0;
    _TRISB15 = 0;
    _TRISC6 = 0;
    _TRISC7 = 0;

    // Configuration des entr�es
    

    // Configuration des pins remappables    
    //*************************************************************
    // Unlock Registers
    //*************************************************************
    __builtin_write_OSCCONL(OSCCON & ~(1<<6)); 
    
    //Assignation des remappable pins
    
    //*************************************************************
    // Lock Registers
    //*************************************************************
    __builtin_write_OSCCONL(OSCCON | (1<<6));
}