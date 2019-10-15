#include "partida.h"
#include "stdlib.h"
#include "stdio.h"

void nueva_partida(tPartida * p, int modo_partida, int comienza, char * j1_nombre, char * j2_nombre){

    p=(tPartida *) malloc (sizeof(tPartida));
    if(p==NULL)
        exit(PART_ERROR_MEMORIA);

    *p=(tPartida) malloc (sizeof(struct partida));
    if((*p)==NULL)
        exit(PART_ERROR_MEMORIA);

    //seteo modo y turno
    (*p)->modo_partida=modo_partida;
    (*p)->turno_de=comienza;       //si es jugador random que?
    (*p)->estado=PART_EN_JUEGO;

    //seteo nombres sin chequeo
    char guarda='\0';
    int i = 0;
    while(j1_nombre[i]!=guarda){
        (*p)->nombre_jugador_1[i]=j1_nombre[i];
        i++;
    }
    i = 0;
    while(j2_nombre[i]!=guarda){
        (*p)->nombre_jugador_2[i]=j2_nombre[i];
        i++;
    }

    //inicializo el tablero
    (*p)->tablero=(tTablero) malloc (sizeof(struct tablero));
    if((*p)->tablero == NULL)
        exit(PART_ERROR_MEMORIA);
    for(int i=0; i<=2; i++){
        for(int j=0; j<=2; j++){
            (*p)->tablero->grilla[i][j]=0;
        }
    }
}

int gano(tPartida p, int ficha){
    tTablero t=p->tablero;
    int gano=0;
    int i=0;
    while(gano!=3 || i<3){
        for(int j=0; j<3; j++){
            if(t->grilla[i][j]==ficha)
                gano++;
            }
        if(gano<3)
            gano=0;
        i++;
    }
    if(gano>=3)
        return 1; //gano
    else
        gano=0;

    int j=0;
    while(gano!=3 || j<3){
        for(int i=0; i<3; i++){
            if(t->grilla[i][j]==ficha)
                gano++;
            }
        if(gano<3)
            gano=0;
        j++;
    }
    if(gano>=3)
        return 1; //gano
    else
        gano=0;

    //diagonal hardcodeada x2
    i=0; j=0;
    while(j<3 && i<3){
        if(t->grilla[i][j]==ficha)
            gano++;
        else{
            j=2;
            i=2;
        }
    i++;
    j++;
    }
    if(gano>=3)
        return 1; //gano
    else
        gano=0;
    while(j>0 && i<3){
        j--;
        i--;
        if(t->grilla[i][j]==ficha)
            gano++;
        else{
            j=0;
            i=0;
        }
    }
    if(gano>=3)
        return 1; //gano
    else
        return gano=0;

}

int nuevo_movimiento(tPartida p, int mov_x, int mov_y){
    tTablero t=p->tablero;

    int lleno=0;

    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            if(t->grilla[i][j]!=0)
                lleno++;
        }
    }

    if(mov_x > 2 || mov_y > 2 || mov_x < 0 || mov_y < 0)
        exit(PART_MOVIMIENTO_ERROR);

    if(lleno>=9){
        p->estado=PART_SIN_MOVIMIENTO;
        return PART_MOVIMIENTO_ERROR;
    }

    if(t->grilla[mov_x][mov_y]!=0)
        return (PART_MOVIMIENTO_ERROR);
    else{
        if(p->turno_de==PART_JUGADOR_1){
            t->grilla[mov_x][mov_y]=1;
            if(gano(p,1))
                return PART_GANA_JUGADOR_1;
        }
        else{
            t->grilla[mov_x][mov_y]=2;
            if(gano(p,2))
                return PART_GANA_JUGADOR_2;
        }
    }
    return PART_MOVIMIENTO_OK;
}

void finalizar_partida(tPartida * p){
    free((*p)->nombre_jugador_1);
    free((*p)->nombre_jugador_2);
    free((*p)->tablero);
    free(*p);
    free(p);
    p=NULL;
}
