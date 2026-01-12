#pragma once
#include "interaccion.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

int carrilesdisp(int, int, int);
void getCarrilActual(Coche* co, Vector2D pos);
int posRelativaCalles(Calle*, Calle*);

using namespace std;
/*Funciones de comportamiento de coches en el semaforo*/
void interaccion::cruce(listaCoches lc, listaSemaforos ls, listaCalles lca, grafo* G, double *tiempo, int iteraciones, int filas, int columnas, int optRuta){
	Vector2D aux;
	for(int i=0;i<lc.numero;i++)
		for(int j=0;j<ls.numero;j++){
			aux=lc.lco[i]->posicion-ls.ls[j]->posicion;
			if(aux.modulo()<(ls.ls[j]->size*1.5f))	//circulo para elegir los coches del semaforo
			cruce(lc,i, ls,j,lca,G,tiempo,iteraciones,i,filas,columnas,optRuta);
		}
}
void interaccion::cruce(listaCoches lco, int indexc, listaSemaforos ls, int indexs, listaCalles lca, grafo* G, double *tiempo, int iteraciones,int icoche, int filas, int columnas, int optRuta){
	Coche* c=lco.lco[indexc];
	Semaforo* s=ls.ls[indexs];
	Vector2D aux=c->posicion-s->posicion;
	float auxx=c->posicion.x-s->posicion.x, auxy=c->posicion.y-s->posicion.y;
	//static int n=0;	//carril al que va a ir en el giro
	float dist=aux.modulo();
	int masc=mascara(c,s,dist);
	if(!lco.lco[indexc]->flagdebug){
		cout << "\nEl coche " << indexc << " tiene el caso: " << hex << masc << endl;
		lco.lco[indexc]->flagdebug=true;
	}
	if(c->flagsem==false){
		c->flagsem=interaccion::acercarse(c,s,dist);
	}
	else{ //2048 - 4095
		//2176 - 2303 || 2432 - 2559
		//2560 - 3071 || 3072 - 3199
		//3328 - 3455 || 3584 - 4095
		if(masc>=0x880 && masc <=0x8FF || masc>=0x980 && masc <=0x9FF 
			|| masc>=0xA00 && masc <=0xBFF || masc>=0xC00 && masc <=0xC7F 
			|| masc>=0xD00 && masc <=0xD7F || masc>=0xE00 && masc <=0xFFF){
			c->changeStop(1);	//parar
		}
		//2048 - 2175 || 2304 - 2431
		//3200 - 3327 || 3456 - 3583
		else if(masc>=0x800 && masc <=0x87F || masc>=0x900 && masc <=0x97F 
			|| masc>=0xC80 && masc <=0xCFF || masc>=0xD80 && masc <=0xDFF){
			c->changeStop(0);	//iniciar
			c->an=0.0125f;
		}
		//0 - 127 || 256 - 383
		//1152 - 1279 || 1408 - 1535
		else if(masc>=0x000 && masc <=0x07F || masc>=0x100 && masc <=0x17F 
			|| masc>=0x480 && masc <=0x4FF || masc>=0x580 && masc <=0x5FF){
			c->changeStop(0);	//velocidad x1
			c->an=0.0125f;
		}
		//512 - 639 || 768 - 895
		//1664 - 1791 || 1920 - 2047
		else if(masc>=0x200 && masc <=0x27F || masc>=0x300 && masc <=0x37F 
			|| masc>=0x680 && masc <=0x6FF || masc>=0x780 && masc <=0x7FF){
			c->changeStop(0);	//velocidad x3
			c->vn=3;
			c->an=0.0375f;
		}
		//2048 - 2175 || 2304 - 2431
		//3200 - 3327 || 3456 - 3583
		else if(masc>=0x800 && masc <=0x87F || masc>=0x900 && masc <=0x97F 
			|| masc>=0xC80 && masc <=0xCFF || masc>=0xD80 && masc <=0xDFF){
			//seguir x1
		}
		else if(masc>=0x800 && masc <=0x87F || masc>=0x900 && masc <=0x97F 
			|| masc>=0xC80 && masc <=0xCFF || masc>=0xD80 && masc <=0xDFF){
			//seguir x3
		}
		c->setVelDir();
		int auxrand = rand() % 2;
		int auxn = 0;
		switch(masc){ //palante x1 y x3
			//girar
			case 0x011:	////
			case 0x013:
			case 0x015:
			case 0x017:
			case 0x019:
			case 0x01B:
			case 0x01D:
			case 0x01F:
			case 0x483:
			case 0x487:
			case 0x48B:
			case 0x48F:
			case 0x493:
			case 0x497:
			case 0x49B:
			case 0x49F:	//COTA MENOR
			case 0x105:
			case 0x107:
			case 0x10D:
			case 0x10F:
			case 0x115:
			case 0x117:
			case 0x11D:
			case 0x11F:
			case 0x589:
			case 0x58B:
			case 0x58D:
			case 0x58F:
			case 0x599:
			case 0x59B:
			case 0x59D:
			case 0x59F:	//COTA MAYOR GIRO IZQUIERDA X1 DESPUES DE LLEGAR
			case 0x045:	////
			case 0x047:
			case 0x04D:
			case 0x04F:
			case 0x055:
			case 0x057:
			case 0x05D:
			case 0x05F:	
			case 0x4C9:
			case 0x4CB:
			case 0x4CD:
			case 0x4CF:
			case 0x4D9:
			case 0x4DB:
			case 0x4DD:
			case 0x4DF:	//COTA MENOR
			case 0x151:
			case 0x153:
			case 0x155:
			case 0x157:
			case 0x159:
			case 0x15B:
			case 0x15D:
			case 0x15F:	
			case 0x5C3:
			case 0x5C7:
			case 0x5CB:
			case 0x5CF:
			case 0x5D3:
			case 0x5D7:
			case 0x5DB:
			case 0x5DF:	//COTA MAYOR GIRO DERECHA X1 DESPUES DE LLEGAR
			case 0x211:	////
			case 0x213:
			case 0x215:
			case 0x217:
			case 0x219:
			case 0x21B:
			case 0x21D:
			case 0x21F:
			case 0x683:
			case 0x687:
			case 0x68B:
			case 0x68F:
			case 0x693:
			case 0x697:
			case 0x69B:
			case 0x69F:	//COTA MENOR
			case 0x305:
			case 0x307:
			case 0x30D:
			case 0x30F:
			case 0x315:
			case 0x317:
			case 0x31D:
			case 0x31F:
			case 0x789:
			case 0x78B:
			case 0x78D:
			case 0x78F:
			case 0x799:
			case 0x79B:
			case 0x79D:
			case 0x79F:	//COTA MAYOR GIRO IZQUIERDA X3 DESPUES DE LLEGAR
			case 0x245:	////
			case 0x247:
			case 0x24D:
			case 0x24F:
			case 0x255:
			case 0x257:
			case 0x25D:
			case 0x25F:	
			case 0x6C9:
			case 0x6CB:
			case 0x6CD:
			case 0x6CF:
			case 0x6D9:
			case 0x6DB:
			case 0x6DD:
			case 0x6DF:	//COTA MENOR
			case 0x351:
			case 0x353:
			case 0x355:
			case 0x357:
			case 0x359:
			case 0x35B:
			case 0x35D:
			case 0x35F:	
			case 0x7C3:
			case 0x7C7:
			case 0x7CB:
			case 0x7CF:
			case 0x7D3:
			case 0x7D7:
			case 0x7DB:
			case 0x7FF:	//COTA MAYOR GIRO DERECHA X3 DESPUES DE LLEGAR
				//cout << "\nEl coche: " << indexc << " va a hacer el giro " << c->getGiro() << endl;
				c->girar();
			break;

			
			//cambiar giro
			case 0x002:
			case 0x003:
			case 0x006:
			case 0x007:
			case 0x042:
			case 0x043:
			case 0x052:
			case 0x053:
			case 0x084:
			case 0x085:
			case 0x08C:
			case 0x08D:
			case 0x0C4:
			case 0x0C5:
			case 0x0C6:
			case 0x0C7:
			case 0x108:
			case 0x109:
			case 0x118:
			case 0x119:
			case 0x148:
			case 0x149:
			case 0x14C:
			case 0x14D:
			case 0x190:
			case 0x191:
			case 0x192:
			case 0x193:
			case 0x1D0:
			case 0x1D1:
			case 0x1D8:
			case 0x1D9:

			case 0x202:
			case 0x203:
			case 0x206:
			case 0x207:
			case 0x242:
			case 0x243:
			case 0x252:
			case 0x253:
			case 0x284:
			case 0x285:
			case 0x28C:
			case 0x28D:
			case 0x2C4:
			case 0x2C5:
			case 0x2C6:
			case 0x2C7:
			case 0x308:
			case 0x309:
			case 0x318:
			case 0x319:
			case 0x348:
			case 0x349:
			case 0x34C:
			case 0x34D:
			case 0x390:
			case 0x391:
			case 0x392:
			case 0x393:
			case 0x3D0:
			case 0x3D1:
			case 0x3D8:
			case 0x3D9:

			case 0x402:
			case 0x403:
			case 0x406:
			case 0x407:
			case 0x442:
			case 0x443:
			case 0x452:
			case 0x453:
			case 0x484:
			case 0x485:
			case 0x48C:
			case 0x48D:
			case 0x4C4:
			case 0x4C5:
			case 0x4C6:
			case 0x4C7:
			case 0x508:
			case 0x509:
			case 0x518:
			case 0x519:
			case 0x548:
			case 0x549:
			case 0x54C:
			case 0x54D:
			case 0x590:
			case 0x591:
			case 0x592:
			case 0x593:
			case 0x5D0:
			case 0x5D1:
			case 0x5D8:
			case 0x5D9:

			case 0x602:
			case 0x603:
			case 0x606:
			case 0x607:
			case 0x642:
			case 0x643:
			case 0x652:
			case 0x653:
			case 0x684:
			case 0x685:
			case 0x68C:
			case 0x68D:
			case 0x6C4:
			case 0x6C5:
			case 0x6C6:
			case 0x6C7:
			case 0x708:
			case 0x709:
			case 0x718:
			case 0x719:
			case 0x748:
			case 0x749:
			case 0x74C:
			case 0x74D:
			case 0x790:
			case 0x791:
			case 0x792:
			case 0x793:
			case 0x7D0:
			case 0x7D1:
			case 0x7D8:
			case 0x7D9:
				cout << "\nSe cambia giro de coche: " << indexc << " de " << c->getGiro() << " a 0" << endl;
				c->setGiro(0);
			break;


			case 0x030:
			case 0x031:
			case 0x050:
			case 0x051:
			case 0x0A2:
			case 0x0A3:
			case 0x0C2:
			case 0x0C3:
			case 0x124:
			case 0x125:
			case 0x144:
			case 0x145:
			case 0x1A8:
			case 0x1A9:
			case 0x1C8:
			case 0x1C9:
			
			case 0x230:
			case 0x231:
			case 0x250:
			case 0x251:
			case 0x2A2:
			case 0x2A3:
			case 0x2C2:
			case 0x2C3:
			case 0x324:
			case 0x325:
			case 0x344:
			case 0x345:
			case 0x3A8:
			case 0x3A9:
			case 0x3C8:
			case 0x3C9:

			case 0x430:
			case 0x431:
			case 0x450:
			case 0x451:
			case 0x4A2:
			case 0x4A3:
			case 0x4C2:
			case 0x4C3:
			case 0x524:
			case 0x525:
			case 0x544:
			case 0x545:
			case 0x5A8:
			case 0x5A9:
			case 0x5C8:
			case 0x5C9:

			case 0x630:
			case 0x631:
			case 0x650:
			case 0x651:
			case 0x6A2:
			case 0x6A3:
			case 0x6C2:
			case 0x6C3:
			case 0x724:
			case 0x725:
			case 0x744:
			case 0x745:
			case 0x7A8:
			case 0x7A9:
			case 0x7C8:
			case 0x7C9:
				cout << "\nSe cambia giro de coche: " << indexc << " de " << c->getGiro() << " a -1" << endl;
				c->setGiro(-1);
			break;


			case 0x004:
			case 0x005:
			case 0x024:
			case 0x025:
			case 0x034:
			case 0x035:
			case 0x088:
			case 0x089:
			case 0x0A8:
			case 0x0A9:
			case 0x0AA:
			case 0x0AB:
			case 0x110:
			case 0x111:
			case 0x130:
			case 0x131:
			case 0x134:
			case 0x135:
			case 0x182:
			case 0x183:
			case 0x1A2:
			case 0x1A3:
			case 0x1AA:
			case 0x1AB:

			case 0x204:
			case 0x205:
			case 0x224:
			case 0x225:
			case 0x234:
			case 0x235:
			case 0x288:
			case 0x289:
			case 0x2A8:
			case 0x2A9:
			case 0x2AA:
			case 0x2AB:
			case 0x310:
			case 0x311:
			case 0x330:
			case 0x331:
			case 0x334:
			case 0x335:
			case 0x382:
			case 0x383:
			case 0x3A2:
			case 0x3A3:
			case 0x3AA:
			case 0x3AB:

			case 0x404:
			case 0x405:
			case 0x424:
			case 0x425:
			case 0x434:
			case 0x435:
			case 0x488:
			case 0x489:
			case 0x4A8:
			case 0x4A9:
			case 0x4AA:
			case 0x4AB:
			case 0x510:
			case 0x511:
			case 0x530:
			case 0x531:
			case 0x534:
			case 0x535:
			case 0x582:
			case 0x583:
			case 0x5A2:
			case 0x5A3:
			case 0x5AA:
			case 0x5AB:

			case 0x604:
			case 0x605:
			case 0x624:
			case 0x625:
			case 0x634:
			case 0x635:
			case 0x688:
			case 0x689:
			case 0x6A8:
			case 0x6A9:
			case 0x6AA:
			case 0x6AB:
			case 0x710:
			case 0x711:
			case 0x730:
			case 0x731:
			case 0x734:
			case 0x735:
			case 0x782:
			case 0x783:
			case 0x7A2:
			case 0x7A3:
			case 0x7AA:
			case 0x7AB:
				cout << "\nSe cambia giro de coche: " << indexc << " de " << c->getGiro() << " a 1" << endl;
				c->setGiro(1);
			break;

		/*case 0x010:	////X1
		case 0x012:
		case 0x014:
		case 0x016:
		case 0x018:
		case 0x01A:
		case 0x01C:
		case 0x01E:
		case 0x210:	////X3
		case 0x212:
		case 0x214:
		case 0x216:
		case 0x218:
		case 0x21A:
		case 0x21C:
		case 0x21E:
			carrilesdisp(s->caf[3],c->giro,c->dir);
		break;
		case 0x482:	////X1
		case 0x486:
		case 0x48A:
		case 0x48E:
		case 0x492:
		case 0x496:
		case 0x49A:
		case 0x49E:	
		case 0x682:	////X3
		case 0x686:
		case 0x68A:
		case 0x68E:
		case 0x692:
		case 0x696:
		case 0x69A:
		case 0x69E:	//COTA MENOR
			carrilesdisp(s->caf[0],c->giro,c->dir);
		break;
		case 0x104:	////X1
		case 0x106:
		case 0x10C:
		case 0x10E:
		case 0x114:
		case 0x116:
		case 0x11C:
		case 0x11E:
		case 0x304:	////X3
		case 0x306:
		case 0x30C:
		case 0x30E:
		case 0x314:
		case 0x316:
		case 0x31C:
		case 0x31E:
			carrilesdisp(s->caf[1],c->giro,c->dir);
		break;
		case 0x588:	////X1
		case 0x58A:
		case 0x58C:
		case 0x58E:
		case 0x598:
		case 0x59A:
		case 0x59C:
		case 0x59E:	
		case 0x788:	////X3
		case 0x78A:
		case 0x78C:
		case 0x78E:
		case 0x798:
		case 0x79A:
		case 0x79C:
		case 0x79E:	//COTA MAYOR GIRO IZQUIERDA X1 ANTES DE LLEGAR
			carrilesdisp(s->caf[2],c->giro,c->dir);
		break;
		case 0x011:	////
		case 0x013:
		case 0x015:
		case 0x017:
		case 0x019:
		case 0x01B:
		case 0x01D:
		case 0x01F:
		case 0x483:
		case 0x487:
		case 0x48B:
		case 0x48F:
		case 0x493:
		case 0x497:
		case 0x49B:
		case 0x49F:	//COTA MENOR
		case 0x105:
		case 0x107:
		case 0x10D:
		case 0x10F:
		case 0x115:
		case 0x117:
		case 0x11D:
		case 0x11F:
		case 0x589:
		case 0x58B:
		case 0x58D:
		case 0x58F:
		case 0x599:
		case 0x59B:
		case 0x59D:
		case 0x59F:	//COTA MAYOR GIRO IZQUIERDA X1 DESPUES DE LLEGAR
		case 0x045:	////
		case 0x047:
		case 0x04D:
		case 0x04F:
		case 0x055:
		case 0x057:
		case 0x05D:
		case 0x05F:	
		case 0x4C9:
		case 0x4CB:
		case 0x4CD:
		case 0x4CF:
		case 0x4D9:
		case 0x4DB:
		case 0x4DD:
		case 0x4DF:	//COTA MENOR
		case 0x151:
		case 0x153:
		case 0x155:
		case 0x157:
		case 0x159:
		case 0x15B:
		case 0x15D:
		case 0x15F:	
		case 0x5C3:
		case 0x5C7:
		case 0x5CB:
		case 0x5CF:
		case 0x5D3:
		case 0x5D7:
		case 0x5DB:
		case 0x5DF:	//COTA MAYOR GIRO DERECHA X1 DESPUES DE LLEGAR
		case 0x211:	////
		case 0x213:
		case 0x215:
		case 0x217:
		case 0x219:
		case 0x21B:
		case 0x21D:
		case 0x21F:
		case 0x683:
		case 0x687:
		case 0x68B:
		case 0x68F:
		case 0x693:
		case 0x697:
		case 0x69B:
		case 0x69F:	//COTA MENOR
		case 0x305:
		case 0x307:
		case 0x30D:
		case 0x30F:
		case 0x315:
		case 0x317:
		case 0x31D:
		case 0x31F:
		case 0x789:
		case 0x78B:
		case 0x78D:
		case 0x78F:
		case 0x799:
		case 0x79B:
		case 0x79D:
		case 0x79F:	//COTA MAYOR GIRO IZQUIERDA X3 DESPUES DE LLEGAR
		case 0x245:	////
		case 0x247:
		case 0x24D:
		case 0x24F:
		case 0x255:
		case 0x257:
		case 0x25D:
		case 0x25F:	
		case 0x6C9:
		case 0x6CB:
		case 0x6CD:
		case 0x6CF:
		case 0x6D9:
		case 0x6DB:
		case 0x6DD:
		case 0x6DF:	//COTA MENOR
		case 0x351:
		case 0x353:
		case 0x355:
		case 0x357:
		case 0x359:
		case 0x35B:
		case 0x35D:
		case 0x35F:	
		case 0x7C3:
		case 0x7C7:
		case 0x7CB:
		case 0x7CF:
		case 0x7D3:
		case 0x7D7:
		case 0x7DB:
		case 0x7FF:	//COTA MAYOR GIRO DERECHA X3 DESPUES DE LLEGAR
			c->girar();
		break;
		case 0x022:	////X1
		case 0x026:
		case 0x02A:
		case 0x02E:
		case 0x032:
		case 0x036:
		case 0x03A:
		case 0x03E:
		case 0x4A4:
		case 0x4A6:
		case 0x4AC:
		case 0x4AE:
		case 0x4B4:
		case 0x4B6:
		case 0x4BC:
		case 0x4BE:
		case 0x128:
		case 0x12A:
		case 0x12C:
		case 0x12E:
		case 0x138:
		case 0x13A:
		case 0x13C:
		case 0x13E:
		case 0x5B0:
		case 0x5B2:
		case 0x5B4:
		case 0x5B6:
		case 0x5B8:
		case 0x5BA:
		case 0x5BC:
		case 0x5BE:	//SEGUIR RECTO X1
		case 0x222:	////X3
		case 0x226:
		case 0x22A:
		case 0x22E:
		case 0x232:
		case 0x236:
		case 0x23A:
		case 0x23E:
		case 0x6A4:
		case 0x6A6:
		case 0x6AC:
		case 0x6AE:
		case 0x6B4:
		case 0x6B6:
		case 0x6BC:
		case 0x6BE:
		case 0x328:
		case 0x32A:
		case 0x32C:
		case 0x32E:
		case 0x338:
		case 0x33A:
		case 0x33C:
		case 0x33E:
		case 0x7B0:
		case 0x7B2:
		case 0x7B4:
		case 0x7B6:
		case 0x7B8:
		case 0x7BA:
		case 0x7BC:
		case 0x7BE:
			cambiarCarril(c, s->caf, s->getPos());
		break;
		case 0x044:	////X1
		case 0x046:
		case 0x04C:
		case 0x04E:
		case 0x054:
		case 0x056:
		case 0x05C:
		case 0x05E:
		case 0x244:	////X3
		case 0x246:
		case 0x24C:
		case 0x24E:
		case 0x254:
		case 0x256:
		case 0x25C:
		case 0x25E:
			carrilesdisp(s->caf[1],c->giro,c->dir);
		break;
		case 0x4C8:	////X1
		case 0x4CA:
		case 0x4CC:
		case 0x4CE:
		case 0x4D8:
		case 0x4DA:
		case 0x4DC:
		case 0x4DE:	
		case 0x6C8:	////X3
		case 0x6CA:
		case 0x6CC:
		case 0x6CE:
		case 0x6D8:
		case 0x6DA:
		case 0x6DC:
		case 0x6DE://COTA MENOR
			carrilesdisp(s->caf[2],c->giro,c->dir);
		break;
		case 0x150:	////X1
		case 0x152:
		case 0x154:
		case 0x156:
		case 0x158:
		case 0x15A:
		case 0x15C:
		case 0x15E:
		case 0x350:	////X3
		case 0x352:
		case 0x354:
		case 0x356:
		case 0x358:
		case 0x35A:
		case 0x35C:
		case 0x35E:
			carrilesdisp(s->caf[3],c->giro,c->dir);
		break;
		case 0x5C2:	////X1
		case 0x5C6:
		case 0x5CA:
		case 0x5CE:
		case 0x5D2:
		case 0x5D6:
		case 0x5DA:
		case 0x5DE:	
		case 0x7C2:	////X3
		case 0x7C6:
		case 0x7CA:
		case 0x7CE:
		case 0x7D2:
		case 0x7D6:
		case 0x7DA:
		case 0x7DE://COTA MAYOR GIRO DERECHA X1 ANTES DE LLEGAR
			carrilesdisp(s->caf[0],c->giro,c->dir);
		break;
/*		case 0x002:	////X1
		case 0x003:
		case 0x006:
		case 0x007:
		case 0x00A:
		case 0x00B:
		case 0x00E:
		case 0x00F:
		case 0x042:
		case 0x043:
		case 0x04A:
		case 0x04B:
		case 0x052:
		case 0x053:
		case 0x05A:
		case 0x05B:
		case 0x484:
		case 0x485:
		case 0x48C:
		case 0x48D:
		case 0x494:
		case 0x495:
		case 0x49C:
		case 0x49D:
		case 0x4C4:
		case 0x4C5:
		case 0x4C6:
		case 0x4C7:
		case 0x4D4:
		case 0x4D5:
		case 0x4D6:
		case 0x4D7:
		case 0x108:
		case 0x109:
		case 0x10A:
		case 0x10B:
		case 0x118:
		case 0x119:
		case 0x11A:
		case 0x11B:
		case 0x148:
		case 0x149:
		case 0x14A:
		case 0x14B:
		case 0x14C:
		case 0x14D:
		case 0x14E:
		case 0x14F:
		case 0x590:
		case 0x591:
		case 0x592:
		case 0x593:
		case 0x594:
		case 0x595:
		case 0x596:
		case 0x597:
		case 0x5D0:
		case 0x5D1:
		case 0x5D4:
		case 0x5D5:
		case 0x5D8:
		case 0x5D9:
		case 0x5DC:
		case 0x5DD:	//CAMBIAR GIRO A RECTO
		case 0x202:	////X3
		case 0x203:
		case 0x206:
		case 0x207:
		case 0x20A:
		case 0x20B:
		case 0x20E:
		case 0x20F:
		case 0x242:
		case 0x243:
		case 0x24A:
		case 0x24B:
		case 0x252:
		case 0x253:
		case 0x25A:
		case 0x25B:
		case 0x684:
		case 0x685:
		case 0x68C:
		case 0x68D:
		case 0x694:
		case 0x695:
		case 0x69C:
		case 0x69D:
		case 0x6C4:
		case 0x6C5:
		case 0x6C6:
		case 0x6C7:
		case 0x6D4:
		case 0x6D5:
		case 0x6D6:
		case 0x6D7:
		case 0x308:
		case 0x309:
		case 0x30A:
		case 0x30B:
		case 0x318:
		case 0x319:
		case 0x31A:
		case 0x31B:
		case 0x348:
		case 0x349:
		case 0x34A:
		case 0x34B:
		case 0x34C:
		case 0x34D:
		case 0x34E:
		case 0x34F:
		case 0x790:
		case 0x791:
		case 0x792:
		case 0x793:
		case 0x794:
		case 0x795:
		case 0x796:
		case 0x797:
		case 0x7D0:
		case 0x7D1:
		case 0x7D4:
		case 0x7D5:
		case 0x7D8:
		case 0x7D9:
		case 0x7DC:
		case 0x7DD:
			c->setGiro(0);
		break;
		case 0x030:	////X1
		//case 0x034:
		case 0x038:
		//case 0x03C:
		case 0x4A2:
		//case 0x4AA:
		case 0x4B2:
		//case 0x4BA:
		case 0x124:
		case 0x126:
		//case 0x134:
		//case 0x136:
		case 0x5A8:
		//case 0x5AA:
		case 0x5AC:
		//case 0x5AE:	
		case 0x230:	////X3
		//case 0x234:
		case 0x238:
		//case 0x23C:
		case 0x6A2:
		//case 0x6AA:
		case 0x6B2:
		//case 0x6BA:
		case 0x324:
		case 0x326:
		//case 0x334:
		//case 0x336:
		case 0x7A8:
		//case 0x7AA:
		case 0x7AC:
		//case 0x7AE://CAMBIAR GIRO A IZQUIERDA	SE PRIORIZA EL GIRO A DERECHA
			if (auxrand)
				c->setGiro(-1);
			else
				c->setGiro(1);
			auxn++;
			cout << "\n////////////////Ahora el coche " << indexc << " tiene el giro " << c->getGiro()<<" se han hecho "<<auxn<<" cambios";
		break;
		case 0x024:	////X1
		case 0x02C:
		case 0x034:
		case 0x03C:
		case 0x4A8:
		case 0x4AA:
		case 0x4B8:
		case 0x4BA:
		case 0x130:
		case 0x132:
		case 0x134:
		case 0x136:
		case 0x5A2:
		case 0x5A6:
		case 0x5AA:
		case 0x5AE:	
		case 0x224:	////X3
		case 0x22C:
		case 0x234:
		case 0x23C:
		case 0x6A8:
		case 0x6AA:
		case 0x6B8:
		case 0x6BA:
		case 0x330:
		case 0x332:
		case 0x334:
		case 0x336:
		case 0x7A2:
		case 0x7A6:
		case 0x7AA:
		case 0x7AE:	//CAMBIAR GIRO A DERECHA
			c->setGiro(1);
		break;*/
		/*


		case 0x002:
		case 0x003:
		case 0x006:
		case 0x007:
		case 0x042:
		case 0x043:
		case 0x052:
		case 0x053:
		case 0x084:
		case 0x085:
		case 0x08C:
		case 0x08D:
		case 0x0C4:
		case 0x0C5:
		case 0x0C6:
		case 0x0C7:
		case 0x008:
		case 0x009:
		case 0x018:
		case 0x019:
		case 0x048:
		case 0x049:
		case 0x04C:
		case 0x04D:
		case 0x090:
		case 0x091:
		case 0x092:
		case 0x093:
		case 0x0D0:
		case 0x0D1:
		case 0x0D8:
		case 0x0D9:

		case 0x102:
		case 0x103:
		case 0x106:
		case 0x107:*/
		}
		
		if(masc>=0x800){	//area grande ambar
			if(c->flagsem=interaccion::acercarse(c,s,dist)){
				/*if(s->luz==0 && c->dir%2==0 || s->luz==2 && c->dir%2){
					c->changeStop(0);
					c->vn=1;	//luz en verde
					
				}
				else 	c->changeStop(1);	//luz en ambar o rojo*/
			}
			/////////////////////////////////////////////////////////
			/////////////////////////////////////////////////////////
			else {	c->changeStop(0);
				//c->vn=1;
				//cout<<"\nel proximo giro es: "<<c->ruta[c->igiros];
				c->igiros++;
				////esto estaba al final
				c->setPosSalida(s->posicion, s->caf);
				if (c->igiros == c->ngiros) {	//se ha completado la ruta
					int ngiros = c->ngiros;
					cout << "\n\tSE HA COMPLETADO LA RUTA\n";
					delete[] c->ruta;
					cout << "\nse ha eliminado la ruta previa\n";
					temporal(&c->tiemporutaini, &c->tiemporutafin, tiempo, iteraciones, ngiros, true);
					ocupacionCalle(lco, lca);	//actualizar pesos variables en calles
					G->crearGrafo(filas, columnas, lca, ls, false);	//actualizar pesos variables en grafos
					setRuta(c, G, getCalleindex(c, lca), lca, icoche, optRuta);
					if (optRuta != 2) temporal(&c->tiemporutaini, &c->tiemporutafin, tiempo, iteraciones, ngiros, false);
				}
				////
				c->giro = c->ruta[c->igiros];//((c->igiros==c->ngiros)?0:c->igiros)];	//decidir proximo giro
				c->flaggiro=false;
				cout << "\nEl coche " << indexc << "\t tiene el giro " << c->getGiro() << "\t y la direccion " << c->getDir();//cambiar quitar esto, es para ver donde falla
				if (c->getGiro() >= 2 || c->getDir() >= 4|| c->getGiro() <=-2 || c->getDir() <=-1) {
					cout << "algo falla\t\tGiro: " <<c->getGiro() <<"\t\tDir: "<<c->getDir();
					int auxx;
					cin >> auxx;
				}////////
				
				/*if(c->dir%2) c->setPos(c->posicion.x,s->posicion.y+3*(2-c->n[1]/2));
				else c->setPos(s->posicion.x+3*(2-c->n[1]/2), c->posicion.y);*/
			}
			c->setVelDir();
		}
	}
}


/////////////////////////////////////
	
	
bool interaccion::acercarse(Coche *c, Semaforo *s, float dist){
	if(dist<1.5f*s->size)
	switch(c->dir){
	case 0:			//arriba
		if((dist=s->posicion.y - c->posicion.y) < 1.5f*s->size && dist >0)
			return true;
		else return false;
		break;
	case 1:			//derecha
		if((dist=s->posicion.x - c->posicion.x) < 1.5f*s->size && dist >0)
			return true;
		else return false;
		break;
	case 2:			//abajo
		if((dist=c->posicion.y - s->posicion.y) < 1.5f*s->size && dist >0)
			return true;
		else return false;
		break;
	case 3:			//izquierda
		if((dist=c->posicion.x - s->posicion.x) < 1.5f*s->size && dist >0)
			return true;
		else return false;
		break;
	default:
		cout<<"\n\tERROR DIRECCION EN FUNCION acercarse(Coche, Semaforo, dist)"; 
		return -1;
		break;
	}
	else return false;
}
////////////////////////////////////////////////////////

/*Funciones para crear coches de forma aleatoria en los cruces*/
void interaccion::crearcoche(listaCoches *lco, listaCalles lcall){
	for(int i=0;i<lcall.numero;i++)
		if(!lcall.lca[i]->getExtremo())
			interaccion::crearcoche(lcall.lca[i], lco);
}
void interaccion::crearcoche(Calle *ca, listaCoches *lco){
	for(int i=0;i<ca->carriles;i++){
		int aux=rand()%3;
		if(!aux)			//crear coche
		switch(ca->tipo){
		case 1:	//1 carril
			lco->agregar(new Coche(ca->posicion, ca->direccion));
			break;
		case 2:	//1 carril
			lco->agregar(new Coche(ca->posicion, ca->direccion+2));
			break;
		case 3:	//2 carriles
			lco->agregar(new Coche({ca->posicion.x+(ca->direccion-1)*((i-0.5f)*ca->anchura),ca->posicion.y+ca->direccion*((i-0.5f)*ca->anchura)}, ca->direccion));
			break;
		case 4:	//2 carriles
			lco->agregar(new Coche({ca->posicion.x+(ca->direccion-1)*((i-0.5f)*ca->anchura),ca->posicion.y+ca->direccion*((i-0.5f)*ca->anchura)}, ca->direccion+2));
			break;
		case 5:	//2 carriles
			lco->agregar(new Coche({ca->posicion.x+(ca->direccion-1)*((i-0.5f)*ca->anchura),ca->posicion.y+ca->direccion*((i-0.5f)*ca->anchura)}, ca->direccion+2*i));
			break;
		case 6:	//3 carriles
			lco->agregar(new Coche({ca->posicion.x+(ca->direccion-1)*((i-1)*ca->anchura),ca->posicion.y+ca->direccion*((i-1)*ca->anchura)}, ca->direccion));
			break;
		case 7:	//3 carriles
			lco->agregar(new Coche({ca->posicion.x+(ca->direccion-1)*((i-1)*ca->anchura),ca->posicion.y+ca->direccion*((i-1)*ca->anchura)}, ca->direccion+2));
			break;
		case 8:	//3 carriles
			lco->agregar(new Coche({ca->posicion.x+(ca->direccion-1)*((i-1)*ca->anchura),ca->posicion.y+ca->direccion*((i-1)*ca->anchura)}, (i<2?(ca->direccion):(ca->direccion+2))));
			break;
		case 9:	//3 carriles
			lco->agregar(new Coche({ca->posicion.x+(ca->direccion-1)*((i-1)*ca->anchura),ca->posicion.y+ca->direccion*((i-1)*ca->anchura)}, (i<1?(ca->direccion):(ca->direccion+2))));
			break;
		case 10:	//4 carriles
			lco->agregar(new Coche({ca->posicion.x+(ca->direccion-1)*((i-1.5f)*ca->anchura),ca->posicion.y+ca->direccion*((i-1.5f)*ca->anchura)}, ca->direccion));
			break;
		case 11:	//4 carriles
			lco->agregar(new Coche({ca->posicion.x+(ca->direccion-1)*((i-1.5f)*ca->anchura),ca->posicion.y+ca->direccion*((i-1.5f)*ca->anchura)}, ca->direccion+2));
			break;
		case 12:	//4 carriles
			lco->agregar(new Coche({ca->posicion.x+(ca->direccion-1)*((i-1.5f)*ca->anchura),ca->posicion.y+ca->direccion*((i-1.5f)*ca->anchura)}, (i<3?(ca->direccion):(ca->direccion+2))));
			break;
		case 13:	//4 carriles
			lco->agregar(new Coche({ca->posicion.x+(ca->direccion-1)*((i-1.5f)*ca->anchura),ca->posicion.y+ca->direccion*((i-1.5f)*ca->anchura)}, (i<2?(ca->direccion):(ca->direccion+2))));
			break;
		case 14:	//4 carriles
			lco->agregar(new Coche({ca->posicion.x+(ca->direccion-1)*((i-1.5f)*ca->anchura),ca->posicion.y+ca->direccion*((i-1.5f)*ca->anchura)}, (i<1?(ca->direccion):(ca->direccion+2))));
			break;
		}
	}
}
void interaccion::destruircoche(listaCoches lco, int aux){
	for(int i=lco.numero; i>0; i--){
		if(rand()%aux==0){
			lco.eliminarindex(i);
			i--;
		}
	}
}
////////////////////////////////////////////////////////////////

/*Funcion para que los coches aparezcan por los otros lados del mundo*/
void interaccion::salirMundo(listaCoches lco, int fil, int col, int lon){
	for(int i=0;i<lco.numero;i++)
		interaccion::salirMundo(lco.lco[i], fil, col, lon);
}
void interaccion::salirMundo(Coche* co, int fil, int col, int lon){
	Vector2D max={(float)col*lon, (float)fil*lon};
	switch(co->dir){
	case 0:
		if(co->posicion.y>max.y) co->posicion.y=-lon;
		break;
	case 1:
		if(co->posicion.x>max.x) co->posicion.x=-lon;
		break;
	case 2:
		if(co->posicion.y<-lon) co->posicion.y=max.y;
		break;
	case 3:
		if(co->posicion.x<-lon) co->posicion.x=max.x;
		break;
	}
}
///////////////////////////////////////////////////////////////////////
int interaccion::dSeg(Coche *c, int v, const char* x){
	c->vn=v;
	c->setVelDir();
		c->r=0.5f;
	return c->vn;
}
/*Funciones para que los coches se paren cuando haya uno delante y evitar choque frontal en la misma direccion*/
void interaccion::distSeg(listaCoches lco){
	for(int i=0;i<lco.numero-1;i++)
	for(int j=i+1;j<lco.numero;j++){	//j>i para que no se repitan combinatorias
		Vector2D aux=lco.lco[i]->getPos()-lco.lco[j]->getPos();
		if(aux.modulo()<8 && lco.lco[i]->getDir()==lco.lco[j]->getDir())	
			interaccion::distSeg(lco,i,j,aux);

	}
}

void interaccion::distSeg(listaCoches lco, int i, int j, Vector2D aux){
	//if(lco.lco[i]->flagsem==lco.lco[j]->flagsem==0){//para que solo funcione fuera de los semaforos, mirar si falla justo en la salida
	int auxx=0;
	switch(lco.lco[i]->getDir()){
	case 0:		//arriba
		//if(sqrt(pow(aux.x,2))<0.5f){
			if(aux.y>=0 && aux.y<6  && !lco.lco[j]->getStop() && !lco.lco[j]->flagsem){	//c1 delante y c2 avanzando
				lco.lco[j]->changeStop(1);	//parar c2
				lco.lco[j]->setflagLateral(1);
				break;
			}
			else if(aux.y>6 && lco.lco[j]->getStop() && !lco.lco[i]->getStop()){	//c1 delante y c2 parado
				lco.lco[j]->changeStop(0);	//iniciar c2
				lco.lco[j]->setflagLateral(0);
				break;
			}
			else if(aux.y<=0 && aux.y>-6 && !lco.lco[i]->getStop() && !lco.lco[i]->flagsem){	//c2 delante y c1 avanzando
				lco.lco[i]->changeStop(1);	//parar c1
				lco.lco[i]->setflagLateral(1);
				break;
			}
			else if(aux.y<-6 && lco.lco[i]->getStop() && !lco.lco[j]->getStop()){	//c2 delante y c1 parado
				lco.lco[i]->changeStop(0);	//iniciar c1
				lco.lco[i]->setflagLateral(0);
				break;
			}
		//}		
	break;
	case 1:		//derecha
		//if(sqrt(pow(aux.y,2))<0.5f){
			if(aux.x>=0 && aux.x<6 && !lco.lco[j]->getStop() && !lco.lco[j]->flagsem){	//c1 delante y c2 avanzando
				lco.lco[j]->changeStop(1);	//parar c2
				lco.lco[j]->setflagLateral(1);
				break;
			}
			else if(aux.x>6 && lco.lco[j]->getStop() && !lco.lco[i]->getStop()){	//c1 delante y c2 parado
				lco.lco[j]->changeStop(0);	//iniciar c2
				lco.lco[j]->setflagLateral(0);
				break;
			}
			else if(aux.x<=0 && aux.x>-6 && !lco.lco[i]->getStop() && !lco.lco[i]->flagsem){	//c2 delante y c1 avanzando
				lco.lco[i]->changeStop(1);	//parar c1
				lco.lco[i]->setflagLateral(1);
				break;
			}
			else if(aux.x<-6 && lco.lco[i]->getStop() && !lco.lco[j]->getStop()){	//c2 delante y c1 parado
				lco.lco[i]->changeStop(0);	//iniciar c1
				lco.lco[i]->setflagLateral(0);
				break;
			}
			
		//}
	break;
	case 2:		//abajo
		//if(sqrt(pow(aux.x,2))<0.5f){
			if(aux.y>=0 && aux.y<6 && !lco.lco[i]->getStop() && !lco.lco[i]->flagsem){	//c2 delante y c1 avanzando
				lco.lco[i]->changeStop(1);	//parar c1
				lco.lco[i]->setflagLateral(1);
				break;
			}
			else if(aux.y>6 && lco.lco[i]->getStop() && !lco.lco[j]->getStop()){	//c2 delante y c1 parado
				lco.lco[i]->changeStop(0);	//iniciar c1
				//cout<<"\ncaso1";
				lco.lco[i]->setflagLateral(0);
				break;
			}
			else if(aux.y<=0 && aux.y>-6 && !lco.lco[j]->getStop() && !lco.lco[j]->flagsem){	//c1 delante y c2 avanzando
				lco.lco[j]->changeStop(1);	//parar c2
				lco.lco[j]->setflagLateral(1);
				break;
			}
			else if(aux.y<-6 && lco.lco[j]->getStop() && !lco.lco[i]->getStop()){	//c1 delante y c2 parado
				lco.lco[j]->changeStop(0);	//iniciar c2
				//cout<<"\ncaso2";
				lco.lco[j]->setflagLateral(0);
				break;
			}
		//}
	break;
	case 3:		//izquierda
		//if(sqrt(pow(aux.y,2))<0.5f){
			if(aux.x>=0 && aux.x<6 && !lco.lco[i]->getStop() && !lco.lco[i]->flagsem){	//c2 delante y c1 avanzando
				lco.lco[i]->changeStop(1);	//parar c1
				lco.lco[i]->setflagLateral(1);
				break;
			}
			else if(aux.x>6 && lco.lco[i]->getStop() && !lco.lco[j]->getStop()){	//c2 delante y c1 parado
				lco.lco[i]->changeStop(0);	//iniciar c1
				//cout<<"\ncaso1";
				lco.lco[i]->setflagLateral(0);
				break;
			}
			else if(aux.x<=0 && aux.x>-6 && !lco.lco[j]->getStop() && !lco.lco[j]->flagsem){	//c1 delante y c2 avanzando
				lco.lco[j]->changeStop(1);	//parar c2
				lco.lco[j]->setflagLateral(1);
				break;
			}
			else if(aux.x<-6 && lco.lco[j]->getStop() && !lco.lco[i]->getStop()){	//c1 delante y c2 parado
				lco.lco[j]->changeStop(0);	//iniciar c2
				//cout<<"\ncaso2";
				lco.lco[j]->setflagLateral(0);
				break;
			}
		//}
	break;
	}
	//if(auxx){cout<<"error pocho3:";
	//cin>>auxx;}
	//}
}

////////////////////////////////////////////////////////////////////////////////////
/*Funciones para saber cuantas calles hay en cada direccion por semaforo*/
void interaccion::callesem(listaCalles lc, listaSemaforos ls){
	Vector2D aux;
	for(int i =0;i<lc.numero;i++)
		for(int j=0;j<ls.numero;j++){
			aux=lc.lca[i]->posicion-ls.ls[j]->posicion;
			if(aux.modulo()<(lc.lca[i]->longitud*1.1f))	//circulo para elegir las calles del semaforo
			callesem(lc.lca[i], ls.ls[j], i);
		}
}
void interaccion::callesem(Calle* c, Semaforo* s, int id){
	if(sqrt(pow(s->posicion.x-c->posicion.x,2))<1 && s->posicion.y<c->posicion.y)//calle arriba
	{
		if(c->tipo==2 || c->tipo==4 || c->tipo==7 || c->tipo==11)
			s->salida[0]=false;
		else s->salida[0]=true;
		s->caf[0]=c->tipo;
		s->cid[0]=id;
	}
	else if(sqrt(pow(s->posicion.x-c->posicion.x,2))<1 && s->posicion.y>c->posicion.y)//calle abajo 
	{
		if(c->tipo==1 || c->tipo==3 || c->tipo==6 || c->tipo==10)
			s->salida[2]=false;
		else s->salida[2]=true;
		s->caf[2]=c->tipo;
		s->cid[2]=id;
	}
	else if(sqrt(pow(s->posicion.y-c->posicion.y,2))<1 && s->posicion.x<c->posicion.x)//calle derecha
	{
		if(c->tipo==2 || c->tipo==4 || c->tipo==7 || c->tipo==11)
			s->salida[1]=false;
		else s->salida[1]=true;
		s->caf[1]=c->tipo;
		s->cid[1]=id;
	}
	else if(sqrt(pow(s->posicion.y-c->posicion.y,2))<1 && s->posicion.x>c->posicion.x)//calle izquierda
	{ 
		if(c->tipo==1 || c->tipo==3 || c->tipo==6 || c->tipo==10)
			s->salida[3]=false;
		else s->salida[3]=true;
		s->caf[3]=c->tipo;
		s->cid[3]=id;
	}
	else { static int count=0;
	count++;
	cout<<"algo va mal en lo de callesem"<<count;
	c->setColor(1,0,0);
	c->anchura/=2;
	}
	
}
int carrilesdisp(int tipo, int giro, int dir){	//devuelve el carril por el que ir en funcion del tipo de calle, del giro que hace el coche y su direccion inicial
	return 4;/*
	static int n=0;
	switch(tipo){
	case 1:
	case 2:
		return 4;
	break;
	case 3:
		return (giro<0?(5):(3));
	break;
	case 4:
		return (giro<0?(3):(5));
	break;
	case 5:
		return ((dir==0||dir==3)?(giro<0?5:3):(giro<0?3:5));
	break;
	case 6:
		return (giro<0?(6):(2));
	break;
	case 7:
		return (giro<0?(2):(6));
	break;
	case 8:
		switch(dir){
		case 0:
			n=giro<0?(6):(2);
		break;
		case 1:
			n=giro<0?(4):(6);
		break;
		case 2:
			n=giro<0?(4):(6);
		break;
		case 3:
			n=giro<0?(2):(6);
		break;
		}
		return n;
	break;
	case 9:
		switch(dir){
		case 0:
			n=giro<0?(4):(2);
		break;
		case 1:
			n=giro<0?(2):(6);
		break;
		case 2:
			n=giro<0?(6):(2);
		break;
		case 3:
			n=giro<0?(4):(2);
		break;
		}
		return n;
	break;
	case 10:
		return giro<0?(7):(1);
	break;
	case 11:
		return giro<0?(1):(7);
	break;
	case 12:
		switch(dir){
		case 0:
			n=giro<0?(7):(1);
		break;
		case 1:
			n=giro<0?(5):(7);
		break;
		case 2:
			n=giro<0?(5):(7);
		break;
		case 3:
			n=giro<0?(7):(1);
		break;
		}
		return n;
	break;
	case 13:
		switch(dir){
		case 0:
			n=giro<0?(5):(1);
		break;
		case 1:
			n=giro<0?(3):(7);
		break;
		case 2:
			n=giro<0?(3):(7);
		break;
		case 3:
			n=giro<0?(5):(1);
		break;
		}
		return n;
	break;
	case 14:
		switch(dir){
		case 0:
			n=giro<0?(3):(1);
		break;
		case 1:
			n=giro<0?(1):(7);
		break;
		case 2:
			n=giro<0?(7):(1);
		break;
		case 3:
			n=giro<0?(3):(1);
		break;
		}
		return n;
	break;
	default:
		cout<<"\n\tERROR FUNCION carrilesDisp(tipo, giro, dir)";
		return -1;
	break;
	}*/
}
//////////////////////////////////////////////////////////////////////////

/*funciones para el desplazamiento lateral*/
void interaccion::cambiarCarril(listaCoches lco, listaCalles lca){
	for(int i=0;i<lco.numero;i++)
		for(int j=0;j<lca.numero;j++){
			Vector2D aux=lco.lco[i]->posicion-lca.lca[j]->posicion;
			if(aux.modulo()<(lca.lca[j]->longitud/2)&&!lco.lco[i]->flagsem){	//la calle en la que esta el coche y que no este en el semaforo
				cambiarCarril(lco.lco[i], lca.lca[j]);
				break;	//sale del bucle for(j...) para ahorrar memoria
			}
		}
}
void interaccion::cambiarCarril(Coche* co, Calle* ca){
		co->n[1]=4;
	/*
	getCarrilActual(co, ca->getPos());	//obtener el carril inicial en el que se encuentra el coche
	
	switch(ca->getTipo()){		//elegir el carril de destino en funcion de la direccion del coche, el giro que vaya a hacer en el proximo cruce y el tipo de calle, se llama desde cambiarCarril en la misma calle
	case 1:
		co->n[1]=4;
	break;
	case 2:
		co->n[1]=4;
	break;
	case 3:
		switch(co->getGiro()){
		case -1:
			co->n[1]=5;
		break;
		case 0:
			co->n[1]=co->n[0];	//cambiar si hay mucho trafico en un carril -futuro
		break;
		case 1:
			co->n[1]=3;
		break;
		}
	break;
	case 4:
		switch(co->getGiro()){
		case -1:
			co->n[1]=3;
		break;
		case 0:
			co->n[1]=co->n[0];	//cambiar si hay mucho trafico en un carril -futuro
		break;
		case 1:
			co->n[1]=5;
		break;
		}
	break;
	case 5:
		co->n[1]=(co->getDir()<2)?(3):(5);
	break;
	case 6:
		switch(co->getGiro()){
		case -1:
			co->n[1]=6;
		break;
		case 0:
			co->n[1]=4;
		break;
		case 1:
			co->n[1]=2;
		break;
		}
	break;
	case 7:
		switch(co->getGiro()){
		case -1:
			co->n[1]=2;
		break;
		case 0:
			co->n[1]=4;
		break;
		case 1:
			co->n[1]=6;
		break;
		}
	break;
	case 8:
		switch(co->getGiro()){
		case -1:
			co->n[1]=(co->getDir()<2)?(4):(6);
		break;
		case 0:
			co->n[1]=co->n[0];	//cambiar si hay mucho trafico en un carril -futuro
		break;
		case 1:
			co->n[1]=(co->getDir()<2)?(2):(6);
		break;
		}
	break;
	case 9:
		switch(co->getGiro()){
		case -1:
			co->n[1]=(co->getDir()<2)?(2):(4);
		break;
		case 0:
			co->n[1]=co->n[0];	//cambiar si hay mucho trafico en un carril -futuro
		break;
		case 1:
			co->n[1]=(co->getDir()<2)?(2):(6);
		break;
		}
	break;
	case 10:
		switch(co->getGiro()){
		case -1:
			co->n[1]=7;
		break;
		case 0:
			co->n[1]=3;//o 5	//cambiar si hay mucho trafico en un carril -futuro
		break;
		case 1:
			co->n[1]=1;
		break;
		}
	break;
	case 11:
		switch(co->getGiro()){
		case -1:
			co->n[1]=1;
		break;
		case 0:
			co->n[1]=3;//o 5	//cambiar si hay mucho trafico en un carril -futuro
		break;
		case 1:
			co->n[1]=7;
		break;
		}
	break;
	case 12:
		switch(co->getGiro()){
		case -1:
			co->n[1]=(co->getDir()<2)?(5):(7);
		break;
		case 0:
			co->n[1]=co->n[0];	//cambiar si hay mucho trafico en un carril -futuro
		break;
		case 1:
			co->n[1]=(co->getDir()<2)?(1):(7);
		break;
		}
	break;
	case 13:
		switch(co->getGiro()){
		case -1:
			co->n[1]=(co->getDir()<2)?(3):(5);
		break;
		case 0:
			co->n[1]=co->n[0];	//cambiar si hay mucho trafico en un carril -futuro
		break;
		case 1:
			co->n[1]=(co->getDir()<2)?(1):(7);
		break;
		}
	break;
	case 14:
		switch(co->getGiro()){
		case -1:
			co->n[1]=(co->getDir()<2)?(1):(3);
		break;
		case 0:
			co->n[1]=co->n[0];	//cambiar si hay mucho trafico en un carril -futuro
		break;
		case 1:
			co->n[1]=(co->getDir()<2)?(1):(7);
		break;
		}
	break;
	default:
		cout<<"\nError en la seleccion de tipo de calle destino\t";
	break;
	}*/
	
	
}
void interaccion::cambiarCarril(Coche* c, int caf[4], Vector2D pos){	//en funcion de los tipos de las calles afectadas
		c->n[1]=4;/*
	getCarrilActual(c, pos);	//obtener el carril en el que esta el coche antes del cruce
	
	switch(caf[c->getDir()]){	//los tipos de la calle destino
	case 1:
		c->n[1]=4;
	break;
	case 2:
		c->n[1]=4;
	break;
	case 3:
		c->n[1]=3;
	break;
	case 4:
		c->n[1]=5;
	break;
	case 5:
		c->n[1]=(c->getDir()<2)?(3):(5);
	break;
	case 6:
		c->n[1]=2;
	break;
	case 7:
		c->n[1]=6;
	break;
	case 8:
		c->n[1]=(c->getDir()<2)?(2):(6);
	break;
	case 9:
		c->n[1]=(c->getDir()<2)?(2):(6);
	break;
	case 10:
		c->n[1]=3;
	break;
	case 11:
		c->n[1]=5;
	break;
	case 12:
		c->n[1]=(c->getDir()<2)?(3):(7);
	break;
	case 13:
		c->n[1]=(c->getDir()<2)?(3):(5);
	break;
	case 14:
		c->n[1]=(c->getDir()<2)?(1):(5);
	break;
	default:
		cout<<"\nError en la seleccion de tipo de calle destino\t";
	break;
	}*/
	
}
void getCarrilActual(Coche* co, Vector2D pos){
				co->n[0]=4;/*
	switch(co->getDir()){
	case 0:	//vertical
	case 2:
		///////esto solo al inicio
		if(!co->getflagLateral()){
			if((co->getPos().x-pos.x)<-3.75){		//carril 7
				co->n[0]=7;
			}
			else if((co->getPos().x-pos.x)<-2.25){	//carril 6
				co->n[0]=6;
			}
			else if((co->getPos().x-pos.x)<-0.75){	//carril 5
				co->n[0]=5;
			}
			else if((co->getPos().x-pos.x)<0.75){	//carril 4
				co->n[0]=4;
			}
			else if((co->getPos().x-pos.x)<2.25){	//carril 3
				co->n[0]=3;
			}
			else if((co->getPos().x-pos.x)<3.75){	//carril 2
				co->n[0]=2;
			}
			else {							//carril 1
				co->n[0]=1;
			}
			co->setflagLateral(true);
		}
		else{
			
			co->despLat(co->n[1]-co->n[0]);
			if((co->getPos().x-pos.x)<-4.45&&(co->getPos().x-pos.x)>-4.55){		//carril 7
				co->n[0]=7;
			}
			else if((co->getPos().x-pos.x)<-2.95&&(co->getPos().x-pos.x)>-3.05){		//carril 6
				co->n[0]=6;
			}
			else if((co->getPos().x-pos.x)<-1.45&&(co->getPos().x-pos.x)>-1.55){		//carril 5
				co->n[0]=5;
			}
			else if((co->getPos().x-pos.x)<0.05&&(co->getPos().x-pos.x)>-0.05){		//carril 4
				co->n[0]=4;
			}
			else if((co->getPos().x-pos.x)<1.55&&(co->getPos().x-pos.x)>1.45){		//carril 3
				co->n[0]=3;
			}
			else if((co->getPos().x-pos.x)<3.05&&(co->getPos().x-pos.x)>2.95){		//carril 2
				co->n[0]=2;
			}
			else if((co->getPos().x-pos.x)<4.55&&(co->getPos().x-pos.x)>4.45){		//carril 1
				co->n[0]=1;
			}
			if(co->n[0]==co->n[1]){	//ha llegado a su carril
				co->setflagLateral(false);	
				//co->setPos(pos.x+6-1.5f*co->n[0],co->getPos().y);
			}
		}
	break;
	case 1:	//horizontal
	case 3:
		///////esto solo al inicio
		if(!co->getflagLateral()){
			if((co->getPos().y-pos.y)<-3.75){		//carril 1
				co->n[0]=1;
			}
			else if((co->getPos().y-pos.y)<-2.25){	//carril 2
				co->n[0]=2;
			}
			else if((co->getPos().y-pos.y)<-0.75){	//carril 3
				co->n[0]=3;
			}
			else if((co->getPos().y-pos.y)<0.75){	//carril 4
				co->n[0]=4;
			}
			else if((co->getPos().y-pos.y)<2.25){	//carril 5
				co->n[0]=5;
			}
			else if((co->getPos().y-pos.y)<3.75){	//carril 6
				co->n[0]=6;
			}
			else {							//carril 7
				co->n[0]=7;
			}
			co->setflagLateral(true);
		}
		else{
			co->despLat(co->n[1]-co->n[0]);
			if((co->getPos().y-pos.y)<-4.45&&(co->getPos().y-pos.y)>-4.55){		//carril 1
				co->n[0]=1;
			}
			else if((co->getPos().y-pos.y)<-2.95&&(co->getPos().y-pos.y)>-3.05){		//carril 2
				co->n[0]=2;
			}
			else if((co->getPos().y-pos.y)<-1.45&&(co->getPos().y-pos.y)>-1.55){		//carril 3
				co->n[0]=3;
			}
			else if((co->getPos().y-pos.y)<0.05&&(co->getPos().y-pos.y)>-0.05){		//carril 4
				co->n[0]=4;
			}
			else if((co->getPos().y-pos.y)<1.55&&(co->getPos().y-pos.y)>1.45){		//carril 5
				co->n[0]=5;
			}
			else if((co->getPos().y-pos.y)<3.05&&(co->getPos().y-pos.y)>2.95){		//carril 6
				co->n[0]=6;
			}
			else if((co->getPos().y-pos.y)<4.55&&(co->getPos().y-pos.y)>4.45){		//carril 7
				co->n[0]=7;
			}
			if(co->n[0]==co->n[1]){	//ha llegado a su carril
				co->setflagLateral(false);
				//co->setPos(co->getPos().x,pos.y-6+1.5f*co->n[0]);
			}
		}
	break;
	}*/

}

/////////////////////////////////////////////
/*funciones para la seleccin de ruta y el comportamiento de los giros*/
void interaccion::setRuta(listaCoches lco, grafo* G, listaCalles lca, int optRuta){
	for(int i=0;i<lco.numero;i++)
		for(int j=0;j<lca.numero;j++){
			Vector2D aux=lco.lco[i]->posicion-lca.lca[j]->posicion;//calle inicio
			if(aux.modulo()<lca.lca[j]->getLong()/1.95f){//cambiar si las calles tienen longitudes diferentes
				cout<<"\nentra en el if de setruta "<<i<<" "<<j<<" "<<optRuta;
				setRuta(lco.lco[i],G,j,lca,i,optRuta);
				cout<<"\nsale de setRuta";
				break;	//sale del bucle for(j...) para ahorrar memoria
			}
		}
}
void interaccion::setRuta(Coche* co, grafo* G, int ind, listaCalles lca, int icoche, int optRuta){
	cout<<"\n\tENTRA EN SETRUTA";
	int ini=-1;
	co->igiros=0;
	switch(optRuta){
	case 0:
		co->ngiros=rand()%3+3;	//cambiar media de giros
		co->ruta = new int[co->ngiros];//crear un vector para guardar los giros que debe hacer el coche hasta llegar al destino
		for (int i = 0; i < co->ngiros; i++)
			co->ruta[i]=rand()%3-1;	//giro aleatorio, puede no ser como en la realidad por las calles de salida disponibles, pero se cambia automáticamente en la función de cruce
		cout << "\n\tel coche " << icoche << " tiene que hacer " << co->ngiros << " giros";
		for (int i = 0; i < co->ngiros; i++)
			cout << "\ngiro " << i << ": " << co->ruta[i];
		cout<<"\n\n\tFIN DE LA ELECCION DE RUTA\n";
				
	break;
	case 1:	//Dijkstra
	{
		for(int i=0;i<G->nverticesconj;i++)	//cambiar mirar si se puede poner un map en vez
			if(G->verticeconj[i].index==ind){
				ini=i;
				break;
			}
		//se actualizan los pesos varibles de las aristas

		Algoritmo::Dijkstra(ini, G,false);		//hallar la ruta mas corta hasta todas las calles
		do{
			G->fin=rand()%G->nverticesconj;	//elegir el fin de la ruta
		} while(G->fin==ini);			//comprobar que el inicio y el fin son distintos
		if(G->fin == ini) {
			int i=0;
			cout<<"******************** Inicio y fin iguales ********************"<<endl;
			cin >> i;
		}
		int aux=G->fin;
		cout<<"\n\tel destino es el vertice: "<<G->fin;
		co->ngiros=G->verticeconj[G->fin].dv.iter;
		cout<<"\n\tel coche "<<icoche<<" tiene que hacer "<<co->ngiros<<" giros";
		co->ruta=new int[co->ngiros];//crear un vector para guardar los giros que debe hacer el coche hasta llegar al destino
		for(int i=G->verticeconj[G->fin].dv.iter-1;i>=0;i--){
			co->ruta[i]=posRelativaCalles(lca.lca[G->verticeconj[G->verticeconj[aux].dv.IDant].index],lca.lca[G->verticeconj[aux].index]);
			aux=G->verticeconj[aux].dv.IDant;
		}
		for(int i=0;i<G->verticeconj[G->fin].dv.iter;i++)
			cout<<"\ngiro "<<i<<": "<<co->ruta[i];
		cout<<"\n\n\tFIN DE LA ELECCION DE RUTA\n";
		
		co->giro=co->ruta[co->igiros];			//elegir el primer giro de la ruta
		G->resetGrafo();	//se inicializan a 0 las variables de Dijkstra
	}
	break;
	case 2:	//Floyd-Warshall
	{
		co->ngiros=1;
		for(int i=0;i<G->nverticesconj;i++)	//cambiar mirar si se puede poner un map en vez
			if(G->verticeconj[i].index==ind){
				ini=i;
				break;
			}
		G->fin=rand()%G->nverticesconj;	//elegir el fin de la ruta
		int aux=G->fin;
		while(G->fin==ini)			//comprobar que el inicio y el fin son distintos
			G->fin=rand()%G->nverticesconj;
		cout<<"\n\tel origen es: "<<ini<<" \tel destino es el vertice: "<<G->fin;
		
		while(G->origenFW[ini][aux]!=ini){
			aux=G->origenFW[ini][aux];
			co->ngiros++;
		}
		
		//co->ngiros=1;//cambiar
		cout<<"\n\tel coche "<<icoche<<" tiene que hacer "<<co->ngiros<<" giros";
		co->ruta=new int[co->ngiros];//crear un vector para guardar los giros que debe hacer el coche hasta llegar al destino
		co->ruta[0]=1;//cambiar
		aux=G->fin;
		for(int i=co->ngiros-1;i>=0;i--){
			co->ruta[i]=posRelativaCalles(lca.lca[G->verticeconj[G->origenFW[ini][aux]].index],lca.lca[G->verticeconj[aux].index]);
			aux=G->origenFW[ini][aux];
		}
		for(int i=0;i<co->ngiros;i++)
			cout<<"\ngiro "<<i<<": "<<co->ruta[i];
		cout<<"\n\n\tFIN DE LA ELECCION DE RUTA\n";
		
		co->giro=co->ruta[co->igiros];			//elegir el primer giro de la ruta
	}
	break;
	default:
		cout<<"\nERROR SELECCIÓN OPTIMIZACIÓN RUTAS";
		cin>>optRuta;
	break;
	}
}

int posRelativaCalles(Calle* ca1, Calle* ca2){//calle inicio y calle destino
	//0=recto 1=derecha -1=izquierda 	en un semaforo
	if(ca1->getDir()==ca2->getDir())
		return 0;
	else{
		if(!ca1->getDir()){//vertical
			if(ca1->getTipo()==2){//abajo
				if(ca2->getTipo()==2)//izquierda
					return 1;
				else//derecha
					return -1;
			}
			else{//arriba
				if(ca2->getTipo()==2)//izquierda
					return -1;
				else//derecha
					return 1;
			}
		}
		else{//horizontal
			if(ca1->getTipo()==2){//izquierda
				if(ca2->getTipo()==2)//abajo
					return -1;
				else//arriba
					return 1;
			}
			else{//derecha
				if(ca2->getTipo()==2)//abajo
					return 1;
				else//arriba
					return -1;
			}
		}
	}
}

void interaccion::calcularCalles(listaCalles lca, float dx, float dy){
//no se para que es esto
}

///////////////////////////////////////////////////////////////////////
/*funcion para llevar un registro del tiempo de simulacion*/
void interaccion::temporal(double* tinicio,double* tfin,double* tiempo,int iteraciones,int ngiros,bool ruta){
	static double tiempomedio=0, tiempototal=0,talgmedio=0;
	static int i=0,ngirosmedio=0;
	if(ruta){	//tiempo para completar la ruta
		*tfin=clock();		//se halla el tiempo final de la ruta
		*tiempo=(double)((*tfin-*tinicio)/CLOCKS_PER_SEC);
		cout<<"\n\n******************** Tiempo: "<<*tiempo<<" segundos ********************\n\n";
		*tinicio=clock();	//se halla el tiempo inicial de la ruta
		
		tiempomedio+=*tiempo;
		ngirosmedio+=ngiros;
		i++;
	}
	else{	//tiempo para el calculo de la ruta
		*tfin=clock();		//se halla el tiempo final del calculo la ruta
		*tiempo=(double)((*tfin-*tinicio)/CLOCKS_PER_SEC);
		cout<<"\n\n******************** Tiempo alg: "<<*tiempo<<" segundos ********************\n\n";
		talgmedio+=*tiempo;
	}
	if(i>=iteraciones){	//se deja xmedio/i para poder sacar una simulacion en caso de colapso de los coches
		tiempomedio=tiempomedio/i;
		ngirosmedio=ngirosmedio/i;
		talgmedio=talgmedio/i;
		tiempototal=(double)(clock()/CLOCKS_PER_SEC);
		cout<< dec << "\n\n ***************************************************************************\n ********************\tTiempo medio: "<<tiempomedio<<" segundos\t********************\n ********************\tTiempo total: "<<tiempototal<<" segundos\t********************\n\n ********************\tTiempo medio alg: "<<talgmedio<<" segundos\t********************\n\n ********************\tNumero giros medio: "<<ngirosmedio<<" giros\t********************\n ********************\tSe han completado: "<<i<<" rutas\t********************\n ******************** FIN DE LA SIMULACION *********************************** \n";
		int aux;
		cin>>aux;
	}
}
////////////////////////////////////////////////////////////

/*funcion para que el tipo de calle sea entre 1 y 14 y no se den casuisticas inalcanzables*/
void interaccion::comprobarInalcanzable(listaSemaforos ls, listaCalles lca){
	bool baux=0, aux2=1;
	
	while(aux2){	//comprobar que si cambia una calle y afecte a otro semáforo, secambie otra calle del semáforo afectado
	aux2=0;
	for(int i=0; i<ls.numero; i++){
		if(ls.ls[i]->salida[0]==false && ls.ls[i]->salida[2]==false && ls.ls[i]->salida[1]==false && ls.ls[i]->salida[3]==false || ((ls.ls[i]->caf[0]==1 || ls.ls[i]->caf[0]==3 || ls.ls[i]->caf[0]==6 || ls.ls[i]->caf[0]==10)&&(ls.ls[i]->caf[1]==1 || ls.ls[i]->caf[1]==3 || ls.ls[i]->caf[1]==6 || ls.ls[i]->caf[1]==10)&&(ls.ls[i]->caf[2]==2 || ls.ls[i]->caf[2]==4 || ls.ls[i]->caf[2]==7 || ls.ls[i]->caf[2]==11)&&(ls.ls[i]->caf[3]==2 || ls.ls[i]->caf[3]==4 || ls.ls[i]->caf[3]==7 || ls.ls[i]->caf[3]==11))){		//comprobar los semaforos
			for(int j=0; j<lca.numero; j++){	//comprobar las calles
				Vector2D aux = lca.lca[j]->posicion-ls.ls[i]->posicion;
				if(aux.modulo()<lca.lca[j]->longitud && lca.lca[j]->getPos().modulo()>ls.ls[i]->getPos().modulo()){
					baux=cambiarTipoCalle((i!=(ls.numero-1))?((ls.ls[i+1]->getPos().y>ls.ls[i]->getPos().y)?(lca.lca[j]):(lca.lca[j+1])):(lca.lca[j-2]), true, (i!=(ls.numero-1))?(ls.ls[i]->salida[0]):(ls.ls[i]->salida[2]));	//cambiar quizas
					/*callesem((i!=(ls.numero-1))?((ls.ls[i+1]->getPos().y>ls.ls[i]->getPos().y)?(lca.lca[j]):(lca.lca[j+1])):(lca.lca[j-2]),ls.ls[i]);
					callesem((i!=(ls.numero-1))?((ls.ls[i+1]->getPos().y>ls.ls[i]->getPos().y)?(lca.lca[j]):(lca.lca[j+1])):(lca.lca[j-2]),ls.ls[i]);*///cambiar para que solo se actualicen los dos semaforos afectados
					if(baux) aux2=baux;
					break;	//hace que solo cambie una de las calles
				}
			}
		}
	}
	callesem(lca,ls);//quitar cuando se cambie lo de arriba
	
	}
	
}
/*funcion para que las calles exteriores sean iguales entre si*/
void interaccion::callesExteriores(listaCalles *calles, int i, int j, int k, int longitud){
	//if(k)	//arriba
	for (int n = 0; n < calles->numero; n++)
		if (calles->lca[n]->getPos().x < 0 && calles->lca[n]->getPos().y == (longitud * j - longitud / 2 * (k - 1)) || calles->lca[n]->getPos().y < 0 && calles->lca[n]->getPos().x == (longitud * i + longitud / 2 * k))
			calles->agregar(new Calle({ (float)(longitud * i + longitud / 2 * k),(float)(longitud * j - longitud / 2 * (k - 1)) }, k, calles->lca[n]->getTipo(), true));
	
}
////////////////////////////////////////////////////////////////
/*funcion para saber si las calles son solo de salida*/
bool interaccion::cambiarTipoCalle(Semaforo* s, Calle* c){	
	if(sqrt(pow(s->posicion.x-c->posicion.x,2))<1 && s->posicion.y<c->posicion.y)//calle arriba
	{
		if(c->tipo==1 || c->tipo==3 || c->tipo==6 || c->tipo==10)
			return true;
		else return false;
	}
	else if(sqrt(pow(s->posicion.x-c->posicion.x,2))<1 && s->posicion.y>c->posicion.y)//calle abajo 
	{
		if(c->tipo==2 || c->tipo==4 || c->tipo==7 || c->tipo==11)
			return true;
		else return false;
	}
	else if(sqrt(pow(s->posicion.y-c->posicion.y,2))<1 && s->posicion.x<c->posicion.x)//calle derecha
	{
		if(c->tipo==1 || c->tipo==3 || c->tipo==6 || c->tipo==10)
			return true;
		else return false;
	}
	else if(sqrt(pow(s->posicion.y-c->posicion.y,2))<1 && s->posicion.x>c->posicion.x)//calle izquierda
	{ 
		if(c->tipo==2 || c->tipo==4 || c->tipo==7 || c->tipo==11)
			return true;
		else return false;
	}
	else{
		cout<<"\n\tERROR FUNCION cambiarTipoCalle(semaforo, calle)";
		return -1;
	}
}
void interaccion::cambiarTipoCalle(listaCalles lc){
	for(int i=0; i<lc.numero; i++)
		bool a=cambiarTipoCalle(lc.lca[i], false, false);
}
bool interaccion::cambiarTipoCalle(Calle* c, bool inalcanzable, bool entrada){
	if(inalcanzable){
		cout<<"se ha cambiado por incompatibilidad";
		if(entrada){
			c->setTipo(2);
			cout<<" de entrada";
			c->setColor(1,0,0);
		}
		else{
			c->setTipo(1);
			cout<<" de salida";
			c->setColor(0,1,0);
		}
		return 1;
	}
	else{
		if(c->getTipo()<1){
			c->setTipo(2);	//cambiar 14 para varios carriles
			cout<<"cambiar calle a 2";
		}
		else if(c->getTipo()>2){	//cambiar 14 para varios carriles
			c->setTipo(1);
			cout<<"cambiar calle a 1";	
		}
		return 0;
	}
}
/*funcion para asignar el valor ocupacion a las calles en funcion de los coches*/
void interaccion::ocupacionCalle(listaCoches lco, listaCalles lca){
	for(int i=0;i<lca.numero;i++){
		lca.lca[i]->setOcupacion(0);	//se reinicia la ocupacion en todas las calles
	}
	for(int i=0;i<lco.numero;i++){
		int aux;	//cambiar en cierto momento esto se va a la mierda en 0 0, el coche 0 empieza un giro y desaparece del mapa
		aux=getCalleindex(lco.lco[i],lca);	//se devuelve el indice de la calle
		lca.lca[aux]->setOcupacion((lca.lca[aux]->getOcupacion()+1));	//se incrementa en 1 la ocupacion por cada coche
	}
}
void interaccion::ocupacionCalle(listaCoches lco, Calle* ca){
	ca->ocupacion=0;	//reiniciar el valor de la ocupacion
	for(int i=0;i<lco.numero;i++){
		if((lco.lco[i]->getPos()-ca->getPos()).modulo()<ca->getLong()/2){	//coche dentro de la calle
			ca->ocupacion++;
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////
/*funcion que devuelve el indice de la calle en la que está el coche*/
int interaccion::getCalleindex(Coche* co, listaCalles lca){
	Vector2D aux;
	for(int i=0;i<lca.numero;i++){
		aux=co->posicion-lca.lca[i]->posicion;
		if(aux.modulo()<(lca.lca[i]->longitud/1.95f))
			return i;
	}
}
//////////////////////////////////////////////////////////////////////

/*Comportamiento del semaforo*/
void interaccion::OnTimerSem(listaCalles& lca,listaCoches& lco, listaSemaforos ls, int opt, grafo* Gsem){
	static bool flagaux=false;
	for(int i=0;i<ls.numero;i++)
		OnTimerSem(lca,lco,ls.ls[i],opt,&Gsem[i],flagaux,i);
	flagaux=true;	//para que la asignacion de tiempos en los casos 1 y 2 solo se haga una vez
}
void interaccion::OnTimerSem(listaCalles& lca, listaCoches& lco, Semaforo* s, int opt, grafo* Gsem, bool flagaux, int ind){
	if(Gsem->getTempaux()>(10*40)){	//cada 0.25s
		switch(s->getEstadoLuces()){
		case 0:	//caso normal con todos los tiempos iguales
			switch(opt){
			case 0:	//caso normal 
				//los tiempos que se han generado al inicio
			break;
			case 1:	//optimizacion local
				Gsem->calcularPesoSem(lca,lco,s,ind);
			break;
			case 2:	//optimizacion general
				cout << "\nOnTimerSem caso opt 2, tiempos iguales" << endl;
			break;
			default:
				cout<<"\nERROR SELECCIÓN OPTIMIZACIÓN SEMÁFOROS 0";
				cin>>opt;
			break;
			}
		break;
		case 1:	//caso variable con más tiempo horizontal
			switch(opt){
			case 0:	//caso normal
				if(!flagaux) s->setTimer(rand()%200+360,rand()%160+320,rand()%200+520,rand()%160+320);	//para que la asignacion solo la haga una vez
			break;
			case 1:	//optimizacion local
				Gsem->calcularPesoSem(lca,lco,s,ind);
			break;
			case 2:	//optimizacion general
				cout << "\nOnTimerSem caso opt 2, mas tiempo horizontal" << endl;
			break;
			default:
				cout<<"\nERROR SELECCIÓN OPTIMIZACIÓN SEMÁFOROS 1";
				cin>>opt;
			break;
			}
		break;
		case 2:	//caso variable con más tiempo vertical
			switch(opt){
			case 0:	//caso normal
				if(!flagaux) s->setTimer(rand()%200+520,rand()%160+320,rand()%200+360,rand()%160+320);	//para que la asignacion solo se haga una vez
			break;
			case 1:	//optimizacion local
				Gsem->calcularPesoSem(lca,lco,s,ind);
			break;
			case 2:	//optimizacion general
				cout << "\nOnTimerSem caso opt 2, mas tiempo vertical" << endl;
			break;
			default:
				cout<<"\nERROR SELECCIÓN OPTIMIZACIÓN SEMÁFOROS 2";
				cin>>opt;
			break;
			}
		break;
		case 3:	//caso fijo horizontal
			s->setTimer(1,1,0,1);
			s->setTemp(0,1);	//evitar fugas de memoria cuando lleva mucho tiempo incrementando el contador
		break;
		case 4:	//caso fijo vertical
			s->setTimer(0,1,1,1);
			s->setTemp(0,1);	//evitar fugas de memoria cuando lleva mucho tiempo incrementando el contador
		break;
		}
		Gsem->setTempaux(0);
	}
	Gsem->setTempaux(Gsem->getTempaux()+1);
	s->maquinaEstados();
	cout << "sale de OnTimerSem" << endl;
}
///////////////////////////////

int interaccion::comprobarDesaparecidos(listaCoches co){
	int aux=0;
	for(int i=0; i<co.numero; i++)
		if(co.lco[i]->posicion.modulo()>150)
			aux++;	
	return aux;
}
//////////////////////////////////////////////////////



int interaccion::mascara(Coche* c, Semaforo* s, float dist){
	int numdec=0, numbin[12]={0,0,0,0,0,0,0,0,0,0,0,0};
	float auxx=c->posicion.x-s->posicion.x, auxy=c->posicion.y-s->posicion.y;
	if(dist<=s->size)
		numbin[11]=0;	//dentro del semaforo
	else if(dist<1.3f*s->size)
		numbin[11]=1;	//fuera del semaforo
	switch(s->luz){
	case 0:		//vv-rh
		numbin[10]=0;
		numbin[9]=0;
	break;
	case 1:		//av-rh
		numbin[10]=0;
		numbin[9]=1;
	break;
	case 2:		//rv-vh
		numbin[10]=1;
		numbin[9]=0;
	break;
	case 3:		//rv-ah
		numbin[10]=1;
		numbin[9]=1;
	break;
	}
	switch(c->dir){
	case 0:		//arriba
		numbin[8]=0;
		numbin[7]=0;
	break;
	case 1:		//derecha
		numbin[8]=0;
		numbin[7]=1;
	break;
	case 2:		//abajo
		numbin[8]=1;
		numbin[7]=0;
	break;
	case 3:		//izquierda
		numbin[8]=1;
		numbin[7]=1;
	break;
	}
	switch(c->giro){
	case -1:		//giro izquierda
		numbin[6]=0;
		numbin[5]=0;
	break;
	case 0:		//seguir recto
		numbin[6]=0;
		numbin[5]=1;
	break;
	case 1:		//giro derecha
		numbin[6]=1;
		numbin[5]=0;
	break;
	} // todo revisar
	if(s->salida[0])		//calle de arriba disponible
		numbin[1]=1;
	if(s->salida[1])		//calle de la derecha disponible
		numbin[2]=1;
	if(s->salida[2])		//calle de abajo disponible
		numbin[3]=1;
	if(s->salida[3])	//calle de la izquierda disponible
		numbin[4]=1;
		
		
	if(c->dir==0 && auxy>=((float)(c->n[1]-6)) || c->dir==2 && auxy<=((float)(c->n[1]-2)) || c->dir==1 && auxx>=((float)(2-c->n[1])) || c->dir==3 && auxx<=((float)(6-c->n[1])))
		numbin[0]=1;
	else
		numbin[0]=0;
	
	
	for(int i=0;i<12;i++)
		numdec+=numbin[i]*pow(2,i);
	
	//if(numdec>500)	
	//cout << "  " << numdec << "  ";
	return numdec;
}


// asociar las calles con las aristas del grafo
int interaccion::asociarAristaIndex(listaCalles calles, Semaforo* s, int posRel, int filas, int columnas, bool flag){
	//posRel=0 arriba, 1 derecha, 2 abajo, 3 izquierda
	for(int i=0;i<calles.numero;i++){
		if((s->getPos()-calles.lca[i]->getPos()).modulo()<calles.lca[i]->getLong()){
			switch(posRel){
			case 0:
				if(s->getPos().y<calles.lca[i]->getPos().y){	//devuelve el indice de la calle, la ocupacion de una calle central, o la ocupacion de dos calles de los extremos
					if(flag) return i;
					else{
						if(calles.lca[i]->getPos().y<((filas-1)*calles.lca[i]->getLong())) return calles.lca[i]->getOcupacion();
						else for(int j=0;j<calles.numero;j++)
		if(calles.lca[i]->getPos().x==calles.lca[j]->getPos().x && calles.lca[j]->getPos().y<0) return calles.lca[i]->getOcupacion()+calles.lca[j]->getOcupacion();
					}
				}
			break;
			case 1:
				if(s->getPos().x<calles.lca[i]->getPos().x){
					if(flag) return i;
					else{
						if(calles.lca[i]->getPos().x<((columnas-1)*calles.lca[i]->getLong())) return calles.lca[i]->getOcupacion();
						else for(int j=0;j<calles.numero;j++)
		if(calles.lca[i]->getPos().y==calles.lca[j]->getPos().y && calles.lca[j]->getPos().x<0) return calles.lca[i]->getOcupacion()+calles.lca[j]->getOcupacion();
					}
				}
			break;
			case 2:
				if(s->getPos().y>calles.lca[i]->getPos().y){
					if(flag) return i;
					else{
						if(calles.lca[i]->getPos().y>0) return calles.lca[i]->getOcupacion();
						else for(int j=0;j<calles.numero;j++)
		if(calles.lca[i]->getPos().x==calles.lca[j]->getPos().x && calles.lca[j]->getPos().y>((filas-1)*calles.lca[i]->getLong())) return calles.lca[i]->getOcupacion()+calles.lca[j]->getOcupacion();
					}
				}
			break;
			case 3:
				if(s->getPos().x>calles.lca[i]->getPos().x){
					if(flag) return i;
					else{
						if(calles.lca[i]->getPos().x>0) return calles.lca[i]->getOcupacion();
						else for(int j=0;j<calles.numero;j++)
		if(calles.lca[i]->getPos().y==calles.lca[j]->getPos().y && calles.lca[j]->getPos().x>((columnas-1)*calles.lca[i]->getLong())) return calles.lca[i]->getOcupacion()+calles.lca[j]->getOcupacion();
					}
				}
			break;
			default:
				return -1;
			}
		}
	}
	
			
}


/*evitar que el coche se quede parado y le cambie la direccion (cambiar: se debería arreglar en otra parte del código pero no se cual)*/
void interaccion::parcheSetDir(listaCoches lco, listaCalles lca){
	for(int i=0;i<lco.numero;i++){
		int cont=0;
		for(int j=0;j<lco.numero;j++){
		if(i!=j){
			Vector2D aux=lco.lco[i]->getPos()-lco.lco[j]->getPos();
			if(aux.modulo()<8||lco.lco[i]->flagsem){	//cerca de otro coche o acercandose a un semaforo
				cont++;
			}
		}
		}
		if(!cont&&lco.lco[i]->getStop()){	//si no ha aumentado el contador y el coche está parado
			lco.lco[i]->changeStop(0);	//se reanuda la marcha del coche cambiar comprobar que no se vaya en otra direccion
			cout<<"\nSe reanuda la marcha del coche: "<<i;
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////