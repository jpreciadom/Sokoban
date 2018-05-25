#include <curses.h>

enum dir {
    arriba, abajo, izquierda, derecha, nada
};

struct point {
    int x;
    int y;
};

struct game {
    int num_cajas;
    int cajas_libres;
    int cajas_enobjetivos = 0;
};

struct limites {
    int x1 = 1;
    int y1 = 1;
    int x2 = 79;
    int y2 = 29;
};

struct fun {
    int c = getch();
    limites l;
};

short color_from_RGB(char r, char g, char b) {
    return (r*36 + g*6 + b*1) + 16;
}

int contador (int tm, int n){
    if (tm > 0){
        n++;
        return contador (tm/10, n);
    } else {
        return n;
    }
}

void bordes (int x1, int x2, int y1, int y2){
    color_set(3,NULL);
    mvaddch(0,0,ACS_ULCORNER);
    for (int yL = y1; yL<y2+1; yL++){
        mvaddch(yL,x1-1,ACS_VLINE);
        mvaddch(yL,x2+1,ACS_VLINE);
    }
    mvaddch(y2+1,0,ACS_LLCORNER);
    mvaddch(0,x2+1,ACS_URCORNER);
    for (int xL = x1; xL<x2+1; xL++){
        mvaddch(y1-1, xL, ACS_HLINE);
        mvaddch(y2+1, xL, ACS_HLINE);
    }
    mvaddch(y2+1,x2+1,ACS_LRCORNER);
}

void cuadro (int x, int y){
    mvaddch(y,x,' ');
    mvaddch(y,x+1,' ');
}

void cuadro2 (int x, int y){
    mvaddch(y,x,' ');
    mvaddch(y,x+1,' ');
    mvaddch(y+1,x,' ');
    mvaddch(y+1,x+1,' ');
    mvaddch(y,x+2,' ');
    mvaddch(y,x+3,' ');
    mvaddch(y+1,x+2,' ');
    mvaddch(y+1,x+3,' ');
}

void info (int level, int cl, int co, int m) {
    color_set(2,NULL);
    mvaddstr(31, 1, "Presiona la tecla 'r' para reiniciar");
    mvaddstr(32, 1, "Presiona la tecla 'z' para deshacer movimientos");
    mvaddstr(33, 1, "Presiona la tecla 'q' para salir del juego");
    mvaddstr(34, 1, "Presiona la tecla 'm' para ir al menú principal");
    mvaddstr(2, 82, "Nivel: ");
    mvaddstr(5, 82, "Cajas libres: ");
    mvaddstr(8, 82, "Cajas en objetivos: ");
    mvaddstr(11, 82, "Movimientos realizados: ");
    color_set(3,NULL);
    mvprintw(2, 89, "%d", level);
    mvprintw(5, 96, "%d", cl);
    mvprintw(8, 102, "%d", co);
    mvprintw(11, 108, "%d", m);
}

void mov (bool &quit, int &x, int &y, dir &d, int &level, bool &ret) {
    fun g;
    clear();
    bordes(g.l.x1,g.l.x2,g.l.y1,g.l.y2);
    switch(g.c){
        case 'q': case 'Q': quit = true;               break;
        case KEY_UP:        y-=2; d = arriba;          break;
        case KEY_DOWN:      y+=2; d = abajo;           break;
        case KEY_LEFT:      x-=4; d= izquierda;        break;
        case KEY_RIGHT:     x+=4; d = derecha;         break;
        case 'm': case 'M': level = 0;                 break;
        case 'r': case 'R': level++; quit = true;      break;
        case 'z': case 'Z': ret = true;                break;
    }
    napms(20);
}

void mainmenu(bool &quit, int &level, int &tm){
    tm = 0;
    int xs = 32;
    int ys = 15;
    while (!quit && level == 0){
        fun g;
        bool conf = false;
        switch (g.c){
            case 'q':
            case 'Q':         quit = true;          break;
            case KEY_UP:      xs = 32;  ys = 15;    break;
            case KEY_DOWN:    xs = 31;  ys = 20;    break;
            case '\n':        conf = true;          break;
        }
        napms(15);
        clear();
        bordes (g.l.x1,g.l.x2,g.l.y1,g.l.y2);
        color_set(1,NULL);
        for (int x = 13; x<=47;x+=2){
            if (((((x-10)%8)-1)!=0)&&(x-31 != 0)){
                cuadro(x,5);
                cuadro(x,9);                              //imprime la parte de arriba y abajo de cada letra
            }
        }
        cuadro(13,6); cuadro(13,7); cuadro(15,7); cuadro(17,7); cuadro(17,8);                              //imprime las demás partes de la s
        cuadro(31,7);                                                                                      //imprime una parte de la k
        cuadro(48,5); cuadro(48,9); cuadro(48,7); cuadro(47,7); cuadro(49,6); cuadro(49,8);                //imprime la b
        cuadro(61,5); cuadro(61,9); cuadro(65,5); cuadro(65,9); cuadro(62,6); cuadro(63,7); cuadro(64,8);  //imprime la n
        for (int y = 7; y<=9; y++){
            cuadro(53,y);   //imprime la izquierda de la a
            cuadro(57,y);   //imprime la derecha de la a
        }
        cuadro(54,6); cuadro (56,6); cuadro(55,5); cuadro(55,8);
        for (int y = 6; y<9; y++){
            cuadro (21, y); //imprime la izquierda de la o
            cuadro (25, y); // imprime la derecha de la o
            cuadro (29, y); //imprime la izquierda de la k
            if (y!=7){
                cuadro (32, y); //imprime la derecha de la k
            }
            cuadro (37, y); //imprime la izquierda de la o
            cuadro (41, y); //imprime la derecha de la o
            cuadro (45, y); //imprime la izquieda de la b
            cuadro (61, y); //imprime la izquierda de la n
            cuadro (65, y); //imprime la derecha de la n
        }
        if (conf==true && ys ==15){
            clear();
            level = 1;
        } else {
            if (conf==true && ys ==20){
                clear();
                level = 18;
            }
        }
        cuadro(xs,ys);
        color_set(2,NULL);
        move(15, 35);
        addstr("¡COMENZAR!");
        move(20, 34);
        addstr("INSTRUCCIONES");
        move(31,1);
        addstr("Utiliza las flechas para moverte.");
        move(32,1);
        addstr("Presiona la tecla enter para seleccionar.");
    }
}

void run_level(bool &quit, int &level, int x, int y, point cajas[], point labs[],point objs[], dir d, game gam, int bloques, int curr_level, int &m, int &tm) {
    bool ret = false;
    int rep = 0;
    int newx = x;
    int newy = y;
    point newcajas[gam.num_cajas];
    point cajasp[gam.num_cajas][10];
    point player[10];
    for (int r = 0; r < 5; r++){
        player[r].x = 0;
        player[r].y = 0;
        for (int c = 0; c < gam.num_cajas; c++){
            cajasp[c][r].x = 0;
            cajasp[c][r].y = 0;
        }
    }
    while (!quit && level == curr_level){
        mov(quit, x, y, d, level, ret);             //lama a la función que mueve el personaje
        gam.cajas_libres = gam.num_cajas;
        gam.cajas_enobjetivos = 0;
        for (int a = 0; a<bloques; a++){
            color_set(6,NULL);
            cuadro2(labs[a].x,labs[a].y);
            if (x == labs[a].x && y == labs[a].y){
                if (x == labs[a].x && d == derecha){
                    x -= 4;
                }
                if (x == labs[a].x && d == izquierda){
                    x += 4;
                }
                if (y == labs[a].y && d == arriba){
                    y += 2;
                }
                if (y == labs[a].y && d == abajo){
                    y -= 2;
                }
            }
            for (int c = 0; c<gam.num_cajas; c++){
                color_set(7,NULL);
                if (x == cajas[c].x && y== cajas[c].y){
                    if (x == cajas[c].x && d == derecha){
                        cajas[c].x += 4;
                    } else{
                        if (x == cajas[c].x && d == izquierda){
                            cajas[c].x -= 4;
                        }
                    }
                    if (y == cajas[c].y && d == arriba){
                        cajas[c].y -= 2;
                    } else {
                        if (y == cajas[c].y && d == abajo){
                            cajas[c].y += 2;
                        }
                    }
                }
                for (int o = 0; o<gam.num_cajas; o++){
                    if ((c!=o) && cajas[c].x == cajas[o].x && cajas[c].y == cajas[o].y){
                        if (cajas[c].x == cajas[o].x && d == izquierda){
                            cajas[c].x+=4;
                            x+=4;
                        } else {
                            if (cajas[c].x == cajas[o].x && d == derecha){
                                cajas[c].x-=4;
                                x-=4;
                            }
                        }
                        if (cajas[c].y == cajas[o].y && d == arriba){
                            cajas[c].y+=2;
                            y+=2;
                        } else {
                            if (cajas[c].y == cajas[o].y && d == abajo){
                                cajas[c].y-=2;
                                y-=2;
                            }
                        }
                    }
                }
                if (cajas[c].x == labs[a].x && cajas[c].y == labs[a].y){
                    if (cajas[c].x==labs[a].x && d == izquierda){
                        cajas[c].x+=4;
                        x+=4;
                    } else {
                        if (cajas[c].x == labs[a].x && d == derecha){
                            cajas[c].x-=4;
                            x-=4;
                        }
                    }
                    if (cajas[c].y== labs[a].y && d == arriba){
                        cajas[c].y+=2;
                        y+=2;
                    } else {
                        if (cajas[c].y == labs[a].y && d == abajo){
                            cajas[c].y-=2;
                            y-=2;
                        }
                    }
                }
                cuadro2(cajas[c].x,cajas[c].y);
            }
        }
        for (int o = 0; o<gam.num_cajas; o++){ //usa las posiciones de los objetivos
            color_set(4, NULL);
            for (int a = 0; a<gam.num_cajas; a++){ //usa las posiciones de las cajas
                if (objs[o].x == cajas[a].x && objs[o].y == cajas[a].y){ // compara si las cajas estan sobre los objetivos
                    gam.cajas_libres--;
                    gam.cajas_enobjetivos++;
                    color_set(1, NULL);
                }
                cuadro2(objs[o].x, objs[o].y); //imprime los objetivos
            }
        }
        if (ret == true && rep > 0){
            x = player[9].x;
            y = player[9].y;
            for (int c = 0; c<gam.num_cajas; c++){
                cajas[c].x = cajasp[c][9].x;
                cajas[c].y = cajasp[c][9].y;
            }
            for (int r = 9; r>0; r--){
                player[r].x = player[r-1].x;
                player[r].y = player[r-1].y;
                for (int c = 0; c<gam.num_cajas; c++){
                    cajasp[c][r].x = cajasp[c][r-1].x;
                    cajasp[c][r].y = cajasp[c][r-1].y;
                }
            }
            player[0].x = 0;
            player[0].y = 0;
            rep--;
        } else {
            if ((newx != x || newy != y)&&ret == false){
                m++;
                if (rep < 10){
                    rep++;
                }
                for (int r = 0; r<=8; r++){
                    player[r].x = player[r+1].x;
                    player[r].y = player[r+1].y;
                    for (int c = 0; c<gam.num_cajas; c++){
                        cajasp[c][r].x = cajasp[c][r+1].x;
                        cajasp[c][r].y = cajasp[c][r+1].y;
                    }
                }
                for (int c = 0; c < gam.num_cajas; c++){
                    cajasp [c][9].x = newcajas[c].x;
                    cajasp [c][9].y = newcajas[c].y;
                }
                player[9].x = newx;
                player[9].y = newy;
            }
        }
        ret = false;
        for (int c = 0; c<gam.num_cajas; c++){
            newcajas[c].x = cajas[c].x;
            newcajas[c].y = cajas[c].y;
        }
        newx = x;
        newy = y;
        color_set(5,NULL);
        cuadro2(x,y);
        info (level, gam.cajas_libres, gam.cajas_enobjetivos, m);
        if (gam.cajas_libres == 0){
            level = curr_level+1;
            clear();
        }
    }
    if (quit == true && level != curr_level){
        level = curr_level;
        quit = false;
    }
    tm += m;
}

void level1(bool &quit, int &level, int m, int &tm){     //El Nivel 1 ya esta. Tomado de Nivel_0
    int x = 37;
    int y = 7;
    point cajas[3] = {{33,11}, {37,11}, {41,11}};
    point labs[39] = {{21,5},{25,5},{29,5},{33,5},{37,5},{41,5},{45,5},{49,5},{53,5},{53,7},{53,9},{53,11},{53,13},{53,15},{53,17},{53,19},{53,21},{49,21},{45,21},{45,19},{41,21},{41,19},{41,17},{37,21},{37,19},{33,21},{33,19},{33,17},{29,21},{29,19},{25,21},{21,21},{21,19},{21,17},{21,15},{21,13},{21,11},{21,9},{21,7}};
    point objs[3] = {{29, 17},{37,17},{45,17}};
    dir d = nada;
    game gam;
    gam.num_cajas = 3;
    int bloques = 39;

    run_level(quit, level, x, y, cajas, labs, objs, d, gam, bloques, 1, m, tm);
}

void level2(bool &quit, int &level, int m, int &tm){     //El Nivel 2 ya esta. Tomado de Nivel_1
    int x = 38;
    int y = 14;
    point cajas[4] = {{34,14}, {34,12}, {38,16}, {42,12}};
    point objs[4] = {{26,14},{34,8},{38,18},{46,12}};
    point labs[28] = {{30,6},{34,6},{38,6},{38,8},{38,10},{42,10},{46,10},{50,10},{50,12},{50,14},{46,14},{42,14},{42,16},{42,18},{42,20},{38,20},{34,20},{34,18},{34,16},{30,16},{26,16},{22,16},{22,14},{22,12},{26,12},{30,12},{30,10},{30,8}};
    dir d = nada;
    game gam;
    gam.num_cajas = 4;
    int bloques = 28;

    run_level(quit, level, x, y, cajas, labs, objs, d, gam, bloques, 2, m, tm);
}

void level3(bool &quit, int &level, int m, int &tm){     //El Nivel 3 ya esta. Tomado de Nivel_2
    int x = 34;
    int y = 10;
    point cajas[4] = {{38,10}, {34,12}, {38,14}, {34,16}};
    point objs[4] = {{30,16},{30,18},{42,16},{42,18}};
    point labs[26] = {{26,8},{26,10},{26,12},{30,12},{30,14},{26,14},{26,16},{26,18},{26,20},{30,20},{34,20},{38,20},{42,20},{46,20},{46,18},{46,16},{46,14},{46,12},{42,12},{42,10},{42,8},{42,6},{38,6},{34,6},{30,6},{30,8}};
    dir d = nada;
    game gam;
    gam.num_cajas = 4;
    int bloques = 26;

    run_level(quit, level, x, y, cajas, labs, objs, d, gam, bloques, 3, m, tm);
}

void level4(bool &quit, int &level, int m, int &tm){     //El Nivel 4 ya esta. Tomado de Nivel_3
    int x = 33;
    int y = 9;
    point cajas[3] = {{29,9},{29,11},{33,11}};
    point objs[3] = {{49,11},{49,13},{49,15}};
    point labs[39] = {{21,5},{25,5},{29,5},{33,5},{37,5},{37,7},{37,9},{37,11},{37,13},{41,13},{45,13},{45,11},{45,9},{49,9},{53,9},{53,11},{53,13},{53,15},{53,17},{53,19},{49,19},{45,19},{41,19},{41,17},{41,21},{37,21},{33,21},{29,21},{25,21},{25,19},{25,17},{25,15},{29,15},{29,13},{25,13},{21,13},{21,11},{21,9},{21,7}};
    dir d = nada;
    game gam;
    gam.num_cajas = 3;
    int bloques = 39;

    run_level(quit, level, x, y, cajas, labs, objs, d, gam, bloques, 4, m, tm);
}

void level5(bool &quit, int &level, int m, int &tm){     //El Nivel 5 ya esta. Tomado de Nivel_4
    int x = 26;
    int y = 12;
    point cajas[5] = {{30,14}, {34,12}, {34,10}, {38,10}, {42,10}};
    point objs[5] = {{34,14},{38,14},{38,12},{42,14},{42,12}};
    point labs[28] = {{22,10},{22,12},{22,14},{22,16},{26,16},{30,16},{34,16},{34,18},{34,20},{38,20},{42,20},{46,20},{46,18},{46,16},{46,14},{50,14},{50,12},{50,10},{50,8},{50,6},{46,6},{42,6},{38,6},{34,6},{30,6},{30,8},{30,10},{26,10}};
    dir d = nada;
    game gam;
    gam.num_cajas = 5;
    int bloques = 28;

    run_level(quit, level, x, y, cajas, labs, objs, d, gam, bloques, 5, m, tm);
}

void level6(bool &quit, int &level, int m, int &tm){     //El Nivel 6 ya esta. Tomado de Nivel_5
    int x = 31;
    int y = 8;
    point cajas[3] = {{35,10},{31,16},{43,18}};
    point objs[3] = {{27,14},{27,16},{27,18}};
    point labs[33] = {{27,6},{31,6},{35,6},{39,6},{39,8},{43,8},{47,8},{47,10},{47,12},{51,12},{51,14},{51,16},{51,18},{51,20},{47,20},{43,20},{39,20},{35,20},{31,20},{27,20},{23,20},{23,18},{23,16},{23,14},{23,12},{27,12},{27,10},{27,8},{31,12},{39,12},{39,14},{43,16},{31,14}};
    dir d = nada;
    game gam;
    gam.num_cajas = 3;
    int bloques = 33;

    run_level(quit, level, x, y, cajas, labs, objs, d, gam, bloques, 6, m, tm);
}

void level7(bool &quit, int &level, int m, int &tm){     //El Nivel 7 ya esta. Tomado de Nivel_6
    int x = 34;
    int y = 18;
    point cajas[4] = {{34,14}, {38,12}, {38,16}, {42,16}};
    point objs[4] = {{34,8},{38,8},{38,10},{42,12}};
    point labs[29] = {{22,14},{22,16},{22,18},{22,20},{26,20},{30,20},{34,20},{34,16},{38,20},{42,20},{46,20},{50,20},{50,18},{50,16},{50,14},{46,14},{46,12},{46,10},{42,10},{42,8},{42,6},{38,6},{34,6},{30,6},{30,8},{30,10},{26,10},{26,12},{26,14}};
    dir d = nada;
    game gam;
    gam.num_cajas = 4;
    int bloques = 29;

    run_level(quit, level, x, y, cajas, labs, objs, d, gam, bloques, 7, m, tm);
}

void level8(bool &quit, int &level, int m, int &tm){     //El Nivel 8 ya esta. Tomado de Nivel_7
    int x = 43;
    int y = 9;
    point cajas[4] = {{35,11},{39,13},{39,15},{43,15}};
    point objs[4] = {{39,11},{35,13},{43,13},{39,15}};
    point labs[26] = {{31,7},{35,7},{39,7},{43,7},{47,7},{47,9},{47,11},{51,11},{51,13},{51,15},{51,17},{47,17},{47,19},{43,19},{39,19},{35,19},{31,19},{31,17},{31,15},{27,15},{23,15},{23,13},{23,11},{23,9},{27,9},{31,9}};
    dir d = nada;
    game gam;
    gam.num_cajas = 4;
    int bloques = 26;

    run_level(quit, level, x, y, cajas, labs, objs, d, gam, bloques, 8, m, tm);
}

void level9(bool &quit, int &level, int m, int &tm){     //El Nivel 9 ya esta. Tomado de Nivel_8
    int x = 32;
    int y = 17;
    point cajas[4] = {{28,11}, {32,11}, {36,11}, {44,15}};
    point objs[4] = {{36,13},{36,15},{40,13},{40,15}};
    point labs[29] = {{20,7},{20,9},{20,11},{20,13},{20,15},{24,15},{24,17},{24,19},{28,19},{32,19},{36,19},{40,19},{44,19},{48,19},{52,19},{52,17},{52,15},{52,13},{48,13},{44,13},{44,11},{40,11},{40,9},{40,7},{36,7},{32,7},{28,7},{24,7},{32,13}};
    dir d = nada;
    game gam;
    gam.num_cajas = 4;
    int bloques = 29;

    run_level(quit, level, x, y, cajas, labs, objs, d, gam, bloques, 9, m, tm);
}

void level10(bool &quit, int &level, int m, int &tm){    //El Nivel 10 ya esta. Tomado de Nivel_9
    int x = 27;
    int y = 13;
    point cajas[6] = {{31,11},{31,13},{31,15},{39,13},{43,11},{43,15}};
    point objs[6] = {{35,11},{35,13},{35,15},{39,11},{39,13},{39,15}};
    point labs[29] = {{23,7},{27,7},{31,7},{35,7},{39,7},{43,7},{47,7},{51,7},{51,9},{51,11},{51,13},{51,15},{51,17},{51,19},{47,19},{43,19},{39,19},{35,19},{31,19},{27,19},{23,19},{23,17},{23,15},{23,13},{23,11},{23,9},{35,9},{35,17},{47,13}};
    dir d = nada;
    game gam;
    gam.num_cajas = 6;
    int bloques = 29;

    run_level(quit, level, x, y, cajas, labs, objs, d, gam, bloques, 10, m, tm);
}

void level11(bool &quit, int &level, int m, int &tm){    //El Nivel 11 ya esta. Tomado de Nivel_10
    int x = 34;
    int y = 8;
    point cajas[3] = {{34,14}, {38,10}, {38,14}};
    point objs[3] = {{34,12},{42,12},{42,16}};
    point labs[30] = {{22,10},{22,12},{22,14},{22,16},{26,16},{30,16},{30,18},{30,20},{34,20},{38,20},{42,20},{46,20},{46,18},{46,16},{46,14},{50,14},{50,12},{50,10},{50,8},{46,8},{42,8},{42,6},{38,6},{34,6},{30,6},{26,6},{26,8},{26,10},{34,10},{38,16}};
    dir d = nada;
    game gam;
    gam.num_cajas = 3;
    int bloques = 30;

    run_level(quit, level, x, y, cajas, labs, objs, d, gam, bloques, 11, m, tm);
}

void level12(bool &quit, int &level, int m, int &tm){    //El Nivel 12 ya esta. Tomado de Nivel_11
    int x = 41;
    int y = 18;
    point cajas[8] = {{37,8},{33,12},{37,12},{41,12},{37,14},{33,16},{37,16},{41,16}};
    point objs[8] = {{29,8},{33,8},{41,8},{45,8},{29,10},{33,10},{41,10},{45,10}};
    point labs[28] = {{25,6},{29,6},{33,6},{37,6},{41,6},{45,6},{49,6},{49,8},{49,10},{49,12},{49,14},{49,16},{49,18},{49,20},{45,20},{41,20},{37,20},{33,20},{29,20},{25,20},{25,18},{25,16},{25,14},{25,12},{25,10},{25,8},{37,10},{37,18}};
    dir d = nada;
    game gam;
    gam.num_cajas = 8;
    int bloques = 28;

    run_level(quit, level, x, y, cajas, labs, objs, d, gam, bloques, 12, m, tm);
}

void level13(bool &quit, int &level, int m, int &tm){    //El Nivel 13 ya esta. Tomado de Nivel_12
    int x = 44;
    int y = 11;
    point cajas[4] = {{36,9}, {32,15}, {40,11}, {40,13}};
    point objs[4] = {{28,11},{28,13},{36,15},{36,19}};
    point labs[28] = {{24,9},{24,11},{24,13},{24,15},{24,17},{28,17},{32,17},{32,19},{32,21},{36,21},{40,21},{40,19},{44,19},{44,17},{44,15},{48,15},{48,13},{48,11},{48,9},{44,9},{44,7},{44,5},{40,5},{36,5},{32,5},{32,7},{32,9},{28,9}};
    dir d = nada;
    game gam;
    gam.num_cajas = 4;
    int bloques = 28;

    run_level(quit, level, x, y, cajas, labs, objs, d, gam, bloques, 13, m, tm);
}

void level14(bool &quit, int &level, int m, int &tm){    //El Nivel 14 ya esta. Tomado de Nivel_13
    int x = 43;
    int y = 9;
    point cajas[5] = {{35,9},{35,11},{35,13},{35,15},{35,17}};
    point objs[5] = {{35,11},{35,13},{35,15},{35,17},{35,19}};
    point labs[27] = {{27,5},{31,5},{35,5},{39,5},{43,5},{47,5},{47,7},{47,9},{47,11},{47,13},{43,13},{43,15},{43,17},{43,19},{43,21},{39,21},{35,21},{31,21},{27,21},{27,19},{27,17},{27,15},{27,13},{27,11},{27,9},{27,7},{31,11}};
    dir d = nada;
    game gam;
    gam.num_cajas = 5;
    int bloques = 27;

    run_level(quit, level, x, y, cajas, labs, objs, d, gam, bloques, 14, m, tm);
}

void level15(bool &quit, int &level, int m, int &tm){    //El Nivel 15 ya esta. Tomado de Nivel_14
    int x = 34;
    int y = 17;
    point cajas[3] = {{38,13}, {38,15}, {42,11}};
    point objs[3] = {{30,11},{30,13},{34,13}};
    point labs[29] = {{22,11},{22,13},{22,15},{22,17},{22,19},{26,19},{30,19},{34,19},{38,19},{42,19},{42,17},{46,17},{50,17},{50,15},{50,13},{50,11},{50,9},{46,9},{46,7},{42,7},{38,7},{34,7},{30,7},{26,7},{26,9},{26,11},{34,11},{38,11},{34,15}};
    dir d = nada;
    game gam;
    gam.num_cajas = 3;
    int bloques = 29;

    run_level(quit, level, x, y, cajas, labs, objs, d, gam, bloques, 15, m, tm);
}

void level16(bool &quit, int &level, int m, int &tm){    //El Nivel 16 ya esta. Tomado de Nivel_15
    int x = 29;
    int y = 16;
    point cajas[5] = {{29,12},{45,12},{37,14},{45,14},{33,16}};
    point objs[5] = {{25,14},{29,14},{25,16},{25,18},{29,18}};
    point labs[34] = {{21,6},{25,6},{29,6},{33,6},{37,6},{37,8},{37,10},{41,8},{45,8},{49,8},{53,8},{53,10},{53,12},{53,14},{49,14},{41,14},{49,16},{49,18},{45,18},{41,18},{41,20},{37,20},{33,20},{33,14},{29,20},{25,20},{21,20},{21,18},{21,16},{21,14},{21,12},{21,10},{29,10},{21,8}};
    dir d = nada;
    game gam;
    gam.num_cajas = 5;
    int bloques = 34;

    run_level(quit, level, x, y, cajas, labs, objs, d, gam, bloques, 16, m, tm);
}

void level17(bool &quit, int &level, int m, int tm){
    fun g;
    nodelay(stdscr, false);
    clear();
    bordes(g.l.x1,g.l.x2,g.l.y1,g.l.y2);
    color_set(1,NULL);
    char f [11][47] = {
        "######  ##  ##  ##      #####   ######  ##    ",
        "##      ##  ### ##      ##  ##  ##      ##    ",
        "####    ##  ######      ##  ##  ####    ##    ",
        "##      ##  ## ###      ##  ##  ##      ##    ",
        "##      ##  ##  ##      #####   ######  ######",
        "                                              ",
        "        ##  ##  ##  ######  ######  ######    ",
        "        ##  ##  ##  ##      ##      ##  ##    ",
        "        ##  ##  ##  ####    ## ###  ##  ##    ",
        "        ##  ##  ##  ##      ##  ##  ##  ##    ",
        "    ######  ######  ######  ######  ######    ",
    };
    for (int y = 0; y<11; y++){
        for (int x = 0; x<46; x++){
            if (f[y][x]== '#'){
                mvaddch(y+5,(78-46)/2+1+x,' ');
            }
        }
    }
    color_set(2, NULL);
    mvaddstr(20, 32, "¡FELICITACIONES!");
    char ft1 [] = "Has pasado el juego en ";
    int cd = contador(tm, 0);
    char ft2 [] = " movimientos";
    int x = ((78-(23+cd+12))/2)+1;
    mvprintw(22, x, ft1);
    x += 23;
    mvprintw(22, x, "%d", tm);
    x += cd;
    mvprintw(22, x, ft2);
    int c = getch();
    switch (c){
        default:  level = 0;   break;
    }
    nodelay(stdscr, true);
}

void level18(bool &quit, int &level){
    fun g;
    clear();
    color_set(2,NULL);
    mvaddstr(2,1, "Usa las flechas para mover a tu personaje.");
    mvaddstr(4,1, "Empuja las cajas de color café hasta los objetivos de color rojo,");
    mvaddstr(5,1, "una vez hecho esto la caja cambiara a color verde.");
    mvaddstr(7,1, "En cualquier momento puedes presionar la tecla 'r' para reiniciar el nivel, ");
    mvaddstr(8,1, "o bien puedes usar la tecla 'z' para retroceder paso por paso ");
    mvaddstr(9,1, "hasta un maximo de 10 movimientos.");
    mvaddstr(11,1, "Una vez finalices todos los niveles se te mostraran la cantidad de movimientos");
    mvaddstr(12,1, "que hiciste para completar el juego");
    mvaddstr(14,1, "Ni reiniciar el nivel ni deshacer pasos disminuye la cantidad ");
    mvaddstr(15,1, "de moviemientos realizados");
    mvaddstr(20,1, "Presiona cualquier tecla para empezar a jugar.");
    bordes(g.l.x1,g.l.x2,g.l.y1,g.l.y2);
    nodelay(stdscr, false);
    getch();
    level = 1;
    nodelay(stdscr, true);
}

int main () {
    ttytype [0] = 40; ttytype [1] = 50;
    ttytype [2] = 80; ttytype [2] = (unsigned char) 160;

    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, true);
    start_color();

    init_pair(1, COLOR_BLACK, COLOR_GREEN); //color de las cajas en los objetivos
    init_pair(2, COLOR_GREEN, COLOR_BLACK); //color de las letras
    init_pair(3, COLOR_WHITE, COLOR_BLACK); //color de las líneas del borde
    init_pair(4, COLOR_BLACK, COLOR_RED);   //color de los objetivos
    init_pair(5, COLOR_BLACK, COLOR_YELLOW); //color del jugador
    init_pair(6, COLOR_BLACK, COLOR_BLUE);  //color del laberinto
    init_pair(7, COLOR_BLACK, color_from_RGB(3,2,1)); //imprime las cajas

    clear();

    nodelay(stdscr, true);

    int level = 0;
    int m = 0;
    int tm = 0;
    bool quit = false;

    while (!quit){
        switch (level){
            case 0:    mainmenu(quit, level,tm);     break;
            case 1:    level1(quit,level,m,tm);      break;
            case 2:    level2(quit,level,m,tm);      break;
            case 3:    level3(quit,level,m,tm);      break;
            case 4:    level4(quit,level,m,tm);      break;
            case 5:    level5(quit,level,m,tm);      break;
            case 6:    level6(quit,level,m,tm);      break;
            case 7:    level7(quit,level,m,tm);      break;
            case 8:    level8(quit,level,m,tm);      break;
            case 9:    level9(quit,level,m,tm);      break;
            case 10:   level10(quit,level,m,tm);     break;
            case 11:   level11(quit,level,m,tm);     break;
            case 12:   level12(quit,level,m,tm);     break;
            case 13:   level13(quit,level,m,tm);     break;
            case 14:   level14(quit,level,m,tm);     break;
            case 15:   level15(quit,level,m,tm);     break;
            case 16:   level16(quit,level,m,tm);     break;
            case 17:   level17(quit,level,m,tm);     break;
            case 18:   level18(quit,level);     break;
            default:   quit = true;
        }
    }
    endwin();
}
