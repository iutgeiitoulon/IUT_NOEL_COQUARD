#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ChipConfig.h"
#include "IO.h"
#include "timer.h"
#include "Robot.h"
#include "PWM.h"
#include "main.h"
#include "grafcet.h"

unsigned char stateRobot;

void OperatingSystemLoop(void) {
    switch (stateRobot) {
        case STATE_ATTENTE:
            timestamp = 0;
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_ATTENTE_EN_COURS;

        case STATE_ATTENTE_EN_COURS:
            if (timestamp > 1000)
                stateRobot = STATE_AVANCE;
            break;

        case STATE_AVANCE:
            PWMSetSpeedConsigne(25, MOTEUR_DROIT);
            PWMSetSpeedConsigne(25, MOTEUR_GAUCHE);
            stateRobot = STATE_AVANCE_EN_COURS;
            break;
        case STATE_AVANCE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_GAUCHE:
            PWMSetSpeedConsigne(10, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_DROITE:
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(10, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_SUR_PLACE_GAUCHE:
            PWMSetSpeedConsigne(10, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-10, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_SUR_PLACE_DROITE:
            PWMSetSpeedConsigne(-10, MOTEUR_DROIT);
            PWMSetSpeedConsigne(10, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        
        case STATE_TOURNE_VITE_DROITE:
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(15, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_VITE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_VITE_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        
        case STATE_TOURNE_VITE_GAUCHE:
            PWMSetSpeedConsigne(15, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_VITE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_VITE_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        default:
            stateRobot = STATE_ATTENTE;
            break;
    }
}

unsigned char nextStateRobot = 0;

void SetNextRobotStateInAutomaticMode() {
    unsigned char positionObstacle = PAS_D_OBSTACLE;

    //D�termination de la position des obstacles en fonction des t�l�m�tres
    if (robotState.distanceTelemetreDroit < 10 &&
            robotState.distanceTelemetreCentre > 20 &&
            robotState.distanceTelemetreGauche < 10 &&
            robotState.distanceTelemetreExtremeDroite < 10 &&
            robotState.distanceTelemetreExtremeGauche < 10 )
        positionObstacle = OBSTACLE_EN_FACE;
    else if (robotState.distanceTelemetreDroit < 25 &&
            robotState.distanceTelemetreCentre > 20 &&
            robotState.distanceTelemetreGauche < 25 &&
            robotState.distanceTelemetreExtremeDroite < 20 &&
            robotState.distanceTelemetreExtremeGauche < 20 )
        positionObstacle = PAS_D_OBSTACLE;
    else if (robotState.distanceTelemetreCentre > 20 &&
            robotState.distanceTelemetreExtremeDroite < 20 &&
            robotState.distanceTelemetreExtremeGauche > 20)
        positionObstacle = OBSTACLE_EXTREME_DROITE;
    else if (robotState.distanceTelemetreCentre > 20 &&
            robotState.distanceTelemetreExtremeDroite > 20 &&
            robotState.distanceTelemetreExtremeGauche < 20)
        positionObstacle = OBSTACLE_EXTREME_GAUCHE;
    else if (robotState.distanceTelemetreExtremeDroite < 20 &&
            robotState.distanceTelemetreCentre < 20)
        positionObstacle  = OBSTACLE_DEVANT_DROITE;
    else if (robotState.distanceTelemetreExtremeGauche < 20 &&
            robotState.distanceTelemetreCentre < 20)
        positionObstacle  = OBSTACLE_DEVANT_GAUCHE;
    else if (robotState.distanceTelemetreDroit < 30 &&
            robotState.distanceTelemetreCentre > 20 &&
            robotState.distanceTelemetreGauche > 30) //Obstacle � droite
        positionObstacle = OBSTACLE_A_DROITE;
    else if (robotState.distanceTelemetreDroit > 30 &&
            robotState.distanceTelemetreCentre > 20 &&
            robotState.distanceTelemetreGauche < 30) //Obstacle � gauche
        positionObstacle = OBSTACLE_A_GAUCHE;
    else if (robotState.distanceTelemetreCentre < 25) //Obstacle en face
        positionObstacle = OBSTACLE_EN_FACE;
     else if (robotState.distanceTelemetreDroit > 30 &&
             robotState.distanceTelemetreCentre > 20 &&
             robotState.distanceTelemetreGauche > 30) //pas d?obstacle
         positionObstacle = PAS_D_OBSTACLE;

    //D�termination de l?�tat � venir du robot
    if (positionObstacle == PAS_D_OBSTACLE)
        nextStateRobot = STATE_AVANCE;
    else if (positionObstacle == OBSTACLE_A_DROITE)
        nextStateRobot = STATE_TOURNE_GAUCHE;
    else if (positionObstacle == OBSTACLE_A_GAUCHE)
        nextStateRobot = STATE_TOURNE_DROITE;
    else if (positionObstacle == OBSTACLE_EN_FACE)
        nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
    else if (positionObstacle == OBSTACLE_EXTREME_GAUCHE)
        nextStateRobot = STATE_TOURNE_VITE_DROITE;
    else if (positionObstacle == OBSTACLE_EXTREME_DROITE)
        nextStateRobot = STATE_TOURNE_VITE_GAUCHE;
    else if (positionObstacle == OBSTACLE_DEVANT_GAUCHE)
        nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE ;
    else if (positionObstacle == OBSTACLE_DEVANT_DROITE)
        nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE ;
    //Si l?on n?est pas dans la transition de l?�tape en cours
    if (nextStateRobot != stateRobot - 1)
        stateRobot = nextStateRobot;
}