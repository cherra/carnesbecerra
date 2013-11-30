/* Funcion que imprime Ticket ya sean de contado o credito
*
* SINTAXIS:
*                               imprimirticket(ID_DE_LA_VENTA , TIPO_TICKET , PAGO);
*  Donde TIPO_TICKET:
*            "credito" ---------> Ticket para credito y en pago se envia 0 o lo que sea
*            "contado" ---------> Ticket para contado y en pago se envia lo que se pago
*	      "factura" ---------->  Impresion de la Factura y en pago se envia 0
*				0 Normal  - Primer valor es numero venta
*				1 CIERRE FACTURA - Credito primer valor es numero factura
*				2 CIERRE FACTURA - Contado
*	      "corte_caja" ------>  Corte de Caja y en pago se envia 0, solo para la caja especifica
*								Si pago es igual a 0, se imprime la leyenda de Corte Parcial
*								Si pago es igual a 1, se imprime la leyenda de Corte Final
*	      "credito_abono" --> Lista de los deudores de credito y cuanto han abonado y se envia la fecha de hoy
*					   el formato es AAAAMMDD
*	      "corte_all_caja" --> Corte de Caja y en pago se en"via 0, sirve para todas las cajas
*					Reporte detallado del corte de caja del dia con formato AAAAMMDD
*        "cfd" ---------> Factura Electronica, se envia el id_factura y en pago 0 o lo que sea
*
*
*  Ultima modificacion: 20 Mayo 2003 - Cherra
*  + 2 Abril 2003 - Se agrego lo de agregar las coordenadas del archivo
*  + 9 Abril 2003 - Se quito el IVA a los productos y se arreglaron algunos bugs y se optimizo lo de imprimr corte_caja
*	Tomando en cuenta la alineacion del texto ya sea izq o derecha
*   + 13 Abril 2003 - Se agrega lo de la lista de credito y abono de los clientes
*   + 25 Abril 2003 - Se modifico el corte de caja
*   + 5 Mayo 2003 - Se agrego el corte de caja detallado para todas las cajas
*   + 5 Mayo 2003 - Se agrego el archivo de configuracion
*   * 20 Mayo 2003 - Se limito la longitud del nombre de los articulos en la impresion de tickets a 39 caracteres.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <mysql/mysql.h>
//#include "funciones.h"
gboolean imprimiendo = FALSE; /* Bandera de impresion !!! */

#define MAXFIL 9999

MYSQL mysql;
MYSQL_RES *resultado, *resultado2;
MYSQL_ROW row, row2;
FILE *fpt, *fpt2, *fptlogo, *fpttemp; //Declaracion del archivo

char tipoPago[20]="";

char c[1500]; //Aqui se guarda las cadenas a imprimir
int fac_nombre[99][4];
int impresora_nombre[99][4];

char Errores[200];

//Variables del ARRAY
int fila, nfilas, ncols;
char *matrix[MAXFIL];
char retVar[500]; //Cadena con la cantidad con letra

int abrir_cajon (char puerto[50]);

//Traduce
void traduce(double numero, char retVar[500]){
// numero   -> Numero a traducir
// retVar     -> Cadena con la traduccion
	int i, j, a;
	int bandera=0;
	int veces;
	char numero_char[20]; //Numero en cadena
	char num[20];
	int num_num, num_num2, num_num3; //Numero Temporales
	char num_tmp[20]; //Temporal de numero

	char respnu[20]; //Respaldo num
	char centavos[20];
	int grande; //Tamao de la cadena
	int largo;
	int reslarg; //Respaldo

	int inicio;
	int fin;
	int inicio_fin;
	char tango[20];
	char pesos[10];


	char unidt[50]; //Unidades
	char decet[50]; //Decenas
	char centt[50]; //Centenas
	char millt[50]; //Millardos
	char decmillt[50]; //Decenamillardos
	char ciecentt[50]; //Cientmillardos
	char veintes[50];
	char textt[300]; //AQUI SE GUARDA LA CADENA

	char *unidad[10];
	char *decena[10];
	char *centena[10];
	char *millar[10];

	unidad[0] = "cero";
	unidad[1] = "un";
	unidad[2] = "dos";
	unidad[3] = "tres";
	unidad[4] = "cuatro";
	unidad[5] = "cinco";
	unidad[6] = "seis";
	unidad[7] = "siete";
	unidad[8] = "ocho";
	unidad[9] = "nueve";

	decena[1] = "diez";
	decena[2] = "veinte";
	decena[3] = "treinta";
	decena[4] = "cuarenta";
	decena[5] = "cincuenta";
	decena[6] = "sesenta";
	decena[7] = "setenta";
	decena[8] = "ochenta";
	decena[9] = "noventa";

	centena[1] = "ciento";
	centena[2] = "doscientos";
	centena[3] = "trescientos";
	centena[4] = "cuatrocientos";
	centena[5] = "quinientos";
	centena[6] = "seiscientos";
	centena[7] = "setecientos";
	centena[8] = "ochocientos";
	centena[9] = "novecientos";

	millar[1] = "mil";
	millar[2] = "dos mil";
	millar[3] = "tres mil";
	millar[4] = "cuatro mil";
	millar[5] = "cinco mil";
	millar[6] = "seis mil";
	millar[7] = "siete mil";
	millar[8] = "ocho mil";
	millar[9] = "nueve mil";

	strcpy(unidt,""); //Unidades
	strcpy(decet,""); //Decenas
	strcpy(centt,""); //Centenas
	strcpy(millt,""); //Millardos
	strcpy(decmillt,""); //Decenamillardos
	strcpy(ciecentt,""); //Cientmillardos

	sprintf(numero_char, "%.2f", numero); //Convierte a cadena
	printf("\nNUMERO A CONVERTIR: %s\n", numero_char);
	grande = strlen(numero_char);
	strcpy(num,"");
	strcpy(centavos,"");
	for ( a=0; a< grande; a++ ) {
		//printf("\nA: %c",numero_char[a]);
		if (numero_char[a] == '.' ) {  //substr(a, 1 );
				//num = numero.substr(0, a );
				strncat(num,numero_char,a);
				if (a < grande - 1){
						j=0;
						for (i=a+1;i<grande;i++) //centavos = numero.substr(a + 1, 2);
						{
							if(j<2){
								centavos[j] = numero_char[i];
								centavos[j+1] = '\0';
							}else
								break;
							j++;
						}

				} else {
						strcpy(centavos,"00");
				}
				/*
				if (parseInt(numero.substr(a + 1, 2)) < 10 && (parseInt(numero.substr( a + 1, 1))) > 0 )
					centavos = centavos + "0";
				*/
				bandera = 99;
		}
	}

	if ((bandera != 99 )) {
		strcpy(num,numero_char);
		strcpy(centavos,"00");
	}
	largo = strlen(num);
	veces = 1;

	if (largo > 6 ) {
			strcpy(respnu,num);
			reslarg = largo;
			strcpy(num_tmp,"");
			strncat(num_tmp,num,largo-6);
			strcpy(num, num_tmp);
			//num = num.substr(0,largo - 6);
			largo = largo - 6;
			veces = 2;
	}

	for ( a=1; a<= veces; a++ ) {

		if (a == 2 ) {
				j=0;
				for(i=largo;i<strlen(respnu);i++){ 		//num = respnu.substring(respnu.length - 6 , respnu.length)
					num[j] = respnu[i];
					num[j+1] = '\0';
					j++;
				}
				largo = 6;
		}



		sprintf(num_tmp, "%s", num); //Convierte a cadena
		num_num = atoi(num_tmp); //Convierte a numero
		if (num_num % 10 != 0 || numero == 0){
				//unidt = unidad[num.substr(largo - 1, 1)];
				sprintf(num_tmp, "%c", num[largo-1]); //Convierte a cadena
				num_num = atoi(num_tmp);
				strcpy(unidt,unidad[num_num]);
			} else {
				strcpy(unidt,"");
		}




		if (largo > 1 ) {
			sprintf(num_tmp, "%c", num[largo-2]); //Convierte a cadena
			num_num = atoi(num_tmp);
			if(num_num > 0 ){
				sprintf(num_tmp, "%c", num[largo-1]); //Convierte a cadena
				num_num = atoi(num_tmp);
				if(num_num > 0 ){
					sprintf(num_tmp, "%c", num[largo-2]); //Convierte a cadena
					num_num = atoi(num_tmp);
					strcpy(decet,decena[num_num]);
					strcat(decet," y ");
				} else {
					sprintf(num_tmp, "%c", num[largo-2]); //Convierte a cadena
					num_num = atoi(num_tmp);
					strcpy(decet,decena[num_num]);
				}
			} else {
				strcpy(decet,"");
			}
		}

		if (largo > 2 ) {

			sprintf(num_tmp, "%c", num[largo-3]); //Convierte a cadena
			num_num = atoi(num_tmp);
			if(num_num > 1 ){
				sprintf(num_tmp, "%c", num[largo-3]); //Convierte a cadena
				num_num = atoi(num_tmp);
				strcpy(centt,centena[num_num]);
				strcat(centt," ");
			} else {

				sprintf(num_tmp, "%c", num[largo-3]); //Convierte a cadena
				num_num = atoi(num_tmp);

				if(num_num == 0 ){
					strcpy(centt," ");
				}


				//if (parseInt(num.substr( largo - 3, 1)) == 1 && (parseInt(num.substr(2,num.length))) > 0 )
				//	centt = centena[parseInt(num.substr(largo - 3, 1))] + " ";
				sprintf(num_tmp, "%c", num[largo-3]); //Primera parte del IF
				num_num = atoi(num_tmp);

				j=2;
				for(i=1;i<strlen(num);i++){
					num_tmp[i-1] = num[j];
					num_tmp[i] = '\0';
					j++;
				}
				num_num2 = atoi(num_tmp);
				if(num_num == 1 && num_num2 > 0){
					sprintf(num_tmp, "%c", num[largo-3]); //Convierte a cadena
					num_num = atoi(num_tmp);
					strcpy(centt,centena[num_num]);
					strcat(centt," ");
				}


				//if (parseInt(num.substr( largo - 3, 1)) == 1 && (parseInt(num.substr(2,num.length))) == 0 )
				//	centt = "cien";

				sprintf(num_tmp, "%c", num[largo-3]); //Primera parte del IF
				num_num = atoi(num_tmp);

				if(num_num == 1 && num_num2 == 0){
					strcpy(centt,"cien");

				}

			}

		}


		if (largo > 3 ) {

			sprintf(num_tmp, "%c", num[largo-4]); //Convierte a cadena
			num_num = atoi(num_tmp);
			if(num_num > 1 ){
				strcpy(millt,unidad[num_num]);
				strcat(millt," mil ");

			} else {
				if(num_num == 1 ){
					strcpy(millt,"un mil ");
				}
				if(num_num == 0 ){
					strcpy(millt," mil ");
				}
			}
		}


		if (largo > 4 ) {
			sprintf(num_tmp, "%c", num[largo-5]); //Convierte a cadena
			num_num = atoi(num_tmp);
			if(num_num > 0 ){
				sprintf(num_tmp, "%c", num[largo-4]); //Convierte a cadena
				num_num = atoi(num_tmp);
				if(num_num > 0 ){
					sprintf(num_tmp, "%c", num[largo-5]); //Convierte a cadena
					num_num = atoi(num_tmp);
					strcpy(decmillt,decena[num_num]);
					strcat(decmillt," y ");
				} else {
					sprintf(num_tmp, "%c", num[largo-5]); //Convierte a cadena
					num_num = atoi(num_tmp);
					strcpy(decmillt,decena[num_num]);
				}

				} else {
					strcpy(decmillt,"");
				}

		}

		if (largo > 5 ) {
			sprintf(num_tmp, "%c", num[largo-6]); //Convierte a cadena
			num_num = atoi(num_tmp);
			if(num_num > 1 ){
				strcpy(ciecentt,centena[num_num]);
				strcat(ciecentt," ");
			} else {
				if(num_num == 0 ){
					strcpy(ciecentt, "");
				}

				j=largo-5;
				for(i=1;i<2;i++){
					num_tmp[i-1] = num[j];
					num_tmp[i] = '\0';
					j++;
				}
				num_num2 = atoi(num_tmp);
				if(num_num == 1 && num_num2 > 0){
					strcpy(ciecentt,centena[num_num]);
					strcat(ciecentt," ");
				}
					if(num_num == 1 && num_num2 == 0){
					strcpy(ciecentt,"cien ");
				}
			}
		}

		if (strcmp(decet,"diez y ") == 0) {
			sprintf(num_tmp, "%c", num[largo-1]); //Convierte a cadena
			num_num = atoi(num_tmp);

			switch ( num_num ){
			case 1:
				strcpy(veintes,"once");
				break;
			case 2:
				strcpy(veintes,"doce");
				break;
			case 3:
				strcpy(veintes,"trece");
				break;
			case 4:
				strcpy(veintes,"catorce");
				break;
			case 5:
				strcpy(veintes,"quince");
				break;
			case 6:
				strcpy(veintes,"dieciseis");
				break;
			case 7:
				strcpy(veintes,"diecisiete");
				break;
			case 8:
				strcpy(veintes,"dieciocho");
				break;
			case 9:
				strcpy(veintes,"diecinueve");
				break;
			case 0:
				strcpy(veintes,"diez");

				break;
   			}
			strcpy(decet,veintes);
			strcpy(unidt,"");
		}


	if (strcmp(decet,"veinte y ") == 0) {
		sprintf(num_tmp, "%c", num[largo-1]); //Convierte a cadena
		num_num = atoi(num_tmp);

		switch ( num_num ){
		case 1:
			strcpy(veintes,"veintiun");
			break;
		case 2:
			strcpy(veintes,"veintidos");
			break;
		case 3:
			strcpy(veintes,"veintitres");
			break;
		case 4:
			strcpy(veintes,"veinticuatro");
			break;
		case 5:
			strcpy(veintes,"veinticinco");
			break;
		case 6:
			strcpy(veintes,"veintiseis");
			break;
		case 7:
			strcpy(veintes,"veintisiete");
			break;
		case 8:
			strcpy(veintes,"veintiocho");
			break;
		case 9:
			strcpy(veintes,"veintinueve");
			break;
		case 0:
			strcpy(veintes,"veinte");
			break;
		}
		strcpy(decet,veintes);
		strcpy(unidt,"");
		}


	if (strcmp(decet,"diez y ") == 0) {
		sprintf(num_tmp, "%c", num[largo-4]); //Convierte a cadena
		num_num = atoi(num_tmp);

		switch ( num_num ){
			case 1:
				strcpy(veintes,"once mil");
				break;
			case 2:
				strcpy(veintes,"doce mil");
				break;
			case 3:
				strcpy(veintes,"trece mil");
				break;
			case 4:
				strcpy(veintes,"catorce mil");
				break;
			case 5:
				strcpy(veintes,"quince mil");
				break;
			case 6:
				strcpy(veintes,"dieciseis mil");
				break;
			case 7:
				strcpy(veintes,"diecisiete mil");
				break;
			case 8:
				strcpy(veintes,"dieciocho mil");
				break;
			case 9:
				strcpy(veintes,"diecinueve mil");
				break;
			case 0:
				strcpy(veintes,"diez mil");
				break;
		}
		strcpy(decmillt,veintes);
		strcpy(millt,"");
		}

	if (strcmp(decet,"veinte y ") == 0) {
		sprintf(num_tmp, "%c", num[largo-4]); //Convierte a cadena
		num_num = atoi(num_tmp);

		switch ( num_num ){
		case 1:
			strcpy(veintes,"veintiun mil");
			break;
		case 2:
			strcpy(veintes,"veintidos mil");
			break;
		case 3:
			strcpy(veintes,"veintitres mil");
			break;
		case 4:
			strcpy(veintes,"veinticuatro mil");
			break;
		case 5:
			strcpy(veintes,"veinticinco mil");
			break;
		case 6:
			strcpy(veintes,"veintiseis mil");
			break;
		case 7:
			strcpy(veintes,"veintisiete mil");
			break;
		case 8:
			strcpy(veintes,"veintiocho mil");
			break;
		case 9:
			strcpy(veintes,"veintinueve mil");
			break;
		case 0:
			strcpy(veintes,"veinte");
			break;
		}
		strcpy(decmillt,veintes);
		strcpy(millt,"");
		}

		if (a == 1 && veces == 2 ) {
				strcpy(textt,ciecentt); //Se concatenan todas las variables
				strcat(textt,decmillt);
				strcat(textt,millt);
				strcat(textt,centt);
				strcat(textt,decet);
				strcat(textt,unidt);


				j=0;
				for(i=1;i<2;i++){
					num_tmp[i-1] = textt[j];
					num_tmp[i] = '\0';
					j++;
				}

				if (strcmp(num_tmp,"un") == 0) {
					strcpy(textt,ciecentt); //Se concatenan todas las variables
					strcat(textt,decmillt);
					strcat(textt,millt);
					strcat(textt,centt);
					strcat(textt,decet);
					strcat(textt,unidt);
					strcat(textt,"milln");
				} else {
					strcpy(textt,ciecentt); //Se concatenan todas las variables
					strcat(textt,decmillt);
					strcat(textt,millt);
					strcat(textt,centt);
					strcat(textt,decet);
					strcat(textt,unidt);
					strcat(textt,"millones");
				}
		}


		if (veces > 1 ) {
			inicio=strlen(num) - 6;
			fin=strlen(num);

			//substring
			if(inicio == fin)
				strcpy(num_tmp,"");
			else{
				if(inicio < 0)
					inicio = 0;
				if(fin >= strlen(num))
					fin = strlen(num)-1;
				if(inicio > fin){
					inicio_fin = inicio;
					inicio = fin;
					fin = inicio_fin;
				}
				j=0;
				for(i=inicio;i<fin;i++){
					num_tmp[j] = textt[i];
					num_tmp[j+1] = '\0';
					j++;
				}
			}
			//Fin funcion substring
			strcpy(tango, num_tmp);

			sprintf(num_tmp, "%c", tango[0]); //Convierte a cadena
			num_num = atoi(num_tmp);
			sprintf(num_tmp, "%c", tango[1]); //Convierte a cadena
			num_num2 = atoi(num_tmp);
			sprintf(num_tmp, "%c", tango[2]); //Convierte a cadena
			num_num3 = atoi(num_tmp);

			if((num_num == 0) && (num_num2 == 0) && (num_num3 == 0)){
				strcpy(millt,"");
			}
		}


		if (a == 1 && veces == 1 ){
			strcpy(textt,ciecentt); //Se concatenan todas las variables
			strcat(textt,decmillt);
			strcat(textt,millt);
			strcat(textt,centt);
			strcat(textt,decet);
			strcat(textt,unidt);
		}

		if (a == 2 && veces == 2 ){
			strcpy(textt,ciecentt); //Se concatenan todas las variables
			strcat(textt,decmillt);
			strcat(textt,millt);
			strcat(textt,centt);
			strcat(textt,decet);
			strcat(textt,unidt);
			}
	}


	if (numero >= 1 && numero < 2){
		strcpy(pesos," peso ");
	} else {
		strcpy(pesos," pesos ");
	}


	num_num = atoi(respnu);
		strcpy(retVar,"");
		//strcpy(retVar,textt);
		//strcat(retVar,centavos);
		//strcat(retVar,"/100 M.N. ");

	if (num_num >= 1000000  && (num_num % 1000000)==0){
		strcpy(retVar,textt);
		strcat(retVar," de pesos ");
		strcat(retVar,centavos);
		strcat(retVar,"/100 M.N. ");
	} else {
		strcpy(retVar,textt);
		strcat(retVar,pesos);
		strcat(retVar,centavos);
		strcat(retVar,"/100 M.N. ");
	}
printf("\nTEXTO: %s",retVar);



}


//Inicializa la MATRIX
void inicializar(){
 	int fila, col, n;
	n = 2;
	for(col=0;col<ncols;++col){
		for(fila=0; fila <=nfilas; ++fila){
			matrix[fila][col] = ' ';
			if(fila == nfilas)
				matrix[fila][col] = '\n';
		}
	}
}


void imprimir(char c[1000], int nX){
	int i, j;
        char cTMP[1000];
		//printf ("### CAD = %s \n",c);
        strcpy(cTMP,""); //Inicializacion
        i=0;
        j=1;
        while(i <= strlen(c)){
				//printf ("# I = %d \n",i);
				cTMP[i] = c[i];
                if(j == nX){
                        printf("\n");
                        fputs("\n",fpt);
                        j=0;
                }
		if (cTMP[i] == '\n')
		{
                        printf("\n");
                        //fputs("\n",fpt);
                        j=0;
		}
		
                printf("%c",cTMP[i]);
                fputc(cTMP[i],fpt);
                i++;
                j++;
        }
}


//Escribe en la consola la salida de la MATRIX
void escribirsalida(char tipo[10]){
 	int fila, col;
	char tmp;
		printf("Escribiendo archivo....\n");
		if(!fpt)
			printf("Hay un error al escribirlo...\n");
		for(col=0;col<ncols;col++)
		{
			for(fila=0; fila <=nfilas; fila++){
    				if(strcmp(tipo,"archivo") == 0){
					tmp = matrix[fila][col];
					printf("%c",tmp);
					fputc(tmp,fpt);
//					imprimir(tmp, 2);
				}else{ // consola
					printf("%c",matrix[fila][col]);
				}
			}
		}
		printf("\n");
}

void inicializarARRAY(int nX){
	int i;
	for(i=0;i<nX-1;i++){
		c[i] = ' ';
		c[i+1] = '\0';
	}
}

void llenarARRAY(int  arr_pos, char cadena[1000])
{
	int i, k, X, X1, Alineacion;
	int tam_cadena = 0;

	tam_cadena = strlen(cadena);
	X = fac_nombre[arr_pos][0];
	X1 = fac_nombre[arr_pos][1];
	Alineacion = fac_nombre[arr_pos][2];
	if((X >= 0)){
		if(Alineacion == 1){ //Alineacion de izquierda a derecha
		 	k=0;
			for(i=X; i<=X1; i++){
				if(k<tam_cadena)
					c[i] = cadena[k];
				else
					break;
				 k++;
			}
		}else{
			k= tam_cadena-1;
			for (i=X1;i>=X;i--){ //Alineacion de derecha a izquierda
				if(k>=0){
					c[i] = cadena [k];
				}else{ //Si se pasa del tamaÃ±o de la cadena sale del FOR
					break;
				}
				k--;
			}
		}
	}
}


//llenarMATRIX(5,2,arr_ano,"cadena"); //Nombre cliente
void llenarMATRIX(int arr_pos, char cadena[1000]){
//Fila X
//Columnas Y
// 0 =y
// 1 = X1
// 2 = X2
// 3 = Alineacion
	int X,X1,Y, Alineacion;
	int i, j, tam_cadena;

	Y = fac_nombre[arr_pos][0];
	X = fac_nombre[arr_pos][1];
	X1 = fac_nombre[arr_pos][2];
	Alineacion = fac_nombre[arr_pos][3];
	//Alieneacion 1 Izquierda -> Derecha
	//		   2 Derecha -> Izquierda1

	tam_cadena = strlen(cadena);
	j=0;
	if((Y >= 0) && (X >= 0)){
		if(Alineacion == 1){ //Alineacion de izquierda a derecha
			for (i=X;i<=X1;i++){
				if(j<tam_cadena){
					matrix[i][Y] = cadena [j];
				}else{ //Si se pasa del tamaÃ±o de la cadena sale del FOR
					break;
				}
				j++;
			}
		}else{

			j= strlen(cadena)-1;
			for (i=X1;i>=X;i--){ //Alineacion de derecha a izquierda
				if(j>=0){
					matrix[i][Y] = cadena [j];
				}else{ //Si se pasa del tamaÃ±o de la cadena sale del FOR
					break;
				}
				j--;
			}
		}
	}
}


void cortarletras(char c[255], int total){
	int i;
	char cTMP[255];
	strcpy(cTMP,"                                                                                                                            "); //Inicializacion
   	for(i=0;i<total;i++){
		if(i < strlen(c))
			cTMP[i] = c[i];
	}
	cTMP[i] = '\0';
	strcpy(c,cTMP);
}

/* EL CODIGO DEBE VENIR EN 12 CARACTERES*/
void genera_codigo_barras(char destino[18], char destino_num[30], char fuente[13])
{
	int i,j,k;
	int sumaImpar=0;
	int sumaPar=0;
	int digito=0;
	char digitoControlchar[2];

	/* Calculo del dígito de control*/	
	printf("Longitud de la cadena: %d\n",strlen(fuente));
	for(i = strlen(fuente); i >= 1; i--)
	{
		digito = (int)(fuente[i-1]);
		digito -= 48;
		if(i%2 != 0)
		{
			sumaPar += digito;
			printf("Se suma el digito en posicion impar: %d\n", digito);
		}
		else
		{
			sumaImpar += digito;
			printf("Se suma el digito en posicion par: %d\n", digito);
		}
	}
	digito = (sumaImpar*3) + sumaPar;
	
	int digitoControl = (10 - (digito % 10) ) % 10;
	sprintf(digitoControlchar,"%d\0",digitoControl);

	for(i=0;i<18;i++)
		destino[i]='\0';
	j=0;
	k=0;
	for(i=0;i<14;i++)
	{
		for(j=0;j<2;j++)
		{
			if(j % 2 == 0)
				destino_num[k]=fuente[i];
			else
				destino_num[k]=' ';
			k++;
		}
	}
	strcat(destino_num,digitoControlchar);

	destino[0]=29;
	destino[1]=107;
	destino[2]=67;	/* 2 = EAN13 */
	destino[3]=13;  

	strcat(destino, fuente);
	strcat(destino, digitoControlchar);
}

int imprimirticket(char *id_venta_char, char tipo[20], double pago_num, ...){
	int id_venta_num;
	va_list lst_param;
	char *VentaPrueba;
	int int_reimpresion;
	int corteFinal;
	gboolean imprime_codigo_barras = FALSE;

	/* Para leer archivos de texto*/
	char *cadena = NULL;
	long filesize;
	
	printf ("#####################################3 \n %s", id_venta_char);
	printf ("\n");
	
        int nX; //TamaÃ±o del papel de largo
/*        int numVenta; //Numero de venta*/
	int i, j, k,m;
	double peso_num=0;
	double cambio_num=0;
	double cambio_num_total=0;
	double piezas_num=0;
	double iva15=0;
	int num_articulos_int=0;
	char temp[9] = "         ";
	char temp2[30] = "                              ";
	char temp3[40] = "                                        ";
	char sql[1500], sql_articulos[1500], monto[14], subtotalVenta[14], iva15Venta[12], listaNombre[20], nombreEmpleado[20]; 
	char id_venta[20], cambio[12], kilos_bascula[10], pago[14], num_articulos[300], peso[300], pieza[300], totales[20], total_cierre_listado[40];
	char fechaTMP[11]; //Variable Temporal para separar las fechas
	char fecha_pagare[11];
	char vendedor[80];
	char cajero[200];
	char fechaCorte[200];
	char cajaCorte[200];
	char *archivoImpresion;
	char codigo_barras[18], codigo_barras_num[30];

	char cadconf[5];
	int err; //Variable de error de la consulta SQL


	//Facturas y Corte Caja
	char id_factura_interno[100];
	int bandera = 0;
	int bandera2 = 0;
	float fac_subtotal_num;
	float fac_total_num;
	float fac_iva_num;
	char fac_subtotal[100];
	char fac_total[100];
	char fac_iva[100];
	char fac_iva15[100];
	char fac_valor_char[100];
	double cantidad_letra_num; //CAntidad con letra NUMERO
	char num_tmp[25]; //Numero temporal
	float fac_cred_subtotal;
	float fac_cred_iva;
	float fac_cred_total;
	float fac_cont_subtotal;
	float fac_cont_iva;
	float fac_cont_total;
	float fac_credcont_subtotal;
	float fac_credcont_iva;
	float fac_credcont_total;

	float fac_cont_total_venta = 0;
	float fac_cred_total_venta = 0;


	//Temporales
	int num_numero_int = 0;
	float num_numero = 0;
	char cad_temporal[255];
	int logoc;
	char cad_temporal_fechas[2];

	//Corte Caja
	float corte_contado_num = 0;
	float corte_retiro_num = 0;
	float corte_dinero_caja_num = 0;
	float corte_peso=0;
	float corte_articulos=0;

	//Variables de la FACTURA
	int num_mayor_X = 0;
	int num_mayor_Y = 0;
	int arr_nombre = 0;
	int arr_num_factura = 0;
	int arr_num_venta = 0;
	int arr_num_cliente = 0;
	int arr_cuenta_contable = 0;
	int arr_cta_contable = 0;
	int arr_direccion = 0;
	int arr_ciudad = 0;
	int arr_colonia = 0;
	int arr_rfc = 0;
	int arr_dia = 0;
	int arr_mes = 0;
	int arr_ano = 0;
	int arr_cantidad_letra = 0;
	int arr_observaciones = 0 ;
	int arr_subtotal = 0;
	int arr_iva = 0;
	int arr_iva15 = 0;
	int arr_total = 0;
	int arr_cantidad_inicio = 0;
	int arr_descripcion_inicio = 0;
	int arr_p_unitario_inicio = 0;
	int arr_importe_inicio = 0;
	int arr_cantidad_inicio_tmp = 0; //Para qe no se pierda el valor despues de incrementar el ARRAY inicio
	int arr_descripcion_inicio_tmp = 0;
	int arr_p_unitario_inicio_tmp = 0;
	int arr_importe_inicio_tmp = 0;
	int arr_cantidad_fin = 0;
	int arr_descripcion_fin = 0;
	int arr_p_unitario_fin = 0;
	int arr_importe_fin = 0;
	int arr_fin_archivo = 0;

	//COBRANZA
	int arr_codigo_cliente  = 0;
	//arr_nombre; //YA EXISTE LA VARIABLE
	int arr_factura_ticket = 0;
	int arr_fecha_emision = 0;
	int arr_fecha_vencimiento = 0;
	int arr_relacion = 0;
	int arr_cargo  = 0;
	int arr_abono = 0;
	int arr_saldo = 0;


//CONFIGURACION IMPRESORA
	int arr_impresora_credito  = 0;
	int arr_impresora_contado  = 0;
	int arr_impresora_factura = 0;
	int arr_impresora_corte_caja  = 0;
	int arr_impresora_credito_abono  = 0;
	int arr_impresora_corte_all_caja  = 0;
	int arr_impresora_pedidos_paso1 = 0;

	char impresora_credito[50];
	char impresora_contado[50];
	char impresora_factura[50] ;
	char impresora_corte_caja[50];
	char impresora_credito_abono[50];
	char impresora_corte_all_caja[50];
	char impresora_pedidos_paso1[50];


//	int tam_seek;

	gchar *listatipos[18]; //Se guarda la consulta SQL
//MINIPRINTER EPSON
	static char salto[4];
	static char reversa[4];
	static char color_rojo[4];
	static char color_negro[4];
	static char defecto[4],cancela[4];
	static char subraya_s[4],negrita_grande[4],negrita_subraya[4],negrita_subraya1[4],negrita[4],negrita1[4],subraya_s1[4], negrita_media[4];
	static char alinea_d[4],alinea_i[4],alinea_c[4];
	static char tamano16[4],tamano1[4], tamano2[4];
	static char resetea[3],corta_papel[3];
	//IMPRESORA LX300 ESC/P2
	static char psalto[2], pcarro[2], psig_pag[2], psalto_l[3];
	static char pt_pagina[4], pmargen_a[4];
	static char pmargen_i[4], pmargen_d[4];
	static char pnegrita[3], pc_negrita[3];
	static char ptabulador[2];
	static char pcpi15[3],pcpi12[3],pcpi10[3];
	static char pcondensed[2], pc_condensed[2];
	static char espaciado[4];
	static char altura_codigo_barras[4];
	
	char servicio_formato[50];
	float servicio;
	
	char domicilio_envio[100]="";
	gchar *observaciones_pedido;
	
	MYSQL_RES *res_enviar_a;
	MYSQL_ROW row_enviar_a;
	/*Variable de Bloqueo del CAjon ##############################*/
	imprimiendo = TRUE;
	
	id_venta_num = atoi (id_venta_char);
	
	espaciado[0]=27;
	espaciado[1]=51;
	espaciado[2]=19;
	espaciado[3]='\0';

//ESC/P2
	psalto[0]=10;
	psalto[1]='\0';
	pcarro[0]=13;
	pcarro[1]='\0';
	psig_pag[0]=12;
	psig_pag[1]='\0';
	psalto_l[0]=13;
	psalto_l[1]=10;
	psalto_l[2]='\0';

	pcpi15[0]=27;
	pcpi15[1]='g';
	pcpi15[2]='\0';
	pcpi12[0]=27;
	pcpi12[1]='M';
	pcpi12[2]='\0';
	pcpi10[0]=27;
	pcpi10[1]='P';
	pcpi10[2]='\0';
	pcondensed[0]=15;
	pcondensed[1]='\0';
	pc_condensed[0]=18;
	pc_condensed[1]='\0';

	pt_pagina[0]=27;
	pt_pagina[1]='C';
	pt_pagina[2]=33;
	pt_pagina[3]='\0';
	pmargen_a[0]=27;
	pmargen_a[1]='N';
	pmargen_a[2]=2;
	pmargen_a[3]='\0';
	pmargen_i[0]=27;
	pmargen_i[1]='l';
	pmargen_i[2]=0;
	pmargen_i[3]='\0';
	pmargen_d[0]=27;
	pmargen_d[1]='Q';
	pmargen_d[2]=0;
	pmargen_d[3]='\0';

	pnegrita[0]=27;
	pnegrita[1]='E';
	pnegrita[2]='\0';
	pc_negrita[0]=27;
	pc_negrita[1]='F';
	pc_negrita[2]='\0';

	ptabulador[0]=9;
	ptabulador[1]='\0';

//MINIPRINTER
	tamano16[0] = 27;
	tamano16[1] = 33;
	tamano16[2] = 16;
	tamano16[3] = '\0';
	
	tamano1[0] = 27;
	tamano1[1] = 33;
	tamano1[2] = 1;
	tamano1[3] = '\0';
	
	tamano2[0] = 27;
	tamano2[1] = 33;
	tamano2[2] = 4;
	tamano2[3] = '\0';
	
	salto[0]=27;
	salto[1]=100;
	salto[2]=1;
	salto[3]='\0';
	
	color_rojo[0]=27;
	color_rojo[1]=114;
	color_rojo[2]=1;
	color_rojo[3]='\0';
	
	color_negro[0]=27;
	color_negro[1]=114;
	color_negro[2]=0;
	color_negro[3]='\0';
	
	reversa[0]=27;
	reversa[1]=101;
	reversa[2]=2;
	reversa[3]='\0';

	corta_papel[0]=27;
	corta_papel[1]=109;
	corta_papel[2]='\0';
	
	espaciado[0]=27;
	espaciado[1]=51;
	espaciado[2]=20;
	espaciado[3]='\0';

	alinea_d[0]=27;
	alinea_d[1]=97;
	alinea_d[2]=2;
	alinea_d[3]='\0';
	alinea_i[0]=27;
	alinea_i[1]=97;
	alinea_i[2]=0;
	alinea_i[3]='\0';
	alinea_c[0]=27;
	alinea_c[1]=97;
	alinea_c[2]=1;
	alinea_c[3]='\0';

	subraya_s[0]=27;
	subraya_s[1]=33;
	subraya_s[2]=128;
	subraya_s[3]='\0';
	negrita_grande[0]=27;
	negrita_grande[1]=33;
	negrita_grande[2]=24;
	negrita_grande[3]='\0';
	
	negrita_subraya[0]=27;
	negrita_subraya[1]=33;
	negrita_subraya[2]=136;
	negrita_subraya[3]='\0';
	
	negrita[0]=27;
	negrita[1]=33;
	negrita[2]=8;
	negrita[3]='\0';
	
	negrita1[0]=27;
	negrita1[1]=33;
	negrita1[2]=9;
	negrita1[3]='\0';
	
	negrita_media[0]=27;
	negrita_media[1]=33;
	negrita_media[2]=10;
	negrita_media[3]='\0';
	
	negrita_subraya1[0]=27;
	negrita_subraya1[1]=33;
	negrita_subraya1[2]=137;
	negrita_subraya1[3]='\0';
	subraya_s1[0]=27;
	subraya_s1[1]=33;
	subraya_s1[2]=129;
	subraya_s1[3]='\0';

	/* CODIGOS DE BARRAS*/
	altura_codigo_barras[0]=29;
	altura_codigo_barras[1]=104;
	altura_codigo_barras[2]=81;
	altura_codigo_barras[3]='\0';

	defecto[0]=27;
	defecto[1]=33;
	defecto[2]=1;
	defecto[3]='\0';
	cancela[0]=27;
	cancela[1]=33;
	cancela[2]=0;
	cancela[3]='\0';

	resetea[0]=27;
	resetea[1]=64;
	resetea[2]='\0';


	int anchoticket = 0;   //Bandera para saber si esta definido el ancho del ticket en el archivo de configuracion de impresoras.
	nX = 40; //Tamaño del ticket de ancho en caracteres


	// SE ACOMODA EL PAGO CON EL FORMATO DE MILES
	if(conecta_bd() == -1)
		printf("Error al conectarme a la base de datos: %s\n", mysql_error(&mysql));
	else
	{
		sprintf(sql, "SELECT FORMAT(%f, 2)", pago_num);
		printf ("%s\n",sql);
		err = mysql_query(&mysql, sql);
		if(err == 0)
		{
			resultado = mysql_store_result(&mysql);
			if(resultado)
			{
				if((row = mysql_fetch_row(resultado)))
					strcpy(pago, row[0]);
			}
		}
		else
			printf("Error pago_num: %s\n", mysql_error(&mysql));
	}
	mysql_close(&mysql);

	sprintf(id_venta, "%d", id_venta_num);
	//sprintf(pago, "%.2f", pago_num);

	//printf("Antes de la configuración de la impresora.\n");
	

/*CONFIGURACION DE LA IMPRESORA
*/
		fpt2 = fopen(ImpresoraConfig,"r");
		if(fpt2 == NULL){
			printf("\nERROR no se puede abrir el archivo de configuracion de las impresoras: %s\n", ImpresoraConfig);
			imprimiendo = FALSE;
			return (1);
		}else{
			m=1;
			impresora_nombre[0][0] = 0; //Inicializa el arreglo fac_nombre
			impresora_nombre[0][1] = 0;
			impresora_nombre[0][2] = 0;
			impresora_nombre[0][3] = 0;
			while(fgets(c, 255, fpt2) != NULL){
				strcat(c," ");
				strcpy(cadconf,"");
				j=0;
				k=0;
				printf("CADENA: %s",c);
				for(i=0;i<strlen(c);i++){
					if(c[i] != '#'){
						if(c[i] == ' ' || c[i] == '\t'){
							j=0;
							if(k==0){
								//Guarda la posicion de las variables.
								if(strcmp(cadconf,"credito") == 0) arr_impresora_credito = m;
								else if(strcmp(cadconf,"contado") == 0) arr_impresora_contado = m;
								else if(strcmp(cadconf,"factura") == 0) 	arr_impresora_factura = m;
								else if(strcmp(cadconf,"corte_caja") == 0) arr_impresora_corte_caja = m;
								else if(strcmp(cadconf,"credito_abono") == 0) arr_impresora_credito_abono = m;
								else if(strcmp(cadconf,"corte_all_caja") == 0) arr_impresora_corte_all_caja = m;
								else if(strcmp(cadconf,"pedidos_paso1") == 0) arr_impresora_pedidos_paso1 = m;
								else if(strcmp(cadconf,"anchoticket") == 0) anchoticket = 1;

								//printf("->nombre %s\n<-",cadconf);
							}else if(k==1){
								if(strcmp(cadconf,"serial") == 0) impresora_nombre[m][0] = 1;  //Serial
								else if(strcmp(cadconf,"paralelo") == 0) impresora_nombre[m][0] = 0; //Paralela
								else if(strcmp(cadconf,"remoto") == 0) impresora_nombre[m][0] = 2; //Red
								else if(anchoticket == 1) 
								{
									nX = atoi(cadconf);
									anchoticket = 0;
								}
								printf ("El ancho del ticket = %d\n", nX);
								//printf("->Paralelo (0) o Serial (1) %s\n<-",cadconf);
							}else if(k==2){
								impresora_nombre[m][1] = atoi(cadconf);
								//printf("->Puerto %s\n<-",cadconf);
							}
							strcpy(cadconf,"");
							k++;
						}else{
							cadconf[j] = c[i];
							cadconf[j+1] = '\0';
							j++;
						}
					}else{
						break;
					}
				}
				impresora_nombre[0][0] = 0;
				m++;
			}
			fclose(fpt2);
		}
	printf("Se temina de leel el archivo de configuración de impresoras...\n");
	//return(0);

	//IMPRESORA CREDITO
	strcpy(impresora_credito,"/dev/");
	if((impresora_nombre[arr_impresora_credito][0]) == 0)
		strcat(impresora_credito,"lp");
	else if((impresora_nombre[arr_impresora_credito][0]) == 1)
		strcat(impresora_credito,"ttyS");
	else
		strcpy(impresora_credito,"-d miniprinter -o raw -h 192.168.1.");
	printf("Configuracion Credito antes de concatenar: %s\n", impresora_credito);
	sprintf(cad_temporal,"%d",impresora_nombre[arr_impresora_credito][1]);
	strcat(impresora_credito,cad_temporal);

	//IMPRESORA CONTADO
	strcpy(impresora_contado,"/dev/");
	if((impresora_nombre[arr_impresora_contado][0]) == 0)
		strcat(impresora_contado,"lp");
	else if((impresora_nombre[arr_impresora_contado][0]) == 1)
		strcat(impresora_contado,"ttyS");
	else
		strcpy(impresora_contado,"-d miniprinter -o raw -h 192.168.1.");
	sprintf(cad_temporal,"%d",impresora_nombre[arr_impresora_contado][1]);
	strcat(impresora_contado,cad_temporal);

	//IMPRESORA FACTURA
	strcpy(impresora_factura,"/dev/");
	if((impresora_nombre[arr_impresora_factura][0]) == 0)
		strcat(impresora_factura,"lp");
	else if((impresora_nombre[arr_impresora_factura][0]) == 1)
		strcat(impresora_factura,"ttyS");
	else
		strcpy(impresora_factura,"-d miniprinter -o raw -h 192.168.1.");
	sprintf(cad_temporal,"%d",impresora_nombre[arr_impresora_factura][1]);
	strcat(impresora_factura,cad_temporal);

	//IMPRESORA CORTE CAJA
	strcpy(impresora_corte_caja,"/dev/");
	if((impresora_nombre[arr_impresora_corte_caja][0]) == 0)
		strcat(impresora_corte_caja,"lp");
	else if((impresora_nombre[arr_impresora_corte_caja][0]) == 1)
		strcat(impresora_corte_caja,"ttyS");
	else
		strcpy(impresora_corte_caja,"-d miniprinter -o raw -h 192.168.1.");
	sprintf(cad_temporal,"%d",impresora_nombre[arr_impresora_corte_caja][1]);
	strcat(impresora_corte_caja,cad_temporal);

	//IMPRESORA CREDITO ABONO
	strcpy(impresora_credito_abono,"/dev/");
	if((impresora_nombre[arr_impresora_credito_abono][0]) == 0)
		strcat(impresora_credito_abono,"lp");
	else if((impresora_nombre[arr_impresora_credito_abono][0]) == 1)
		strcat(impresora_credito_abono,"ttyS");
	else
		strcpy(impresora_credito_abono,"-d miniprinter -o raw -h 192.168.1.");
	sprintf(cad_temporal,"%d",impresora_nombre[arr_impresora_credito_abono][1]);
	strcat(impresora_credito_abono,cad_temporal);

	//IMPRESORA CORTE ALL CAJA
	strcpy(impresora_corte_all_caja,"/dev/");
	if((impresora_nombre[arr_impresora_corte_all_caja][0]) == 0)
		strcat(impresora_corte_all_caja,"lp");
	else if((impresora_nombre[arr_impresora_corte_all_caja][0]) == 1)
		strcat(impresora_corte_all_caja,"ttyS");
	else
		strcpy(impresora_corte_all_caja,"-d miniprinter -o raw -h 192.168.1.");
	sprintf(cad_temporal,"%d",impresora_nombre[arr_impresora_corte_all_caja][1]);
	strcat(impresora_corte_all_caja,cad_temporal);

	//IMPRESORA pedidos_paso1 Para el carnicero

	strcpy(impresora_pedidos_paso1,"/dev/");
	if((impresora_nombre[arr_impresora_pedidos_paso1][0]) == 0)
		strcat(impresora_pedidos_paso1,"lp");
	else if((impresora_nombre[arr_impresora_pedidos_paso1][0]) == 1)
		strcat(impresora_pedidos_paso1,"ttyS");
	else
		strcpy(impresora_pedidos_paso1,"-d miniprinter -o raw -h 192.168.1.");
	sprintf(cad_temporal,"%d",impresora_nombre[arr_impresora_pedidos_paso1][1]);
	strcat(impresora_pedidos_paso1,cad_temporal);

	printf("\ncredito: %s",impresora_credito);
	printf("\ncontado: %s",impresora_contado);
	printf("\nfactura: %s",impresora_factura);
	printf("\ncorte_caja: %s",impresora_corte_caja);
	printf("\ncredito_abono: %s",impresora_credito_abono);
	printf("\ncorte_all_caja: %s",impresora_corte_all_caja);
	printf("\npedidos_paso1: %s",impresora_pedidos_paso1);


/*
TERMINA LA CONFIGURACION DE LA IMPRESORA*/




	 if(strcmp(tipo, "factura") == 0){
	printf("FACTURA\n");

	//Es Factura
		fpt2 = fopen(FacturaConfig,"r");
		if(fpt2 == NULL){
			printf("\nERROR no se puede abrir el archivo de configuracion");
			imprimiendo = FALSE;
			return (1);
		}else{
			m=1;
			fac_nombre[0][0] = 0; //Inicializa el arreglo fac_nombre
			fac_nombre[0][1] = 0;
			fac_nombre[0][2] = 0;
			fac_nombre[0][3] = 0;
			while(fgets(c, 255, fpt2) != NULL){
				strcat(c," ");
				strcpy(cadconf,"");
				j=0;
				k=0;
				printf("%s",c);
				for(i=0;i<strlen(c);i++){
					if(c[i] != '#'){
						if(c[i] == ' '){
							j=0;
							if(k==0){
								//Guarda la posicion de las variables.
								if(strcmp(cadconf,"nombre") == 0) arr_nombre = m;
								else if(strcmp(cadconf,"direccion") == 0) arr_direccion = m;
								else if(strcmp(cadconf,"colonia") == 0) arr_colonia = m;
								else if(strcmp(cadconf,"ciudad") == 0) 	arr_ciudad = m;
								else if(strcmp(cadconf,"rfc") == 0) arr_rfc = m;
								else if(strcmp(cadconf,"dia") == 0) arr_dia = m;
								else if(strcmp(cadconf,"mes") == 0) arr_mes = m;
								else if(strcmp(cadconf,"ano") == 0) arr_ano = m;
								else if(strcmp(cadconf,"cantidad_letra") == 0) arr_cantidad_letra = m;
								else if(strcmp(cadconf,"observaciones") == 0) arr_observaciones = m;
								else if(strcmp(cadconf,"subtotal") == 0) 	arr_subtotal = m;
								else if(strcmp(cadconf,"iva") == 0) arr_iva = m;
								else if(strcmp(cadconf,"iva15") == 0) arr_iva15 = m;
								else if(strcmp(cadconf,"total") == 0) arr_total = m;
								else if(strcmp(cadconf,"cantidad_inicio") == 0) {
									arr_cantidad_inicio = m;
									arr_cantidad_inicio_tmp = m;
								}
								else if(strcmp(cadconf,"descripcion_inicio") == 0){
									arr_descripcion_inicio = m;
									arr_descripcion_inicio_tmp = m;
								}
								else if(strcmp(cadconf,"p_unitario_inicio") == 0){
									arr_p_unitario_inicio = m;
									arr_p_unitario_inicio_tmp = m;
								}
								else if(strcmp(cadconf,"importe_inicio") == 0){
									arr_importe_inicio = m;
									arr_importe_inicio_tmp = m;
								}
								else if(strcmp(cadconf,"cantidad_fin") == 0) arr_cantidad_fin = m;
								else if(strcmp(cadconf,"descripcion_fin") == 0) arr_descripcion_fin = m;
								else if(strcmp(cadconf,"p_unitario_fin") == 0) arr_p_unitario_fin = m;
								else if(strcmp(cadconf,"importe_fin") == 0) arr_importe_fin = m;
								else if(strcmp(cadconf,"fin_archivo") == 0) arr_fin_archivo = m;
								else if(strcmp(cadconf,"num_factura") == 0) arr_num_factura = m;
								else if(strcmp(cadconf,"num_venta") == 0) arr_num_venta = m;
								else if(strcmp(cadconf,"num_cliente") == 0) arr_num_cliente = m;
								else if(strcmp(cadconf,"cuenta_contable") == 0) arr_cta_contable = m;

								//printf("->nombre %s\n<-",cadconf);
							}else if(k==1){
								fac_nombre[m][0] = atoi(cadconf)-1;
								//printf("->Y %s\n<-",cadconf);
							}else if(k==2){
								fac_nombre[m][1] = atoi(cadconf)-1;
								//printf("->X1 %s\n<-",cadconf);
							}else if(k==3){
								fac_nombre[m][2] = atoi(cadconf)-1;
								//printf("->X2 %s\n<-",cadconf);
							}else if(k==4){
								fac_nombre[m][3] = atoi(cadconf);
								//printf("->ALINEACION %s\n<-",cadconf);
							}
							strcpy(cadconf,"");
							k++;
						}else{
							cadconf[j] = c[i];
							cadconf[j+1] = '\0';
							j++;
						}
					}else{
						break;
					}
				}

				fac_nombre[0][0] = 0;
				m++;

			}

			fclose(fpt2);
		}




//Buscar el Y mayor
			num_mayor_Y = fac_nombre[arr_nombre][0];
			if(fac_nombre[arr_direccion][0] > num_mayor_Y) num_mayor_Y = fac_nombre[arr_direccion][0];
			if(fac_nombre[arr_colonia][0] > num_mayor_Y) num_mayor_Y = fac_nombre[arr_colonia][0];
			if(fac_nombre[arr_ciudad][0] > num_mayor_Y) num_mayor_Y = fac_nombre[arr_ciudad][0];
			if(fac_nombre[arr_rfc][0] > num_mayor_Y) num_mayor_Y = fac_nombre[arr_rfc][0];
			if(fac_nombre[arr_dia][0] > num_mayor_Y) num_mayor_Y = fac_nombre[arr_dia][0];
			if(fac_nombre[arr_mes][0] > num_mayor_Y) num_mayor_Y = fac_nombre[arr_mes][0];
			if(fac_nombre[arr_ano][0] > num_mayor_Y) num_mayor_Y = fac_nombre[arr_ano][0];
			if(fac_nombre[arr_cantidad_letra][0] > num_mayor_Y) num_mayor_Y = fac_nombre[arr_cantidad_letra][0];
			if(fac_nombre[arr_observaciones][0] > num_mayor_Y) num_mayor_Y = fac_nombre[arr_observaciones][0];
			if(fac_nombre[arr_subtotal][0] > num_mayor_Y) num_mayor_Y = fac_nombre[arr_subtotal][0];
			if(fac_nombre[arr_iva][0] > num_mayor_Y) num_mayor_Y = fac_nombre[arr_iva][0];
			if(fac_nombre[arr_iva15][0] > num_mayor_Y) num_mayor_Y = fac_nombre[arr_iva15][0];
			if(fac_nombre[arr_total][0] > num_mayor_Y) num_mayor_Y = fac_nombre[arr_total][0];
			if(fac_nombre[arr_cantidad_inicio][0] > num_mayor_Y) num_mayor_Y = fac_nombre[arr_cantidad_inicio][0];
			if(fac_nombre[arr_descripcion_inicio][0] > num_mayor_Y) num_mayor_Y = fac_nombre[arr_descripcion_inicio][0];
			if(fac_nombre[arr_p_unitario_inicio][0] > num_mayor_Y) num_mayor_Y = fac_nombre[arr_p_unitario_inicio][0];
			if(fac_nombre[arr_importe_inicio][0] > num_mayor_Y) num_mayor_Y = fac_nombre[arr_importe_inicio][0];
			if(fac_nombre[arr_cantidad_fin][0] > num_mayor_Y) num_mayor_Y = fac_nombre[arr_cantidad_fin][0];
			if(fac_nombre[arr_descripcion_fin][0] > num_mayor_Y) num_mayor_Y = fac_nombre[arr_descripcion_fin][0];
			if(fac_nombre[arr_p_unitario_fin][0] > num_mayor_Y) num_mayor_Y = fac_nombre[arr_p_unitario_fin][0];
			if(fac_nombre[arr_importe_fin][0] > num_mayor_Y) num_mayor_Y = fac_nombre[arr_importe_fin][0];
			if(fac_nombre[arr_fin_archivo][0] > num_mayor_Y) num_mayor_Y = fac_nombre[arr_fin_archivo][0];
			if(fac_nombre[arr_num_factura][0] > num_mayor_Y) num_mayor_Y = fac_nombre[arr_num_factura][0];
			if(fac_nombre[arr_num_venta][0] > num_mayor_Y) num_mayor_Y = fac_nombre[arr_num_venta][0];
			if(fac_nombre[arr_num_cliente][0] > num_mayor_Y) num_mayor_Y = fac_nombre[arr_num_cliente][0];
			if(fac_nombre[arr_cta_contable][0] > num_mayor_Y) num_mayor_Y = fac_nombre[arr_cta_contable][0];


			//Fin de buscar el Y mayor

			//Buscar el X mayor
			num_mayor_X = fac_nombre[arr_nombre][2];
			if(fac_nombre[arr_direccion][2] > num_mayor_X) num_mayor_X = fac_nombre[arr_direccion][2];
			if(fac_nombre[arr_colonia][2] > num_mayor_X) num_mayor_X = fac_nombre[arr_colonia][2];
			if(fac_nombre[arr_ciudad][2] > num_mayor_X) num_mayor_X = fac_nombre[arr_ciudad][2];
			if(fac_nombre[arr_rfc][2] > num_mayor_X) num_mayor_X = fac_nombre[arr_rfc][2];
			if(fac_nombre[arr_dia][2] > num_mayor_X) num_mayor_X = fac_nombre[arr_dia][2];
			if(fac_nombre[arr_mes][2] > num_mayor_X) num_mayor_X = fac_nombre[arr_mes][2];
			if(fac_nombre[arr_ano][2] > num_mayor_X) num_mayor_X = fac_nombre[arr_ano][2];
			if(fac_nombre[arr_cantidad_letra][2] > num_mayor_X) num_mayor_X = fac_nombre[arr_cantidad_letra][2];
			if(fac_nombre[arr_observaciones][2] > num_mayor_X) num_mayor_X = fac_nombre[arr_observaciones][2];
			if(fac_nombre[arr_subtotal][2] > num_mayor_X) num_mayor_X = fac_nombre[arr_subtotal][2];
			if(fac_nombre[arr_iva][2] > num_mayor_X) num_mayor_X = fac_nombre[arr_iva][2];
			if(fac_nombre[arr_iva15][2] > num_mayor_X) num_mayor_X = fac_nombre[arr_iva15][2];
			if(fac_nombre[arr_total][2] > num_mayor_X) num_mayor_X = fac_nombre[arr_total][2];
			if(fac_nombre[arr_cantidad_inicio][2] > num_mayor_X) num_mayor_X = fac_nombre[arr_cantidad_inicio][2];
			if(fac_nombre[arr_descripcion_inicio][2] > num_mayor_X) num_mayor_X = fac_nombre[arr_descripcion_inicio][2];
			if(fac_nombre[arr_p_unitario_inicio][2] > num_mayor_X) num_mayor_X = fac_nombre[arr_p_unitario_inicio][2];
			if(fac_nombre[arr_importe_inicio][2] > num_mayor_X) num_mayor_X = fac_nombre[arr_importe_inicio][2];
			if(fac_nombre[arr_cantidad_fin][2] > num_mayor_X) num_mayor_X = fac_nombre[arr_cantidad_fin][2];
			if(fac_nombre[arr_descripcion_fin][2] > num_mayor_X) num_mayor_X = fac_nombre[arr_descripcion_fin][2];
			if(fac_nombre[arr_p_unitario_fin][2] > num_mayor_X) num_mayor_X = fac_nombre[arr_p_unitario_fin][2];
			if(fac_nombre[arr_importe_fin][2] > num_mayor_X) num_mayor_X = fac_nombre[arr_importe_fin][2];
			if(fac_nombre[arr_fin_archivo][2] > num_mayor_X) num_mayor_X = fac_nombre[arr_fin_archivo][2];
			if(fac_nombre[arr_num_factura][2] > num_mayor_X) num_mayor_X = fac_nombre[arr_num_factura][2];
			if(fac_nombre[arr_num_venta][2] > num_mayor_X) num_mayor_X = fac_nombre[arr_num_venta][2];
			if(fac_nombre[arr_num_cliente][2] > num_mayor_X) num_mayor_X = fac_nombre[arr_num_cliente][2];
			if(fac_nombre[arr_cta_contable][2] > num_mayor_X) num_mayor_X = fac_nombre[arr_cta_contable][2];

			//Fin de buscar el X mayor



			nfilas = num_mayor_X + 1;
			ncols = num_mayor_Y + 1;
			/*Reserva inicial de memoria */
			printf("X: %d\n",nfilas);
			printf("Y: %d\n",ncols);
			for(fila=0; fila<=nfilas; fila++){
				printf("Aqui si entra....\n");
				matrix[fila] = (char *) malloc (ncols * sizeof(char));
			}



			//mysql_init(&mysql);

			//if(!mysql_real_connect(&mysql, "192.168.0.3", "caja", "caja", "CarnesBecerra", 0, NULL, 0)){
			if(conecta_bd_2(&mysql) == -1){
				printf("\nNo me puedo conectar a la base de datos =(\n");
				imprimiendo = FALSE;
				return (1);
			}else{

				//Con ID FACTURA


//SELECT Venta_Factura.id_factura, Venta_Factura.fecha, Venta_Factura.observacion, Cliente.nombre, Cliente.domicilio, Cliente.ciudad_estado, Cliente.rfc, Venta_Factura_Relacion.num_factura FROM Venta_Factura, Venta, Cliente, Venta_Factura_Relacion WHERE Venta.id_venta = Venta_Factura_Relacion.id_venta AND Venta_Factura_Relacion.id_factura = Venta_Factura.id_factura AND Venta.id_cliente = Cliente.id_cliente AND Venta.id_venta = 1
				/*strcpy(sql,"SELECT Venta_Factura.id_factura, Venta_Factura.fecha, Venta_Factura.observacion, Cliente.nombre, Cliente.domicilio, Cliente.ciudad_estado, Cliente.rfc, Venta_Factura_Relacion.num_factura, Venta_Factura_Relacion.id_venta, Cliente.id_cliente, Cliente.cuenta_contable FROM Venta_Factura, Venta, Cliente, Venta_Factura_Relacion WHERE Venta.id_venta = Venta_Factura_Relacion.id_venta AND Venta_Factura_Relacion.id_factura = Venta_Factura.id_factura AND Venta.id_cliente = Cliente.id_cliente AND Venta.id_venta = ");
				strcat(sql, id_venta);
				strcat(sql, " ORDER BY Cliente.id_cliente");*/
				sprintf(sql,"SELECT Venta.fecha_factura, Venta.observacion, Cliente.nombre, CONCAT(Cliente.domicilio, REPEAT(' ', 57-LENGTH(Cliente.domicilio)), 'C.P.  ', Cliente.cp), Cliente.ciudad_estado, Cliente.rfc, Venta.num_factura, Venta.id_venta, Cliente.id_cliente, Cliente.cuenta_contable, Cliente.colonia FROM Venta, Cliente WHERE Venta.id_cliente = Cliente.id_cliente AND Venta.id_venta = %s ORDER BY Cliente.id_cliente",id_venta); 
				
				printf("El id de la venta: %s\n", id_venta);
				printf("El SQL: %s\n", sql);
				if((err = mysql_query(&mysql, sql)) != 0){
					printf("Error al consultar los tipos de documentos: %s\n", mysql_error(&mysql));
					imprimiendo = FALSE;
					return (1);
				}else{
					resultado = mysql_store_result(&mysql); //Saca la consulta SQL
						if(resultado){
							if (mysql_num_rows(resultado) > 0){
								while((row = mysql_fetch_row(resultado)))
								{
									fpt = fopen(TicketImpresion,"w+");
									if(fpt == NULL){
									        printf("\nNo pudo imprimir la factura %s",row[0]);
										printf("\nERROR no se puede abrir el archivo a imprimir");
										imprimiendo = FALSE;
										return (1);
									}else{
										inicializar(); //Inicializa MATRIX
										for(i=0; i < 12;i++) //Guarda todo el arreglo en listapos
											listatipos[i] =  row[i];

										//strcpy(id_factura_interno,listatipos[0]);

										//Separamos el aÃ±o
										fechaTMP[0] = listatipos[0][0];
										fechaTMP[1] = listatipos[0][1];
										fechaTMP[2] = listatipos[0][2];
										fechaTMP[3] = listatipos[0][3];
										fechaTMP[4] = '\0';
										llenarMATRIX(arr_ano,fechaTMP); //DIA
										//Separamos el mes
										fechaTMP[0] = listatipos[0][5];
										fechaTMP[1] = listatipos[0][6];
										fechaTMP[2] = '\0';
										llenarMATRIX(arr_mes,fechaTMP); //MES
										//Separamos el dia
										fechaTMP[0] = listatipos[0][8];
										fechaTMP[1] = listatipos[0][9];
										fechaTMP[2] = '\0';
										llenarMATRIX(arr_dia,fechaTMP); //DIA

										llenarMATRIX(arr_observaciones, listatipos[1]); //Observaciones

										llenarMATRIX(arr_num_cliente, listatipos[8]); //ID cliente
										llenarMATRIX(arr_cta_contable, listatipos[9]); //ID cliente
										llenarMATRIX(arr_nombre, listatipos[2]); //Nombre cliente
										llenarMATRIX(arr_direccion, listatipos[3]); //Direccion cliente
										llenarMATRIX(arr_colonia, listatipos[10]); //Colonia
										llenarMATRIX(arr_ciudad, listatipos[4]); //Ciudad y estado
										llenarMATRIX(arr_rfc, listatipos[5]); //RFC

										llenarMATRIX(arr_num_factura, listatipos[6]); //Numero de Factura
										llenarMATRIX(arr_num_venta, listatipos[7]); //Numero de la Venta

										//Con ID FACTURA



//							strcpy(sql,"SELECT /*FORMAT(*/Venta_Articulo.cantidad/*,3)*/, Articulo.nombre, /*FORMAT(*/Venta_Articulo.precio/*,2)*/ AS precio_IVA, /*FORMAT(*/Venta_Articulo.monto/*,2)*/ AS monto_IVA, /*FORMAT(*/Venta_Articulo.precio/*,2)*/ AS precio, /*FORMAT(*/Venta_Articulo.monto/*,2)*/ AS monto FROM Venta_Articulo, Articulo WHERE Venta_Articulo.id_articulo = Articulo.id_articulo AND Venta_Articulo.id_venta = ");

										sprintf(sql, "SELECT FORMAT(Venta_Articulo.cantidad,3), CONCAT(Articulo.nombre, REPEAT(' ', 75 - CHAR_LENGTH(Articulo.nombre) - CHAR_LENGTH(FORMAT(Articulo.porcentaje_iva * 100,2)) -1 ) , FORMAT(Articulo.porcentaje_iva * 100,2),'%%' ) , FORMAT(Venta_Articulo.precio,2) AS precio_IVA, FORMAT(Venta_Articulo.subtotal,2) AS monto_IVA, FORMAT(Venta_Articulo.precio,2) AS precio, FORMAT(Venta_Articulo.subtotal,2) AS monto FROM Venta_Articulo INNER JOIN Articulo ON Venta_Articulo.id_articulo=Articulo.id_articulo WHERE Venta_Articulo.id_venta = %s ORDER BY Venta_Articulo.id_venta_articulo", id_venta );
/*CONCAT(Articulo.nombre,
REPEAT(' ', 75 - CHAR_LENGTH(Articulo.nombre) - CHAR_LENGTH(FORMAT(Articulo.porcentaje_iva * 100,2)) -1 ) ,
FORMAT(Articulo.porcentaje_iva * 100,2)
,'%'
)     */
										
										printf("%s\n",sql);
										err = mysql_query(&mysql, sql);
										if(err != 0){
											printf("Error al consultar la venta de factura: %s\n", mysql_error(&mysql));
											imprimiendo = FALSE;
											return (1);
										}else{
											resultado2 = mysql_store_result(&mysql); //Saca la consulta SQL
											if(resultado2){
											
												fac_subtotal_num = 0;
												fac_total_num = 0;
												while((row2 = mysql_fetch_row(resultado2))){
													printf("---%s\n",row[0]);
													/*for(i=0; i < 6;i++) //Guarda todo el arreglo en listapos
														listatipos[i] =  row2[i];*/


/*													num_numero = atof(listatipos[0]);
													sprintf(fac_valor_char, "%.3f", num_numero);*/
													printf("Cantidad: %s\n", row2[0]);
													sprintf(fac_valor_char, "%s", row2[0]);
													llenarMATRIX(arr_cantidad_inicio, fac_valor_char); //Cantidad
													//llenarMATRIX(arr_cantidad_inicio, listatipos[0]);
													//sprintf(fac_valor_char, "%s", row2[1]);
													llenarMATRIX(arr_descripcion_inicio, row2[1] ); //Descripcion
													//iva15 += atof(row2[7]);
													/*num_numero = atof(listatipos[2]);
													sprintf(fac_valor_char, "%.2f", num_numero);*/
													sprintf(fac_valor_char, "%s", row2[2]);
													llenarMATRIX(arr_p_unitario_inicio, fac_valor_char); //P.Unitario
													//llenarMATRIX(arr_p_unitario_inicio, listatipos[2]);
													/*num_numero = atof(listatipos[3]);
													sprintf(fac_valor_char, "%.2f", num_numero);*/
													sprintf(fac_valor_char, "%s", row2[3]);
													llenarMATRIX(arr_importe_inicio,fac_valor_char); //Importe
													//llenarMATRIX(arr_importe_inicio,listatipos[3]);
/*													fac_subtotal_num = fac_subtotal_num + atof(listatipos[3]);
													fac_total_num = fac_total_num + atof(listatipos[3]);*/

													//Incrementa las posiciones en Y

													fac_nombre[arr_cantidad_inicio][0] ++;
													fac_nombre[arr_descripcion_inicio][0] ++;
													fac_nombre[arr_p_unitario_inicio][0] ++;
													fac_nombre[arr_importe_inicio][0] ++;
													if(fac_nombre[arr_cantidad_inicio][0] >= fac_nombre[arr_cantidad_fin][0]){
														printf("\n");
														printf("\nERROR AL GUARDAR EN EL ARCHIVO, SE SUPERO EL TAMAï¿œ DE LA CADENA PERMITIDA");
														printf("\n");
														//return (1);
														break;
													}
												}
												//sprintf(sql, "SELECT FORMAT(SUM(Venta_Articulo.monto),2) as total, SUM(Venta_Articulo.monto) as total_letra FROM Venta_Articulo WHERE id_venta = %s AND id_factura = %s ORDER BY id_venta_articulo", id_venta, id_factura_interno);
												sprintf(sql, "SELECT FORMAT( Venta.monto, 2) as total, Venta.monto as total_letra, FORMAT(Venta.iva,2), FORMAT(Venta.subtotal,2) FROM Venta INNER JOIN Venta_Articulo ON Venta_Articulo.id_venta = Venta.id_venta WHERE Venta.id_venta = %s GROUP BY Venta.id_venta", id_venta);
												err = mysql_query(&mysql, sql);
												if(err != 0){
													printf("Error al consultar la venta de factura: %s\n", mysql_error(&mysql));
													imprimiendo = FALSE;
													return (1);
												}else{
													resultado2 = mysql_store_result(&mysql); //Saca la consulta
													if(resultado2)
													{
														row = mysql_fetch_row(resultado2);
														//fac_iva_num = fac_total_num - fac_subtotal_num;
														fac_iva_num = 0;
														sprintf(fac_iva, "%.2f", fac_iva_num);
														sprintf(fac_iva15, "%s",row[2]);
														sprintf(fac_subtotal, "%s", row[3]);
														sprintf(fac_total, "%s", row[0]);
														sprintf(num_tmp, "%s", row[1]); //Convierte a cadena
													}
												}
												arr_cantidad_inicio = arr_cantidad_inicio_tmp;
												arr_descripcion_inicio = arr_descripcion_inicio_tmp;
												arr_p_unitario_inicio = arr_p_unitario_inicio_tmp;
												arr_importe_inicio = arr_importe_inicio_tmp;

												//printf("\nTOTAL: %s",peso);
												llenarMATRIX(arr_subtotal, fac_subtotal); //Importe SUBTOTAL
												llenarMATRIX(arr_iva, fac_iva); //Importe IVA
												llenarMATRIX(arr_iva15, fac_iva15); //Importe IVA15
												llenarMATRIX(arr_total, fac_total); //Importe TOTAL

												cantidad_letra_num = atof(num_tmp);

												traduce(cantidad_letra_num, retVar);
												printf("\nCANIDAD LETRA: %s ",retVar);
												llenarMATRIX(arr_cantidad_letra, retVar); //Importe Cantidad con letra

											}else{
												imprimiendo = FALSE;
												return (1);
											}
										}


									fputs(resetea, fpt);
									fputs(pt_pagina,fpt);
									fputs(pmargen_a,fpt);
									//fputs(pmargen_i,fpt);
									//fputs(pmargen_d,fpt);
									fputs(pcpi10,fpt);
									fputs(pcondensed,fpt);

									escribirsalida("");
									escribirsalida("archivo");

									fputs(psig_pag,fpt);
									fputs(resetea, fpt);
									fclose(fpt);
									//system("lpr -#1 impresion/impresiones-tmp.txt");
									//FACTURA
									//system("lpr -Pminiprinter impresion/impresiones-tmp.txt");
									if((impresora_nombre[arr_impresora_factura][0]) == 2)
									{
										strcpy(cad_temporal,"lp.cups ");
										strcat(cad_temporal,impresora_factura);
										strcat(cad_temporal," ");
										strcat(cad_temporal,TicketImpresion);
									}
									else
									{
										strcpy(cad_temporal,"cat ");
										strcat(cad_temporal,TicketImpresion);
										strcat(cad_temporal,"> ");
										strcat(cad_temporal,impresora_factura);
									}
									if (manda_imprimir (TicketImpresion,"factura") != 0)
									{
										Err_Info ("Error de impresion");
									}
									printf ("TEMP = %s\n",cad_temporal);
									//system(cad_temporal);
									mysql_close(&mysql); //Cierra conexion SQL
									imprimiendo = FALSE;
									return (1);
								}

								}
							}else{
								printf("No se pudo la factura....\n");
								imprimiendo = FALSE;
								return(1);

							}
						}else{
							imprimiendo = FALSE;
							return (1);
						}
				}
				mysql_close(&mysql); //Cierra conexion SQL
			}
			printf("\n\n\nCerrando la conexion a la BD\n\n\n");
			mysql_close(&mysql); //Cierra conexion SQL
			
	}else if(strcmp(tipo, "cfd") == 0){ //Es factura electronica

		fpt = fopen(TicketImpresion,"w");
		if(fpt == NULL){
			printf("\nERROR no se puede abrir el archivo a imprimir");
			imprimiendo = FALSE;
			return (1);
		}
		else
		{
		//Abre el encabezado de archivo
		fpt2 = fopen(TicketArriba,"r");
		if(fpt2 == NULL){
			printf("\nERROR no se puede abrir el archivo del encabezado");
			imprimiendo = FALSE;
			return (1);
		}else
		{
			imprimir(espaciado,nX);
			imprimir(alinea_c, nX);
			imprimir(negrita_grande,nX);
			imprimir("CARNES BECERRA",nX);
			imprimir(cancela,nX);
			imprimir(salto,nX);
			imprimir(salto,nX);
			imprimir(tamano1,nX);
			imprimir(alinea_i, nX);
			while(fgets(c, 255, fpt2) != NULL)
			{
				imprimir(c,nX);
			}
			fclose(fpt2);
			imprimir(salto,nX);
			imprimir(cancela,nX);
			imprimir(tamano1,nX);
			imprimir(alinea_i,nX);

			//sprintf(sql, "SELECT Venta.id_venta, DATE_FORMAT(Venta.fecha,\"%%d-%%m-%%Y\"), FORMAT(ROUND(Venta.monto,2),2), Venta_Factura_Relacion.id_venta, Venta.monto FROM Venta INNER JOIN Venta_Factura_Relacion ON Venta.id_venta = Venta_Factura_Relacion.id_venta WHERE Venta_Factura_Relacion.num_factura = %s AND Venta.cancelada = 'n' ORDER BY Venta.id_venta", id_venta);
				sprintf(sql,"SELECT factura.eRfc, factura.eCurp, factura.eNombre, factura.eCalle, factura.eNoexterior, factura.eNointerior, factura.eColonia, factura.eMunicipio, factura.eEstado, factura.ePais, factura.eCp, factura.rRfc, factura.rNombre, factura.rCalle,factura. rNoexterior, factura.rNointerior, factura.rColonia, factura.rMunicipio, factura.rEstado, factura.rPais, factura.rCp, DATE_FORMAT(factura.fecha, '%%d-%%m-%%Y') AS fecha, DATE_FORMAT(factura.fecha, '%%H:%%i:%%s') AS hora, factura.descuento, FORMAT(factura.subtotal,2), FORMAT(factura.impuesto,2), FORMAT(factura.total,2), factura.cadenaOriginal, factura.noCertificado, factura.sello, factura.estado, factura_folio.serie, factura_folio.folio, factura_folio.aprobacion, DATE_FORMAT(factura_folio.fecha,'%%Y') AS fechaAprobacion, factura.expCalle, factura.expNoexterior, factura.expNointerior, factura.expColonia, factura.expMunicipio, factura.expEstado, factura.expPais, factura.expCp, factura.observaciones, Venta.id_venta, Cliente.id_cliente, Cliente.cuenta_contable, factura.total, Venta.tipo, DATE_FORMAT(DATE_ADD(Venta.fecha,INTERVAL Cliente.vencimiento DAY),'%%d/%%m/%%Y') AS vencimiento FROM factura INNER JOIN factura_folio ON factura.id_factura_folio = factura_folio.id_factura_folio INNER JOIN Venta ON factura.id_factura = Venta.id_factura INNER JOIN Cliente ON Venta.id_cliente = Cliente.id_cliente WHERE factura.id_factura = '%s'", id_venta) ;
			//sprintf(sql, "SELECT Venta.id_venta, DATE_FORMAT(Venta.fecha,\"%%d-%%m-%%Y\"), FORMAT(ROUND(Venta.monto,2),2), Venta.monto FROM Venta WHERE Venta.num_factura = %s AND Venta.cancelada = 'n' ORDER BY Venta.id_venta", id_venta);
	        /*
	        0 = eRfc = 1
	        1 = eCurp = 2
	        2 = eNombre = 3
	        3- eCalle = 4
	        4- eNoexterior = 5
	        5- eNointerior = 6
	        6- eColonia = 7
	        7- eMunicipio = 8
	        8- eEstado = 9
	        9- ePais = 10
	        10- eCp = 11
	        11- rRfc = 12
	        12- rNombre = 13
	        13- rCalle = 14
	        14- rNoexterior = 15
	        15- rNointerior = 16
	        16- rColonia = 17
	        17- rMunicipio = 18
	        18- rEstado = 19
	        19- rPais = 20
	        20- rCp = 21
	        21- fecha = 22
	        22- hora = 23
	        23- descuento = 24
	        24- subtotal = 25
	        25- impuesto = 26
	        26- total (con formato)
	        27- cadenaOriginal = 28
	        28- noCertificado = 29
	        29- sello = 30
	        30- estado = 31
	        31- serie = 32
	        32- folio = 33
	        33- aprobacion = 34
	        34- fecha = 35
		35- expCalle
		36- expNoexterior
		37- expNointerior
		38- expColonia
		39- expMunicipio
		40- expEstado
		41- expPais
		42- expCp
		43- observaciones
		44- Folio Venta
		45- Numero de Cliente
		46- Cuenta contable
		47- Total (sin formato)
		48- Tipo de venta (credito, contado)
		49- Fecha de vencimiento
	        */
			printf("Consulta: %s\n", sql);

			if(conecta_bd() == 1)
			{
				err = mysql_query(&mysql, sql);
				if(err == 0)
				{
					resultado = mysql_store_result(&mysql);
					if(resultado)
					{
						imprimir(cancela, nX);
						imprimir(defecto, nX);
						imprimir(alinea_i, nX);
					    if((row=mysql_fetch_row(resultado)))
						{
							sprintf(c,"EXPEDIDO EN:");
							imprimir(c,nX);
							imprimir(salto,nX);
							sprintf(c,"%s %s %s  %s",row[35],row[36],row[37],row[38]);
							imprimir(c,nX);
							imprimir(salto,nX);
							sprintf(c,"%s, %s, %s  C.P. %s",row[39],row[40],row[41],row[42]);
							imprimir(c,nX);
							imprimir(salto,nX);
							imprimir(salto,nX);
						        sprintf(c,"FACTURA: %s-%s    Ticket: %s",row[31],row[32],row[44]);
							imprimir(negrita,nX);
						        imprimir(c,nX);
						        imprimir(salto,nX);
							imprimir(cancela, nX);
							imprimir(defecto, nX);
							imprimir(alinea_i, nX);
						        sprintf(c,"Fecha: %s  Hora: %s", row[21],row[22]);
						        imprimir(c,nX);
						        imprimir(salto,nX);
						        sprintf(c,"Cliente: %s", row[12]);
						        imprimir(c,nX);
						        imprimir(salto,nX);
						        sprintf(c,"%s",row[11]);
						        imprimir(c,nX);
						        imprimir(salto,nX);
						        sprintf(c,"%s %s %s %s", row[13],row[14],row[15],row[16]);
						        imprimir(c,nX);
						        imprimir(salto,nX);
						        sprintf(c, "%s, %s, %s, C.P. %s", row[17],row[18],row[19],row[20]);
						        imprimir(c,nX);
						        imprimir(salto,nX);
							sprintf(c,"No. cliente: %s || %s", row[45],row[46]);
						        imprimir(c,nX);
						        imprimir(salto,nX);
						        sprintf(c, "Articulo    Cant.  Precio Importe");
								imprimir(subraya_s,nX);
								imprimir(c,nX);
								imprimir(cancela,nX);
								imprimir(defecto,nX);
								imprimir(salto,nX);
								/*strcpy(c, row[1]);
								strncat(c, temp3, 12-strlen(row[0]));
								strcat(c, row[0]);
								strncat(c, temp3, 16-strlen(row[2]));
								strcat(c, row[2]);
								imprimir(c,nX);
								imprimir(salto,nX);
								sprintf(total_cierre_listado, "%f", atof(total_cierre_listado)+atof(row[3]));*/
						}
					    sprintf(sql, "SELECT factura_articulo.cantidad, IF( factura_articulo.impuesto =0, factura_articulo.descripcion, CONCAT( \"*\", factura_articulo.descripcion ) ) , FORMAT( factura_articulo.valorunitario, 2 ) , FORMAT( factura_articulo.importe, 2 ) , factura_articulo.unidad FROM factura_articulo WHERE factura_articulo.id_factura =%s ORDER BY factura_articulo.id_factura_articulo", id_venta);
						//imprimir(salto,nX);
						//sprintf(sql, "SELECT FORMAT(%s,2)",total_cierre_listado);
						err = mysql_query(&mysql, sql);
						if(err == 0)
						{
							resultado = mysql_store_result(&mysql);
							if(resultado)
							{
								while((row2=mysql_fetch_row(resultado)))
								{
								    strcpy(c,"");
									//Nombre Producto
									if(strlen(row2[1]) > 12)
									{
										imprimir(alinea_i, nX);
									   	sprintf(c, "%s%s", row2[1],salto);
									}
									else
									{
										imprimir(alinea_d, nX);
										sprintf(c, "%s", row2[1]);
										strncat(c, temp3, 13-strlen(row[0]));
									}

									imprimir(c,nX);
									//imprimir(salto,nX);
									strcpy(c, "");

									//strcpy(cad_temporal, row[1]);
									//Cantidad
									//if(strlen(row2[0]) > 12)
										sprintf(c, "%s%s%s", alinea_d,row2[0],row2[4]);
									//else
									//	sprintf(c, "%s%s", row2[0],row2[4]);

									strcat(c, " $");

									strcpy(cad_temporal, row2[2]);
									strncat(c, temp, 10-strlen(cad_temporal)); //Precio
									strcat(c, cad_temporal);

									strcat(c, " $");


									strcpy(cad_temporal, row2[3]);
									strncat(c, temp, 10-strlen(cad_temporal)); //Monto
									strcat(c, cad_temporal);

									imprimir(c,nX);
									imprimir(salto,nX);
									if(strcmp(row2[4],"pz")==0)
									{
										num_articulos_int++;
										piezas_num = piezas_num + atof(row2[0]);
									}
									else
									{
										num_articulos_int++;
										peso_num = peso_num + atof(row2[0]);
									}
								}
																	//strcpy(total_cierre_listado, row[0]);
									//imprimir(salto,nX);
									imprimir(alinea_d,nX);
									imprimir("---------------",nX);
									imprimir(salto,nX);
									sprintf(c,"Subtotal: $ %s", row[24]);
									imprimir(c,nX);
									imprimir(salto,nX);
									sprintf(c,"IVA: $ %s",row[25]);
									imprimir(c,nX);
									imprimir(salto,nX);
									imprimir(negrita_grande,nX);
									sprintf(c,"Total: $ %s",row[26]);
									imprimir(c,nX);
									imprimir(salto,nX);

									//Cantidad con letra
									sprintf(num_tmp,"%s",row[47]); 
									cantidad_letra_num = atof(num_tmp);
									traduce(cantidad_letra_num, retVar);
									printf("\nCANTIDAD LETRA: %s ",retVar);
									sprintf(c,"%s",retVar);
									imprimir(cancela,nX);
									imprimir(alinea_i,nX);
									imprimir(tamano1,nX);
									imprimir(c,nX);
									imprimir(salto,nX);
									imprimir(salto,nX);

									/*Codigo para agregar el total de articulos, piezas y kilos */
									strcpy(temp2,"");
									strcpy(c,"");
									strcpy(temp2,"Articulos: ");
									strncat(c,temp3,18-strlen(temp2));
									strcat(c, temp2);
									sprintf(num_articulos, "%d", num_articulos_int);

									strncat(c,temp3,4-strlen(num_articulos));
									strcat(c,num_articulos );

									imprimir(c,nX);
									imprimir(salto,nX);

									strcpy(temp2,"");
									strcpy(c,"");
									imprimir(alinea_i, nX);
									strcpy(temp2,"Peso: ");
									strncat(c,temp3,18-strlen(temp2));
									strcat(c, temp2);

									sprintf(peso, "%.3fkg", peso_num);
									strncat(c,temp3,10-strlen(peso));
									strcat(c,peso );
									strcat(c, salto);
									imprimir(c,nX);
				
				
									strcpy(temp2,"");
									strcpy(c,"");
									imprimir(alinea_i, nX);	
									strcpy(temp2,"Piezas: ");
									strncat(c,temp3,18-strlen(temp2));
									strcat(c, temp2);

									sprintf(pieza, "%.3f  ", piezas_num);
									strncat(c,temp3,10-strlen(pieza));
									strcat(c,pieza );
									strcat(c, salto);
									imprimir(c,nX);
									imprimir(salto,nX);
									/* Se agregaron los totales de articulos */
						
									sprintf(c,"OBSERVACIONES: %s",row[43]);
									imprimir(c,nX);
									imprimir(salto,nX);
									sprintf(c,"*Productos con IVA.");
									imprimir(c,nX);
									imprimir(salto,nX);
									imprimir(salto,nX);
									imprimir(alinea_c, nX);
									sprintf(c,"PAGO HECHO EN UNA SOLA EXHIBICION");
									imprimir(c,nX);
									imprimir(salto,nX);
									imprimir(cancela,nX);
									imprimir(alinea_i,nX);
									imprimir(tamano1,nX);
									sprintf(c,"---------------------------------------");
									imprimir(c,nX);
									imprimir(salto,nX);
									sprintf(c,"CERTIFICADO DIGITAL: %s",row[28]);
									imprimir(c,nX);
									imprimir(salto,nX);
									sprintf(c,"NUMERO DE APROBACION: %s",row[33]);
									imprimir(c,nX);
									imprimir(salto,nX);
									sprintf(c,"AÑO DE APROBACION: %s",row[34]);
									imprimir(c,nX);
									imprimir(salto,nX);
									sprintf(c,"Cadena original:");
									imprimir(negrita,nX);
									imprimir(c,nX);
									imprimir(salto,nX);
									imprimir(cancela,nX);
									imprimir(defecto, nX);
									sprintf(c,"%s",row[27]);
									imprimir(c,nX);
									imprimir(salto,nX);
									sprintf(c,"Sello digital:");
									imprimir(negrita,nX);
									imprimir(c,nX);
									imprimir(salto,nX);
									imprimir(cancela,nX);
									imprimir(defecto, nX);
									sprintf(c,"%s",row[29]);
									imprimir(c,nX);
									imprimir(salto,nX);
									sprintf(c,"---------------------------------------");
									imprimir(c,nX);
									imprimir(salto,nX);
									sprintf(c,"ESTE DOCUMENTO ES UNA REPRESENTACION IMPRESA DE UN CFD");
									imprimir(c,nX);
									imprimir(salto,nX);
									imprimir(resetea,nX);
							}
						}
						else
						{
							sprintf(Errores,"%s",mysql_error(&mysql));
							Err_Info(Errores);
						}
					
						
						/* Aqui va el pagaré en caso de que la venta sea de crédito */
						if(strcmp(row[48],"credito") == 0){
							fpt2 = NULL;
							fpt2 = fopen(TicketAbajoCredito,"r");
							if(fpt2 == NULL){
								printf("\nERROR no se puede abrir el archivo del pie de pagina");
								imprimiendo = FALSE;
								return (1);
							}else{
								fseek(fpt2,0,SEEK_END);
								//filesize = ftell(fpt2);
								rewind(fpt2);

								/*cadena = (char*) malloc (sizeof(char)*filesize);

								printf("Tamaño del archivo footer: %ld\n\n",filesize);
								if(cadena == NULL){
									printf("No se puede leer el archivo de TicketAbajo\n");
									free(cadena);
								}else{
									printf("Si se lee el archivo footer\n\n");
									if(fread(cadena, 1, filesize, fpt2) == filesize){
										cadena = str_replace(cadena, "<importe>", monto);
										cadena = str_replace(cadena, "<cliente>", listatipos[5]);
										cadena = str_replace(cadena, "<fecha>", fechaTMP);
										strcpy(c,cadena);
										imprimir(c,nX); //Funcion para imprimir y separa las letras
									}else
										printf("Error al leer el archivo footer\n\n");
									free(cadena);							
								}*/
								imprimir(espaciado,nX);
								imprimir(salto,nX);
								sprintf(c,"---------------------------------------");
								imprimir(c,nX);
								imprimir(salto,nX);
								i = 0;
								while(fgets(c, 40, fpt2) != NULL){
									imprimir(cancela,nX);
									imprimir(defecto,nX);
									if(strcmp(tipo,"contado") == 0)
									{
										imprimir(negrita,nX);
										imprimir(alinea_c,nX);
									}
									else
										imprimir(alinea_i, nX);
									//cadena = (char*) malloc (sizeof(char)c);
									cadena = c;
									sprintf(cad_temporal,"%s$%s%s",negrita1,row[26],tamano1);
									cadena = str_replace(cadena, "<importe>", cad_temporal);
									cadena = str_replace(cadena, "<cliente>", row[12]);
									sprintf(cad_temporal,"%s%s%s",negrita1,row[21],tamano1);
									cadena = str_replace(cadena, "<fecha>", cad_temporal);
									sprintf(cad_temporal,"%s%s%s",negrita1,row[49],tamano1);
									cadena = str_replace(cadena, "<fecha_vencimiento>", cad_temporal);
									//Cantidad con letra
									cantidad_letra_num = atof(row[47]);
									traduce(cantidad_letra_num, retVar);
									printf("\nCANTIDAD LETRA: %s ",retVar);
									cadena = str_replace(cadena, "<importe_letra>",retVar);

									strcpy(c,cadena);
									if(i == 0)
										imprimir(negrita,nX);
									imprimir(c,nX); //Funcion para imprimir y separa las letras
									i++;
								}
								imprimir(cancela,nX);
								imprimir(defecto,nX);
								fclose(fpt2);
							}
						}
					
					}
				}
				else
				{
					sprintf(Errores,"%s",mysql_error(&mysql));
					Err_Info(Errores);
				}
			}
			else
			{
				sprintf(Errores,"%s",mysql_error(&mysql));
				Err_Info(Errores);
			}
			mysql_close(&mysql);
		}
		imprimir(salto,nX);
		imprimir(alinea_d,nX);
		//imprimir(negrita1,nX);
		imprimir("Matica",nX);
		imprimir(salto,nX);
		imprimir(cancela,nX);
		imprimir(defecto,nX);
		imprimir(salto,nX);
		imprimir(salto,nX);
		imprimir(salto,nX);
		imprimir(salto,nX);
		imprimir(salto,nX);
		imprimir(salto,nX);
		printf("\n\n\n\n\n");
		imprimir(corta_papel,nX);

//		imprimir(c,nX);
		imprimir(cancela,nX);
		imprimir(defecto,nX);
		imprimir(resetea,nX);
		fclose(fpt);
		printf("\n\n\nIMPRIMIENDO....\n");
		//TICKET
		if((impresora_nombre[arr_impresora_contado][0]) == 2)
		{
			strcpy(cad_temporal,"lp.cups ");
			strcat(cad_temporal,impresora_contado);
			strcat(cad_temporal," ");
			strcat(cad_temporal,TicketImpresion);
		}
		else
		{
			strcpy(cad_temporal,"cat ");
			strcat(cad_temporal,TicketImpresion);
			strcat(cad_temporal,"> ");
			strcat(cad_temporal,impresora_contado);
		}
  		manda_imprimir (TicketImpresion,"factura");
		printf ("TEMP = %s\n",cad_temporal);
  		}

	}else if((strcmp(tipo, "credito") == 0) || (strcmp(tipo, "contado") == 0)){ //Es credito o contado
		//se obtiene el ultimo parametro enviado a la funcion para saber si es un ticket reimpreso

		//printf ("Se reimprime un tickect");
		
		int_reimpresion =0;
		
		va_start(lst_param, pago_num);
		int_reimpresion = va_arg(lst_param,int); 
		va_end(lst_param);
	
		printf("\nEl parametro de reimpresion: %d\n\n", int_reimpresion);
		
		if(int_reimpresion != 1)
		{
			va_start(lst_param, pago_num);
			//int_reimpresion = va_arg(lst_param,int);
			VentaPrueba = va_arg(lst_param,char*); 
			va_end(lst_param);
		}
		else
		{
			va_start(lst_param, pago_num);
                        int_reimpresion = va_arg(lst_param,int);
                        VentaPrueba = va_arg(lst_param,char*);
                        va_end(lst_param);
			//gbox ("Entra aqui")
			//VentaPrueba = "";
		}
		
		///va_end(lst_param);
		
		if (!VentaPrueba)
			VentaPrueba = "";

		//Info (VentaPrueba);
		
		if(strcmp(tipo, "credito") == 0){
			strcpy(TicketAbajo,TicketAbajoCredito);
		}
		else if (strcmp(tipo, "contado") == 0){ //es contado
			strcpy(TicketAbajo,TicketAbajoContado);
		}
		
		fpt = fopen(TicketImpresion,"w");
		if(fpt == NULL){
			printf("\nERROR no se puede abrir el archivo a imprimir");
			imprimiendo = FALSE;
			return (1);
		}else
		{
		//Abre el encabezado de archivo
		fpt2 = fopen(TicketArriba,"r");
		if(fpt2 == NULL){
			printf("\nERROR no se puede abrir el archivo del encabezado");
			imprimiendo = FALSE;
			return (1);
		}else{
			imprimir(resetea,nX);
			imprimir(defecto,nX);
			
			if (strcmp(VentaPrueba,"Pruebas") != 0 && strcmp(VentaPrueba,"SalidasVarias") != 0 /*&& strcmp(VentaPrueba,"VentaConsumo") != 0*/ )//se imprime si el ticket no es de pruebas
			{
				imprimir(espaciado,nX);
				imprimir(alinea_i, nX);
				while(fgets(c, 255, fpt2) != NULL){
					imprimir(c,nX);
				}
				fclose(fpt2);
				imprimir(salto,nX);
				imprimir(alinea_c,nX);
				imprimir(negrita_grande,nX);
				imprimir("CARNES BECERRA",nX);
				imprimir(salto,nX);
				imprimir(salto,nX);
				imprimir(cancela,nX);
			}
			imprimir(tamano1,nX);
			imprimir(alinea_i,nX);
			imprimir(espaciado, nX);
			//Inserta la informaciï¿œn del ticket
			if(conecta_bd() == -1){
				g_print("\nNo me puedo conectar a la base de datos\n");
				imprimiendo = FALSE;
				return (1);
			}else{
				sprintf(sql, "SELECT Pedido.id_pedido FROM Pedido WHERE Pedido.id_venta = %s", id_venta);
				err = mysql_query(&mysql, sql);
				if(err == 0)
				{
					resultado = mysql_store_result(&mysql);
					if(resultado)
					{
						if(mysql_num_rows(resultado) > 0)
						{
							if (strcmp(VentaPrueba,"Pruebas") == 0)
							{
								sprintf(sql, "SELECT Cliente_Envio.domicilio FROM Cliente_Envio INNER JOIN Cliente ON Cliente.id_cliente=Cliente_Envio.id_cliente INNER JOIN Pruebas ON Pruebas.id_cliente=Cliente.id_cliente WHERE Pruebas.id_venta = %s", id_venta);
							}
							else
							{
								if (strcmp(VentaPrueba,"VentaConsumo") == 0)
								{
									sprintf(sql, "SELECT Cliente_Envio.domicilio FROM Cliente_Envio INNER JOIN Cliente ON Cliente.id_cliente=Cliente_Envio.id_cliente INNER JOIN VentaConsumo ON VentaConsumo.id_cliente=Cliente.id_cliente WHERE VentaConsumo.id_venta = %s", id_venta);
								}
								else
								{
									if (strcmp(VentaPrueba,"SalidasVarias") == 0)
										sprintf(sql, "SELECT Cliente_Envio.domicilio FROM Cliente_Envio INNER JOIN Cliente ON Cliente.id_cliente=Cliente_Envio.id_cliente INNER JOIN SalidasVarias ON SalidasVarias.id_cliente=Cliente.id_cliente WHERE SalidasVarias.id_venta = %s", id_venta);
									else
										sprintf(sql, "SELECT Cliente_Envio.domicilio FROM Cliente_Envio INNER JOIN Cliente ON Cliente.id_cliente=Cliente_Envio.id_cliente INNER JOIN Venta ON Venta.id_cliente=Cliente.id_cliente WHERE Venta.id_venta = %s", id_venta);
								}
							}
							err = mysql_query(&mysql, sql);
							if(err == 0)
							{
								resultado = mysql_store_result(&mysql);
								if(resultado)
								{
									if(mysql_num_rows(resultado)>0)
									{
										if((row = mysql_fetch_row(resultado)))
										{
											if(strlen(row[0])>0)
											{
												if (strcmp(VentaPrueba,"Pruebas") == 0)
													sprintf(sql,"SELECT Pruebas.id_venta, Pruebas.fecha, Pruebas.hora, FORMAT(Pruebas.monto,2), LEFT(CONCAT( Usuario.nombre,  ' ', Usuario.apellido ),27)  AS cajero, LEFT(Cliente.nombre,38) AS cliente, Cliente_Envio.domicilio, Cliente_Envio.colonia, Cliente_Envio.entre_calles, Cliente.id_cliente, Cliente_Envio.ciudad_estado, FORMAT(%f - Pruebas.monto,2), FORMAT(Pruebas.subtotal,2), FORMAT(Pruebas.iva,2), Lista.nombre, Empleado.nombre, '' FROM Pruebas INNER JOIN Usuario ON Pruebas.id_usuario = Usuario.id_usuario INNER JOIN Cliente ON Pruebas.id_cliente = Cliente.id_cliente INNER JOIN Cliente_Envio ON Cliente.id_cliente = Cliente_Envio.id_cliente INNER JOIN Lista On Lista.id_lista = Cliente.id_lista INNER JOIN Empleado ON Empleado.id_empleado = Pruebas.id_empleado WHERE Pruebas.id_venta = %s GROUP BY Pruebas.id_venta", pago_num, id_venta);
												else
												if (strcmp(VentaPrueba,"VentaConsumo") == 0)
													sprintf(sql,"SELECT VentaConsumo.id_venta, VentaConsumo.fecha, VentaConsumo.hora, FORMAT(VentaConsumo.monto,2), LEFT(CONCAT( Usuario.nombre,  ' ', Usuario.apellido ),27)  AS cajero, LEFT(Cliente.nombre,38) AS cliente, Cliente_Envio.domicilio, Cliente_Envio.colonia, Cliente_Envio.entre_calles, Cliente.id_cliente, Cliente_Envio.ciudad_estado, FORMAT(%f - Pruebas.monto,2), FORMAT(VentaConsumo.subtotal,2), FORMAT(VentaConsumo.iva,2), Lista.nombre, Empleado.nombre, '' FROM VentaConsumo INNER JOIN Usuario ON VentaConsumo.id_usuario = Usuario.id_usuario INNER JOIN Cliente ON VentaConsumo.id_cliente = Cliente.id_cliente INNER JOIN Cliente_Envio ON Cliente.id_cliente = Cliente_Envio.id_cliente INNER JOIN Lista On Lista.id_lista = Cliente.id_lista INNER JOIN Empleado ON Empleado.id_empleado = VentaConsumo.id_empleado WHERE VentaConsumo.id_venta = %s GROUP BY VentaConsumo.id_venta", pago_num, id_venta);
												else
												if (strcmp(VentaPrueba,"SalidasVarias") == 0)
													sprintf(sql,"SELECT SalidasVarias.id_venta, SalidasVarias.fecha, SalidasVarias.hora, FORMAT(SalidasVarias.monto,2), LEFT(CONCAT( Usuario.nombre,  ' ', Usuario.apellido ),27)  AS cajero, LEFT(Cliente.nombre,38) AS cliente, Cliente_Envio.domicilio, Cliente_Envio.colonia, Cliente_Envio.entre_calles, Cliente.id_cliente, Cliente_Envio.ciudad_estado, FORMAT(%f - SalidasVarias.monto,2), FORMAT(SalidasVarias.subtotal,2), FORMAT(SalidasVarias.iva,2), Lista.nombre, Empleado.nombre, Concepto_SalidasVarias.concepto, Concepto_SalidasVarias.codigo_barras, '' FROM SalidasVarias INNER JOIN Usuario ON SalidasVarias.id_usuario = Usuario.id_usuario INNER JOIN Cliente ON SalidasVarias.id_cliente = Cliente.id_cliente INNER JOIN Cliente_Envio ON Cliente.id_cliente = Cliente_Envio.id_cliente INNER JOIN Concepto_SalidasVarias ON Concepto_SalidasVarias.id_concepto = SalidasVarias.id_concepto_salidasvarias INNER JOIN Lista On Lista.id_lista = Cliente.id_lista INNER JOIN Empleado ON Empleado.id_empleado = SalidasVarias.id_empleado WHERE SalidasVarias.id_venta = %s GROUP BY SalidasVarias.id_venta", pago_num, id_venta);
												else
													sprintf(sql,"SELECT Venta.id_venta, Venta.fecha, Venta.hora, FORMAT(Venta.monto,2), LEFT(CONCAT( Usuario.nombre,  ' ', Usuario.apellido ),27)  AS cajero, Cliente.nombre AS cliente, Cliente_Envio.domicilio, Cliente_Envio.colonia, Cliente_Envio.entre_calles, Cliente.id_cliente, Cliente_Envio.ciudad_estado, FORMAT(%f - Venta.monto,2), FORMAT(Venta.subtotal,2), FORMAT(Venta.iva,2), Lista.nombre, Empleado.nombre, DATE_FORMAT(DATE_ADD(Venta.fecha,INTERVAL Cliente.vencimiento DAY),'%%d/%%m/%%Y'), Venta.tipo_venta, Venta.monto  FROM Venta INNER JOIN Usuario ON Venta.id_usuario = Usuario.id_usuario INNER JOIN Cliente ON Venta.id_cliente = Cliente.id_cliente INNER JOIN Cliente_Envio ON Cliente.id_cliente = Cliente_Envio.id_cliente INNER JOIN Lista On Lista.id_lista = Cliente.id_lista LEFT JOIN Empleado ON Empleado.id_empleado = Venta.id_empleado WHERE Venta.id_venta = %s GROUP BY Venta.id_venta", pago_num, id_venta);
											}
											else
											{
												if (strcmp(VentaPrueba,"Pruebas") == 0)
													sprintf(sql,"SELECT Pruebas.id_venta, Pruebas.fecha, Pruebas.hora, FORMAT(Pruebas.monto,2), LEFT(CONCAT( Usuario.nombre,  ' ', Usuario.apellido ),27)  AS cajero, LEFT(Cliente.nombre,38) AS cliente, Cliente.domicilio, Cliente.colonia, Cliente.entre_calles, Cliente.id_cliente, Cliente.ciudad_estado, FORMAT(%f - Pruebas.monto,2), FORMAT(Pruebas.subtotal,2), FORMAT(Pruebas.iva,2), Lista.nombre, Empleado.nombre, '' FROM Pruebas INNER JOIN Usuario ON Pruebas.id_usuario = Usuario.id_usuario INNER JOIN Cliente ON Pruebas.id_cliente = Cliente.id_cliente INNER JOIN Lista On Lista.id_lista = Cliente.id_lista INNER JOIN Empleado ON Empleado.id_empleado = Pruebas.id_empleado WHERE Pruebas.id_venta = %s GROUP BY Pruebas.id_venta", pago_num, id_venta);
												else
												if (strcmp(VentaPrueba,"VentaConsumo") == 0)
													sprintf(sql,"SELECT VentaConsumo.id_venta, VentaConsumo.fecha, VentaConsumo.hora, FORMAT(VentaConsumo.monto,2), LEFT(CONCAT( Usuario.nombre,  ' ', Usuario.apellido ),27)  AS cajero, LEFT(Cliente.nombre,38) AS cliente, Cliente.domicilio, Cliente.colonia, Cliente.entre_calles, Cliente.id_cliente, Cliente.ciudad_estado, FORMAT(%f - VentaConsumo.monto,2), FORMAT(VentaConsumo.subtotal,2), FORMAT(VentaConsumo.iva,2), Lista.nombre, Empleado.nombre, '' FROM VentaConsumo INNER JOIN Usuario ON VentaConsumo.id_usuario = Usuario.id_usuario INNER JOIN Cliente ON VentaConsumo.id_cliente = Cliente.id_cliente INNER JOIN Lista On Lista.id_lista = Cliente.id_lista INNER JOIN Empleado ON Empleado.id_empleado = VentaConsumo.id_empleado WHERE VentaConsumo.id_venta = %s GROUP BY VentaConsumo.id_venta", pago_num, id_venta);
												else
												if (strcmp(VentaPrueba,"SalidasVarias") == 0)
													sprintf(sql,"SELECT SalidasVarias.id_venta, SalidasVarias.fecha, SalidasVarias.hora, FORMAT(SalidasVarias.monto,2), LEFT(CONCAT( Usuario.nombre,  ' ', Usuario.apellido ),27)  AS cajero, LEFT(Cliente.nombre,38) AS cliente, Cliente.domicilio, Cliente.colonia, Cliente.entre_calles, Cliente.id_cliente, Cliente.ciudad_estado, FORMAT(%f - SalidasVarias.monto,2), FORMAT(SalidasVarias.subtotal,2), FORMAT(SalidasVarias.iva,2), Lista.nombre, Empleado.nombre, Concepto_SalidasVarias.concepto, Concepto_SalidasVarias.codigo_barras, '' FROM SalidasVarias INNER JOIN Usuario ON SalidasVarias.id_usuario = Usuario.id_usuario INNER JOIN Cliente ON SalidasVarias.id_cliente = Cliente.id_cliente INNER JOIN Concepto_SalidasVarias ON Concepto_SalidasVarias.id_concepto = SalidasVarias.id_concepto_salidasvarias INNER JOIN Lista On Lista.id_lista = Cliente.id_lista INNER JOIN Empleado ON Empleado.id_empleado = SalidasVarias.id_empleado WHERE Usuario.id_usuario = SalidasVarias.id_usuario AND Cliente.id_cliente = SalidasVarias.id_cliente AND SalidasVarias.id_venta = %s GROUP BY SalidasVarias.id_venta", pago_num, id_venta);
												else
													sprintf(sql,"SELECT Venta.id_venta, Venta.fecha, Venta.hora, FORMAT(Venta.monto,2), LEFT(CONCAT( Usuario.nombre,  ' ', Usuario.apellido ),27)  AS cajero, Cliente.nombre AS cliente, Cliente.domicilio, Cliente.colonia, Cliente.entre_calles, Cliente.id_cliente, Cliente.ciudad_estado, FORMAT(%f - Venta.monto,2), FORMAT(Venta.subtotal,2), FORMAT(Venta.iva,2), Lista.nombre, Empleado.nombre, DATE_FORMAT(DATE_ADD(Venta.fecha,INTERVAL Cliente.vencimiento DAY),'%%d/%%m/%%Y'), Venta.tipo_venta, Venta.monto FROM Venta LEFT JOIN Usuario ON Venta.id_usuario = Usuario.id_usuario LEFT JOIN Cliente ON Venta.id_cliente = Cliente.id_cliente INNER JOIN Lista On Lista.id_lista = Cliente.id_lista LEFT JOIN Empleado ON Empleado.id_empleado = Venta.id_empleado WHERE Venta.id_venta = %s GROUP BY Venta.id_venta", pago_num, id_venta);
											}

										}
									}
									else
									{ 
										if (strcmp(VentaPrueba,"Pruebas") == 0)
											sprintf(sql,"SELECT Pruebas.id_venta, Pruebas.fecha, Pruebas.hora, FORMAT(Pruebas.monto,2), LEFT(CONCAT( Usuario.nombre,  ' ', Usuario.apellido ),27)  AS cajero, LEFT(Cliente.nombre,38) AS cliente, Cliente.domicilio, Cliente.colonia, Cliente.entre_calles, Cliente.id_cliente, Cliente.ciudad_estado, FORMAT(%f - Pruebas.monto,2), FORMAT(Pruebas.subtotal,2), FORMAT(Pruebas.iva,2), Lista.nombre, Empleado.nombre, '' FROM Pruebas INNER JOIN Usuario ON Pruebas.id_usuario = Usuario.id_usuario INNER JOIN Cliente ON Pruebas.id_cliente = Cliente.id_cliente INNER JOIN Lista On Lista.id_lista = Cliente.id_lista INNER JOIN Empleado ON Empleado.id_empleado = Pruebas.id_empleado WHERE Pruebas.id_venta = %s GROUP BY Pruebas.id_venta", pago_num, id_venta);
										else
										if (strcmp(VentaPrueba,"VentaConsumo") == 0)
											sprintf(sql,"SELECT VentaConsumo.id_venta, VentaConsumo.fecha, VentaConsumo.hora, FORMAT(VentaConsumo.monto,2), LEFT(CONCAT( Usuario.nombre,  ' ', Usuario.apellido ),27)  AS cajero, LEFT(Cliente.nombre,38) AS cliente, Cliente.domicilio, Cliente.colonia, Cliente.entre_calles, Cliente.id_cliente, Cliente.ciudad_estado, FORMAT(%f - VentaConsumo.monto,2), FORMAT(VentaConsumo.subtotal,2), FORMAT(VentaConsumo.iva,2), Lista.nombre, Empleado.nombre, '' FROM VentaConsumo INNER JOIN Usuario ON VentaConsumo.id_usuario = Usuario.id_usuario INNER JOIN Cliente ON VentaConsumo.id_cliente = Cliente.id_cliente INNER JOIN Lista On Lista.id_lista = Cliente.id_lista INNER JOIN Empleado ON Empleado.id_empleado = VentaConsumo.id_empleado WHERE VentaConsumo.id_venta = %s GROUP BY VentaConsumo.id_venta", pago_num, id_venta);
										else
										if (strcmp(VentaPrueba,"SalidasVarias") == 0)
											sprintf(sql,"SELECT SalidasVarias.id_venta, SalidasVarias.fecha, SalidasVarias.hora, FORMAT(SalidasVarias.monto,2), LEFT(CONCAT( Usuario.nombre,  ' ', Usuario.apellido ),27)  AS cajero, LEFT(Cliente.nombre,38) AS cliente, Cliente.domicilio, Cliente.colonia, Cliente.entre_calles, Cliente.id_cliente, Cliente.ciudad_estado, FORMAT(%f - SalidasVarias.monto,2), FORMAT(SalidasVarias.subtotal,2), FORMAT(SalidasVarias.iva,2), Lista.nombre, Empleado.nombre, Concepto_SalidasVarias.concepto, Concepto_SalidasVarias.codigo_barras, '' FROM SalidasVarias INNER JOIN Usuario ON SalidasVarias.id_usuario = Usuario.id_usuario INNER JOIN Cliente ON SalidasVarias.id_cliente = Cliente.id_cliente INNER JOIN Concepto_SalidasVarias ON Concepto_SalidasVarias.id_concepto = SalidasVarias.id_concepto_salidasvarias INNER JOIN Lista On Lista.id_lista = Cliente.id_lista INNER JOIN Empleado ON Empleado.id_empleado = SalidasVarias.id_empleado WHERE SalidasVarias.id_venta = %s GROUP BY SalidasVarias.id_venta", pago_num, id_venta);
										else
											sprintf(sql,"SELECT Venta.id_venta, Venta.fecha, Venta.hora, FORMAT(Venta.monto,2), LEFT(CONCAT( Usuario.nombre,  ' ', Usuario.apellido ),27)  AS cajero, Cliente.nombre AS cliente, Cliente.domicilio, Cliente.colonia, Cliente.entre_calles, Cliente.id_cliente, Cliente.ciudad_estado, FORMAT(%f - Venta.monto,2), FORMAT(Venta.subtotal,2), FORMAT(Venta.iva,2), Lista.nombre, Empleado.nombre, DATE_FORMAT(DATE_ADD(Venta.fecha,INTERVAL Cliente.vencimiento DAY),'%%d/%%m/%%Y'), Venta.tipo_venta, Venta.monto FROM Venta LEFT JOIN Usuario ON Venta.id_usuario = Usuario.id_usuario LEFT JOIN Cliente ON Venta.id_cliente = Cliente.id_cliente INNER JOIN Lista On Lista.id_lista = Cliente.id_lista LEFT JOIN Empleado ON Empleado.id_empleado = Venta.id_empleado WHERE Venta.id_venta = %s GROUP BY Venta.id_venta", pago_num, id_venta);
									}
								}
							}
							else
								printf("Error: %s", mysql_error(&mysql));
						}
						else 
							if (strcmp(VentaPrueba,"Pruebas") == 0)
								sprintf(sql,"SELECT Pruebas.id_venta, Pruebas.fecha, Pruebas.hora, FORMAT(Pruebas.monto,2), LEFT(CONCAT( Usuario.nombre,  ' ', Usuario.apellido ),27)  AS cajero, LEFT(Cliente.nombre,38) AS cliente, Cliente.domicilio, Cliente.colonia, Cliente.entre_calles, Cliente.id_cliente, Cliente.ciudad_estado, FORMAT(%f - Pruebas.monto,2), FORMAT(Pruebas.subtotal,2), FORMAT(Pruebas.iva,2), Lista.nombre, Empleado.nombre, '' FROM Pruebas INNER JOIN Usuario ON Pruebas.id_usuario = Usuario.id_usuario INNER JOIN Cliente ON Pruebas.id_cliente = Cliente.id_cliente INNER JOIN Lista On Lista.id_lista = Cliente.id_lista INNER JOIN Empleado ON Empleado.id_empleado = Pruebas.id_empleado WHERE Pruebas.id_venta = %s GROUP BY Pruebas.id_venta", pago_num, id_venta);
							else
							if (strcmp(VentaPrueba,"VentaConsumo") == 0)
								sprintf(sql,"SELECT VentaConsumo.id_venta, VentaConsumo.fecha, VentaConsumo.hora, FORMAT(VentaConsumo.monto,2), LEFT(CONCAT( Usuario.nombre,  ' ', Usuario.apellido ),27)  AS cajero, LEFT(Cliente.nombre,38) AS cliente, Cliente.domicilio, Cliente.colonia, Cliente.entre_calles, Cliente.id_cliente, Cliente.ciudad_estado, FORMAT(%f - VentaConsumo.monto,2), FORMAT(VentaConsumo.subtotal,2), FORMAT(VentaConsumo.iva,2), Lista.nombre, Empleado.nombre, '' FROM VentaConsumo INNER JOIN Usuario ON VentaConsumo.id_usuario = Usuario.id_usuario INNER JOIN Cliente ON VentaConsumo.id_cliente = Cliente.id_cliente INNER JOIN Lista On Lista.id_lista = Cliente.id_lista INNER JOIN Empleado ON Empleado.id_empleado = VentaConsumo.id_empleado WHERE VentaConsumo.id_venta = %s GROUP BY VentaConsumo.id_venta", pago_num, id_venta);
							else
							if (strcmp(VentaPrueba,"SalidasVarias") == 0)
								sprintf(sql,"SELECT SalidasVarias.id_venta, SalidasVarias.fecha, SalidasVarias.hora, FORMAT(SalidasVarias.monto,2), LEFT(CONCAT( Usuario.nombre,  ' ', Usuario.apellido ),27)  AS cajero, LEFT(Cliente.nombre,38) AS cliente, Cliente.domicilio, Cliente.colonia, Cliente.entre_calles, Cliente.id_cliente, Cliente.ciudad_estado, FORMAT(%f - SalidasVarias.monto,2), FORMAT(SalidasVarias.subtotal,2), FORMAT(SalidasVarias.iva,2), Lista.nombre, Empleado.nombre, Concepto_SalidasVarias.concepto, Concepto_SalidasVarias.codigo_barras, '' FROM SalidasVarias INNER JOIN Usuario ON SalidasVarias.id_usuario = Usuario.id_usuario INNER JOIN Cliente ON SalidasVarias.id_cliente = Cliente.id_cliente INNER JOIN Concepto_SalidasVarias ON Concepto_SalidasVarias.id_concepto = SalidasVarias.id_concepto_salidasvarias INNER JOIN Lista On Lista.id_lista = Cliente.id_lista INNER JOIN Empleado ON Empleado.id_empleado = SalidasVarias.id_empleado WHERE SalidasVarias.id_venta = %s GROUP BY SalidasVarias.id_venta", pago_num, id_venta);
							else
								sprintf(sql,"SELECT Venta.id_venta, Venta.fecha, Venta.hora, FORMAT(Venta.monto,2), LEFT(CONCAT( Usuario.nombre,  ' ', Usuario.apellido ),27)  AS cajero, Cliente.nombre AS cliente, Cliente.domicilio, Cliente.colonia, Cliente.entre_calles, Cliente.id_cliente, Cliente.ciudad_estado, FORMAT(%f - Venta.monto,2), FORMAT(Venta.subtotal,2), FORMAT(Venta.iva,2), Lista.nombre, Empleado.nombre, DATE_FORMAT(DATE_ADD(Venta.fecha,INTERVAL Cliente.vencimiento DAY),'%%d/%%m/%%Y'), Venta.tipo_venta, Venta.monto FROM Venta LEFT JOIN Usuario ON Venta.id_usuario = Usuario.id_usuario LEFT JOIN Cliente ON Venta.id_cliente = Cliente.id_cliente INNER JOIN Lista On Lista.id_lista = Cliente.id_lista LEFT JOIN Empleado ON Empleado.id_empleado = Venta.id_empleado WHERE Venta.id_venta = %s GROUP BY Venta.id_venta", pago_num, id_venta);

					}
				}
			
				

				printf("%s\n", sql);
				err = mysql_query(&mysql, sql);

				if(err != 0){
					printf("Error al consultar los tipos de documentos: %s\n", mysql_error(&mysql));
					imprimiendo = FALSE;
					return (1);
				}else{
					resultado = mysql_store_result(&mysql);
					if(resultado){
						
						if (strcmp(VentaPrueba,"") == 0)
							sprintf(sql, " SELECT tipo_pago FROM Venta WHERE id_venta = %s ", id_venta);
						else if(strcmp(VentaPrueba, "SalidasVarias") == 0)
							sprintf(sql," SELECT SalidasVarias.tipo_pago, Concepto_SalidasVarias.codigo_barras FROM SalidasVarias INNER JOIN Concepto_SalidasVarias ON SalidasVarias.id_concepto_salidasvarias = Concepto_SalidasVarias.id_concepto WHERE SalidasVarias.id_venta = %s",id_venta);
						else
							sprintf(sql, " SELECT tipo_pago FROM %s WHERE id_venta = %s ",VentaPrueba, id_venta);
						
						printf("%s\n", sql);
						err = mysql_query(&mysql, sql);
						if(err != 0){
							printf("Error al consultar los tipos de documentos: %s\n", mysql_error(&mysql));
							imprimiendo = FALSE;
							return (1);
						}else{
							resultado2 = mysql_store_result(&mysql);
							if(resultado2){
								row2 = mysql_fetch_row(resultado2);
								sprintf(tipoPago, "%s", row2[0]);
								if(strcmp(VentaPrueba,"SalidasVarias") == 0)
								{
									printf("\n** Lleva codigo de barras: ");
									if(strcmp(row2[1],"s") == 0)
									{
										imprime_codigo_barras = TRUE;
										printf(" SI  **\n\n");
									}
								}
							}
						}

						if((row = mysql_fetch_row(resultado))){
							if (mysql_num_rows(resultado) > 0){
								for(i=0; i < 19;i++) //Guarda todo el arreglo en listapos
									if(row[i])
										listatipos[i] =  row[i];
								sprintf(monto,          "%s", listatipos[3]);
								sprintf(cambio,         "%s", listatipos[11]);
								sprintf(subtotalVenta,  "%s", listatipos[12]);
								sprintf(iva15Venta,     "%s", listatipos[13]);
								//sprintf(nombreEmpleado, "%s", listatipos[15]);
								//printf ("### IVA %s \n", listatipos[13]);
								
								sprintf(c, "Fecha: %c%c/%c%c/%c%c%c%c       Hora: %s",listatipos[1][8],listatipos[1][9],listatipos[1][5],listatipos[1][6],listatipos[1][0],listatipos[1][1],listatipos[1][2],listatipos[1][3],listatipos[2]);
								sprintf(fecha_pagare,"%c%c/%c%c/%c%c%c%c",listatipos[1][8],listatipos[1][9],listatipos[1][5],listatipos[1][6],listatipos[1][0],listatipos[1][1],listatipos[1][2],listatipos[1][3]);
								imprimir(c,nX);
								imprimir(salto,nX);
								if (strcmp(VentaPrueba,"Pruebas") != 0 && strcmp(VentaPrueba,"SalidasVarias") != 0 && strcmp(VentaPrueba,"VentaConsumo") != 0)
								{
									sprintf(c, "Folio interno: %s\n", listatipos[0]);
									imprimir(c,nX);
								}
								else
								{
									if (strcmp(VentaPrueba,"VentaConsumo") == 0)
									{
										sprintf(c, "VENTA A CONSUMO\n", listatipos[0]);
										imprimir(c,nX);
										//imprimir(salto,nX);
										sprintf(c, "Folio interno: %s\n", listatipos[0]);
										imprimir(c,nX);
									}
									else
									{
										if (strcmp(VentaPrueba,"SalidasVarias") == 0)
										{
											sprintf(c, "Concepto: %s\n", listatipos[16]);
											imprimir(c,nX);
										}
										sprintf(c, "[%s]\n", listatipos[0]);
										imprimir(c,nX);
									}
								}
								if (strcmp(VentaPrueba,"Pruebas") != 0 && strcmp(VentaPrueba,"SalidasVarias") != 0)
								{
								sprintf(c, "Cajero(a): %s", listatipos[4]);
								imprimir(c,nX);
								imprimir(salto,nX);
								
								sprintf(c, "Empleado: %s", listatipos[15]);
								imprimir(c,nX);
								imprimir(salto,nX);								}
								/*imprimir(negrita,nX);
								imprimir(alinea_c,nX);
								sprintf(c, "%s\n",listatipos[14]);
								imprimir(c,nX);
								imprimir(salto,nX);
								imprimir(tamano1,nX);
								imprimir(alinea_i,nX);*/
								
								
								sprintf(c, "Cliente: %s\n",listatipos[9]);
								imprimir(c,nX);
								sprintf(c, "%s", listatipos[5]);
								imprimir(c,nX);
								imprimir(salto,nX);
								if(strlen(listatipos[6]) != 0 ){ //Domicilio
									sprintf(c, "%s\n", listatipos[6]);
									imprimir(c,nX);
								}
								if(strlen(listatipos[7]) != 0 ){ //Colonia
									sprintf(c, "%s\n", listatipos[7]);
									imprimir(c,nX);
								}
								if(strlen(listatipos[10]) != 0 ){ //Ciudad Estado
									sprintf(c, "%s\n", listatipos[10]); imprimir(c,nX);
								}
								if(strlen(listatipos[8]) != 0 ){ //Entre calles
									sprintf(c, "Entre: %s\n", listatipos[8]); imprimir(c,nX);
								}
							}else{
								imprimiendo = FALSE;
								return (1);
							}
						}else{
							imprimiendo = FALSE;
							return (1);
						}
					}else{
						imprimiendo = FALSE;
						return (1);
					}
				}
				strcpy(c, "\n");
				fputs(c,fpt);
				
				if ( int_reimpresion == 1 ) //SE IMPRIME LA LEYENDA DE REIMPRESION
				{
					imprimir("############# REIMPRESION #############",nX);
					imprimir(salto,nX);
					imprimir(salto,nX);
				}
				
				if (strcmp(VentaPrueba,"Pruebas") == 0)
					sprintf(sql,"SELECT CONCAT(LEFT(Articulo.nombre,28), ' ', FORMAT(Pruebas_Articulo.porcentaje_iva*100,2), '%%' ), FORMAT(Pruebas_Articulo.cantidad,3), FORMAT(Pruebas_Articulo.precio,2), FORMAT(Pruebas_Articulo.subtotal,2), Articulo.tipo, Pruebas_Articulo.cantidad FROM Pruebas_Articulo, Articulo WHERE Articulo.id_articulo = Pruebas_Articulo.id_articulo AND Pruebas_Articulo.id_venta = %s", id_venta);
				else
				if (strcmp(VentaPrueba,"VentaConsumo") == 0)
					sprintf(sql,"SELECT CONCAT(LEFT(Articulo.nombre,28), ' ', FORMAT(VentaConsumo_Articulo.porcentaje_iva*100,2), '%%' ), FORMAT(VentaConsumo_Articulo.cantidad,3), FORMAT(VentaConsumo_Articulo.precio,2), FORMAT(VentaConsumo_Articulo.subtotal,2), Articulo.tipo, VentaConsumo_Articulo.cantidad FROM VentaConsumo_Articulo, Articulo WHERE Articulo.id_articulo = VentaConsumo_Articulo.id_articulo AND VentaConsumo_Articulo.id_venta = %s", id_venta);
				else
				if (strcmp(VentaPrueba,"SalidasVarias") == 0)
					sprintf(sql,"SELECT CONCAT(LEFT(Articulo.nombre,28), ' ', FORMAT(SalidasVarias_Articulo.porcentaje_iva*100,2), '%%'), FORMAT(SalidasVarias_Articulo.cantidad,3), FORMAT(SalidasVarias_Articulo.precio,2), FORMAT(SalidasVarias_Articulo.subtotal,2), Articulo.tipo, SalidasVarias_Articulo.cantidad, CONCAT(Subproducto.codigo,Articulo.codigo), CONCAT('9',Subproducto.codigo,Articulo.codigo, LPAD(TRUNCATE(SalidasVarias_Articulo.cantidad,0),3,'000'), RPAD(TRUNCATE(MOD(SalidasVarias_Articulo.cantidad,1)*100,0),2,'00'),RIGHT(SalidasVarias.id_empleado,2)) FROM SalidasVarias_Articulo INNER JOIN Articulo ON Articulo.id_articulo = SalidasVarias_Articulo.id_articulo INNER JOIN Subproducto ON Articulo.id_subproducto = Subproducto.id_subproducto INNER JOIN SalidasVarias ON SalidasVarias_Articulo.id_venta = SalidasVarias.id_venta WHERE SalidasVarias_Articulo.id_venta = %s", id_venta);
				else
					sprintf(sql,"SELECT CONCAT(LEFT(Articulo.nombre,28), ' ', FORMAT(Venta_Articulo.porcentaje_iva*100,2), '%%'), FORMAT(Venta_Articulo.cantidad,3), FORMAT(Venta_Articulo.precio,2), FORMAT(Venta_Articulo.subtotal,2), Articulo.tipo, Venta_Articulo.cantidad FROM Venta_Articulo, Articulo WHERE Articulo.id_articulo = Venta_Articulo.id_articulo AND Venta_Articulo.id_venta = %s", id_venta);

				strcpy(sql_articulos,sql);
				printf("\n\nEl SQL para los articulos: %s\n\n",sql);
				err = mysql_query(&mysql, sql);
				if(err != 0){
					printf("Error al consultar los tipos de documentos: %s\n", mysql_error(&mysql));
					imprimiendo = FALSE;
					return (1);
				}else{
					resultado = mysql_store_result(&mysql);
					if(resultado){
							if (mysql_num_rows(resultado) > 0){
								//sprintf(num_articulos, "%d", mysql_num_rows(resultado));

								sprintf(c, "Articulo    Cant.  Precio   Monto");
								imprimir(subraya_s,nX);
								imprimir(c,nX);
								imprimir(cancela,nX);
								imprimir(defecto,nX);
								imprimir(salto,nX);
								peso_num = 0; //Se inicializa el peso o cantidad de cada articulo
								while((row = mysql_fetch_row(resultado))){
									strcpy(c,"");

									if(strcmp(row[4], "peso")==0)
									{
										num_articulos_int = num_articulos_int + 1;
										strcpy(num_tmp,"kg");
										peso_num = peso_num + atof(row[5]);
									}
									else
									{
										piezas_num = piezas_num + atof(row[5]);
										num_articulos_int = num_articulos_int + 1;// + atof(row[5]);
										strcpy(num_tmp,"  ");
									}
									//Nombre Producto
									if(strlen(row[0]) > 12)
									{
										imprimir(alinea_i, nX);
									    	sprintf(c, "%s%s", row[0],salto);
									}
									else
									{
										imprimir(alinea_d, nX);
										sprintf(c, "%s", row[0]);
										//strncat(c, temp3, 13-strlen(row[0]));
									}

									imprimir(c,nX);
									//imprimir(salto,nX);
									strcpy(c, "");

									strcpy(cad_temporal, row[1]);
									//Cantidad
									if(strlen(row[0]) > 12)
										sprintf(c, "%s%s%s", alinea_d,row[1],num_tmp);
									else
										sprintf(c, "%s%s", row[1],num_tmp);

									strcat(c, " $");

									strcpy(cad_temporal, row[2]);
									strncat(c, temp, 6-strlen(cad_temporal)); //Precio
									strcat(c, cad_temporal);

									strcat(c, " $");


									strcpy(cad_temporal, row[3]);
									strncat(c, temp, 9-strlen(cad_temporal)); //Monto
									strcat(c, cad_temporal);

									imprimir(c,nX);
									imprimir(salto,nX);
								}
							}else{
								imprimiendo = FALSE;
								return (1);
							}
					}else{
						imprimiendo = FALSE;
						return (1);
					}
				}
				sprintf(c, "                               --------");
				imprimir(c,nX);
				strcpy(temp2,"");
				strcpy(c,"");
				sprintf(c, "SUBTOTAL  :$ %s", subtotalVenta);
				imprimir(c,nX);
				imprimir(salto,nX);
				
				/*sprintf(c, "IVA (0%%)  :$ 0.00");
				imprimir(c,nX);
				imprimir(salto,nX);*/
				
				sprintf(c, "IVA       :$ %s", iva15Venta);
				imprimir(c,nX);
				imprimir(salto,nX);

				sprintf(c, "TOTAL :$ %s", monto);

				imprimir(alinea_d,nX);
				imprimir(tamano16,nX);
				imprimir(c,nX);
				imprimir(salto,nX);
				imprimir(alinea_i,nX);
				imprimir(tamano1,nX);

				if(strcmp(tipoPago,"efectivo") == 0)
				{
					if(strcmp(tipo, "contado") == 0)
					{
	
						strcpy(temp2,"");
						strcpy(c,"");
	
						sprintf(c, "EFECTIVO : $ %s", pago);
	
						imprimir(alinea_d,nX);
						imprimir(c,nX);
	
						strcpy(c,"");
						imprimir(salto,nX);
						
						//Calcula el cambio
						
						strcpy(temp2,"");
						strcpy(c,"");
						
						sprintf(c, "SU CAMBIO :    $ %s", cambio);
						
						imprimir(c,nX);
						imprimir(salto,nX);
					}
				}
				else
				{
					if(strcmp(tipoPago,"cheque") == 0)
						sprintf(c, "Cheque");
					else if(strcmp(tipoPago,"tarjeta") == 0)
						sprintf(c, "Tarjeta");
					
					imprimir(alinea_d,nX);
					imprimir(c,nX);
					strcpy(c,"");
					imprimir(salto,nX);
				}
				imprimir(alinea_i, nX);
				imprimir(salto,nX);
				strcpy(temp2,"");
				strcpy(c,"");
				strcpy(temp2,"Articulos: ");
				strncat(c,temp3,18-strlen(temp2));
				strcat(c, temp2);
				sprintf(num_articulos, "%d", num_articulos_int);

				strncat(c,temp3,4-strlen(num_articulos));
				strcat(c,num_articulos );

				imprimir(c,nX);
				imprimir(salto,nX);

				strcpy(temp2,"");
				strcpy(c,"");
				imprimir(alinea_i, nX);
				strcpy(temp2,"Peso: ");
				strncat(c,temp3,18-strlen(temp2));
				strcat(c, temp2);

				sprintf(peso, "%.3fkg", peso_num);
				strncat(c,temp3,10-strlen(peso));
				strcat(c,peso );
				strcat(c, salto);
				imprimir(c,nX);
				
				
				strcpy(temp2,"");
				strcpy(c,"");
				imprimir(alinea_i, nX);	
				strcpy(temp2,"Piezas: ");
				strncat(c,temp3,18-strlen(temp2));
				strcat(c, temp2);

				sprintf(pieza, "%.3f  ", piezas_num);
				strncat(c,temp3,10-strlen(pieza));
				strcat(c,pieza );
				strcat(c, salto);
				imprimir(c,nX);

				/* SE LISTAN LOS VENDEDORES QUE ATENDIERON */
				sprintf(sql,"SELECT Empleado.nombre FROM Empleado INNER JOIN Venta_Articulo ON Empleado.id_empleado = Venta_Articulo.id_empleado WHERE Venta_Articulo.id_venta = %s GROUP BY Empleado.id_empleado", id_venta);

				err = mysql_query(&mysql, sql);
				if(err != 0){
					printf("Error al consultar los vendedores: %s\n", mysql_error(&mysql));
					imprimiendo = FALSE;
					return (1);
				}else{
					resultado = mysql_store_result(&mysql);
					if(resultado){
						strcpy(c,"");
						strcpy(c, salto);
						strcat(c,"--ATENDIO--");
						strcat(c, salto);
						imprimir(c,nX);
						strcpy(c,"");
						while((row = mysql_fetch_row(resultado)))
						{
							strcat(c, row[0]);
							strcat(c, salto);
						}
						imprimir(c,nX);
					}
				}

				mysql_close(&mysql); //Cierra conexion SQL

				strcpy(c,"");
				imprimir(salto,nX);
			}
			//Fin de la insercion del ticket
			//Abre el pie de pagina
			
			//Observaciones del Ticket
			if (strcmp(VentaPrueba,"Pruebas") != 0 && strcmp(VentaPrueba,"SalidasVarias") != 0)
			{
				if(conecta_bd() == -1){
					g_print("\nNo me puedo conectar a la base de datos\n");
					imprimiendo = FALSE;
					return (1);
				}
				else
				{
						
					//Observaciones en el tickets
					sprintf(sql, "SELECT observacion_ticket FROM Configuracion");
					
					//printf ("\n############### SQL =%s \n###############\n",sql);
					err = mysql_query(&mysql, sql);
					if(err == 0)
					{
						//printf ("Paso1\n");
						resultado = mysql_store_result(&mysql);
						if(resultado)
						{
							//printf ("Paso2\n");
							if((row = mysql_fetch_row(resultado)))
							{
								if (row[0] != "")
								{
									imprimir(salto,nX);
									sprintf(c,"%s",row[0]);
									//imprimir(row[0],nX);
									imprimir(c,nX);
									printf ("\nObservaciones %s\n",row[0]);
									imprimir(salto,nX);
								}
							}
							else
								printf ("Error en el row\n");
						}
						else
							printf ("Error en el resultado\n");
					}
					else
					{
						printf ("Error en la cadena SQL\n");
					}
				}
				mysql_close(&mysql);
			}

			//Firmas para los Vales
			if (strcmp(VentaPrueba,"SalidasVarias") == 0)
			{
				imprimir(cancela,nX);
				imprimir(defecto,nX);
				imprimir(salto,nX);
				imprimir(alinea_c,nX);
				sprintf(c,"Autoriza");
				imprimir(c,nX);
				imprimir(salto,nX);
				imprimir(salto,nX);
				imprimir(salto,nX);
				imprimir(salto,nX);
				sprintf(c,"----------------------------------");
				imprimir(c,nX);
				imprimir(salto,nX);
				sprintf(c,"Nombre y Firma");
				imprimir(c,nX);
				imprimir(salto,nX);
				imprimir(salto,nX);
				imprimir(salto,nX);
				sprintf(c,"Recibe");
				imprimir(c,nX);
				imprimir(salto,nX);
				imprimir(salto,nX);
				imprimir(salto,nX);
				imprimir(salto,nX);
				sprintf(c,"----------------------------------");
				imprimir(c,nX);
				imprimir(salto,nX);
				sprintf(c,"Nombre y Firma");
				imprimir(c,nX);
				imprimir(salto,nX);
				imprimir(salto,nX);
				imprimir(salto,nX);
			}
			
			if (strcmp(VentaPrueba,"Pruebas") != 0 && strcmp(VentaPrueba,"SalidasVarias") != 0)
			{
				printf("El tipo de venta: %s\n",listatipos[17]);
				
				// Si la venta es con factura, no se imprime el pagaré porque se va a imprimir en la factura
				if((strcmp(listatipos[17],"t") == 0 && strcmp(tipo,"credito") == 0) || strcmp(tipo,"contado") == 0)
				{
					fpt2 = NULL;
					fpt2 = fopen(TicketAbajo,"r");
					if(fpt2 == NULL){
						printf("\nERROR no se puede abrir el archivo del pie de pagina");
						imprimiendo = FALSE;
						return (1);
					}else{
						fseek(fpt2,0,SEEK_END);
						//filesize = ftell(fpt2);
						rewind(fpt2);

						/*cadena = (char*) malloc (sizeof(char)*filesize);

						printf("Tamaño del archivo footer: %ld\n\n",filesize);
						if(cadena == NULL){
							printf("No se puede leer el archivo de TicketAbajo\n");
							free(cadena);
						}else{
							printf("Si se lee el archivo footer\n\n");
							if(fread(cadena, 1, filesize, fpt2) == filesize){
								cadena = str_replace(cadena, "<importe>", monto);
								cadena = str_replace(cadena, "<cliente>", listatipos[5]);
								cadena = str_replace(cadena, "<fecha>", fechaTMP);
								strcpy(c,cadena);
								imprimir(c,nX); //Funcion para imprimir y separa las letras
							}else
								printf("Error al leer el archivo footer\n\n");
							free(cadena);							
						}*/
						sprintf(c,"---------------------------------------");
						imprimir(c,nX);
						imprimir(salto,nX);
						i = 0;
						while(fgets(c, 40, fpt2) != NULL){
							imprimir(cancela,nX);
							imprimir(defecto,nX);
							if(strcmp(tipo,"contado") == 0)
							{
								imprimir(negrita,nX);
								imprimir(alinea_c,nX);
							}
							else
								imprimir(alinea_i, nX);
							//cadena = (char*) malloc (sizeof(char)c);
							cadena = c;
							sprintf(cad_temporal,"%s$%s%s",negrita1,monto,tamano1);
							cadena = str_replace(cadena, "<importe>", cad_temporal);
							cadena = str_replace(cadena, "<cliente>", listatipos[5]);
							sprintf(cad_temporal,"%s%s%s",negrita1,fecha_pagare,tamano1);
							cadena = str_replace(cadena, "<fecha>", cad_temporal);
							sprintf(cad_temporal,"%s%s%s",negrita1,listatipos[16],tamano1);
							cadena = str_replace(cadena, "<fecha_vencimiento>", cad_temporal);
							//Cantidad con letra
							cantidad_letra_num = atof(listatipos[18]);
							traduce(cantidad_letra_num, retVar);
							printf("\nCANTIDAD LETRA: %s ",retVar);
							cadena = str_replace(cadena, "<importe_letra>",retVar);

							strcpy(c,cadena);
							if(i == 0)
								imprimir(negrita,nX);
							imprimir(c,nX); //Funcion para imprimir y separa las letras
							i++;
						}
						imprimir(cancela,nX);
						imprimir(defecto,nX);
						fclose(fpt2);
					}
				}
			}
			//printf("\n\n<------------------>\nSe guarda en el archivo\n");
		}
		imprimir(salto,nX);
		
		if (strcmp(VentaPrueba,"Pruebas") != 0 && strcmp(VentaPrueba,"SalidasVarias") != 0)
		{
		imprimir(alinea_d,nX);
		//imprimir(negrita1,nX);
		imprimir("Matica",nX);
		imprimir(salto,nX);
		imprimir(negrita_subraya1,nX);
		imprimir("software libre",nX);
		imprimir(salto,nX);
		imprimir(cancela,nX);
		imprimir(defecto,nX);
		}
		imprimir(salto,nX);
		imprimir(salto,nX);
		imprimir(salto,nX);
		imprimir(salto,nX);
		imprimir(salto,nX);
		imprimir(salto,nX);
		imprimir(salto,nX);
		imprimir(salto,nX);
		imprimir(salto,nX);
		imprimir(salto,nX);
		printf("\n\n\n\n\n");
		imprimir(corta_papel,nX);

		/*imprimir (reversa,nX);
		imprimir (reversa,nX);
		imprimir (reversa,nX);
		imprimir (reversa,nX);*/
			
//		imprimir(c,nX);
		imprimir(cancela,nX);
		imprimir(defecto,nX);
		imprimir(resetea,nX);


		/* TIRA DE CODIGOS DE BARRAS PARA ENTRADAS A ALMACEN*/
		if(imprime_codigo_barras)
		{
			if(conecta_bd() == -1){
				g_print("\nNo me puedo conectar a la base de datos\n");
				imprimiendo = FALSE;
				return (1);
			}
			else
			{
				err = mysql_query(&mysql, sql_articulos);
				if(err != 0){
					printf("Error al consultar los articulos para generar los codigos de barras: %s\n", mysql_error(&mysql));
					imprimiendo = FALSE;
					return (1);
				}else{
					resultado = mysql_store_result(&mysql);
					if(resultado){
						if (mysql_num_rows(resultado) > 0){
							/* Reiniciamos la impresora*/
							imprimir(resetea,nX);
							imprimir(alinea_c,nX);
							imprimir(negrita_grande,nX);
							imprimir("CODIGOS PARA INVENTARIO",nX);
							imprimir(salto,nX);
							imprimir(negrita,nX);
							imprimir("CARNES BECERRA",nX);
							imprimir(salto,nX);
							imprimir(salto,nX);
							imprimir(cancela,nX);
							
							imprimir(tamano2,nX);
							imprimir(alinea_i,nX);
							//imprimir(espaciado, nX);

							sprintf(c, "Fecha: %c%c/%c%c/%c%c%c%c       Hora: %s",listatipos[1][8],listatipos[1][9],listatipos[1][5],listatipos[1][6],listatipos[1][0],listatipos[1][1],listatipos[1][2],listatipos[1][3],listatipos[2]);
							imprimir(c,nX);
							imprimir(salto,nX);
							sprintf(c, "Derivado del vale: ");
							imprimir(c,nX);
							imprimir(negrita,nX);
							sprintf(c, "%s", listatipos[0]);
							imprimir(c,nX);
							imprimir(salto,nX);
							imprimir(cancela,nX);
							sprintf(c, "Concepto: ");
							imprimir(c,nX);
							imprimir(negrita,nX);
							sprintf(c, "%.*s", nX-10, listatipos[16]);
							imprimir(c,nX);
							imprimir(salto,nX);
	
							imprimir(cancela,nX);
							imprimir(tamano2,nX);
							imprimir(alinea_i,nX);
							imprimir(salto,nX);
							imprimir(salto,nX);
							
							sprintf(c,"");
							num_articulos_int = 0;
							peso_num = 0;
							while((row = mysql_fetch_row(resultado))){
								strcpy(c,"");
								strcpy(codigo_barras,"");
								//strcpy(codigo_barras_cadena,"");
								if(strcmp(row[4], "peso")==0)
								{
									num_articulos_int = num_articulos_int + 1;
									strcpy(num_tmp,"kg");
									peso_num = peso_num + atof(row[5]);
								}
								else
								{
									piezas_num = piezas_num + atof(row[5]);
									num_articulos_int = num_articulos_int + 1;// + atof(row[5]);
									strcpy(num_tmp,"  ");
								}
								imprimir(tamano2,nX);
								sprintf(c,"%s  %.*s  %s%s",row[6],23,row[0],row[1],num_tmp);
								imprimir(c,nX);
								imprimir(salto,nX);
								imprimir(tamano1,nX);
								imprimir(altura_codigo_barras,nX);
								//sprintf(codigo_barras_cadena,"%s%s
								printf("\n\nEl codigo de barras: %s\n\n",row[7] );
								genera_codigo_barras(codigo_barras, codigo_barras_num, row[7]);
								//genera_codigo_barras(codigo_barras, codigo_barras_num, "823300410805");
								imprimir(codigo_barras,nX);
								imprimir(codigo_barras_num,nX);
								imprimir(salto,nX);
								imprimir(salto,nX);
								imprimir(salto,nX);
							}
							imprimir(salto,nX);
							imprimir(tamano2,nX);
							sprintf(c,"Articulos: %d",num_articulos_int);
							imprimir(c,nX);
							imprimir(salto,nX);
							sprintf(c,"Total kilos: %.3fkg",peso_num);
							imprimir(c,nX);
							imprimir(salto,nX);
							sprintf(c,"Total piezas: %.3f",piezas_num);
							imprimir(c,nX);
							imprimir(salto,nX);
							imprimir(salto,nX);
							imprimir(salto,nX);
							imprimir(salto,nX);
							imprimir(salto,nX);
							imprimir(salto,nX);
							imprimir(salto,nX);

							imprimir(corta_papel,nX);
							imprimir(cancela,nX);
							imprimir(defecto,nX);
							imprimir(resetea,nX);
						}else{
							imprimiendo = FALSE;
							return (1);
						}
					}else{
						imprimiendo = FALSE;
						return (1);
					}
				}
			}
		}

		fclose(fpt);
		//fclose(fpt2);
		printf("\n\n\nIMPRIMIENDO....\n");
		//TICKET
		if(strcmp(tipo, "credito") == 0)
		{
			if((impresora_nombre[arr_impresora_credito][0]) == 2)
			{
				strcpy(cad_temporal,"   ");
				strcat(cad_temporal,impresora_credito);
				strcat(cad_temporal," ");
				strcat(cad_temporal,TicketImpresion);
			}
			else
			{
				strcpy(cad_temporal,"cat ");
				strcat(cad_temporal,TicketImpresion);
				strcat(cad_temporal,"> ");
				strcat(cad_temporal,impresora_credito);
			}
		}
		else //Es contado
		{
			if((impresora_nombre[arr_impresora_contado][0]) == 2)
			{
				strcpy(cad_temporal,"lp.cups ");
				strcat(cad_temporal,impresora_contado);
				strcat(cad_temporal," ");
				strcat(cad_temporal,TicketImpresion);
			}
			else
			{
				strcpy(cad_temporal,"cat ");
				strcat(cad_temporal,TicketImpresion);
				strcat(cad_temporal,"> ");
				strcat(cad_temporal,impresora_contado);
			}
		}
		
		if( (strcmp(tipo, "credito") == 0) )
		{
			//Info("Credito");
			//IMPRIME EL LOGOTIPO
			/*fptlogo = fopen(TicketLogo,"rb");
        	        fpttemp = fopen(impresora_contado,"w");
                	if(fptlogo)
	                {
							fputc(color_rojo[0], fpttemp);
                 			fputc(color_rojo[1], fpttemp);
    	           			fputc(color_rojo[2], fpttemp);
        	                 fputc(27, fpttemp);
                	         fputc(97, fpttemp);
                        	 fputc(1, fpttemp);
	                         while(!feof(fptlogo)){
        	                          logoc = fgetc(fptlogo);
                	                  sprintf(c, "%c", logoc);
                        	          fputc(logoc, fpttemp);
	                         }
							fputc(color_negro[0], fpttemp);
                 			fputc(color_negro[1], fpttemp);
    	           			fputc(color_negro[2], fpttemp);
        	                 fclose(fptlogo);
                	         fclose(fpttemp);
	                 }
					 
			if ( int_reimpresion == 1 ) //SE IMPRIME LA LEYENDA DE REIMPRESION
			{
			
   		        fpttemp = fopen(impresora_contado,"w");
			
				fptlogo = fopen(TicketReimpresion,"rb");				
				
				if(conecta_bd() == -1){
				g_print("\nNo me puedo conectar a la base de datos\n");
				imprimiendo = FALSE;
				return (1);
				}else{
					
				    //sprintf(sql,"SELECT DATE_FORMAT(CURDATE(),\"\"), CURTIME()");
					sprintf(sql,"SELECT DATE_FORMAT(CURDATE(),\"%%d/%%m/%%Y\"), CURTIME()");
					//printf ("######################## \n SQL = %s",sql);
					err = mysql_query(&mysql, sql);
					if(err == 0)
					{
						resultado = mysql_store_result(&mysql);
						if(resultado){
							row = mysql_fetch_row(resultado);
							fprintf (fpttemp,"Fecha de reimpresion : %s %s%s Hora de reimpresion: %s %s%s", row[0], salto, salto, row[1], salto,salto);
							imprimir (row[0], nX);
							imprimir ("Hora de reimpresion : \n", nX);
							imprimir (row[1], nX);
						}
					}
					mysql_close(&mysql);
				}
				
        	   	if(fptlogo)
    	        {
					fputc(color_rojo[0], fpttemp);
                 	fputc(color_rojo[1], fpttemp);
    	           	fputc(color_rojo[2], fpttemp);
	                fputc(27, fpttemp);
                 	fputc(97, fpttemp);
    	           	fputc(1, fpttemp);
		            while(!feof(fptlogo)){
        	         	logoc = fgetc(fptlogo);
        	            sprintf(c, "%c", logoc);
    	                fputc(logoc, fpttemp);
		            }
					fputc(color_negro[0], fpttemp);
                 	fputc(color_negro[1], fpttemp);
    	           	fputc(color_negro[2], fpttemp);
		        }
				
				for (i=0; i<20; i++)
					for (j=0; j<4; j++)
						fputc(reversa[j], fpttemp);
					
				fclose(fptlogo);
        	    fclose(fpttemp);

			}*/
					 
			if (manda_imprimir (TicketImpresion,"credito") != 0)
			{
				Err_Info ("Error de impresion");
			}
			if ( int_reimpresion != 1 )
			{
				/*if (manda_imprimir (TicketImpresion,"credito") != 0)
				{
					Err_Info ("Error de impresion");
				}*/
				
				/*if (dobleImpresion)
				{
					if (manda_imprimir (TicketImpresion,"credito") != 0)
					{
						Err_Info ("Error de impresion");
					}
				}*/
			}
		}
		else
		{
			//ABRIR CAJON DE DINERO
			if ( int_reimpresion != 1 )
			//abrir_cajon("/dev/usb/lp0");
			abrir_cajon(AbrirCajon);
			//abrir_cajon(impresora_contado);
			//IMPRIME EL LOGOTIPO
			/*fptlogo = fopen(TicketLogo,"rb");
			fpttemp = fopen("/dev/usb/lp0","w");        	        
			//fpttemp = fopen(impresora_contado,"w");
                	if(fptlogo && fpttemp)
	                {
					printf("\nLOGO LOGO LOGO \nSe abrieron los puertos para logotipo\n\n");
					fputc(color_rojo[0], fpttemp);
                 			fputc(color_rojo[1], fpttemp);
    	           			fputc(color_rojo[2], fpttemp);
        	                fputc(27, fpttemp);
                	        fputc(97, fpttemp);
                        	fputc(1, fpttemp);
	                        while(!feof(fptlogo)){
        	                          logoc = fgetc(fptlogo);
                	                  sprintf(c, "%c", logoc);
                        	          fputc(logoc, fpttemp);
	                        }
							 
							fputc(color_negro[0], fpttemp);
                 			fputc(color_negro[1], fpttemp);
    	           			fputc(color_negro[2], fpttemp);
							 
        	                 fclose(fptlogo);
                	         fclose(fpttemp);
	                 }
			
			if ( int_reimpresion == 1 ) //SE IMPRIME LA LEYENDA DE REIMPRESION
			{
				
				fpttemp = fopen(impresora_contado,"w");
			
				fptlogo = fopen(TicketReimpresion,"rb");
				
				if(conecta_bd() == -1){
				g_print("\nNo me puedo conectar a la base de datos\n");
				imprimiendo = FALSE;
				return (1);
				}else{
					sprintf(sql,"SELECT DATE_FORMAT(CURDATE(),\"%%d/%%m/%%Y\"), CURTIME()");
					//printf ("######################## SQL = \n %s",sql);
					//printf ("Imprimio SQL");
					err = mysql_query(&mysql, sql);
					//printf ("Imprimio SQL");
					if(err == 0)
					{
						resultado = mysql_store_result(&mysql);
						if(resultado){
							row = mysql_fetch_row(resultado);
							fprintf (fpttemp,"Fecha de reimpresion : %s %s%s Hora de reimpresion: %s %s%s", row[0], salto, salto, row[1], salto,salto);
							/*imprimir (row[0], nX);
							imprimir ("Hora de reimpresion : \n", nX);
							imprimir (row[1], nX);*/
			/*			}
					}
					mysql_close(&mysql);
				}
			
        	   	if(fptlogo)
    	        	{
					fputc(color_rojo[0], fpttemp);
                 	fputc(color_rojo[1], fpttemp);
    	           	fputc(color_rojo[2], fpttemp);
					
	                fputc(27, fpttemp);
                 	fputc(97, fpttemp);
    	           	fputc(1, fpttemp);
		            while(!feof(fptlogo)){
        	         	logoc = fgetc(fptlogo);
        	            sprintf(c, "%c", logoc);
    	                fputc(logoc, fpttemp);
		        }
					
			 fputc(color_negro[0], fpttemp);
                 	 fputc(color_negro[1], fpttemp);
    	           	 fputc(color_negro[2], fpttemp);
		        }
				
				for (i=0; i<20; i++)
					for (j=0; j<4; j++)
						fputc(reversa[j], fpttemp);
					
				fclose(fptlogo);
        	    fclose(fpttemp);

			}*/
			if (manda_imprimir (AbrirCajon,"contado") != 0)
			{
				Err_Info ("Error de impresion");
				printf("ERROR\n");
			}
			if (manda_imprimir (TicketImpresion,"contado") != 0)
			{
				Err_Info ("Error de impresion");
				printf("ERROR\n");
			}
			if ( int_reimpresion != 1 )
            		{
				if (dobleImpresion)
				{
					if (manda_imprimir (TicketImpresion,"contado") != 0)
					{
						Err_Info ("Error de impresion");
						printf("ERROR\n");
					}
				}
			}
		}
		
		printf ("#######################3\n IMPIRME REVERSA\n");
		
		printf ("TEMP = %s\n",cad_temporal);
		/*system(cad_temporal);*/
		}
		printf("Termina la impresión... \n");
		return(0);

	}
	else if( strcmp(tipo, "servicio_domicilio") == 0 )
	{
	printf("Antes de que termine la funcion de impresion\n");	fpt = fopen(TicketImpresion,"w");
		if(fpt == NULL){
			printf("\nERROR no se puede abrir el archivo a imprimir");
			imprimiendo = FALSE;
			return (1);
		}
		else
		{
		fpt2 = fopen(TicketArribaServicioDomicilio,"r");
		if(fpt2 == NULL){
			printf("\nERROR no se puede abrir el archivo del encabezado Arriba");
			imprimiendo = FALSE;
			return (1);
		}
		else
		{
			imprimir(resetea,nX);
			imprimir(defecto,nX);
			
			imprimir(espaciado,nX);
			imprimir(alinea_i, nX);
			while(fgets(c, 255, fpt2) != NULL){
				imprimir(c,nX);
			}
			fclose(fpt2);
				
			imprimir(salto,nX);
			imprimir(alinea_c,nX);
			imprimir(negrita_grande,nX);
			imprimir("CARNES BECERRA",nX);
			imprimir(salto,nX);
			imprimir(cancela,nX);
			imprimir(alinea_c,nX);
			imprimir("SERVICIO",nX);
			imprimir(salto,nX);
			imprimir(salto,nX);
			imprimir(alinea_i,nX);
			imprimir(cancela,nX);
				
			if(conecta_bd() == -1){
				g_print("\nNo me puedo conectar a la base de datos\n");
				imprimiendo = FALSE;
				return (1);
			}else{
				sprintf(sql, "SELECT Venta.id_venta, Venta.fecha, FORMAT(Venta.monto,2), FORMAT(ServicioDomicilio.monto,2), FORMAT(Venta.monto + ServicioDomicilio.monto,2) FROM ServicioDomicilio INNER JOIN Venta ON Venta.id_venta = ServicioDomicilio.id_venta WHERE ServicioDomicilio.id_servicio_domicilio = %s", id_venta_char);
				printf ("\nSQL Servicio = %s\n",sql);
				err = mysql_query(&mysql, sql);
				if(err == 0)
				{
					resultado = mysql_store_result(&mysql);
					if(resultado)
					{
						if (mysql_num_rows(resultado) > 0)
						{
						if((row = mysql_fetch_row(resultado)))
						{
							for(i=0; i < 5; i++) //Guarda todo el arreglo en listapos
								listatipos[i] =  row[i];
							
							//sprintf(monto, "%s", listatipos[3]);
							//sprintf(cambio, "%s", listatipos[11]);
							sprintf(c, "Fecha: %c%c/%c%c/%c%c%c%c ",listatipos[1][8],listatipos[1][9],listatipos[1][5],listatipos[1][6],listatipos[1][0],listatipos[1][1],listatipos[1][2],listatipos[1][3]);
							imprimir(c,nX);
							imprimir(salto,nX);
							
							sprintf(c, "Folio de Venta: %s", listatipos[0]);
							imprimir(c,nX);
							imprimir(salto,nX);
							
							sprintf(c, "Folio: %s", id_venta_char);
							imprimir(c,nX);
							imprimir(salto,nX);
							imprimir(salto,nX);
							
							sprintf(c, "COMPRA    : %s", listatipos[2]);
							imprimir(c,nX);
							imprimir(salto,nX);
							
							sprintf(c, "SERVICIO  : %s", listatipos[3]);
							imprimir(c,nX);
							imprimir(salto,nX);
							
							sprintf(c, "TOTAL     : %s", listatipos[4]);
							imprimir(c,nX);
							imprimir(salto,nX);
							imprimir(salto,nX);
							
							fpt2 = fopen(TicketAbajoServicioDomicilio,"r");
							if(fpt2 == NULL){
								printf("\nERROR no se puede abrir el archivo del encabezado");
								imprimiendo = FALSE;
								return (1);
							}else{
								imprimir(resetea,nX);
								imprimir(defecto,nX);
								
								imprimir(espaciado,nX);
								imprimir(alinea_i, nX);
								while(fgets(c, 255, fpt2) != NULL)
								{
									imprimir(c,nX);
								}
								fclose(fpt2);
							}
                                                        imprimir(salto,nX);
                                                        imprimir(salto,nX);
                                                        imprimir(salto,nX);
                                                        imprimir(salto,nX);
                                                        imprimir(salto,nX);
                                                        imprimir(salto,nX);
                                                        imprimir(salto,nX);
                                                        imprimir(salto,nX);
                                                        imprimir(salto,nX);
                                                        imprimir(salto,nX);
							
							fclose(fpt);
						}
						}
					}
				}
				else
				{
					printf ("\nError en la cadena SQL de Servicios a domicilio\n");
				}
				mysql_close (&mysql);
			}
			
			if (manda_imprimir (TicketImpresion,"contado") != 0)
			{
				Err_Info ("Error de impresion");
			}
		}
		}
	}
	else if( strcmp(tipo, "archivo") == 0 )
	{
		va_start(lst_param, pago_num);
		archivoImpresion = va_arg(lst_param,char*); 
		va_end(lst_param);
		
		printf("\n###Archivo = %s\n", archivoImpresion);

		if (manda_imprimir (archivoImpresion,"contado") != 0)
		{
			Err_Info ("Error de impresion");
		}
	}
	else if(strcmp(tipo, "corte_caja_pruebas") == 0){
	 //Corte Caja
		//Se obtiene el parametro para saber si se imprime la leyenda de "Corte Parcial" o "Corte Final"
		corteFinal = pago_num;
		//mysql_init(&mysql);
		//if(!mysql_real_connect(&mysql, "192.168.0.3", "caja", "caja", "CarnesBecerra", 0, NULL, 0)){
		if(conecta_bd() == -1){
			g_print("\nNo me puedo conectar a la base de datos =(\n");
			imprimiendo = FALSE;
			return (1);
		}else{
			//Informacion del corte de caja
			strcpy(sql,"SELECT Corte_Caja_Pruebas.id_caja, Corte_Caja_Pruebas.fecha, Corte_Caja_Pruebas.hora, CONCAT_WS(  ' ', Usuario.nombre, Usuario.apellido )  AS usuario FROM Corte_Caja_Pruebas INNER  JOIN Usuario ON Usuario.id_usuario = Corte_Caja_Pruebas.id_usuario WHERE Corte_Caja_Pruebas.id_corte_caja = ");
			strcat(sql, id_venta);
			printf("%s\n",sql);
			if((err = mysql_query(&mysql, sql)) != 0){
				printf("Error al consultar los tipos de documentos: %s\n", mysql_error(&mysql));
				imprimiendo = FALSE;
				return (1);
			}else{
				if((resultado = mysql_store_result(&mysql))){
					if((row = mysql_fetch_row(resultado))){
						if (mysql_num_rows(resultado) > 0){

							fpt = fopen(TicketImpresion,"w");
							if(fpt == NULL){
								printf("\nERROR no se puede abrir el archivo a imprimir");
								imprimiendo = FALSE;
								return (1);
							}else{
								for(i=0; i < 5;i++) //Guarda todo el arreglo en listapos
									listatipos[i] =  row[i];

								corte_dinero_caja_num = 0;
								corte_contado_num = 0;
								corte_retiro_num =0;
								imprimir(resetea, nX);
								/*imprimir(alinea_c, nX);
								imprimir(negrita_subraya,nX);
								imprimir("CARNES BECERRA",nX);
								imprimir(cancela,nX);
								imprimir(salto,nX);
								imprimir(alinea_c, nX);
								imprimir(negrita_grande,nX);
								imprimir("CORTE DE CAJA",nX);
								imprimir(salto,nX);*/
								imprimir(cancela,nX);
								imprimir(tamano1,nX);
								imprimir(alinea_i,nX);
								imprimir(negrita,nX);
								sprintf(c, "Caja: %s\n", listatipos[0]); imprimir(c,nX);
								sprintf(c, "Fecha: %c%c-%c%c-%c%c%c%c Hora: %s",listatipos[1][8],listatipos[1][9],listatipos[1][5],listatipos[1][6],listatipos[1][0],listatipos[1][1],listatipos[1][2],listatipos[1][3],listatipos[2]);
								imprimir(c,nX);
								imprimir(salto,nX);
								imprimir(resetea,nX);
								imprimir(espaciado, nX);
								sprintf(c, "Cajero(a): ");
								imprimir(c,nX);
								imprimir(salto,nX);
								sprintf(c,"%s",listatipos[3]);
								imprimir(c,nX);
								imprimir(salto,nX);
								
								sprintf(c, "_________________");
								imprimir(c,nX);
								imprimir(salto,nX);


								//Credito y contado TOTAL e IVA
								sprintf(sql,"SELECT Pruebas.tipo, FORMAT(SUM( Pruebas_Articulo.monto ),2)  AS monto_IVA, FORMAT(SUM( Pruebas_Articulo.monto )  / 1.15,2) AS monto, SUM(Pruebas_Articulo.monto) FROM Corte_Caja_Pruebas INNER JOIN Pruebas ON Corte_Caja_Pruebas.id_caja=Pruebas.id_caja INNER JOIN Pruebas_Articulo ON Pruebas.id_venta=Pruebas_Articulo.id_venta INNER JOIN Articulo ON Pruebas_Articulo.id_articulo=Articulo.id_articulo WHERE Pruebas.id_venta BETWEEN Corte_Caja_Pruebas.id_venta_inicio AND Corte_Caja_Pruebas.id_venta_fin AND Pruebas.cancelada =  'n' AND Corte_Caja_Pruebas.id_corte_caja = %s GROUP BY Pruebas.tipo", id_venta);
								//printf("\n%s",sql);
								if((err = mysql_query(&mysql, sql)) != 0){
									printf("Error al consultar los tipos de documentos de Venta Total: %s\n", mysql_error(&mysql));
									imprimiendo = FALSE;
									return (1);
								}
								else{
									if((resultado = mysql_store_result(&mysql))){
										fac_subtotal_num = 0;
										fac_iva_num = 0;
										fac_total_num = 0;
										bandera = 0;
										cambio_num_total = 0;
										fac_cred_total_venta = 0;
										fac_cont_total_venta = 0;
										if (mysql_num_rows(resultado) > 0){
											imprimir(negrita1,nX);
											sprintf(c, "VENTA TOTAL:");
											imprimir(c,nX);
											imprimir(salto,nX);
											imprimir(defecto,nX);
											while((row = mysql_fetch_row(resultado)))
											{
												for(i=0; i < 4;i++) //Guarda todo el arreglo en listapos
													listatipos[i] =  row[i];
												bandera ++;
												//Es credito o contado
												if(strcmp(listatipos[0], "contado") == 0)
												{
													strcpy(totales, listatipos[1]);
													cambio_num = atof(listatipos[3]);
													cambio_num_total = cambio_num_total + cambio_num;
													fac_cont_total_venta = cambio_num;
													//printf("\n\nEl total de la venta: %.2s\n\n", lista);
													strcpy(temp2,"");
													strcpy(c,"");
													strcpy(temp2,"Contado: $ ");
													strncat(c,temp3,23-strlen(temp2));
													strcat(c, temp2);
													strncat(c,temp3,11-strlen(totales));
													strcat(c, totales);
													imprimir(c,nX);
													imprimir(salto,nX);


												}else if(strcmp(listatipos[0], "credito") == 0){
													strcpy(totales, listatipos[1]);
													cambio_num = atof(listatipos[3]);
													fac_cred_total_venta = cambio_num;
													cambio_num_total = cambio_num_total + cambio_num;
													strcpy(temp2,"");
													strcpy(c,"");
													strcpy(temp2,"Credito: $ ");
													strncat(c,temp3,23-strlen(temp2));
													strcat(c, temp2);
													strncat(c,temp3,11-strlen(totales));
													strcat(c, totales);
													imprimir(c,nX);
													imprimir(salto,nX);
												}

												//Guardo la cantidad de contado para despues sacar el dinero en caja
												if(strcmp(listatipos[0], "contado") == 0){
													//corte_contado_num = cambio_num;

													sprintf(cambio, "%.2f", atof(listatipos[3]));
													corte_contado_num = atof(cambio);
												}

											}

											if(bandera>1)
											{ //Verifica si existe Credito y contado para mostrar la suma de los 2
												sprintf(sql,"SELECT Pruebas.tipo, FORMAT(SUM( Pruebas_Articulo.monto ),2)  AS monto_IVA, FORMAT(SUM( Pruebas_Articulo.monto )  / 1.15,2) AS monto FROM Corte_Caja_Pruebas, Pruebas, Pruebas_Articulo, Articulo WHERE Corte_Caja_Pruebas.id_caja = Pruebas.id_caja AND Pruebas.id_venta BETWEEN Corte_Caja_Pruebas.id_venta_inicio AND Corte_Caja_Pruebas.id_venta_fin AND Pruebas_Articulo.id_venta = Pruebas.id_venta AND Articulo.id_articulo = Pruebas_Articulo.id_articulo AND Pruebas.cancelada =  'n' AND Corte_Caja_Pruebas.id_corte_caja = %s GROUP BY Corte_Caja_Pruebas.id_corte_caja", id_venta);
												if((err = mysql_query(&mysql, sql)) != 0)
												{
													printf("Error al consultar los tipos de documentos de Venta Total: %s\n", mysql_error(&mysql));
													imprimiendo = FALSE;
													return (1);
												}
												else
												{
													if((resultado = mysql_store_result(&mysql)))
													{
														row = mysql_fetch_row(resultado);
														sprintf(cambio,"%s", row[1]);
													}
												}
												//Calcula el Total

												strcpy(temp2,"");
												strcpy(c,"");
												strcpy(temp2,"TOTAL: $ ");
												strncat(c,temp3,23-strlen(temp2));
												strcat(c, temp2);
												strncat(c,temp3,11-strlen(cambio));
												strcat(c, cambio);
												imprimir(c,nX);
												imprimir(salto,nX);
											}
										}//Fin del > 0
									}else{
										imprimiendo = FALSE;
										return (1);
									}
								}


								//FACTURAS ya sea de credito o contado
								/*sprintf(sql,"SELECT Venta.tipo, FORMAT(SUM(Venta.monto),2) AS Monto_IVA, FORMAT((SUM(Venta.monto)  / 1.15),2) AS Monto, SUM(Venta.monto) FROM Corte_Caja INNER JOIN Venta ON Corte_Caja.id_caja = Venta.id_caja WHERE Venta.id_venta BETWEEN Corte_Caja.id_venta_inicio AND Corte_Caja.id_venta_fin AND Venta.cancelada = 'n' AND Venta.num_factura <> 0 AND Corte_Caja.id_corte_caja = %s GROUP BY Venta.tipo", id_venta);
								printf("\n%s\n",sql);
								if((err = mysql_query(&mysql, sql)) != 0){
									printf("Error al consultar los tipos de documentos: %s\n", mysql_error(&mysql));
									imprimiendo = FALSE;
									return (1);
								}
								else{
									if((resultado = mysql_store_result(&mysql))){
										fac_subtotal_num = 0;
										fac_iva_num = 0;
										fac_total_num = 0;
										bandera = 0;
										bandera2 = 0;
										fac_cont_subtotal = 0;
										fac_cont_iva = 0;

										fac_cont_total = 0;
										fac_cred_subtotal = 0;
										fac_credcont_total = 0;
										fac_credcont_iva = 0;
										fac_credcont_subtotal = 0;
										fac_cred_iva = 0;
										fac_cred_total = 0;
										cambio_num_total = 0;
										cambio_num = 0;



										if (mysql_num_rows(resultado) > 0){
											//sprintf(c, "_________________\n"); imprimir(c,nX);
											//sprintf(c, "FACTURAS:\n"); imprimir(c,nX);
											imprimir(negrita1,nX);
											sprintf(c, "\nVENTA FACTURADA:\n"); imprimir(c,nX);
											imprimir(defecto,nX);
											while((row = mysql_fetch_row(resultado)))
											{
												for(i=0; i < 4;i++) //Guarda todo el arreglo en listapos
													listatipos[i] =  row[i];
												printf("Tipo: %s\n", row[0]);
												printf("Contado: %s\n", row[1]);
												printf("Credito: %s\n", row[2]);
												//Es credito o contado

												if(strcmp(row[0], "contado") == 0 )
												{
													fac_cont_total = fac_cont_total + atof(row[3]);
													if(fac_cont_total > 0)
													{
														cambio_num_total = cambio_num_total + fac_cont_total;
														sprintf(cambio, "%.2f", fac_cont_total);
														strcpy(temp2,"");
														strcpy(c,"");
														strcpy(temp2,"Contado: $ ");
														strncat(c,temp3,23-strlen(temp2));
														strcat(c, temp2);
														strncat(c,temp3,11-strlen(row[1]));
														strcat(c, row[1]);
           													imprimir(c,nX);
														imprimir(salto,nX);
														bandera++;
													}
												}
												else if(strcmp(row[0], "credito") == 0)
												{
													fac_cred_total = fac_cred_total + atof(row[3]);
													if(fac_cred_total > 0)
													{
														cambio_num_total = cambio_num_total + fac_cred_total;
														sprintf(cambio, "%.2f", fac_cred_total);
														strcpy(temp2,"");
														strcpy(c,"");
														strcpy(temp2,"Credito: $ ");
														strncat(c,temp3,23-strlen(temp2));
														strcat(c, temp2);
														strncat(c,temp3,11-strlen(row[1]));
														strcat(c, row[1]);
														imprimir(c,nX);
														imprimir(salto,nX);
														bandera ++;
													}
												}
											}

											//Verifica si existe Credito y contado para mostrar la suma de los 2
											if(bandera == 2)
											{
												sprintf(sql,"SELECT FORMAT(SUM(Venta.monto),2) AS Monto_IVA, FORMAT((SUM(Venta.monto)  / 1.15),2) AS Monto, SUM(Venta.monto) FROM Corte_Caja INNER  JOIN Venta ON Corte_Caja.id_caja = Venta.id_caja WHERE Venta.id_venta BETWEEN Corte_Caja.id_venta_inicio AND Corte_Caja.id_venta_fin AND Venta.cancelada = 'n' AND Venta.num_factura <> 0 AND Corte_Caja.id_corte_caja = %s GROUP BY Corte_Caja.id_corte_caja", id_venta);
												if((err = mysql_query(&mysql, sql)) != 0)
												{
													printf("Error al consultar los tipos de documentos de Venta Total: %s\n", mysql_error(&mysql));
													imprimiendo = FALSE;
													return (1);
												}
												else
												{
													if((resultado = mysql_store_result(&mysql)))
													{
														row = mysql_fetch_row(resultado);
														sprintf(cambio, row[0]);
													}
												}

												//sprintf(cambio, "%.2f", cambio_num_total);
												strcpy(temp2,"");
												strcpy(c,"");
												strcpy(temp2,"TOTAL: $ ");
												strncat(c,temp3,23-strlen(temp2));
												strcat(c, temp2);
												strncat(c,temp3,11-strlen(cambio));
												strcat(c, cambio);
												imprimir(c,nX);
												imprimir(salto,nX);
											}
											
										}
									}else{
										imprimiendo = FALSE;
										return (1);
									}
								}*/


								/*imprimir(salto,nX);
								imprimir(negrita1,nX);
								sprintf(c, "VENTA TICKETS:");
								imprimir(c,nX);
								imprimir(salto,nX);
								imprimir(defecto,nX);

								sprintf(cambio, "%.2f", fac_cont_total_venta - fac_cont_total);
								strcpy(temp2,"");
								strcpy(c,"");
								strcpy(temp2,"Contado: $ ");
								strncat(c,temp3,23-strlen(temp2));
								strcat(c, temp2);
									sprintf(sql, "SELECT FORMAT(%s, 2)", cambio);
									err = mysql_query(&mysql, sql);
									if(err == 0)
									{
										resultado = mysql_store_result(&mysql);
										if(resultado)
										{
											if((row=mysql_fetch_row(resultado)))
												strcpy(cambio, row[0]);
										}
									}
									else
									{
										sprintf(Errores,"%s",mysql_error(&mysql));
										Err_Info(Errores);
									}
								strncat(c,temp3,11-strlen(cambio));
								strcat(c, cambio);
        							imprimir(c,nX);
								imprimir(salto,nX);

								sprintf(cambio, "%.2f", fac_cred_total_venta - fac_cred_total);
								strcpy(temp2,"");
								strcpy(c,"");
								strcpy(temp2,"Credito: $ ");
								strncat(c,temp3,23-strlen(temp2));
								strcat(c, temp2);
									sprintf(sql, "SELECT FORMAT(%s, 2)", cambio);
									err = mysql_query(&mysql, sql);
									if(err == 0)
									{
										resultado = mysql_store_result(&mysql);
										if(resultado)
										{
											if((row=mysql_fetch_row(resultado)))
												strcpy(cambio, row[0]);
										}
									}
									else
									{
										sprintf(Errores,"%s",mysql_error(&mysql));
										Err_Info(Errores);
									}
								strncat(c,temp3,11-strlen(cambio));
								strcat(c, cambio);
        							imprimir(c,nX);
								imprimir(salto,nX);


								sprintf(cambio, "%.2f", (fac_cred_total_venta - fac_cred_total) + (fac_cont_total_venta - fac_cont_total));
								strcpy(temp2,"");
								strcpy(c,"");
								strcpy(temp2,"TOTAL: $ ");
								strncat(c,temp3,23-strlen(temp2));
								strcat(c, temp2);
									sprintf(sql, "SELECT FORMAT(%s, 2)", cambio);
									err = mysql_query(&mysql, sql);
									if(err == 0)
									{
										resultado = mysql_store_result(&mysql);
										if(resultado)
										{
											if((row=mysql_fetch_row(resultado)))
												strcpy(cambio, row[0]);
										}
									}
									else
									{
										sprintf(Errores,"%s",mysql_error(&mysql));
										Err_Info(Errores);
									}
								strncat(c,temp3,11-strlen(cambio));
								strcat(c, cambio);
								imprimir(c,nX);
								imprimir(salto,nX);
								imprimir(salto,nX);*/
								
								//Cancelaciones
								strcpy(sql,"SELECT Pruebas_Cancelada.id_venta, Pruebas_Cancelada.observaciones, Cliente.nombre FROM Pruebas_Cancelada INNER  JOIN Pruebas ON Pruebas_Cancelada.id_venta = Pruebas.id_venta INNER  JOIN Cliente ON Cliente.id_cliente = Pruebas.id_cliente INNER  JOIN Corte_Caja_Pruebas ON Pruebas.id_venta BETWEEN Corte_Caja_Pruebas.id_venta_inicio AND Corte_Caja_Pruebas.id_venta_fin AND Corte_Caja_Pruebas.id_caja = Pruebas.id_caja WHERE Pruebas.cancelada =  's' AND Corte_Caja_Pruebas.id_corte_caja = ");
								strcat(sql, id_venta);

								if((err = mysql_query(&mysql, sql)) != 0){
									printf("Error al consultar los tipos de documentos: %s\n", mysql_error(&mysql));
									return (1);
								}else{
									if((resultado = mysql_store_result(&mysql))){

										if (mysql_num_rows(resultado) > 0){
											sprintf(c, "_________________\n"); imprimir(c,nX);
											imprimir(negrita1,nX);
											sprintf(c, "CANCELACIONES:\n"); imprimir(c,nX);
											imprimir(defecto,nX);
											while((row = mysql_fetch_row(resultado))){
												for(i=0; i < 3;i++) //Guarda todo el arreglo en listapos
													listatipos[i] =  row[i];

												//Cancelaciones FOLIO
												sprintf(c, "    Folio: %s\n",listatipos[0]); imprimir(c,nX);
												//Cancelaciones Cliente
												sprintf(c, "    Cliente:\n"); imprimir(c,nX);
												sprintf(c,"%s\n",listatipos[2]); imprimir(c,nX);
												sprintf(c, "    Observaciones:\n"); imprimir(c,nX);
												sprintf(c,"%s",listatipos[1]); imprimir(c,nX);
												sprintf(c,"\n\n"); imprimir(c,nX);
											}
										}else{
											//return (1);
										}
									}else{
										imprimiendo = FALSE;
										return (1);
									}
								}

								//Retiro Efectivo
								/*strcpy(sql,"SELECT Retiro.fecha, Retiro.hora, Retiro.monto, CONCAT_WS(  '  ', Cajero.nombre, Cajero.apellido )  AS cajero, CONCAT_WS(  '  ', UsuarioRetira.nombre, UsuarioRetira.apellido )  AS UsuarioRetira FROM Usuario Cajero, Usuario UsuarioRetira, Corte_Caja INNER  JOIN Retiro ON Retiro.id_retiro BETWEEN Corte_Caja.id_retiro_inicio AND Corte_Caja.id_retiro_fin AND Corte_Caja.id_caja = Retiro.id_caja WHERE Cajero.id_usuario = Retiro.id_usuario_cajero AND UsuarioRetira.id_usuario = Retiro.id_usuario_retira AND Corte_Caja.id_corte_caja = ");
								strcat(sql, id_venta);

								if((err = mysql_query(&mysql, sql)) != 0){
									printf("Error al consultar los tipos de documentos: %s\n", mysql_error(&mysql));
									imprimiendo = FALSE;
									return (1);
								}else{
									if((resultado = mysql_store_result(&mysql))){

										if (mysql_num_rows(resultado) > 0){
											sprintf(c, "_________________\n"); imprimir(c,nX);
											imprimir(negrita1,nX);
											sprintf(c, "RETIRO DE EFECTIVO:\n"); imprimir(c,nX);
											imprimir(defecto,nX);
											while((row = mysql_fetch_row(resultado))){
												for(i=0; i < 5;i++) //Guarda todo el arreglo en listapos
													listatipos[i] =  row[i];
												//Fecha Y Hora
												sprintf(c, "Fecha: %c%c-%c%c-%c%c%c%c Hora: %s\n",listatipos[0][8],listatipos[0][9],listatipos[0][5],listatipos[0][6],listatipos[0][0],listatipos[0][1],listatipos[0][2],listatipos[0][3],listatipos[1]); imprimir(c,nX);
												//Cajero
												sprintf(c, "Cajero(a): %s\n",listatipos[3]); imprimir(c,nX);
												//Cancelaciones Cliente
												sprintf(c, "Retiro: %s\n",listatipos[4]); imprimir(c,nX);
												//Monto dle retiro
												fac_subtotal_num = atof(listatipos[2]);
												sprintf(cambio, "%.2f", fac_subtotal_num);
												strcpy(temp2,"");
												strcpy(c,"");
												strcpy(temp2,"Cantidad: $ ");
												strncat(c,temp3,12-strlen(temp2));
												strcat(c, temp2);
													sprintf(sql, "SELECT FORMAT(%s, 2)", cambio);
													err = mysql_query(&mysql, sql);
													if(err == 0)
													{
														resultado2 = mysql_store_result(&mysql);
														if(resultado2)
														{
															if((row2=mysql_fetch_row(resultado2)))
																strcpy(cambio, row2[0]);
														}
													}
													else
													{
														sprintf(Errores,"%s",mysql_error(&mysql));
														Err_Info(Errores);
													}
												strncat(c,temp3,11-strlen(cambio));
												strcat(c, cambio);
												strcat(c, "\n\n");
												imprimir(c,nX);
												corte_retiro_num = corte_retiro_num + fac_subtotal_num;
											}
											strcpy(temp2,"");
											strcpy(c,""); strcpy(cambio,"---------");
											strcpy(temp2," ");
											strncat(c,temp3,23-strlen(temp2));
											strcat(c, temp2);
											strncat(c,temp3,11-strlen(cambio));
											strcat(c, cambio);
											strcat(c, "\n");
											imprimir(c,nX);
											//Calcula el Total
											sprintf(cambio, "%.2f", corte_retiro_num);
											strcpy(temp2,"");
											strcpy(c,"");
											strcpy(temp2,"TOTAL: $ ");
											strncat(c,temp3,23-strlen(temp2));
											strcat(c, temp2);
												sprintf(sql, "SELECT FORMAT(%s, 2)", cambio);
												err = mysql_query(&mysql, sql);
												if(err == 0)
												{
													resultado = mysql_store_result(&mysql);
													if(resultado)
													{
														if((row=mysql_fetch_row(resultado)))
															strcpy(cambio, row[0]);
													}
												}
												else
												{
													sprintf(Errores,"%s",mysql_error(&mysql));
													Err_Info(Errores);
												}
											strncat(c,temp3,11-strlen(cambio));
											strcat(c, cambio);
											imprimir(c,nX);
											imprimir(salto,nX);
											imprimir(salto,nX);

										}else{
											//return (1);
										}
									}else{
										imprimiendo = FALSE;
										return (1);
									}
								}*/
								
							/*sprintf(c, "_________________\n"); imprimir(c,nX);
							//Monto dle retiro
							corte_dinero_caja_num = corte_contado_num - corte_retiro_num;
							sprintf(cambio, "%.2f", corte_dinero_caja_num);
							strcpy(temp2,"");
							strcpy(c,"");
							strcpy(temp2,"DINERO EN CAJA: $ ");
							strncat(c,temp3,18-strlen(temp2));
							strcat(c, temp2);
								sprintf(sql, "SELECT FORMAT(%s, 2)", cambio);
								err = mysql_query(&mysql, sql);
								if(err == 0)
								{
									resultado = mysql_store_result(&mysql);
									if(resultado)
									{
										if((row=mysql_fetch_row(resultado)))
											strcpy(cambio, row[0]);
									}
								}
								else
								{
									sprintf(Errores,"%s",mysql_error(&mysql));
									Err_Info(Errores);
								}
							strncat(c,temp3,16-strlen(cambio));
							strcat(c, cambio);
							imprimir(negrita1,nX);
							imprimir(c,nX);
							imprimir(salto,nX);
							imprimir(defecto,nX);*/


							//Numero de Clientes

							sprintf(sql,"SELECT COUNT(*) FROM Corte_Caja_Pruebas INNER JOIN Pruebas ON Corte_Caja_Pruebas.id_caja = Pruebas.id_caja WHERE Pruebas.id_venta BETWEEN Corte_Caja_Pruebas.id_venta_inicio AND Corte_Caja_Pruebas.id_venta_fin AND Pruebas.cancelada='n' AND Corte_Caja_Pruebas.id_corte_caja = %s GROUP BY Corte_Caja_Pruebas.id_corte_caja", id_venta);
							
							if((err = mysql_query(&mysql, sql)) != 0){
								printf("Error al consultar el total de clientes: %s\n", mysql_error(&mysql));
								imprimiendo = FALSE;
								return (1);
							}else{
								if((resultado = mysql_store_result(&mysql))){
									if (mysql_num_rows(resultado) > 0){
										row = mysql_fetch_row(resultado);
	 						//			sprintf(c, "_____________________________________\n"); imprimir(c,nX);

										sprintf(cambio, "%s", row[0]);
										strcpy(temp2,"");
										strcpy(c,"");
										strcpy(temp2,"DINERO EN CAJA: $ ");
										strcpy(temp2,"CLIENTES ATENDIDOS: ");
										strncat(c,temp3,20-strlen(temp2));
										strcat(c, temp2);
										strncat(c,temp3,14-strlen(cambio));
										strcat(c, cambio);
										strcat(c, "\n\n");
										imprimir(c,nX);
										
										//sprintf(c, "CLIENTES ATENDIDOS: %s\n", row[0]); imprimir(c,nX);

									}else{
										//return (1);
									}
								}else{
									imprimiendo = FALSE;
									return (1);
								}
							}

							//Resumen de documentos
							sprintf(sql,"SELECT Pruebas.id_venta, ' ', FORMAT(Pruebas.monto,2) FROM Pruebas INNER JOIN Corte_Caja_Pruebas ON Pruebas.id_caja = Corte_Caja_Pruebas.id_caja WHERE Pruebas.tipo='credito' AND Pruebas.cancelada='n' AND Pruebas.id_venta BETWEEN Corte_Caja_Pruebas.id_venta_inicio AND Corte_Caja_Pruebas.id_venta_fin AND Corte_Caja_Pruebas.id_corte_caja=%s", id_venta);
							if((err = mysql_query(&mysql, sql)) != 0)
							{
								printf("Error al consultar los tipos de documentos: %s\n", mysql_error(&mysql));
								imprimiendo = FALSE;
								return (1);
							}
							else
							{
								resultado = mysql_store_result(&mysql);
								if(resultado)
								{
									sprintf(c,"_____________________");imprimir(c,nX);
									sprintf(c,"%s",salto);imprimir(c,nX);
									imprimir(negrita1,nX);
									sprintf(c,"DOCUMENTOS DE CREDITO");imprimir(c,nX);
									imprimir(defecto,nX);
									sprintf(c,"%s",salto);imprimir(c,nX);
									sprintf(c,"FOLIO                          MONTO");imprimir(c,nX);
									sprintf(c,"%s",salto);imprimir(c,nX);
									sprintf(c,"------------------------------------");imprimir(c,nX);
									sprintf(c,"%s",salto);imprimir(c,nX);
									while((row=mysql_fetch_row(resultado)))
									{
										sprintf(c, "%6s", row[0]);
										if(row[1])
										{
											strncat(c, temp3, 12-strlen(row[1]));
											strcat(c, row[1]);
										}
										else
											strncat(c, temp3, 12);
										
										strncat(c, temp3, 18-strlen(row[2]));
										strcat(c, row[2]);
										imprimir(c,nX);
										sprintf(c,"%s",salto);
										imprimir(c,nX);
									}
									sprintf(c,"                        ------------");imprimir(c,nX);
									imprimir(salto,nX);
									sprintf(sql,"SELECT FORMAT(SUM(Pruebas.monto),2) FROM Pruebas INNER JOIN Corte_Caja_Pruebas ON Pruebas.id_caja = Corte_Caja_Pruebas.id_caja WHERE Pruebas.tipo='credito' AND Pruebas.cancelada='n' AND Pruebas.id_venta BETWEEN Corte_Caja_Pruebas.id_venta_inicio AND Corte_Caja_Pruebas.id_venta_fin AND Corte_Caja_Pruebas.id_corte_caja=%s GROUP BY Corte_Caja_Pruebas.id_corte_caja", id_venta);
									if((err = mysql_query(&mysql, sql)) != 0)
									{
										printf("Error al consultar los tipos de documentos: %s\n", mysql_error(&mysql));
										return (1);
									}
									else
									{
										resultado = mysql_store_result(&mysql);
										if(resultado)
										{
											if((row=mysql_fetch_row(resultado)))
											{
												sprintf(c,"TOTAL DOCUMENTOS: $");
												strncat(c,temp3,17-strlen(row[0]));
												strcat(c,row[0]);
												imprimir(c,nX);
											}
										}
									}
									sprintf(c,"%s",salto);imprimir(c,nX);
									sprintf(c,"%s",salto);imprimir(c,nX);
								}
								else
									printf("Ocurrió un error al obtener el resultado de documentos: %s\n", mysql_error(&mysql));
							}
						
							//if(corte_final)
							{
								imprimir(salto,nX);
	                                                        imprimir(salto,nX);
        	                                                //imprimir(corta_papel,nX);


							//Total de kilos de credito y total de contado
							sprintf(sql,"SELECT FORMAT(SUM( Pruebas_Articulo.cantidad),2), Pruebas.tipo FROM Corte_Caja_Pruebas INNER JOIN Pruebas ON Corte_Caja_Pruebas.id_caja = Pruebas.id_caja INNER JOIN Pruebas_Articulo ON Pruebas_Articulo.id_venta = Pruebas.id_venta WHERE Pruebas_Articulo.id_venta BETWEEN Corte_Caja_Pruebas.id_venta_inicio AND Corte_Caja_Pruebas.id_venta_fin AND Pruebas.cancelada = 'n' AND Corte_Caja_Pruebas.id_corte_caja = %s GROUP  BY Pruebas.tipo ORDER BY tipo,cantidad DESC", id_venta);
							printf (sql,"SQL K = %s\n",sql);

							if((err = mysql_query(&mysql, sql)) != 0){
                                                                printf("Error al consultar los tipos de documentos: %s\n", mysql_error(&mysql));
                                                                imprimiendo = FALSE;
                                                                return (1);
                                                        }else{
                                                                if((resultado = mysql_store_result(&mysql))){
                                                                        if (mysql_num_rows(resultado) > 0){
										while((row = mysql_fetch_row(resultado))){
                                                                                
											printf (sql,"K = %s %s\n",row[1],row[0]);
                                                                                        if(strcmp(row[1], "contado")==0)
                                                                                        {
                                                                                                sprintf(c, "KILOS CONTADO %s", row[0]);
                                                                                        }
                                                                                        else if(strcmp(row[1], "credito")==0)
                                                                                        {
                                                                                                sprintf(c, "KILOS CREDITO %s",row[0]);
                                                                                                corte_articulos = corte_articulos + fac_subtotal_num;
                                                                                        }
										//sprintf(c, "TOTAL KILOS:  %s",cambio);
                                                                                imprimir(salto,nX);
                                                                                //imprimir(negrita1,nX);
                                                                                imprimir(c,nX);
										}
									}
								}
							}

					


							//Cantidad vendida de kilos
							sprintf(sql,"SELECT Pruebas_Articulo.id_articulo, SUM( Pruebas_Articulo.monto ), (SUM( Pruebas_Articulo.monto )  / 1.15), SUM( Pruebas_Articulo.cantidad ), Articulo.nombre, Articulo.tipo AS tipo FROM Corte_Caja_Pruebas INNER JOIN Pruebas ON Corte_Caja_Pruebas.id_caja = Pruebas.id_caja INNER JOIN Pruebas_Articulo ON Pruebas_Articulo.id_venta = Pruebas.id_venta INNER JOIN Articulo ON Articulo.id_articulo = Pruebas_Articulo.id_articulo WHERE Pruebas_Articulo.id_venta BETWEEN Corte_Caja_Pruebas.id_venta_inicio AND Corte_Caja_Pruebas.id_venta_fin AND Pruebas.cancelada = 'n' AND Corte_Caja_Pruebas.id_corte_caja = %s GROUP  BY Pruebas_Articulo.id_articulo ORDER BY tipo,cantidad DESC", id_venta);

							if((err = mysql_query(&mysql, sql)) != 0){
								printf("Error al consultar los tipos de documentos: %s\n", mysql_error(&mysql));
								imprimiendo = FALSE;
								return (1);
							}else{
								if((resultado = mysql_store_result(&mysql))){
									if (mysql_num_rows(resultado) > 0){
	 									/*sprintf(c, "_________________");
										imprimir(c,nX);
										imprimir(salto,nX);
										sprintf(c, "ARTICULOS VENDIDOS:");
										imprimir(negrita1,nX);
										imprimir(c,nX);
										imprimir(salto,nX);
										imprimir(defecto,nX);
										sprintf(c, "Articulo                     Cantidad");
										imprimir(c,nX);
										imprimir(salto,nX);
										sprintf(c, "----------                 ----------");
										imprimir(c,nX);
										imprimir(salto,nX);*/
										corte_dinero_caja_num = 0;
										while((row = mysql_fetch_row(resultado))){
											for(i=0; i < 6;i++) //Guarda todo el arreglo en listapos
												listatipos[i] =  row[i];

											fac_subtotal_num = atof(listatipos[3]);
											if(strcmp(row[5], "peso")==0)
											{
												sprintf(cambio, "%.3f kg", fac_subtotal_num);
												corte_peso = corte_peso + fac_subtotal_num;
											}
											else if(strcmp(row[5], "pieza")==0)
											{
												sprintf(cambio, "%.3f pz", fac_subtotal_num);
												corte_articulos = corte_articulos + fac_subtotal_num;
											}

											//corte_dinero_caja_num = corte_dinero_caja_num + atof(cambio);
											strcpy(temp2,"");
											strcpy(c,"");
											for(i=0;i<25;i++){
												if( i >=strlen(listatipos[4]))
													temp2[i] = ' ';
												else
													temp2[i] = listatipos[4][i];
												temp2[i+1] = '\0';
											}
											strcat(c, temp2);
											strncat(c,temp3,12-strlen(cambio));
											strcat(c, cambio);
											//imprimir(c,nX);
											//imprimir(salto,nX);


										}
										//Monto dle retiro
										strcpy(cambio,"");
										sprintf(cambio, "%.3f",corte_peso);
										sprintf(c, "TOTAL KILOS:  %s",cambio);
										imprimir(salto,nX);
										imprimir(negrita1,nX);
										imprimir(c,nX);
										imprimir(salto,nX);
										imprimir(defecto,nX);
										strcpy(cambio,"");
										sprintf(cambio, "%.2f",corte_articulos);
										sprintf(c, "TOTAL PIEZAS: %s",cambio);
										imprimir(c,nX);
										imprimir(salto,nX);
									}else{
										//return (1);
									}
								}else{
									imprimiendo = FALSE;
									return (1);
								}
							}
							imprimir(salto,nX);
							}
							
							/*
							//SE OBTIENE LOS KILOS VENDIDOS POR BASCULA
							if (corte_final)
							{
								sprintf(sql,"SELECT FORMAT(SUM(Venta_Articulo.cantidad * Articulo_Lista.precio),2), FORMAT(SUM(Venta_Articulo.cantidad),3), Basculas.bascula, Venta_Articulo.id_bascula FROM Corte_Caja INNER  JOIN Venta ON Corte_Caja.id_caja = Venta.id_caja INNER  JOIN Venta_Articulo ON Venta_Articulo.id_venta = Venta.id_venta INNER JOIN Articulo_Lista ON Articulo_Lista.id_articulo = Venta_Articulo.id_articulo INNER JOIN Basculas ON Basculas.id_bascula = Venta_Articulo.id_bascula WHERE Venta_Articulo.id_venta BETWEEN Corte_Caja.id_venta_inicio AND Corte_Caja.id_venta_fin AND Venta.cancelada = 'n' AND Corte_Caja.id_corte_caja = %s AND Articulo_Lista.id_lista = 1 GROUP BY Venta_Articulo.id_bascula", id_venta);
								
								//printf ("####### SWL = %s",sql);
								if((err = mysql_query(&mysql, sql)) != 0)
								{
									printf("Error al consultar los tipos de documentos de Venta Total: %s\n", mysql_error(&mysql));
									imprimiendo = FALSE;
									return (1);
								}
								else
								{
									if((resultado = mysql_store_result(&mysql)))
									{
										while (row = mysql_fetch_row(resultado))
										{
											//sprintf(kilos_bascula, row[0]);
											//sprintf(cambio, "%.2f", cambio_num_total);
											strcpy(temp2,"");
											strcpy(c,row[2]);
											imprimir (c,nX);
											imprimir(salto,nX);
											
											//SE IMPRIME LA VENTA DE LA BASCULA
											strcpy(c,"   IMPORTE: ");
											imprimir(c,nX);
											sprintf(c, row[0]);
											imprimir(c,nX);
											imprimir(salto,nX);
											
											//SE IMPRIMEN LOS KILOS VENDIDOS
											strcpy(c,"   KILOS: ");
											imprimir(c,nX);
											sprintf(c, row[1]);
											strcat (c," kg");
											imprimir(c,nX);
											
											//strncat(c,temp3,23-strlen(temp2));
											//strcat(c, temp2);
											//strncat(c,temp3,11-strlen(kilos_bascula));
											//strcat(c, kilos_bascula);
											//imprimir(c,nX);
											imprimir(salto,nX);	
											imprimir(salto,nX);	
										}
									}
								}
							}*/
							
							//KILOS POR VENDEDOR
							/*if (corte_final)
							{
									sprintf(sql,"SELECT FORMAT(SUM(Venta_Articulo.cantidad * Articulo_Lista.precio),2), FORMAT(SUM(Venta_Articulo.cantidad),3), CONCAT(Empleado.nombre,' ',Empleado.apellido) AS Vendedor, Basculas.bascula AS Bascula FROM Corte_Caja INNER  JOIN Venta ON Corte_Caja.id_caja = Venta.id_caja INNER JOIN Venta_Articulo ON Venta_Articulo.id_venta = Venta.id_venta INNER JOIN Articulo_Lista ON Articulo_Lista.id_articulo = Venta_Articulo.id_articulo INNER JOIN Empleado ON Empleado.id_empleado = Venta.id_empleado INNER JOIN Basculas ON Venta_Articulo.id_bascula = Basculas.id_bascula WHERE Venta_Articulo.id_venta BETWEEN Corte_Caja.id_venta_inicio AND Corte_Caja.id_venta_fin AND Venta.cancelada = 'n' AND Corte_Caja.id_corte_caja = %s AND Articulo_Lista.id_lista = 1 GROUP BY Venta.id_empleado, Basculas.id_bascula ORDER BY Vendedor, Bascula", id_venta);
								
								//printf ("####### SWL = %s",sql);
								if((err = mysql_query(&mysql, sql)) != 0)
								{
									printf("Error al consultar los tipos de documentos de Venta Total: %s\n", mysql_error(&mysql));
									imprimiendo = FALSE;
									return (1);
								}
								else
								{
									if((resultado = mysql_store_result(&mysql)))
									{
										sprintf(vendedor, "");
										while (row = mysql_fetch_row(resultado))
										{
											if(strcmp(vendedor,row[2]) != 0)
											{
												//sprintf(kilos_bascula, row[0]);
												//sprintf(cambio, "%.2f", cambio_num_total);
												imprimir(salto,nX);
												strcpy(c,row[2]);
												imprimir(negrita1,nX);
												imprimir (c,nX);
												imprimir(salto,nX);
												imprimir(defecto,nX);
												sprintf(vendedor,"%s",row[2]);
											}
											
											//SE IMPRIME LA VENTA DEL VENDEDOR
											strcpy(c,"   BASCULA: ");
											imprimir(c,nX);
											sprintf(c, row[3]);
											imprimir(c,nX);
											imprimir(salto,nX);
											
											//SE IMPRIME LA VENTA DEL VENDEDOR
											strcpy(c,"   IMPORTE: ");
											imprimir(c,nX);
											sprintf(c, row[0]);
											imprimir(c,nX);
											imprimir(salto,nX);
											
											//SE IMPRIMEN LOS KILOS VENDIDOS
											strcpy(c,"   KILOS: ");
											imprimir(c,nX);
											sprintf(c, row[1]);
											strcat (c," kg");
											imprimir(c,nX);
											
											//strncat(c,temp3,23-strlen(temp2));
											//strcat(c, temp2);
											//strncat(c,temp3,11-strlen(kilos_bascula));
											//strcat(c, kilos_bascula);
											//imprimir(c,nX);
											imprimir(salto,nX);
											imprimir(salto,nX);
										}
									}
								}
							}*/
							
							//CLIENTES POR VENDEDOR
							/*if (corte_final)
							{
									sprintf(sql,"SELECT CONCAT(Empleado.nombre,' ',Empleado.apellido) AS Vendedor, COUNT( Venta.id_venta ) FROM Corte_Caja INNER JOIN Venta ON Corte_Caja.id_caja = Venta.id_caja INNER JOIN Empleado ON Venta.id_empleado = Empleado.id_empleado WHERE Venta.id_venta BETWEEN Corte_Caja.id_venta_inicio AND Corte_Caja.id_venta_fin AND Venta.cancelada =  'n' AND Corte_Caja.id_corte_caja = %s GROUP BY Empleado.id_empleado ORDER BY Vendedor", id_venta);
								
								//printf ("####### SWL = %s",sql);
								if((err = mysql_query(&mysql, sql)) != 0)
								{
									printf("Error al consultar los clientes por Vendedor: %s\n", mysql_error(&mysql));
									imprimiendo = FALSE;
									return (1);
								}
								else
								{
									if((resultado = mysql_store_result(&mysql)))
									{
										sprintf(vendedor, "");
										sprintf(c,"CLIENTES POR VENDEDOR");
										imprimir(negrita1,nX);
										imprimir (c,nX);
										imprimir(salto,nX);
										imprimir(defecto,nX);
										while (row = mysql_fetch_row(resultado))
										{
											//sprintf(kilos_bascula, row[0]);
											//sprintf(cambio, "%.2f", cambio_num_total);
											sprintf(c,"%s: %s",row[0],row[1]);
											imprimir (c,nX);
											imprimir(salto,nX);
										}
									}
								}
							}*/

							imprimir(salto,nX);
							imprimir(salto,nX);
							imprimir(salto,nX);
							imprimir(salto,nX);
							imprimir(salto,nX);
							imprimir(salto,nX);
							imprimir(salto,nX);
							imprimir(salto,nX);
							imprimir(corta_papel,nX);
							fclose(fpt);
							//CORTE DE CAJA
							if((impresora_nombre[arr_impresora_corte_caja][0]) == 2)
							{
								strcpy(cad_temporal,"lp.cups ");
								strcat(cad_temporal,impresora_corte_caja);
								strcat(cad_temporal," ");
								strcat(cad_temporal,TicketImpresion);
							}
							else
							{
								strcpy(cad_temporal,"cat ");
								strcat(cad_temporal,TicketImpresion);
								strcat(cad_temporal,"> ");
								strcat(cad_temporal,impresora_corte_caja);
							}
							//system(cad_temporal);
							//ABRE CAJON DE DINERO
							abrir_cajon(impresora_contado);
							if (manda_imprimir (TicketImpresion,"corte_caja") != 0) 
							{
								Err_Info ("Error de impresion");
							}
							printf ("TEMP = %s\n",cad_temporal);
							}
						}else{
							imprimiendo = FALSE;
							return (1);
						}
					}else{
						imprimiendo = FALSE;
						return (1);
					}
				}else{
					imprimiendo = FALSE;
					return (1);
				 }
			mysql_close(&mysql); //Cierra conexion SQL
			}
		}

	}
	else if(strcmp(tipo, "corte_caja") == 0){ //Corte Caja
		//Se obtiene el parametro para saber si se imprime la leyenda de "Corte Parcial" o "Corte Final"
		corteFinal = pago_num;
		//mysql_init(&mysql);
		//if(!mysql_real_connect(&mysql, "192.168.0.3", "caja", "caja", "CarnesBecerra", 0, NULL, 0)){
		if(conecta_bd() == -1){
			g_print("\nNo me puedo conectar a la base de datos =(\n");
			imprimiendo = FALSE;
			return (1);
		}else{
			//Informacion del corte de caja
			strcpy(sql,"SELECT Corte_Caja.id_caja, Corte_Caja.fecha, Corte_Caja.hora, CONCAT_WS(  ' ', Usuario.nombre, Usuario.apellido )  AS usuario FROM Corte_Caja INNER  JOIN Usuario ON Usuario.id_usuario = Corte_Caja.id_usuario WHERE Corte_Caja.id_corte_caja = ");
			strcat(sql, id_venta);
			printf("%s\n",sql);
			if((err = mysql_query(&mysql, sql)) != 0){
				printf("Error al consultar los tipos de documentos: %s\n", mysql_error(&mysql));
				imprimiendo = FALSE;
				return (1);
			}else{
				if((resultado = mysql_store_result(&mysql))){
					if((row = mysql_fetch_row(resultado))){
						if (mysql_num_rows(resultado) > 0){

							fpt = fopen(TicketImpresion,"w");
							if(fpt == NULL){
								printf("\nERROR no se puede abrir el archivo a imprimir");
								imprimiendo = FALSE;
								return (1);
							}else{
								for(i=0; i < 5;i++) //Guarda todo el arreglo en listapos
									listatipos[i] =  row[i];

								corte_dinero_caja_num = 0;
								corte_contado_num = 0;
								corte_retiro_num =0;
								imprimir(resetea, nX);
								imprimir(alinea_c, nX);
								imprimir(negrita_subraya,nX);
								imprimir("CARNES BECERRA",nX);
								imprimir(cancela,nX);
								imprimir(salto,nX);
								imprimir(alinea_c, nX);
								imprimir(negrita_grande,nX);
								imprimir("CORTE DE CAJA",nX);
								imprimir(salto,nX);
								imprimir(tamano1,nX);
								imprimir(alinea_c, nX);
								
								if (corteFinal <= 0)
									imprimir("PARCIAL",nX);
								else
									imprimir("FINAL",nX);

								//printf ("%s\n\n",listatipos[3]);
								sprintf (cajero,"%s\n",listatipos[3]);
								cajero[30] = '\0';
								sprintf (fechaCorte,"Fecha: %c%c-%c%c-%c%c%c%c Hora: %s\n",listatipos[1][8],listatipos[1][9],listatipos[1][5],listatipos[1][6],listatipos[1][0],listatipos[1][1],listatipos[1][2],listatipos[1][3],listatipos[2]);
								fechaCorte[32] ='\0';
								sprintf (cajaCorte,"%s\n",listatipos[0]);
								//printf ("\n\n### %s\n\n", fechaCorte);
								
								imprimir(salto,nX);
								imprimir(salto,nX);
								imprimir(cancela,nX);
								imprimir(tamano1,nX);
								imprimir(alinea_i,nX);
								imprimir(negrita,nX);
								sprintf(c, "Caja: %s\n",cajaCorte);
								imprimir(c,nX);
								printf("Despues de Caja:\n");
								//sprintf(c, "Fecha: %c%c-%c%c-%c%c%c%c Hora: %s",listatipos[1][8],listatipos[1][9],listatipos[1][5],listatipos[1][6],listatipos[1][0],listatipos[1][1],listatipos[1][2],listatipos[1][3],listatipos[2]);
								sprintf (c,"%s", fechaCorte);
								imprimir(c,nX);
								imprimir(salto,nX);
								imprimir(resetea,nX);
								imprimir(espaciado, nX);
								sprintf(c, "Cajero(a): ");
								imprimir(c,nX);
								imprimir(salto,nX);
								sprintf(c,"%s",cajero);
								imprimir(c,nX);
								imprimir(salto,nX);


								sprintf(c, "_________________");
								imprimir(c,nX);
								imprimir(salto,nX);
								
								//Pagos en efectivo, con tarjeta o cheque
								//Se corrigió la consulta para separar ventas por cajero en el mismo número de caja (INNER JOIN id_usuario)
								sprintf(sql,"SELECT Venta.tipo_pago, FORMAT(SUM( Venta_Articulo.monto ),2)  AS monto_IVA, FORMAT(SUM( Venta_Articulo.monto )  / 1.15,2) AS monto, SUM(Venta_Articulo.monto) FROM Corte_Caja INNER JOIN Venta ON (Corte_Caja.id_caja = Venta.id_caja AND Corte_Caja.id_usuario = Venta.id_usuario) INNER JOIN Venta_Articulo ON Venta.id_venta=Venta_Articulo.id_venta INNER JOIN Articulo ON Venta_Articulo.id_articulo=Articulo.id_articulo WHERE Venta.id_venta BETWEEN Corte_Caja.id_venta_inicio AND Corte_Caja.id_venta_fin AND Venta.cancelada =  'n' AND Corte_Caja.id_corte_caja = %s AND Venta.tipo='contado' GROUP BY Venta.tipo_pago", id_venta);
								//printf("\n%s",sql);
								if((err = mysql_query(&mysql, sql)) != 0){
									printf("Error al consultar los tipos de documentos de Venta Total: %s\n", mysql_error(&mysql));
									imprimiendo = FALSE;
									return (1);
								}
								else{
									if((resultado = mysql_store_result(&mysql))){
										fac_subtotal_num = 0;
										fac_iva_num = 0;
										fac_total_num = 0;
										bandera = 0;
										cambio_num_total = 0;
										fac_cred_total_venta = 0;
										fac_cont_total_venta = 0;
										if (mysql_num_rows(resultado) > 0){
											imprimir(negrita1,nX);
											sprintf(c, "PAGOS:");
											imprimir(c,nX);
											imprimir(salto,nX);
											imprimir(defecto,nX);
											while((row = mysql_fetch_row(resultado)))
											{
												for(i=0; i < 4;i++) //Guarda todo el arreglo en listapos
													listatipos[i] =  row[i];
												bandera ++;
												//Es credito o contado
												if(strcmp(listatipos[0], "efectivo") == 0)
												{
													printf("Entra al if\n");
													strcpy(totales, listatipos[1]);
													printf("Pasa el strcpy\n");
													cambio_num = atof(listatipos[3]);
													cambio_num_total = cambio_num_total + cambio_num;
													fac_cont_total_venta = cambio_num;
													printf("Pasa el atof\n");
													//printf("\n\nEl total de la venta: %.2s\n\n", lista);
													strcpy(temp2,"");
													strcpy(c,"");
													strcpy(temp2,"Efectivo: $ ");
													printf("Pasa los strcpy\n");
													strncat(c,temp3,23-strlen(temp2));
													strcat(c, temp2);
													strncat(c,temp3,11-strlen(totales));
													strcat(c, totales);
													imprimir(c,nX);
													imprimir(salto,nX);


												}else if(strcmp(listatipos[0], "tarjeta") == 0){
													strcpy(totales, listatipos[1]);
													cambio_num = atof(listatipos[3]);
													fac_cred_total_venta = cambio_num;
													cambio_num_total = cambio_num_total + cambio_num;
													strcpy(temp2,"");
													strcpy(c,"");
													strcpy(temp2,"Tarjeta: $ ");
													strncat(c,temp3,23-strlen(temp2));
													strcat(c, temp2);
													strncat(c,temp3,11-strlen(totales));
													strcat(c, totales);
													imprimir(c,nX);
													imprimir(salto,nX);
												}
												else if(strcmp(listatipos[0], "cheque") == 0){
													/*strcpy(totales, listatipos[1]);
													cambio_num = atof(listatipos[3]);
													fac_cred_total_venta = cambio_num;
													cambio_num_total = cambio_num_total + cambio_num;
													strcpy(temp2,"");
													strcpy(c,"");
													strcpy(temp2,"Cheque: $ ");
													strncat(c,temp3,23-strlen(temp2));
													strcat(c, temp2);
													strncat(c,temp3,11-strlen(totales));
													strcat(c, totales);
													imprimir(c,nX);
													imprimir(salto,nX);*/
													sprintf(sql,"SELECT '', FORMAT(SUM(IF(Venta.dinero_recibido > Venta.monto, Venta.dinero_recibido, Venta.monto)),2)  AS monto_IVA, FORMAT(SUM(IF(Venta.dinero_recibido > Venta.monto, Venta.dinero_recibido, Venta.monto))  / 1.15,2) AS monto, SUM(IF(Venta.dinero_recibido > Venta.monto, Venta.dinero_recibido, Venta.monto)) FROM Corte_Caja INNER JOIN Venta ON (Corte_Caja.id_caja = Venta.id_caja AND Corte_Caja.id_usuario = Venta.id_usuario) WHERE Venta.id_venta BETWEEN Corte_Caja.id_venta_inicio AND Corte_Caja.id_venta_fin AND Venta.cancelada =  'n' AND Corte_Caja.id_corte_caja = %s AND Venta.tipo='contado' AND Venta.tipo_pago = 'cheque' GROUP BY Venta.tipo_pago", id_venta);
													printf("\n SQL CHEQUES : %s \n\n",sql);
													if((err = mysql_query(&mysql, sql)) != 0){
														printf("Error al consultar los tipos de documentos de Venta Total: %s\n", mysql_error(&mysql));
														imprimiendo = FALSE;
														return (1);
													}
													else
													{
														if((resultado2 = mysql_store_result(&mysql)))
														{
															if (mysql_num_rows(resultado2) > 0)
															{
																if((row2 = mysql_fetch_row(resultado2)))
																{
																	strcpy(totales, row[1]);
																	cambio_num = atof(row[3]);
																	fac_cred_total_venta = cambio_num;
																	cambio_num_total = cambio_num_total + cambio_num;
																	strcpy(temp2,"");
																	strcpy(c,"");
																	strcpy(temp2,"Cheque: $ ");
																	strncat(c,temp3,23-strlen(temp2));
																	strcat(c, temp2);
																	strncat(c,temp3,11-strlen(totales));
																	strcat(c, totales);
																	imprimir(c,nX);
																	imprimir(salto,nX);
																}
															}
														}
													}
												}
												
												//Guardo la cantidad de contado para despues sacar el dinero en caja
												/*if(strcmp(listatipos[0], "contado") == 0){
													//corte_contado_num = cambio_num;
													sprintf(cambio, "%.2f", atof(listatipos[3]));
													corte_contado_num = atof(cambio);
												}*/

											}

											if(bandera>1)
											{ //Verifica si existe Credito y contado para mostrar la suma de los 2
												sprintf(sql,"SELECT Venta.tipo_pago, FORMAT(SUM( Venta_Articulo.monto ),2)  AS monto_IVA, FORMAT(SUM( Venta_Articulo.monto )  / 1.15,2) AS monto FROM Corte_Caja, Venta, Venta_Articulo, Articulo WHERE Corte_Caja.id_caja = Venta.id_caja AND Corte_Caja.id_usuario = Venta.id_usuario AND Venta.id_venta BETWEEN Corte_Caja.id_venta_inicio AND Corte_Caja.id_venta_fin AND Venta_Articulo.id_venta = Venta.id_venta AND Articulo.id_articulo = Venta_Articulo.id_articulo AND Venta.cancelada =  'n' AND Corte_Caja.id_corte_caja = %s AND Venta.tipo = 'contado' GROUP BY Corte_Caja.id_corte_caja", id_venta);
												if((err = mysql_query(&mysql, sql)) != 0)
												{
													printf("Error al consultar los tipos de documentos de Venta Total: %s\n", mysql_error(&mysql));
													imprimiendo = FALSE;
													return (1);
												}
												else
												{
													if((resultado = mysql_store_result(&mysql)))
													{
														row = mysql_fetch_row(resultado);
														sprintf(cambio,"%s", row[1]);
													}
												}
												//Calcula el Total

												strcpy(temp2,"");
												strcpy(c,"");
												strcpy(temp2,"TOTAL: $ ");
												strncat(c,temp3,23-strlen(temp2));
												strcat(c, temp2);
												strncat(c,temp3,11-strlen(cambio));
												strcat(c, cambio);
												imprimir(c,nX);
												imprimir(salto,nX);
											}
										}//Fin del > 0
									}else{
										imprimiendo = FALSE;
										return (1);
									}
								}
								//FIN DE TIPOS DE PAGO
								imprimir(salto,nX);
								

								sprintf(c, "_________________");
								imprimir(c,nX);
								imprimir(salto,nX);
								
								//Credito y contado TOTAL e IVA
								sprintf(sql,"SELECT Venta.tipo, FORMAT(SUM( Venta_Articulo.monto ),2)  AS monto_IVA, FORMAT(SUM( Venta_Articulo.monto )  / 1.15,2) AS monto, SUM(Venta_Articulo.monto) FROM Corte_Caja INNER JOIN Venta ON (Corte_Caja.id_caja = Venta.id_caja AND Corte_Caja.id_usuario = Venta.id_usuario) INNER JOIN Venta_Articulo ON Venta.id_venta=Venta_Articulo.id_venta INNER JOIN Articulo ON Venta_Articulo.id_articulo=Articulo.id_articulo WHERE Venta.id_venta BETWEEN Corte_Caja.id_venta_inicio AND Corte_Caja.id_venta_fin AND Venta.cancelada =  'n' AND Corte_Caja.id_corte_caja = %s GROUP BY Venta.tipo", id_venta);
								//printf("\n%s",sql);
								if((err = mysql_query(&mysql, sql)) != 0){
									printf("Error al consultar los tipos de documentos de Venta Total: %s\n", mysql_error(&mysql));
									imprimiendo = FALSE;
									return (1);
								}
								else{
									if((resultado = mysql_store_result(&mysql))){
										fac_subtotal_num = 0;
										fac_iva_num = 0;
										fac_total_num = 0;
										bandera = 0;
										cambio_num_total = 0;
										fac_cred_total_venta = 0;
										fac_cont_total_venta = 0;
										if (mysql_num_rows(resultado) > 0){
											imprimir(negrita1,nX);
											sprintf(c, "VENTA TOTAL:");
											imprimir(c,nX);
											imprimir(salto,nX);
											imprimir(defecto,nX);
											while((row = mysql_fetch_row(resultado)))
											{
												for(i=0; i < 4;i++) //Guarda todo el arreglo en listapos
													listatipos[i] =  row[i];
												bandera ++;
												//Es credito o contado
												if(strcmp(listatipos[0], "contado") == 0)
												{
													strcpy(totales, listatipos[1]);
													cambio_num = atof(listatipos[3]);
													cambio_num_total = cambio_num_total + cambio_num;
													fac_cont_total_venta = cambio_num;
													//printf("\n\nEl total de la venta: %.2s\n\n", lista);
													strcpy(temp2,"");
													strcpy(c,"");
													strcpy(temp2,"Contado: $ ");
													strncat(c,temp3,23-strlen(temp2));
													strcat(c, temp2);
													strncat(c,temp3,11-strlen(totales));
													strcat(c, totales);
													imprimir(c,nX);
													imprimir(salto,nX);


												}else if(strcmp(listatipos[0], "credito") == 0){
													strcpy(totales, listatipos[1]);
													cambio_num = atof(listatipos[3]);
													fac_cred_total_venta = cambio_num;
													cambio_num_total = cambio_num_total + cambio_num;
													strcpy(temp2,"");
													strcpy(c,"");
													strcpy(temp2,"Credito: $ ");
													strncat(c,temp3,23-strlen(temp2));
													strcat(c, temp2);
													strncat(c,temp3,11-strlen(totales));
													strcat(c, totales);
													imprimir(c,nX);
													imprimir(salto,nX);
												}

												//Guardo la cantidad de contado para despues sacar el dinero en caja
												if(strcmp(listatipos[0], "contado") == 0){
													//corte_contado_num = cambio_num;

													sprintf(cambio, "%.2f", atof(listatipos[3]));
													corte_contado_num = atof(cambio);
												}

											}

											if(bandera>1)
											{ //Verifica si existe Credito y contado para mostrar la suma de los 2
												sprintf(sql,"SELECT Venta.tipo, FORMAT(SUM( Venta_Articulo.monto ),2)  AS monto_IVA, FORMAT(SUM( Venta_Articulo.monto )  / 1.15,2) AS monto FROM Corte_Caja, Venta, Venta_Articulo, Articulo WHERE Corte_Caja.id_caja = Venta.id_caja AND Corte_Caja.id_usuario = Venta.id_usuario AND Venta.id_venta BETWEEN Corte_Caja.id_venta_inicio AND Corte_Caja.id_venta_fin AND Venta_Articulo.id_venta = Venta.id_venta AND Articulo.id_articulo = Venta_Articulo.id_articulo AND Venta.cancelada =  'n' AND Corte_Caja.id_corte_caja = %s GROUP BY Corte_Caja.id_corte_caja", id_venta);
												if((err = mysql_query(&mysql, sql)) != 0)
												{
													printf("Error al consultar los tipos de documentos de Venta Total: %s\n", mysql_error(&mysql));
													imprimiendo = FALSE;
													return (1);
												}
												else
												{
													if((resultado = mysql_store_result(&mysql)))
													{
														row = mysql_fetch_row(resultado);
														sprintf(cambio,"%s", row[1]);
													}
												}
												//Calcula el Total

												strcpy(temp2,"");
												strcpy(c,"");
												strcpy(temp2,"TOTAL: $ ");
												printf("Pasa el TOTAL\n\n");
												strncat(c,temp3,23-strlen(temp2));
												strcat(c, temp2);
												strncat(c,temp3,11-strlen(cambio));
												strcat(c, cambio);
												imprimir(c,nX);
												imprimir(salto,nX);
											}
										}//Fin del > 0
									}else{
										imprimiendo = FALSE;
										return (1);
									}
								}
								
								//FACTURAS ya sea de credito o contado
								sprintf(sql,"SELECT Venta.tipo, FORMAT(SUM(Venta.monto),2) AS Monto_IVA, FORMAT((SUM(Venta.monto)  / 1.15),2) AS Monto, SUM(Venta.monto) FROM Corte_Caja INNER JOIN Venta ON (Corte_Caja.id_caja = Venta.id_caja AND Corte_Caja.id_usuario = Venta.id_usuario) WHERE Venta.id_venta BETWEEN Corte_Caja.id_venta_inicio AND Corte_Caja.id_venta_fin AND Venta.cancelada = 'n' AND Venta.num_factura <> 0 AND Corte_Caja.id_corte_caja = %s GROUP BY Venta.tipo", id_venta);
								printf("\n%s\n",sql);
								if((err = mysql_query(&mysql, sql)) != 0){
									printf("Error al consultar los tipos de documentos: %s\n", mysql_error(&mysql));
									imprimiendo = FALSE;
									return (1);
								}
								else{
									if((resultado = mysql_store_result(&mysql))){
										fac_subtotal_num = 0;
										fac_iva_num = 0;
										fac_total_num = 0;
										bandera = 0;
										bandera2 = 0;
										fac_cont_subtotal = 0;
										fac_cont_iva = 0;

										fac_cont_total = 0;
										fac_cred_subtotal = 0;
										fac_credcont_total = 0;
										fac_credcont_iva = 0;
										fac_credcont_subtotal = 0;
										fac_cred_iva = 0;
										fac_cred_total = 0;
										cambio_num_total = 0;
										cambio_num = 0;



										if (mysql_num_rows(resultado) > 0){
											//sprintf(c, "_________________\n"); imprimir(c,nX);
											//sprintf(c, "FACTURAS:\n"); imprimir(c,nX);
											imprimir(negrita1,nX);
											sprintf(c, "\nVENTA FACTURADA:\n"); imprimir(c,nX);
											imprimir(defecto,nX);
											while((row = mysql_fetch_row(resultado)))
											{
												for(i=0; i < 4;i++) //Guarda todo el arreglo en listapos
													listatipos[i] =  row[i];
												printf("Tipo: %s\n", row[0]);
												printf("Contado: %s\n", row[1]);
												printf("Credito: %s\n", row[2]);
												//Es credito o contado

												if(strcmp(row[0], "contado") == 0 )
												{
													fac_cont_total = fac_cont_total + atof(row[3]);
													if(fac_cont_total > 0)
													{
														cambio_num_total = cambio_num_total + fac_cont_total;
														sprintf(cambio, "%.2f", fac_cont_total);
														strcpy(temp2,"");
														strcpy(c,"");
														strcpy(temp2,"Contado: $ ");
														strncat(c,temp3,23-strlen(temp2));
														strcat(c, temp2);
														strncat(c,temp3,11-strlen(row[1]));
														strcat(c, row[1]);
           													imprimir(c,nX);
														imprimir(salto,nX);
														bandera++;
													}
												}
												else if(strcmp(row[0], "credito") == 0)
												{
													fac_cred_total = fac_cred_total + atof(row[3]);
													if(fac_cred_total > 0)
													{
														cambio_num_total = cambio_num_total + fac_cred_total;
														sprintf(cambio, "%.2f", fac_cred_total);
														strcpy(temp2,"");
														strcpy(c,"");
														strcpy(temp2,"Credito: $ ");
														strncat(c,temp3,23-strlen(temp2));
														strcat(c, temp2);
														strncat(c,temp3,11-strlen(row[1]));
														strcat(c, row[1]);
														imprimir(c,nX);
														imprimir(salto,nX);
														bandera ++;
													}
												}
											}

											//Verifica si existe Credito y contado para mostrar la suma de los 2
											if(bandera == 2)
											{
												sprintf(sql,"SELECT FORMAT(SUM(Venta.monto),2) AS Monto_IVA, FORMAT((SUM(Venta.monto)  / 1.15),2) AS Monto, SUM(Venta.monto) FROM Corte_Caja INNER JOIN Venta ON (Corte_Caja.id_caja = Venta.id_caja AND Corte_Caja.id_usuario = Venta.id_usuario) WHERE Venta.id_venta BETWEEN Corte_Caja.id_venta_inicio AND Corte_Caja.id_venta_fin AND Venta.cancelada = 'n' AND Venta.num_factura <> 0 AND Corte_Caja.id_corte_caja = %s GROUP BY Corte_Caja.id_corte_caja", id_venta);
												if((err = mysql_query(&mysql, sql)) != 0)
												{
													printf("Error al consultar los tipos de documentos de Venta Total: %s\n", mysql_error(&mysql));
													imprimiendo = FALSE;
													return (1);
												}
												else
												{
													if((resultado = mysql_store_result(&mysql)))
													{
														row = mysql_fetch_row(resultado);
														sprintf(cambio,"%s", row[0]);
													}
												}

												//sprintf(cambio, "%.2f", cambio_num_total);
												strcpy(temp2,"");
												strcpy(c,"");
												strcpy(temp2,"TOTAL: $ ");
												strncat(c,temp3,23-strlen(temp2));
												strcat(c, temp2);
												strncat(c,temp3,11-strlen(cambio));
												strcat(c, cambio);
												imprimir(c,nX);
												imprimir(salto,nX);
											}
											
										}
									}else{
										imprimiendo = FALSE;
										return (1);
									}
								}


								imprimir(salto,nX);
								imprimir(negrita1,nX);
								sprintf(c, "VENTA TICKETS:");
								imprimir(c,nX);
								imprimir(salto,nX);
								imprimir(defecto,nX);

								sprintf(cambio, "%.2f", fac_cont_total_venta - fac_cont_total);
								strcpy(temp2,"");
								strcpy(c,"");
								strcpy(temp2,"Contado: $ ");
								strncat(c,temp3,23-strlen(temp2));
								strcat(c, temp2);
									sprintf(sql, "SELECT FORMAT(%s, 2)", cambio);
									err = mysql_query(&mysql, sql);
									if(err == 0)
									{
										resultado = mysql_store_result(&mysql);
										if(resultado)
										{
											if((row=mysql_fetch_row(resultado)))
												strcpy(cambio, row[0]);
										}
									}
									else
									{
										sprintf(Errores,"%s",mysql_error(&mysql));
										Err_Info(Errores);
									}
								strncat(c,temp3,11-strlen(cambio));
								strcat(c, cambio);
        							imprimir(c,nX);
								imprimir(salto,nX);

								sprintf(cambio, "%.2f", fac_cred_total_venta - fac_cred_total);
								strcpy(temp2,"");
								strcpy(c,"");
								strcpy(temp2,"Credito: $ ");
								strncat(c,temp3,23-strlen(temp2));
								strcat(c, temp2);
									sprintf(sql, "SELECT FORMAT(%s, 2)", cambio);
									err = mysql_query(&mysql, sql);
									if(err == 0)
									{
										resultado = mysql_store_result(&mysql);
										if(resultado)
										{
											if((row=mysql_fetch_row(resultado)))
												strcpy(cambio, row[0]);
										}
									}
									else
									{
										sprintf(Errores,"%s",mysql_error(&mysql));
										Err_Info(Errores);
									}
								strncat(c,temp3,11-strlen(cambio));
								strcat(c, cambio);
        							imprimir(c,nX);
								imprimir(salto,nX);


								sprintf(cambio, "%.2f", (fac_cred_total_venta - fac_cred_total) + (fac_cont_total_venta - fac_cont_total));
								strcpy(temp2,"");
								strcpy(c,"");
								strcpy(temp2,"TOTAL: $ ");
								strncat(c,temp3,23-strlen(temp2));
								strcat(c, temp2);
									sprintf(sql, "SELECT FORMAT(%s, 2)", cambio);
									err = mysql_query(&mysql, sql);
									if(err == 0)
									{
										resultado = mysql_store_result(&mysql);
										if(resultado)
										{
											if((row=mysql_fetch_row(resultado)))
												strcpy(cambio, row[0]);
										}
									}
									else
									{
										sprintf(Errores,"%s",mysql_error(&mysql));
										Err_Info(Errores);
									}
								strncat(c,temp3,11-strlen(cambio));
								strcat(c, cambio);
								imprimir(c,nX);
								imprimir(salto,nX);
								imprimir(salto,nX);
								
								//Cancelaciones
								strcpy(sql,"SELECT Venta_Cancelada.id_venta, Venta_Cancelada.observaciones, Cliente.nombre FROM Venta_Cancelada INNER  JOIN Venta ON Venta_Cancelada.id_venta = Venta.id_venta INNER  JOIN Cliente ON Cliente.id_cliente = Venta.id_cliente INNER JOIN Corte_Caja ON (Venta.id_venta BETWEEN Corte_Caja.id_venta_inicio AND Corte_Caja.id_venta_fin AND Corte_Caja.id_caja = Venta.id_caja AND Corte_Caja.id_usuario = Venta.id_usuario) WHERE Venta.cancelada =  's' AND Corte_Caja.id_corte_caja = ");
								strcat(sql, id_venta);

								if((err = mysql_query(&mysql, sql)) != 0){
									printf("Error al consultar los tipos de documentos: %s\n", mysql_error(&mysql));
									return (1);
								}else{
									if((resultado = mysql_store_result(&mysql))){

										if (mysql_num_rows(resultado) > 0){
											sprintf(c, "_________________\n"); imprimir(c,nX);
											imprimir(negrita1,nX);
											sprintf(c, "CANCELACIONES:\n"); imprimir(c,nX);
											imprimir(defecto,nX);
											while((row = mysql_fetch_row(resultado))){
												for(i=0; i < 3;i++) //Guarda todo el arreglo en listapos
													listatipos[i] =  row[i];

												//Cancelaciones FOLIO
												sprintf(c, "    Folio: %s\n",listatipos[0]); imprimir(c,nX);
												//Cancelaciones Cliente
												sprintf(c, "    Cliente:\n"); imprimir(c,nX);
												sprintf(c,"%s\n",listatipos[2]); imprimir(c,nX);
												sprintf(c, "    Observaciones:\n"); imprimir(c,nX);
												sprintf(c,"%s",listatipos[1]); imprimir(c,nX);
												sprintf(c,"\n\n"); imprimir(c,nX);
											}
										}else{
											//return (1);
										}
									}else{
										imprimiendo = FALSE;
										return (1);
									}
								}

								//Retiro Efectivo
								strcpy(sql,"SELECT Retiro.fecha, Retiro.hora, /*FORMAT(*/Retiro.monto/*,2)*/, CONCAT_WS(  '  ', Cajero.nombre, Cajero.apellido )  AS cajero, CONCAT_WS(  '  ', UsuarioRetira.nombre, UsuarioRetira.apellido )  AS UsuarioRetira FROM Usuario Cajero, Usuario UsuarioRetira, Corte_Caja INNER  JOIN Retiro ON Retiro.id_retiro BETWEEN Corte_Caja.id_retiro_inicio AND Corte_Caja.id_retiro_fin AND Corte_Caja.id_caja = Retiro.id_caja WHERE Cajero.id_usuario = Retiro.id_usuario_cajero AND UsuarioRetira.id_usuario = Retiro.id_usuario_retira AND Corte_Caja.id_corte_caja = ");
								strcat(sql, id_venta);

								if((err = mysql_query(&mysql, sql)) != 0){
									printf("Error al consultar los tipos de documentos: %s\n", mysql_error(&mysql));
									imprimiendo = FALSE;
									return (1);
								}else{
									if((resultado = mysql_store_result(&mysql))){

										if (mysql_num_rows(resultado) > 0){
											sprintf(c, "_________________\n"); imprimir(c,nX);
											imprimir(negrita1,nX);
											sprintf(c, "RETIRO DE EFECTIVO:\n"); imprimir(c,nX);
											imprimir(defecto,nX);
											while((row = mysql_fetch_row(resultado))){
												for(i=0; i < 5;i++) //Guarda todo el arreglo en listapos
													listatipos[i] =  row[i];
												//Fecha Y Hora
												sprintf(c, "Fecha: %c%c-%c%c-%c%c%c%c Hora: %s\n",listatipos[0][8],listatipos[0][9],listatipos[0][5],listatipos[0][6],listatipos[0][0],listatipos[0][1],listatipos[0][2],listatipos[0][3],listatipos[1]); imprimir(c,nX);
												//Cajero
												sprintf(c, "Cajero(a): %s\n",listatipos[3]); imprimir(c,nX);
												//Cancelaciones Cliente
												sprintf(c, "Retiro: %s\n",listatipos[4]); imprimir(c,nX);
												//Monto dle retiro
												fac_subtotal_num = atof(listatipos[2]);
												sprintf(cambio, "%.2f", fac_subtotal_num);
												strcpy(temp2,"");
												strcpy(c,"");
												strcpy(temp2,"Cantidad: $ ");
												strncat(c,temp3,12-strlen(temp2));
												strcat(c, temp2);
													sprintf(sql, "SELECT FORMAT(%s, 2)", cambio);
													err = mysql_query(&mysql, sql);
													if(err == 0)
													{
														resultado2 = mysql_store_result(&mysql);
														if(resultado2)
														{
															if((row2=mysql_fetch_row(resultado2)))
																strcpy(cambio, row2[0]);
														}
													}
													else
													{
														sprintf(Errores,"%s",mysql_error(&mysql));
														Err_Info(Errores);
													}
												strncat(c,temp3,11-strlen(cambio));
												strcat(c, cambio);
												strcat(c, "\n\n");
												imprimir(c,nX);
												corte_retiro_num = corte_retiro_num + fac_subtotal_num;
											}
											strcpy(temp2,"");
											strcpy(c,""); strcpy(cambio,"---------");
											strcpy(temp2," ");
											strncat(c,temp3,23-strlen(temp2));
											strcat(c, temp2);
											strncat(c,temp3,11-strlen(cambio));
											strcat(c, cambio);
											strcat(c, "\n");
											imprimir(c,nX);
											//Calcula el Total
											sprintf(cambio, "%.2f", corte_retiro_num);
											strcpy(temp2,"");
											strcpy(c,"");
											strcpy(temp2,"TOTAL: $ ");
											strncat(c,temp3,23-strlen(temp2));
											strcat(c, temp2);
												sprintf(sql, "SELECT FORMAT(%s, 2)", cambio);
												err = mysql_query(&mysql, sql);
												if(err == 0)
												{
													resultado = mysql_store_result(&mysql);
													if(resultado)
													{
														if((row=mysql_fetch_row(resultado)))
															strcpy(cambio, row[0]);
													}
												}
												else
												{
													sprintf(Errores,"%s",mysql_error(&mysql));
													Err_Info(Errores);
												}
											strncat(c,temp3,11-strlen(cambio));
											strcat(c, cambio);
											imprimir(c,nX);
											imprimir(salto,nX);
											imprimir(salto,nX);

										}else{
											//return (1);
										}
									}else{
										imprimiendo = FALSE;
										return (1);
									}
								}
							sprintf(c, "_________________\n"); imprimir(c,nX);
							//Monto dle retiro
							corte_dinero_caja_num = corte_contado_num - corte_retiro_num;
							sprintf(cambio, "%.2f", corte_dinero_caja_num);
							strcpy(temp2,"");
							strcpy(c,"");
							strcpy(temp2,"DINERO EN CAJA: $ ");
							strncat(c,temp3,18-strlen(temp2));
							strcat(c, temp2);
								sprintf(sql, "SELECT FORMAT(%s, 2)", cambio);
								err = mysql_query(&mysql, sql);
								if(err == 0)
								{
									resultado = mysql_store_result(&mysql);
									if(resultado)
									{
										if((row=mysql_fetch_row(resultado)))
											strcpy(cambio, row[0]);
									}
								}
								else
								{
									sprintf(Errores,"%s",mysql_error(&mysql));
									Err_Info(Errores);
								}
							strncat(c,temp3,16-strlen(cambio));
							strcat(c, cambio);
							imprimir(negrita1,nX);
							imprimir(c,nX);
							imprimir(salto,nX);
							imprimir(defecto,nX);


							//Numero de Clientes

							sprintf(sql,"SELECT COUNT(*) FROM Corte_Caja INNER JOIN Venta ON (Corte_Caja.id_caja = Venta.id_caja AND Corte_Caja.id_usuario = Venta.id_usuario) WHERE Venta.id_venta BETWEEN Corte_Caja.id_venta_inicio AND Corte_Caja.id_venta_fin AND Venta.cancelada='n' AND Corte_Caja.id_corte_caja = %s GROUP BY Corte_Caja.id_corte_caja", id_venta);
							
							if((err = mysql_query(&mysql, sql)) != 0){
								printf("Error al consultar el total de clientes: %s\n", mysql_error(&mysql));
								imprimiendo = FALSE;
								return (1);
							}else{
								if((resultado = mysql_store_result(&mysql))){
									if (mysql_num_rows(resultado) > 0){
										row = mysql_fetch_row(resultado);
	 						//			sprintf(c, "_____________________________________\n"); imprimir(c,nX);

										sprintf(cambio, "%s", row[0]);
										strcpy(temp2,"");
										strcpy(c,"");
										strcpy(temp2,"DINERO EN CAJA: $ ");
										strcpy(temp2,"CLIENTES ATENDIDOS: ");
										strncat(c,temp3,20-strlen(temp2));
										strcat(c, temp2);
										strncat(c,temp3,14-strlen(cambio));
										strcat(c, cambio);
										strcat(c, "\n\n");
										imprimir(c,nX);
										
										//sprintf(c, "CLIENTES ATENDIDOS: %s\n", row[0]); imprimir(c,nX);

									}else{
										//return (1);
									}
								}else{
									imprimiendo = FALSE;
									return (1);
								}
							}

							//Resumen de documentos
							sprintf(sql,"SELECT Venta.id_venta, Venta.num_factura, FORMAT(Venta.monto,2) FROM Venta INNER JOIN Corte_Caja ON (Corte_Caja.id_caja = Venta.id_caja AND Corte_Caja.id_usuario = Venta.id_usuario) WHERE Venta.tipo='credito' AND Venta.cancelada='n' AND Venta.id_venta BETWEEN Corte_Caja.id_venta_inicio AND Corte_Caja.id_venta_fin AND Corte_Caja.id_corte_caja=%s", id_venta);
							if((err = mysql_query(&mysql, sql)) != 0)
							{
								printf("Error al consultar los tipos de documentos: %s\n", mysql_error(&mysql));
								imprimiendo = FALSE;
								return (1);
							}
							else
							{
								resultado = mysql_store_result(&mysql);
								if(resultado)
								{
									sprintf(c,"_____________________");imprimir(c,nX);
									sprintf(c,"%s",salto);imprimir(c,nX);
									imprimir(negrita1,nX);
									sprintf(c,"DOCUMENTOS DE CREDITO");imprimir(c,nX);
									imprimir(defecto,nX);
									sprintf(c,"%s",salto);imprimir(c,nX);
									sprintf(c,"FOLIO      FACTURA             MONTO");imprimir(c,nX);
									sprintf(c,"%s",salto);imprimir(c,nX);
									sprintf(c,"------------------------------------");imprimir(c,nX);
									sprintf(c,"%s",salto);imprimir(c,nX);
									while((row=mysql_fetch_row(resultado)))
									{
										sprintf(c, "%s", row[0]);
										if(row[1])
										{
											strncat(c, temp3, 13-strlen(row[1]));
											strcat(c, row[1]);
										}
										else
											strncat(c, temp3, 13);
										
										strncat(c, temp3, 18-strlen(row[2]));
										strcat(c, row[2]);
										imprimir(c,nX);
										sprintf(c,"%s",salto);
										imprimir(c,nX);
									}
									sprintf(c,"                        ------------");imprimir(c,nX);
									imprimir(salto,nX);
									sprintf(sql,"SELECT FORMAT(SUM(Venta.monto),2) FROM Venta INNER JOIN Corte_Caja ON (Corte_Caja.id_caja = Venta.id_caja AND Corte_Caja.id_usuario = Venta.id_usuario) WHERE Venta.tipo='credito' AND Venta.cancelada='n' AND Venta.id_venta BETWEEN Corte_Caja.id_venta_inicio AND Corte_Caja.id_venta_fin AND Corte_Caja.id_corte_caja=%s GROUP BY Corte_Caja.id_corte_caja", id_venta);
									if((err = mysql_query(&mysql, sql)) != 0)
									{
										printf("Error al consultar los tipos de documentos: %s\n", mysql_error(&mysql));
										return (1);
									}
									else
									{
										resultado = mysql_store_result(&mysql);
										if(resultado)
										{
											if((row=mysql_fetch_row(resultado)))
											{
												sprintf(c,"TOTAL DOCUMENTOS: $");
												strncat(c,temp3,17-strlen(row[0]));
												strcat(c,row[0]);
												imprimir(c,nX);
											}
										}
									}
									sprintf(c,"%s",salto);imprimir(c,nX);
									sprintf(c,"%s",salto);imprimir(c,nX);
								}
								else
									printf("Ocurrió un error al obtener el resultado de documentos: %s\n", mysql_error(&mysql));
							}
							
							//Resumen de cheques
							sprintf(sql,"SELECT Venta.id_venta, Venta.referencia_pago, FORMAT(Venta.monto,2) FROM Venta INNER JOIN Corte_Caja ON (Corte_Caja.id_caja = Venta.id_caja AND Corte_Caja.id_usuario = Venta.id_usuario) WHERE Venta.tipo_pago='cheque' AND Venta.cancelada='n' AND Venta.id_venta BETWEEN Corte_Caja.id_venta_inicio AND Corte_Caja.id_venta_fin AND Corte_Caja.id_corte_caja=%s", id_venta);
							if((err = mysql_query(&mysql, sql)) != 0)
							{
								printf("Error al consultar los tipos de documentos: %s\n", mysql_error(&mysql));
								imprimiendo = FALSE;
								return (1);
							}
							else
							{
								resultado = mysql_store_result(&mysql);
								if(resultado)
								{
									sprintf(c,"_____________________");imprimir(c,nX);
									sprintf(c,"%s",salto);imprimir(c,nX);
									imprimir(negrita1,nX);
									sprintf(c,"CHEQUES ");imprimir(c,nX);
									imprimir(defecto,nX);
									sprintf(c,"%s",salto);imprimir(c,nX);
									sprintf(c,"FOLIO              CHEQUE      MONTO");imprimir(c,nX);
									sprintf(c,"%s",salto);imprimir(c,nX);
									sprintf(c,"------------------------------------");imprimir(c,nX);
									sprintf(c,"%s",salto);imprimir(c,nX);
									while((row=mysql_fetch_row(resultado)))
									{
										sprintf(c, "%s", row[0]);
										if(row[1])
										{
											strncat(c, temp3, 20-strlen(row[1]));
											strcat(c, row[1]);
										}
										else
											strncat(c, temp3, 20);
										
										strncat(c, temp3, 11-strlen(row[2]));
										strcat(c, row[2]);
										imprimir(c,nX);
										sprintf(c,"%s",salto);
										imprimir(c,nX);
									}
									sprintf(c,"                        ------------");imprimir(c,nX);
									imprimir(salto,nX);
									sprintf(sql,"SELECT FORMAT(SUM(Venta.monto),2) FROM Venta INNER JOIN Corte_Caja ON (Corte_Caja.id_caja = Venta.id_caja AND Corte_Caja.id_usuario = Venta.id_usuario) WHERE Venta.tipo_pago='cheque' AND Venta.cancelada='n' AND Venta.id_venta BETWEEN Corte_Caja.id_venta_inicio AND Corte_Caja.id_venta_fin AND Corte_Caja.id_corte_caja=%s GROUP BY Corte_Caja.id_corte_caja", id_venta);
									if((err = mysql_query(&mysql, sql)) != 0)
									{
										printf("Error al consultar los tipos de documentos: %s\n", mysql_error(&mysql));
										return (1);
									}
									else
									{
										resultado = mysql_store_result(&mysql);
										if(resultado)
										{
											if((row=mysql_fetch_row(resultado)))
											{
												sprintf(c,"TOTAL CHEQUES: $");
												strncat(c,temp3,17-strlen(row[0]));
												strcat(c,row[0]);
												imprimir(c,nX);
											}
										}
									}
									sprintf(c,"%s",salto);imprimir(c,nX);
									sprintf(c,"%s",salto);imprimir(c,nX);
								}
								else
									printf("Ocurrió un error al obtener el resultado de documentos: %s\n", mysql_error(&mysql));
							}//Fin de cheques
							
							if(corte_final)
							{
								imprimir(salto,nX);
	                                                        imprimir(salto,nX);
        	                                                //imprimir(corta_papel,nX);
							//Cantidad vendida de kilos
							sprintf(sql,"SELECT Venta_Articulo.id_articulo, SUM( Venta_Articulo.monto ), (SUM( Venta_Articulo.monto )  / 1.15), SUM( Venta_Articulo.cantidad ), Articulo.nombre, Articulo.tipo AS tipo FROM Corte_Caja INNER JOIN Venta ON (Corte_Caja.id_caja = Venta.id_caja AND Corte_Caja.id_usuario = Venta.id_usuario) INNER JOIN Venta_Articulo ON Venta_Articulo.id_venta = Venta.id_venta INNER JOIN Articulo ON Articulo.id_articulo = Venta_Articulo.id_articulo WHERE Venta_Articulo.id_venta BETWEEN Corte_Caja.id_venta_inicio AND Corte_Caja.id_venta_fin AND Venta.cancelada = 'n' AND Corte_Caja.id_corte_caja = %s GROUP  BY Venta_Articulo.id_articulo ORDER BY tipo,cantidad DESC", id_venta);

							if((err = mysql_query(&mysql, sql)) != 0){
								printf("Error al consultar los tipos de documentos: %s\n", mysql_error(&mysql));
								imprimiendo = FALSE;
								return (1);
							}else{
								if((resultado = mysql_store_result(&mysql))){
									if (mysql_num_rows(resultado) > 0){
	 									sprintf(c, "_________________");
										imprimir(c,nX);
										imprimir(salto,nX);
										sprintf(c, "ARTICULOS VENDIDOS:");
										imprimir(negrita1,nX);
										imprimir(c,nX);
										imprimir(salto,nX);
										imprimir(defecto,nX);
										sprintf(c, "Articulo                     Cantidad");
										imprimir(c,nX);
										imprimir(salto,nX);
										sprintf(c, "----------                 ----------");
										imprimir(c,nX);
										imprimir(salto,nX);
										corte_dinero_caja_num = 0;
										while((row = mysql_fetch_row(resultado))){
											for(i=0; i < 6;i++) //Guarda todo el arreglo en listapos
												listatipos[i] =  row[i];

											fac_subtotal_num = atof(listatipos[3]);
											if(strcmp(row[5], "peso")==0)
											{
												sprintf(cambio, "%.3f kg", fac_subtotal_num);
												corte_peso = corte_peso + fac_subtotal_num;
											}
											else if(strcmp(row[5], "pieza")==0)
											{
												sprintf(cambio, "%.3f pz", fac_subtotal_num);
												corte_articulos = corte_articulos + fac_subtotal_num;
											}

											//corte_dinero_caja_num = corte_dinero_caja_num + atof(cambio);
											strcpy(temp2,"");
											strcpy(c,"");
											for(i=0;i<25;i++){
												if( i >=strlen(listatipos[4]))
													temp2[i] = ' ';
												else
													temp2[i] = listatipos[4][i];
												temp2[i+1] = '\0';
											}
											strcat(c, temp2);
											strncat(c,temp3,12-strlen(cambio));
											strcat(c, cambio);
											imprimir(c,nX);
											imprimir(salto,nX);


										}
										//Monto dle retiro
										strcpy(cambio,"");
										sprintf(cambio, "%.3f",corte_peso);
										sprintf(c, "TOTAL KILOS:  %s",cambio);
										imprimir(salto,nX);
										imprimir(negrita1,nX);
										imprimir(c,nX);
										imprimir(salto,nX);
										imprimir(defecto,nX);
										strcpy(cambio,"");
										sprintf(cambio, "%.2f",corte_articulos);
										sprintf(c, "TOTAL PIEZAS: %s",cambio);
										imprimir(c,nX);
										imprimir(salto,nX);
									}else{
										//return (1);
									}
								}else{
									imprimiendo = FALSE;
									return (1);
								}
							}
							imprimir(salto,nX);
							}

							//SE IMPRIMEN NUEVAMENTE LOS DATOS DEL(A) CAJERO(A)
								imprimir(salto,nX);
								imprimir(cancela,nX);
								imprimir(tamano1,nX);
								imprimir(alinea_i,nX);
								imprimir(negrita,nX);
								sprintf(c, "Caja: %s\n",cajaCorte);
								imprimir(c,nX);
								//sprintf(c, "Fecha: %c%c-%c%c-%c%c%c%c Hora: %s",listatipos[1][8],listatipos[1][9],listatipos[1][5],listatipos[1][6],listatipos[1][0],listatipos[1][1],listatipos[1][2],listatipos[1][3],listatipos[2]);
								sprintf (c,"%s", fechaCorte);
								imprimir(c,nX);
								imprimir(salto,nX);
								imprimir(resetea,nX);
								imprimir(espaciado, nX);
								sprintf(c, "Cajero(a): ");
								imprimir(c,nX);
								imprimir(salto,nX);
								sprintf(c,"%s",cajero);
								imprimir(c,nX);
								imprimir(salto,nX);
							
							//SE OBTIENE LOS KILOS VENDIDOS POR BASCULA
							if (corte_final)
							{
								sprintf(sql,"SELECT FORMAT(SUM(Venta_Articulo.cantidad * Articulo_Lista.precio),2), FORMAT(SUM(Venta_Articulo.cantidad),3), Basculas.bascula, Venta_Articulo.id_bascula FROM Corte_Caja INNER  JOIN Venta ON (Corte_Caja.id_caja = Venta.id_caja AND Corte_Caja.id_usuario = Venta.id_usuario) INNER  JOIN Venta_Articulo ON Venta_Articulo.id_venta = Venta.id_venta INNER JOIN Articulo_Lista ON Articulo_Lista.id_articulo = Venta_Articulo.id_articulo INNER JOIN Basculas ON Basculas.id_bascula = Venta_Articulo.id_bascula WHERE Venta_Articulo.id_venta BETWEEN Corte_Caja.id_venta_inicio AND Corte_Caja.id_venta_fin AND Venta.cancelada = 'n' AND Corte_Caja.id_corte_caja = %s AND Articulo_Lista.id_lista = 1 GROUP BY Venta_Articulo.id_bascula", id_venta);
								
								//printf ("####### SWL = %s",sql);
								if((err = mysql_query(&mysql, sql)) != 0)
								{
									printf("Error al consultar los tipos de documentos de Venta Total: %s\n", mysql_error(&mysql));
									imprimiendo = FALSE;
									return (1);
								}
								else
								{
									if((resultado = mysql_store_result(&mysql)))
									{
										while (row = mysql_fetch_row(resultado))
										{
											//sprintf(kilos_bascula, row[0]);
											//sprintf(cambio, "%.2f", cambio_num_total);
											strcpy(temp2,"");
											strcpy(c,row[2]);
											imprimir (c,nX);
											imprimir(salto,nX);
											
											//SE IMPRIME LA VENTA DE LA BASCULA
											strcpy(c,"   IMPORTE: ");
											imprimir(c,nX);
											sprintf(c,"%s", row[0]);
											imprimir(c,nX);
											imprimir(salto,nX);
											
											//SE IMPRIMEN LOS KILOS VENDIDOS
											strcpy(c,"   KILOS: ");
											imprimir(c,nX);
											sprintf(c,"%s", row[1]);
											strcat (c," kg");
											imprimir(c,nX);
											
											//strncat(c,temp3,23-strlen(temp2));
											/*strcat(c, temp2);
											//strncat(c,temp3,11-strlen(kilos_bascula));
											strcat(c, kilos_bascula);
											imprimir(c,nX);*/
											imprimir(salto,nX);	
											imprimir(salto,nX);	
										}
									}
								}
							}
							
							//KILOS POR VENDEDOR
							if (corte_final)
							{
								sprintf(sql,"SELECT FORMAT(SUM(Venta_Articulo.cantidad * Articulo_Lista.precio),2), FORMAT(SUM(Venta_Articulo.cantidad),3), CONCAT(Empleado.nombre,' ',Empleado.apellido) AS Vendedor, Basculas.bascula AS Bascula FROM Corte_Caja INNER  JOIN Venta ON (Corte_Caja.id_caja = Venta.id_caja AND Corte_Caja.id_usuario = Venta.id_usuario) INNER JOIN Venta_Articulo ON Venta_Articulo.id_venta = Venta.id_venta INNER JOIN Articulo_Lista ON Articulo_Lista.id_articulo = Venta_Articulo.id_articulo INNER JOIN Empleado ON Empleado.id_empleado = Venta.id_empleado INNER JOIN Basculas ON Venta_Articulo.id_bascula = Basculas.id_bascula WHERE Venta_Articulo.id_venta BETWEEN Corte_Caja.id_venta_inicio AND Corte_Caja.id_venta_fin AND Venta.cancelada = 'n' AND Corte_Caja.id_corte_caja = %s AND Articulo_Lista.id_lista = 1 GROUP BY Venta.id_empleado, Basculas.id_bascula ORDER BY Vendedor, Bascula", id_venta);
								
								printf ("####### SWL = %s",sql);
								if((err = mysql_query(&mysql, sql)) != 0)
								{
									printf("Error al consultar los tipos de documentos de Venta Total: %s\n", mysql_error(&mysql));
									imprimiendo = FALSE;
									return (1);
								}
								else
								{
									if((resultado = mysql_store_result(&mysql)))
									{
										sprintf(vendedor, "");
										while (row = mysql_fetch_row(resultado))
										{
											if(strcmp(vendedor,row[2]) != 0)
											{
												//sprintf(kilos_bascula, row[0]);
												//sprintf(cambio, "%.2f", cambio_num_total);
												imprimir(salto,nX);
												strcpy(c,row[2]);
												imprimir(negrita1,nX);
												imprimir (c,nX);
												imprimir(salto,nX);
												imprimir(defecto,nX);
												sprintf(vendedor,"%s",row[2]);
											}
											
											//SE IMPRIME LA VENTA DEL VENDEDOR
											strcpy(c,"   BASCULA: ");
											imprimir(c,nX);
											sprintf(c,"%s", row[3]);
											imprimir(c,nX);
											imprimir(salto,nX);
											
											//SE IMPRIME LA VENTA DEL VENDEDOR
											strcpy(c,"   IMPORTE: ");
											imprimir(c,nX);
											sprintf(c, "%s",row[0]);
											imprimir(c,nX);
											imprimir(salto,nX);
											
											//SE IMPRIMEN LOS KILOS VENDIDOS
											strcpy(c,"   KILOS: ");
											imprimir(c,nX);
											sprintf(c,"%s", row[1]);
											strcat (c," kg");
											imprimir(c,nX);
											
											//strncat(c,temp3,23-strlen(temp2));
											/*strcat(c, temp2);
											//strncat(c,temp3,11-strlen(kilos_bascula));
											strcat(c, kilos_bascula);
											imprimir(c,nX);*/
											imprimir(salto,nX);
											imprimir(salto,nX);
										}
									}
								}
							}
							
							//CLIENTES POR VENDEDOR
							if (corte_final)
							{
								sprintf(sql,"SELECT CONCAT(Empleado.nombre,' ',Empleado.apellido) AS Vendedor, COUNT( Venta.id_venta ) FROM Corte_Caja INNER JOIN Venta ON (Corte_Caja.id_caja = Venta.id_caja AND Corte_Caja.id_usuario = Venta.id_usuario) INNER JOIN Empleado ON Venta.id_empleado = Empleado.id_empleado WHERE Venta.id_venta BETWEEN Corte_Caja.id_venta_inicio AND Corte_Caja.id_venta_fin AND Venta.cancelada =  'n' AND Corte_Caja.id_corte_caja = %s GROUP BY Empleado.id_empleado ORDER BY Vendedor", id_venta);
								
								//printf ("####### SWL = %s",sql);
								if((err = mysql_query(&mysql, sql)) != 0)
								{
									printf("Error al consultar los clientes por Vendedor: %s\n", mysql_error(&mysql));
									imprimiendo = FALSE;
									return (1);
								}
								else
								{
									if((resultado = mysql_store_result(&mysql)))
									{
										sprintf(vendedor, "");
										sprintf(c,"CLIENTES POR VENDEDOR");
										imprimir(negrita1,nX);
										imprimir (c,nX);
										imprimir(salto,nX);
										imprimir(defecto,nX);
										while (row = mysql_fetch_row(resultado))
										{
											//sprintf(kilos_bascula, row[0]);
											//sprintf(cambio, "%.2f", cambio_num_total);
											sprintf(c,"%s: %s",row[0],row[1]);
											imprimir (c,nX);
											imprimir(salto,nX);
										}
									}
								}
							}
							
							imprimir(salto,nX);
							imprimir(salto,nX);
							imprimir(salto,nX);
							
							//SERVICIOS
							sprintf(sql,"SELECT ServicioDomicilio.id_servicio_domicilio, FORMAT( IF(ServicioDomicilio.cancelado = 'n' AND Venta.cancelada = 'n',ServicioDomicilio.monto,0),2), IF(ServicioDomicilio.cancelado = 'n' AND Venta.cancelada = 'n',ServicioDomicilio.monto,0) FROM Corte_Caja INNER JOIN Venta ON (Corte_Caja.id_caja = Venta.id_caja AND Corte_Caja.id_usuario = Venta.id_usuario) INNER JOIN ServicioDomicilio ON ServicioDomicilio.id_venta = Venta.id_venta INNER JOIN Empleado ON Venta.id_empleado = Empleado.id_empleado WHERE Venta.id_venta BETWEEN Corte_Caja.id_venta_inicio AND Corte_Caja.id_venta_fin AND Corte_Caja.id_corte_caja = %s GROUP BY ServicioDomicilio.id_servicio_domicilio ORDER BY ServicioDomicilio.id_servicio_domicilio", id_venta);
							
							//printf ("####### SWL = %s",sql);
							if((err = mysql_query(&mysql, sql)) != 0)
							{
								printf("Error al consultar los clientes por Vendedor: %s\n", mysql_error(&mysql));
								imprimiendo = FALSE;
								return (1);
							}
							else
							{
								if((resultado = mysql_store_result(&mysql)))
								{
									fac_cont_total_venta = 0;
									
									sprintf(vendedor, "");
									sprintf(c,"SERVICIOS");
									imprimir(negrita1,nX);
									imprimir (c,nX);
									imprimir(salto,nX);
									imprimir(salto,nX);
									
									sprintf (c,"Folio           Monto");
									imprimir(c,nX);
									imprimir(salto,nX);
									
									sprintf (c,"------------------------");
									imprimir(c,nX);
									imprimir(salto,nX);
									
									imprimir(defecto,nX);
									
									while (row = mysql_fetch_row(resultado))
									{
										//sprintf(kilos_bascula, row[0]);
										//sprintf(cambio, "%.2f", cambio_num_total);
										sprintf(c,"%5s          %8s",row[0],row[1]);
										imprimir (c,nX);
										imprimir(salto,nX);
										
										fac_cont_total_venta += atof(row[2]);
									}
									
									sprintf(c,"TOTAL: %.2f",fac_cont_total_venta);
									imprimir (c,nX);
									imprimir(salto,nX);
								}
							}
							
							imprimir(salto,nX);
							imprimir(salto,nX);
							imprimir(salto,nX);
							imprimir(salto,nX);
							imprimir(salto,nX);
							imprimir(salto,nX);
							imprimir(salto,nX);
							imprimir(salto,nX);
							imprimir(corta_papel,nX);
							fclose(fpt);
							//CORTE DE CAJA
							if((impresora_nombre[arr_impresora_corte_caja][0]) == 2)
							{
								strcpy(cad_temporal,"lp.cups ");
								strcat(cad_temporal,impresora_corte_caja);
								strcat(cad_temporal," ");
								strcat(cad_temporal,TicketImpresion);
							}
							else
							{
								strcpy(cad_temporal,"cat ");
								strcat(cad_temporal,TicketImpresion);
								strcat(cad_temporal,"> ");
								strcat(cad_temporal,impresora_corte_caja);
							}
							//system(cad_temporal);
							//ABRE CAJON DE DINERO
							abrir_cajon(impresora_contado);
							if (manda_imprimir (TicketImpresion,"corte_caja") != 0) 
							{
								Err_Info ("Error de impresion");
							}
							printf ("TEMP = %s\n",cad_temporal);
							}
						}else{
							imprimiendo = FALSE;
							return (1);
						}
					}else{
						imprimiendo = FALSE;
						return (1);
					}
				}else{
					imprimiendo = FALSE;
					return (1);
				 }
			mysql_close(&mysql); //Cierra conexion SQL
			}
		}

	}
	else if(strcmp(tipo, "credito_abono") == 0)
	{ //Credito Abono

	    printf("\n---------------------------------------------");
	    printf("\nSe imprime Credito Abono con la fecha mayor a %s\n",id_venta);



	    fpt2 = fopen(CobranzaConfig,"r");
	    if(fpt2 == NULL)
	    {
			printf("\nERROR no se puede abrir el archivo de configuracion");
			imprimiendo = FALSE;
			return (1);
	    }
	    else
	    {
		m=1;
		fac_nombre[0][0] = 0; //Inicializa el arreglo fac_nombre
		fac_nombre[0][1] = 0;
		fac_nombre[0][2] = 0;
		fac_nombre[0][3] = 0;
		while(fgets(c, 255, fpt2) != NULL)
		{
		    strcat(c," ");
		    strcpy(cadconf,"");
		    j=0;
		    k=0;
		    printf("%s",c);
		    for(i=0;i<strlen(c);i++)
		    {
			if(c[i] != '#')
			{
		    	    if(c[i] == ' ')
			    {
				j=0;
				if(k==0)
				{
								//Guarda la posicion de las variables.

				    if(strcmp(cadconf,"codigo_cliente") == 0) 
					arr_codigo_cliente = m;
				    else if(strcmp(cadconf,"nombre") == 0) 
					arr_nombre = m;
				    else if(strcmp(cadconf,"factura_ticket") == 0)
					arr_factura_ticket = m;
				    else if(strcmp(cadconf,"fecha_emision") == 0) 
					arr_fecha_emision = m;
				    else if(strcmp(cadconf,"fecha_vencimiento") == 0) 
					arr_fecha_vencimiento = m;
				    else if(strcmp(cadconf,"relacion") == 0)
					arr_relacion = m;
				    else if(strcmp(cadconf,"cargo") == 0) 
					arr_cargo = m;
				    else if(strcmp(cadconf,"abono") == 0) 
					arr_abono = m;
				    else if(strcmp(cadconf,"saldo") == 0) 
					arr_saldo = m;

				}
				else if(k==1)
				{ //X1
				    fac_nombre[m][0] = atoi(cadconf)-1;
				}
				else if(k==2)
				{ //X2
				    fac_nombre[m][1] = atoi(cadconf)-1;
				}
				else if(k==3)
				{ //ALINEACION
				    fac_nombre[m][2] = atoi(cadconf)-1;
				}
				strcpy(cadconf,"");
				k++;
			    }
			    else
			    {
				cadconf[j] = c[i];
				cadconf[j+1] = '\0';
				j++;
			    }
			}
			else
			{
			    break;
			}
			fac_nombre[0][0] = 0;
			m++;
		    }
			fclose(fpt2);
		}


		//Buscar el X mayor
		num_mayor_X = fac_nombre[arr_nombre][1];
		if(fac_nombre[arr_codigo_cliente][1] > num_mayor_X) num_mayor_X = fac_nombre[arr_codigo_cliente][1];
		if(fac_nombre[arr_factura_ticket][1] > num_mayor_X) num_mayor_X = fac_nombre[arr_factura_ticket][1];
		if(fac_nombre[arr_fecha_emision][1] > num_mayor_X) num_mayor_X = fac_nombre[arr_fecha_emision][1];
		if(fac_nombre[arr_fecha_vencimiento][1] > num_mayor_X) num_mayor_X = fac_nombre[arr_fecha_vencimiento][1];
		if(fac_nombre[arr_relacion][1] > num_mayor_X) num_mayor_X = fac_nombre[arr_relacion][1];
		if(fac_nombre[arr_cargo][1] > num_mayor_X) num_mayor_X = fac_nombre[arr_cargo][1];
		if(fac_nombre[arr_abono][1] > num_mayor_X) num_mayor_X = fac_nombre[arr_abono][1];
		if(fac_nombre[arr_saldo][1] > num_mayor_X) num_mayor_X = fac_nombre[arr_saldo][1];
		//Fin de buscar el X mayor
		nX = num_mayor_X + 2; //TAMAï¿œ MAXIMO DE X

		inicializarARRAY(nX);



		if(conecta_bd() == -1)
    		{
      		    printf("No se pudo conectar a la base de datos. Error: %s\n", mysql_error(&mysql));
    		}
		else
		{
		    printf("Si se conecta a la base de datos...!\n");
		    //Informacion incicial
		    //SELECT Cliente.id_cliente, Cliente.nombre, Venta.id_venta, Venta.fecha, Credito.vencimiento, Venta.monto, SUM( Abono.abono )  AS abono, ( Venta.monto - SUM( Abono.abono )  ) AS Saldo FROM Credito INNER  JOIN Venta ON Credito.id_venta = Venta.id_venta INNER  JOIN Cliente ON Venta.id_cliente = Cliente.id_cliente INNER  JOIN Abono ON Abono.id_credito = Credito.id_credito WHERE Credito.vencimiento <=  '2003-04-13' GROUP  BY Credito.id_credito HAVING ( Venta.monto - SUM( Abono.abono )  ) > 0;

		    //Informacion sobre la relacion de las facturas.
		    //SELECT  DISTINCT ( Venta_Factura_Relacion.num_factura ) FROM Venta_Factura_Relacion INNER  JOIN Venta ON Venta.id_venta = Venta_Factura_Relacion.id_venta WHERE Venta.id_venta = 2
		    //Cambia el formato de la fecha
		    sprintf(cad_temporal_fechas, "%c",id_venta[0]); strcpy(cad_temporal,cad_temporal_fechas);
		    sprintf(cad_temporal_fechas, "%c",id_venta[1]); strcat(cad_temporal,cad_temporal_fechas);
		    sprintf(cad_temporal_fechas, "%c",id_venta[2]); strcat(cad_temporal,cad_temporal_fechas);
		    sprintf(cad_temporal_fechas, "%c",id_venta[3]); strcat(cad_temporal,cad_temporal_fechas);
		    strcat(cad_temporal,"-");
		    sprintf(cad_temporal_fechas, "%c",id_venta[4]); strcat(cad_temporal,cad_temporal_fechas);
		    sprintf(cad_temporal_fechas, "%c",id_venta[5]); strcat(cad_temporal,cad_temporal_fechas);
		    strcat(cad_temporal,"-");
		    sprintf(cad_temporal_fechas, "%c",id_venta[6]); strcat(cad_temporal,cad_temporal_fechas);
		    sprintf(cad_temporal_fechas, "%c",id_venta[7]); strcat(cad_temporal,cad_temporal_fechas);


    		    strcpy(sql,"SELECT Cliente.id_cliente, Cliente.nombre, Venta.id_venta, Venta.fecha, Credito.vencimiento, /*FORMAT(*/Venta.monto/*,2)*/, /*FORMAT(*/SUM( Abono.abono )/*,2)*/  AS abono, /*FORMAT(*/( Venta.monto - SUM( Abono.abono )  )/*,2)*/ AS Saldo FROM Credito INNER  JOIN Venta ON Credito.id_venta = Venta.id_venta INNER  JOIN Cliente ON Venta.id_cliente = Cliente.id_cliente LEFT JOIN Abono ON Abono.id_credito = Credito.id_credito WHERE Credito.vencimiento <=  '");
		    strcat(sql, cad_temporal); //Fecha con formato AAAA-MM-DD
		    strcat(sql,"' GROUP  BY Credito.id_credito HAVING ( Venta.monto - SUM( Abono.abono )  ) > 0");
		    printf("\n%s",sql);
		    printf("\n");
		    if((err = mysql_query(&mysql, sql)) != 0)
		    {
				printf("Error al consultar los tipos de documentos: %s\n", mysql_error(&mysql));
				imprimiendo = FALSE;
				return (1);
		    }
		    else
		    {
			if((resultado = mysql_store_result(&mysql)))
			{
			    if (mysql_num_rows(resultado) > 0)
			    {
				fpt = fopen(TicketImpresion,"w");
				if(fpt == NULL)
				{
				    printf("\nERROR no se puede abrir el archivo a imprimir");
					imprimiendo = FALSE;
				    return (1);
				}
				else
				{
				    //IMPRIME ENCABEZADO
				    inicializarARRAY(nX);
				    strcpy(c,"************************************************************\n");
				    imprimir(c,nX);
				    inicializarARRAY(nX);
				    inicializarARRAY(nX);
				    strcpy(c,"************************* COBRANZA *************************\n");
				    imprimir(c,nX);
      				    inicializarARRAY(nX);
				    inicializarARRAY(nX);
				    strcpy(c,"************************************************************\n\n\n\n\n");
				    imprimir(c,nX);
				    inicializarARRAY(nX);
				    inicializarARRAY(nX);
				    strcpy(c,"Fecha: ");
				    sprintf(fechaTMP, "%c%c-%c%c-%c%c%c%c",cad_temporal[8],cad_temporal[9],cad_temporal[5],cad_temporal[6],cad_temporal[0],cad_temporal[1],cad_temporal[2],cad_temporal[3]);
				    strcat(c,fechaTMP);
				    imprimir(c,nX);
				    inicializarARRAY(nX); 	imprimir(c,nX);
				    inicializarARRAY(nX); 	imprimir(c,nX);
				    inicializarARRAY(nX); 	imprimir(c,nX);
	    
				    llenarARRAY(arr_codigo_cliente, "NUMERO");
				    llenarARRAY(arr_nombre, "NOMBRE");
				    llenarARRAY(arr_factura_ticket, "NUMERO");
				    llenarARRAY(arr_fecha_emision, " FECHA");
				    llenarARRAY(arr_fecha_vencimiento, "  FECHA");
				    llenarARRAY(arr_relacion, "RELACION");
				    llenarARRAY(arr_cargo, "CARGO ");
				    llenarARRAY(arr_abono, "ABONO");
				    llenarARRAY(arr_saldo, "SALDO ");
      				    imprimir(c,nX);
				    inicializarARRAY(nX); //Inicializa Array
				    llenarARRAY(arr_codigo_cliente, "CLIENTE");
				    llenarARRAY(arr_factura_ticket, "VENTA");
				    llenarARRAY(arr_fecha_emision, "EMISION");
				    llenarARRAY(arr_fecha_vencimiento, "VENCIMIENTO");
				    llenarARRAY(arr_relacion, "O FACTURA");
      				    imprimir(c,nX);
				    for(i=0;i<nX-1;i++)
				    {
					c[i] = '-';
					c[i+1] = '\0';
				    }
				    imprimir(c,nX);
				    corte_dinero_caja_num = 0;
				    while((row = mysql_fetch_row(resultado)))
				    {
					inicializarARRAY(nX); //Inicializa Array
					llenarARRAY(arr_codigo_cliente, row[0]); //Codigo Cliente
					llenarARRAY(arr_nombre, row[1]); //Nombre
					llenarARRAY(arr_factura_ticket, row[2]); //Factura o ticket
					sprintf(fechaTMP, "%c%c-%c%c-%c%c%c%c",row[3][8],row[3][9],row[3][5],row[3][6],row[3][0],row[3][1],row[3][2],row[3][3]);
					llenarARRAY(arr_fecha_emision, fechaTMP); //Fecha Emision
					sprintf(fechaTMP, "%c%c-%c%c-%c%c%c%c",row[4][8],row[4][9],row[4][5],row[4][6],row[4][0],row[4][1],row[4][2],row[4][3]);
					llenarARRAY(arr_fecha_vencimiento, fechaTMP); //Fecha Vencimiento
					//llenarARRAY(arr_relacion,  row[5]);
					num_numero = atof(row[5]);
					sprintf(cad_temporal,"%.2f", num_numero);
					llenarARRAY(arr_cargo,  cad_temporal); //Cargo
					num_numero = atof(row[6]);
					sprintf(cad_temporal,"%.2f", num_numero);
					llenarARRAY(arr_abono,  cad_temporal); //Abono
					num_numero = atof(row[7]);
					sprintf(cad_temporal,"%.2f", num_numero);
					llenarARRAY(arr_saldo,  cad_temporal); //Saldo

					strcpy(sql,"SELECT Venta_Factura_Relacion.num_factura FROM Venta INNER  JOIN Venta_Factura_Relacion ON Venta.id_venta = Venta_Factura_Relacion.id_venta INNER  JOIN Venta_Factura ON Venta_Factura.id_factura = Venta_Factura_Relacion.id_factura WHERE Venta.id_venta = ");
					strcat(sql, row[2]); //Numero de Venta
//					printf("\n%s",sql);
					if((err = mysql_query(&mysql, sql)) != 0)
					{
					    printf("Error al consultar las relaciones de la factura: %s\n", mysql_error(&mysql));
						imprimiendo = FALSE;
					    return (1);
					}
					else
					{
					    if((resultado2 = mysql_store_result(&mysql)))
					    {
						if (mysql_num_rows(resultado2) > 0)
						{
						    while((row2 = mysql_fetch_row(resultado2)))
						    {
							llenarARRAY(arr_relacion,  row2[0]);
							imprimir(c,nX);
							inicializarARRAY(nX);
						    }
						}
						else
						{
						    llenarARRAY(arr_relacion,  "0");
						    imprimir(c,nX);
						}
					    }
					}

				    }//Fin While
				    fputs("\n\n\n\n\n\n", fpt);
				    fclose(fpt);//Cierre del archivo
							//Credito_Abono - Cobranza
							if((impresora_nombre[arr_impresora_credito_abono][0]) == 2)
							{
								strcpy(cad_temporal,"lp.cups ");
								strcat(cad_temporal,impresora_credito_abono);
								strcat(cad_temporal," ");
								strcat(cad_temporal,TicketImpresion);
							}
							else
							{
								strcpy(cad_temporal,"cat ");
								strcat(cad_temporal,TicketImpresion);
								strcat(cad_temporal,"> ");
								strcat(cad_temporal,impresora_credito_abono);
							}
							//system(cad_temporal);
							printf ("TEMP = %s\n",cad_temporal);
							if (manda_imprimir (TicketImpresion,"credito_abono") != 0 )
							{
								Err_Info ("Error de impresion");
							}
				}//Fin del IF DEL ARCHIVO NULL
			    }
			}
		    }

			mysql_close(&mysql); //Cierra conexion SQL
		}
	    }

	}else if(strcmp(tipo, "corte_all_caja") == 0){ //Corte Caja detallado por caja
	printf("\n\n");
		if(conecta_bd() == -1){
			g_print("\nNo me puedo conectar a la base de datos =(\n");
			imprimiendo = FALSE;
			return (1);
		}else{
			//Informacion del corte de caja

			sprintf(cad_temporal_fechas, "%c",id_venta[0]); strcpy(cad_temporal,cad_temporal_fechas);
			sprintf(cad_temporal_fechas, "%c",id_venta[1]); strcat(cad_temporal,cad_temporal_fechas);
			sprintf(cad_temporal_fechas, "%c",id_venta[2]); strcat(cad_temporal,cad_temporal_fechas);
			sprintf(cad_temporal_fechas, "%c",id_venta[3]); strcat(cad_temporal,cad_temporal_fechas);
			strcat(cad_temporal,"-");
			sprintf(cad_temporal_fechas, "%c",id_venta[4]); strcat(cad_temporal,cad_temporal_fechas);
			sprintf(cad_temporal_fechas, "%c",id_venta[5]); strcat(cad_temporal,cad_temporal_fechas);
			strcat(cad_temporal,"-");
			sprintf(cad_temporal_fechas, "%c",id_venta[6]); strcat(cad_temporal,cad_temporal_fechas);
			sprintf(cad_temporal_fechas, "%c",id_venta[7]); strcat(cad_temporal,cad_temporal_fechas);


			strcpy(sql,"SELECT Corte_Caja.id_caja, Corte_Caja.fecha, Corte_Caja.hora, CONCAT_WS(  ' ', Usuario.nombre, Usuario.apellido )  AS usuario FROM Corte_Caja INNER  JOIN Usuario ON Usuario.id_usuario = Corte_Caja.id_usuario WHERE Corte_Caja.fecha = '");
			strcat(sql, cad_temporal);
			strcat(sql, "'");
			printf("%s\n",sql);
			if((err = mysql_query(&mysql, sql)) != 0){
				printf("Error al consultar los tipos de documentos: %s\n", mysql_error(&mysql));
				imprimiendo = FALSE;
				return (1);
			}else{
				if((resultado = mysql_store_result(&mysql))){
						if (mysql_num_rows(resultado) > 0)
						{
							fpt = fopen(TicketImpresion,"w");
							if(fpt == NULL){
								printf("\nERROR no se puede abrir el archivo a imprimir");
								imprimiendo = FALSE;
								return (1);
							}else{
								imprimir(resetea,nX);
								imprimir(espaciado,nX);
								while((row = mysql_fetch_row(resultado)))
								{
									for(i=0; i < 5;i++) //Guarda todo el arreglo en listapos
										listatipos[i] =  row[i];

									corte_dinero_caja_num = 0;
									corte_contado_num = 0;
									corte_retiro_num =0;

									sprintf(cad_temporal, "%s", listatipos[0]);
									strcpy(temp2,"");
									strcpy(c,"");
									strcpy(temp2,"Caja: ");
									strncat(c,temp3,9-strlen(temp2));
									strcat(c, temp2);
									strncat(c,temp3,2-strlen(cad_temporal));
									strcat(c, cad_temporal);
									sprintf(cad_temporal, " %s\n", listatipos[3]);
									strcat(c, cad_temporal);
									imprimir(c,nX);
									sprintf(c, "       Fecha: %c%c/%c%c/%c%c%c%c Hora: %s\n",listatipos[1][8],listatipos[1][9],listatipos[1][5],listatipos[1][6],listatipos[1][0],listatipos[1][1],listatipos[1][2],listatipos[1][3],listatipos[2]); imprimir(c,nX);
								}


								//Credito y contado TOTAL e IVA
								sprintf(sql,"SELECT Venta.tipo, SUM( Venta.monto ) AS monto_IVA, (SUM( Venta.monto )  / 1.15) AS monto FROM Corte_Caja INNER JOIN Venta ON (Corte_Caja.id_caja = Venta.id_caja AND Corte_Caja.id_usuario = Venta.id_usuario) WHERE Venta.id_venta BETWEEN Corte_Caja.id_venta_inicio AND Corte_Caja.id_venta_fin AND Venta.cancelada =  'n' AND Corte_Caja.fecha = '%s' GROUP  BY Venta.tipo",id_venta);
								//printf("\n%s",sql);
								if((err = mysql_query(&mysql, sql)) != 0){
									printf("Error al consultar los tipos de documentos de Venta Total: %s\n", mysql_error(&mysql));
									imprimiendo = FALSE;
									return (1);
								}
								else{
									if((resultado2 = mysql_store_result(&mysql))){
										fac_subtotal_num = 0;
										fac_iva_num = 0;
										fac_total_num = 0;
										bandera = 0;
										cambio_num_total = 0;
										fac_cred_total_venta = 0;
										fac_cont_total_venta = 0;
										if (mysql_num_rows(resultado2) > 0){
											imprimir(negrita1,nX);
											sprintf(c, "VENTA TOTAL:\n"); imprimir(c,nX);
											imprimir(defecto,nX);
											while((row2 = mysql_fetch_row(resultado2))){
												for(i=0; i < 3;i++) //Guarda todo el arreglo en listapos
													listatipos[i] =  row2[i];
												bandera ++;
												if(strcmp(listatipos[0], "contado") == 0){ //Es credito o contado
													//sprintf(c, "CONTADO:\n"); imprimir(c,nX);
													cambio_num = atof(listatipos[1]);
													fac_cont_total_venta = cambio_num;
													cambio_num_total = cambio_num_total + cambio_num;
													sprintf(cambio, "%.2f", cambio_num);
													strcpy(temp2,"");
													strcpy(c,"");
													strcpy(temp2,"Contado: $ ");
													strncat(c,temp3,23-strlen(temp2));
													strcat(c, temp2);
													strncat(c,temp3,11-strlen(cambio));
													strcat(c, cambio);
													strcat(c, "\n");
													imprimir(c,nX);

												}else if(strcmp(listatipos[0], "credito") == 0){
													//sprintf(c, "CREDITO:\n"); imprimir(c,nX);
	/*												cambio_num = cambio_num + atof(listatipos[1]);*/
													cambio_num = atof(listatipos[1]);
													fac_cred_total_venta = cambio_num;
													cambio_num_total = cambio_num_total + cambio_num;
													sprintf(cambio, "%.2f", cambio_num);
													strcpy(temp2,"");
													strcpy(c,"");
													strcpy(temp2,"Credito: $ ");
													strncat(c,temp3,23-strlen(temp2));
													strcat(c, temp2);
													strncat(c,temp3,11-strlen(cambio));
													strcat(c, cambio);
													strcat(c, "\n");
													imprimir(c,nX);
												}
												//Guardo la cantidad de contado para despues sacar el dinero en caja
												if(strcmp(listatipos[0], "contado") == 0){
													//corte_contado_num = cambio_num;
													sprintf(cambio, "%.2f", atof(listatipos[1]));
													corte_contado_num = atof(cambio);
												}
											} //FIN WHILE
											if(bandera>1){ //Verifica si existe Credito y contado para mostrar la suma de los 2
												//Calcula el Total
												sprintf(cambio, "%.2f", cambio_num_total);
												strcpy(temp2,"");
												strcpy(c,"");
												strcpy(temp2,"TOTAL: $ ");
												strncat(c,temp3,23-strlen(temp2));
												strcat(c, temp2);
												strncat(c,temp3,11-strlen(cambio));
												strcat(c, cambio);
												strcat(c, "\n");
												imprimir(c,nX);
											} //FIN DE LA BANDERA
										}//Fin del > 0
									}else{
										imprimiendo = FALSE;
										return (1);
									}
								}


								//FACTURAS ya sea de credito o contado
								sprintf(sql,"SELECT Venta.tipo, Venta.monto AS Monto_IVA, (Venta.monto  / 1.15) AS Monto FROM Corte_Caja INNER  JOIN Venta ON (Corte_Caja.id_caja = Venta.id_caja AND Corte_Caja.id_usuario = Venta.id_usuario) WHERE Venta.id_venta BETWEEN Corte_Caja.id_venta_inicio AND Corte_Caja.id_venta_fin AND Venta.cancelada =  'n' AND Venta.num_factura<>0 AND Corte_Caja.fecha = '%s' GROUP  BY Venta.id_venta",id_venta);
								//printf("\n%s",sql);
								if((err = mysql_query(&mysql, sql)) != 0){
									printf("Error al consultar los tipos de documentos: %s\n", mysql_error(&mysql));
									imprimiendo = FALSE;
									return (1);
								}
								else{
									if((resultado = mysql_store_result(&mysql))){
										fac_subtotal_num = 0;
										fac_iva_num = 0;
										fac_total_num = 0;
										bandera = 0;
										bandera2 = 0;
										fac_cont_subtotal = 0;
										fac_cont_iva = 0;

										fac_cont_total = 0;
										fac_cred_subtotal = 0;
										fac_credcont_total = 0;
										fac_credcont_iva = 0;
										fac_credcont_subtotal = 0;
										fac_cred_iva = 0;
										fac_cred_total = 0;
										cambio_num_total = 0;
										cambio_num = 0;



										if (mysql_num_rows(resultado) > 0){
											//sprintf(c, "_________________\n"); imprimir(c,nX);
											//sprintf(c, "FACTURAS:\n"); imprimir(c,nX);
											imprimir(negrita1,nX);
											sprintf(c, "\nVENTA FACTURADA:\n"); imprimir(c,nX);
											imprimir(defecto,nX);
											while((row = mysql_fetch_row(resultado))){
												for(i=0; i < 3;i++) //Guarda todo el arreglo en listapos
													listatipos[i] =  row[i];

												if(strcmp(listatipos[0], "contado") == 0 ){ //Es credito o contado
													fac_cont_total = fac_cont_total + atof(listatipos[1]);



												}else if(strcmp(listatipos[0], "credito") == 0){
													fac_cred_total = fac_cred_total + atof(listatipos[1]);

												}
											}

											if(fac_cont_total > 0){
												cambio_num_total = cambio_num_total + fac_cont_total;
												sprintf(cambio, "%.2f", fac_cont_total);
												strcpy(temp2,"");
												strcpy(c,"");
												strcpy(temp2,"Contado: $ ");
												strncat(c,temp3,23-strlen(temp2));
												strcat(c, temp2);
												strncat(c,temp3,11-strlen(cambio));
												strcat(c, cambio);
												strcat(c, "\n");
												imprimir(c,nX);
												bandera++;
											}


											if(fac_cred_total > 0){
												cambio_num_total = cambio_num_total + fac_cred_total;
												sprintf(cambio, "%.2f", fac_cred_total);
												strcpy(temp2,"");
												strcpy(c,"");
												strcpy(temp2,"Credito: $ ");
												strncat(c,temp3,23-strlen(temp2));
												strcat(c, temp2);
												strncat(c,temp3,11-strlen(cambio));
												strcat(c, cambio);
												strcat(c, "\n");
												imprimir(c,nX);
												bandera ++;
											}


											if(bandera == 2){ //Verifica si existe Credito y contado para mostrar la suma de los 2
												sprintf(cambio, "%.2f", cambio_num_total);
												strcpy(temp2,"");
												strcpy(c,"");
												strcpy(temp2,"TOTAL: $ ");
												strncat(c,temp3,23-strlen(temp2));
												strcat(c, temp2);
												strncat(c,temp3,11-strlen(cambio));
												strcat(c, cambio);
												strcat(c, "\n");
												imprimir(c,nX);
											}
										}
									}else{
										imprimiendo = FALSE;
										return (1);
									}
								}
								imprimir(negrita1,nX);
								sprintf(c, "\nVENTA TICKETS:\n"); imprimir(c,nX);
								imprimir(defecto,nX);
								sprintf(cambio, "%.2f", fac_cont_total_venta - fac_cont_total);
								strcpy(temp2,"");
								strcpy(c,"");
								strcpy(temp2,"Contado: $ ");
								strncat(c,temp3,23-strlen(temp2));
								strcat(c, temp2);
								strncat(c,temp3,11-strlen(cambio));
								strcat(c, cambio);
								strcat(c, "\n");
								imprimir(c,nX);

								sprintf(cambio, "%.2f", fac_cred_total_venta - fac_cred_total);
								strcpy(temp2,"");
								strcpy(c,"");
								strcpy(temp2,"Credito: $ ");
								strncat(c,temp3,23-strlen(temp2));
								strcat(c, temp2);
								strncat(c,temp3,11-strlen(cambio));
								strcat(c, cambio);
								strcat(c, "\n");
								imprimir(c,nX);


								sprintf(cambio, "%.2f", (fac_cred_total_venta - fac_cred_total) + (fac_cont_total_venta - fac_cont_total));
								strcpy(temp2,"");
								strcpy(c,"");
								strcpy(temp2,"TOTAL: $ ");
								strncat(c,temp3,23-strlen(temp2));
								strcat(c, temp2);
								strncat(c,temp3,11-strlen(cambio));
								strcat(c, cambio);
								strcat(c, "\n\n");
								imprimir(c,nX);






								//Cancelaciones
								strcpy(sql,"SELECT Venta_Cancelada.id_venta, Venta_Cancelada.observaciones, Cliente.nombre FROM Venta_Cancelada INNER  JOIN Venta ON Venta_Cancelada.id_venta = Venta.id_venta INNER  JOIN Cliente ON Cliente.id_cliente = Venta.id_cliente INNER  JOIN Corte_Caja ON Venta.id_venta BETWEEN Corte_Caja.id_venta_inicio AND Corte_Caja.id_venta_fin AND (Corte_Caja.id_caja = Venta.id_caja AND Corte_Caja.id_usuario = Venta.id_usuario) WHERE Venta.cancelada =  's' AND Corte_Caja.fecha = '");
								strcat(sql, id_venta);
								strcat(sql,"'");
								//printf("\n%s",sql);
								if((err = mysql_query(&mysql, sql)) != 0){
									printf("Error al consultar los tipos de documentos: %s\n", mysql_error(&mysql));
									return (1);
								}else{
									if((resultado = mysql_store_result(&mysql))){

										if (mysql_num_rows(resultado) > 0)
										{
											sprintf(c, "_________________\n"); imprimir(c,nX);
											imprimir(negrita1,nX);
											sprintf(c, "CANCELACIONES:\n"); imprimir(c,nX);
											imprimir(defecto,nX);
											while((row = mysql_fetch_row(resultado))){
												for(i=0; i < 3;i++) //Guarda todo el arreglo en listapos
													listatipos[i] =  row[i];

												//Cancelaciones FOLIO
												sprintf(c, "    Folio: %s\n",listatipos[0]); imprimir(c,nX);
												//Cancelaciones Cliente
												sprintf(c, "    Cliente:\n"); imprimir(c,nX);
												sprintf(c,"%s\n",listatipos[2]); imprimir(c,nX);
												sprintf(c, "    Observaciones:\n"); imprimir(c,nX);
												sprintf(c,"%s",listatipos[1]); imprimir(c,nX);
												sprintf(c,"\n\n"); imprimir(c,nX);
											}
										}else{
											//return (1);
										}
									}else{
										imprimiendo = FALSE;
										return (1);
									}
								}

								//Retiro Efectivo
								strcpy(sql,"SELECT Retiro.fecha, Retiro.hora, /*FORMAT(*/Retiro.monto/*,2)*/, CONCAT_WS(  '  ', Cajero.nombre, Cajero.apellido )  AS cajero, CONCAT_WS(  '  ', UsuarioRetira.nombre, UsuarioRetira.apellido )  AS UsuarioRetira FROM Usuario Cajero, Usuario UsuarioRetira, Corte_Caja INNER  JOIN Retiro ON Retiro.id_retiro BETWEEN Corte_Caja.id_retiro_inicio AND Corte_Caja.id_retiro_fin AND (Corte_Caja.id_caja = Retiro.id_caja AND Corte_Caja.id_usuario = Retiro.id_usuario_cajero) WHERE Cajero.id_usuario = Retiro.id_usuario_cajero AND UsuarioRetira.id_usuario = Retiro.id_usuario_retira AND Corte_Caja.fecha = '");
								strcat(sql, id_venta);
								strcat(sql,"'");
								//printf("\n%s",sql);
								if((err = mysql_query(&mysql, sql)) != 0){
									printf("Error al consultar los tipos de documentos: %s\n", mysql_error(&mysql));
									imprimiendo = FALSE;
									return (1);
								}else{
									if((resultado = mysql_store_result(&mysql))){

										if (mysql_num_rows(resultado) > 0){
											sprintf(c, "_________________\n"); imprimir(c,nX);
											imprimir(negrita1,nX);
											sprintf(c, "RETIRO DE EFECTIVO:\n"); imprimir(c,nX);
											imprimir(defecto,nX);
											while((row = mysql_fetch_row(resultado))){
												for(i=0; i < 5;i++) //Guarda todo el arreglo en listapos
													listatipos[i] =  row[i];
												//Fecha Y Hora
												sprintf(c, "Fecha: %c%c-%c%c-%c%c%c%c Hora: %s\n",listatipos[0][8],listatipos[0][9],listatipos[0][5],listatipos[0][6],listatipos[0][0],listatipos[0][1],listatipos[0][2],listatipos[0][3],listatipos[1]); imprimir(c,nX);
												//Cajero
												sprintf(c, "Cajero(a): %s\n",listatipos[3]); imprimir(c,nX);
												//Cancelaciones Cliente
												sprintf(c, "Retirï¿œ: %s\n",listatipos[4]); imprimir(c,nX);
												//Monto dle retiro
												fac_subtotal_num = atof(listatipos[2]);
												sprintf(cambio, "%.2f", fac_subtotal_num);
												strcpy(temp2,"");
												strcpy(c,"");
												strcpy(temp2,"Cantidad: $ ");
												strncat(c,temp3,12-strlen(temp2));
												strcat(c, temp2);
												strncat(c,temp3,11-strlen(cambio));
												strcat(c, cambio);
												strcat(c, "\n\n");
												imprimir(c,nX);
												corte_retiro_num = corte_retiro_num + fac_subtotal_num;
											}
											strcpy(temp2,"");
											strcpy(c,""); strcpy(cambio,"---------");
											strcpy(temp2," ");
											strncat(c,temp3,23-strlen(temp2));
											strcat(c, temp2);
											strncat(c,temp3,11-strlen(cambio));
											strcat(c, cambio);
											strcat(c, "\n");
											imprimir(c,nX);
											//Calcula el Total
											sprintf(cambio, "%.2f", corte_retiro_num);
											strcpy(temp2,"");
											strcpy(c,"");
											strcpy(temp2,"TOTAL: $ ");
											strncat(c,temp3,23-strlen(temp2));
											strcat(c, temp2);
											strncat(c,temp3,11-strlen(cambio));
											strcat(c, cambio);
											strcat(c, "\n\n");
											imprimir(c,nX);

										}else{
											//return (1);
										}
									}else{
										imprimiendo = FALSE;
										return (1);
									}
								}
							sprintf(c, "_________________\n"); imprimir(c,nX);
							//Monto dle retiro
							corte_dinero_caja_num = corte_contado_num - corte_retiro_num;
							sprintf(cambio, "%.2f", corte_dinero_caja_num);
							strcpy(temp2,"");
							strcpy(c,"");
							strcpy(temp2,"DINERO EN CAJA: $ ");
							strncat(c,temp3,18-strlen(temp2));
							strcat(c, temp2);
							strncat(c,temp3,16-strlen(cambio));
							strcat(c, cambio);
							strcat(c, "\n");
							imprimir(negrita1,nX);
							imprimir(c,nX);
							imprimir(defecto,nX);


							//Numero de Clientes

//							strcpy(sql,"SELECT Venta.id_cliente FROM Corte_Caja, Venta WHERE Corte_Caja.id_caja = Venta.id_caja AND Venta.id_venta BETWEEN Corte_Caja.id_venta_inicio AND Corte_Caja.id_venta_fin AND Venta.cancelada =  'n' AND Corte_Caja.fecha =  '");
							strcpy(sql,"SELECT Venta.id_venta FROM Venta WHERE Venta.cancelada = 'n' AND Venta.fecha = '");
							strcat(sql, id_venta);
							strcat(sql,"'");
							printf("%s\n",sql);
							if((err = mysql_query(&mysql, sql)) != 0){
								printf("Error al consultar el total de clientes: %s\n", mysql_error(&mysql));
								imprimiendo = FALSE;
								return (1);
							}else{
								if((resultado = mysql_store_result(&mysql))){
									if (mysql_num_rows(resultado) > 0){

	 						//			sprintf(c, "_____________________________________\n"); imprimir(c,nX);
										num_numero_int =mysql_num_rows(resultado);
										sprintf(cambio, "%d",num_numero_int);
										strcpy(temp2,"");
										strcpy(c,"");
										strcpy(temp2,"CLIENTES ATENDIDOS: ");
										strncat(c,temp3,20-strlen(temp2));
										strcat(c, temp2);
										strncat(c,temp3,14-strlen(cambio));
										strcat(c, cambio);
										strcat(c, "\n\n");
										imprimir(c,nX);

										//sprintf(c, "CLIENTES ATENDIDOS: %s\n", row[0]); imprimir(c,nX);

									}else{
										//return (1);
									}
								}else{
									imprimiendo = FALSE;
									return (1);
								}
							}



							//Cantidad vendida de kilos
							// Cuando se cambiaron los tipos a DECIMAL en la base de datos, ésta consulta se hizo lentísima. Por eso se sustituyó por una nueva.
							//sprintf(sql,"SELECT Venta_Articulo.id_articulo, SUM(Venta_Articulo.cantidad), (SUM( Venta_Articulo.monto )  / 1.15) AS monto, FORMAT(SUM( Venta_Articulo.cantidad ),2)  AS cantidad, Articulo.nombre, Linea.nombre AS linea, Linea.id_linea FROM Corte_Caja INNER JOIN Venta ON (Corte_Caja.id_caja = Venta.id_caja AND Corte_Caja.id_usuario = Venta.id_usuario) INNER JOIN Venta_Articulo ON Venta.id_venta=Venta_Articulo.id_venta INNER JOIN Articulo ON Venta_Articulo.id_articulo=Articulo.id_articulo INNER JOIN Linea ON Articulo.id_linea = Linea.id_linea WHERE Venta_Articulo.id_venta BETWEEN Corte_Caja.id_venta_inicio AND Corte_Caja.id_venta_fin AND Venta.cancelada =  'n' AND Corte_Caja.fecha = '%s' GROUP BY Venta_Articulo.id_articulo ORDER BY Linea.nombre, cantidad",id_venta);
							sprintf(sql,"SELECT Venta_Articulo.id_articulo, SUM(Venta_Articulo.cantidad) AS cantidad, (SUM( Venta_Articulo.monto )  / 1.15) AS monto, FORMAT(SUM( Venta_Articulo.cantidad ),2)  AS cantidad, Articulo.nombre, IFNULL(Linea.nombre, 'SIN ASIGNAR') AS linea, IFNULL(Linea.id_linea,'0') as id_linea FROM Venta INNER JOIN Venta_Articulo ON Venta.id_venta=Venta_Articulo.id_venta INNER JOIN Articulo ON Venta_Articulo.id_articulo=Articulo.id_articulo LEFT JOIN Linea ON Articulo.id_linea = Linea.id_linea WHERE Venta.cancelada = 'n' AND Venta.fecha = '%s' GROUP BY Articulo.id_articulo ORDER BY Linea.nombre, cantidad",id_venta);
							printf("%s",sql);
							if((err = mysql_query(&mysql, sql)) != 0){
								printf("Error al consultar los tipos de documentos: %s\n", mysql_error(&mysql));
								imprimiendo = FALSE;
								return (1);
							}else{
								if((resultado = mysql_store_result(&mysql))){
									if (mysql_num_rows(resultado) > 0){
	 									sprintf(c, "_________________\n"); imprimir(c,nX);
										imprimir(negrita1,nX);
										sprintf(c, "ARTICULOS VENDIDOS:\n"); imprimir(c,nX);
										imprimir(defecto,nX);
										sprintf(c, "Articulo                     Cantidad\n");
										imprimir(c,nX);
										sprintf(c, "----------                ----------\n");
										imprimir(c,nX);
										corte_dinero_caja_num = 0;
          									bandera=0;
										num_numero=0;
										strcpy(cad_temporal,"");
										while((row = mysql_fetch_row(resultado)))
										{
											for(i=0; i < 6;i++) //Guarda todo el arreglo en listapos
												listatipos[i] =  row[i];


          										if(strcmp(cad_temporal,row[6]) != 0)
											{ //Primera vez que se mete a inicializar la variable
            											sprintf(cad_temporal, "%s",row[6]);
												bandera=1;
											}

											if(bandera==1)
											{
												imprimir(alinea_d,nX);
												sprintf(c,"%.2f",corte_dinero_caja_num);
												sprintf(sql,"SELECT FORMAT(%s,2)", c);
												err = mysql_query(&mysql, sql);
												if(err == 0)
												{
													resultado2 = mysql_store_result(&mysql);
													if(resultado2)
													{
														if((row2=mysql_fetch_row(resultado2)))
														{
															imprimir(alinea_d,nX);
															sprintf(c,"--------");
															imprimir(c,nX);
															imprimir(salto,nX);
															imprimir(negrita1,nX);
															sprintf(c,"%s",row2[0]);
															imprimir(c,nX);
															imprimir(salto,nX);
															imprimir(defecto,nX);
														}
													}
												}
												else
													printf("Error: %s\n", mysql_error(&mysql));
												sprintf(c, "\n-- %s --\n",row[5]);
												imprimir(alinea_c,nX);
												imprimir(negrita1,nX);
												imprimir(c,nX);
												imprimir(defecto,nX);
												imprimir(alinea_d,nX);
												bandera=2;
												corte_dinero_caja_num = 0;
											}

											corte_dinero_caja_num = corte_dinero_caja_num + atof(row[1]);
											num_numero = num_numero + atof(row[1]);
											strcpy(temp2,"");
											strcpy(c,"");
											for(i=0;i<26;i++)
											{
												if( i >=strlen(listatipos[4]))
													temp2[i] = ' ';
												else
													temp2[i] = listatipos[4][i];
												temp2[i+1] = '\0';
											}
											strcat(c, temp2);
											strncat(c,temp3,12-strlen(listatipos[3]));
											strcat(c, row[3]);
											imprimir(c,nX);
											imprimir(salto,nX);
										}
										sprintf(c,"%.2f",corte_dinero_caja_num);
										sprintf(sql,"SELECT FORMAT(%s,2)", c);
										err = mysql_query(&mysql, sql);
										if(err == 0)
										{
											resultado2 = mysql_store_result(&mysql);
											if(resultado2)
											{
												if((row2=mysql_fetch_row(resultado2)))
												{
													imprimir(alinea_d,nX);
													sprintf(c,"--------");
													imprimir(c,nX);
													imprimir(salto,nX);
													imprimir(negrita1,nX);
													sprintf(c,"%s",row2[0]);
													imprimir(c,nX);
													imprimir(salto,nX);
													imprimir(defecto,nX);
												}
											}
										}
										else
											printf("Error: %s\n", mysql_error(&mysql));
										imprimir(alinea_i,nX);
										
										//Monto dle retiro
										strcpy(cambio,"");
										sprintf(cambio, "%.2f",num_numero);
										imprimir(negrita1,nX);
										sprintf(c, "\nTOTAL CANTIDAD:  %s\n",cambio); imprimir(c,nX);
										imprimir(defecto,nX);
										strcpy(cambio,"");
										num_numero_int =mysql_num_rows(resultado);
										sprintf(cambio, "%d",num_numero_int);
										sprintf(c, "TOTAL ARTICULOS: %s\n",cambio); imprimir(c,nX);
									}else{
										//return (1);
									}
								}else{
									imprimiendo = FALSE;
									return (1);
								}
							}




							imprimir(salto,nX);
							imprimir(salto,nX);
							imprimir(salto,nX);
							imprimir(salto,nX);
							imprimir(salto,nX);
							imprimir(salto,nX);
							imprimir(salto,nX);
							imprimir(salto,nX);
							imprimir(corta_papel,nX);
							//fputs(c,fpt);
							fclose(fpt);
							//CORTE DE CAJA ALL
							if((impresora_nombre[arr_impresora_corte_all_caja][0]) == 2)
							{
								strcpy(cad_temporal,"lp.cups ");
								strcat(cad_temporal,impresora_corte_all_caja);
								strcat(cad_temporal," ");
								strcat(cad_temporal,TicketImpresion);
							}
							else
							{
								strcpy(cad_temporal,"cat ");
								strcat(cad_temporal,TicketImpresion);
								strcat(cad_temporal,"> ");
								strcat(cad_temporal,impresora_corte_all_caja);
							}
							//system(cad_temporal);
							if (manda_imprimir(TicketImpresion,"corte_all_caja") != 0)
							{
								Err_Info ("Error de impresion");
							}
							printf ("TEMP = %s\n",cad_temporal);
							}
						}
						else
						{
							imprimiendo = FALSE;
							return (1);
						}


				}else{
					imprimiendo = FALSE;
					return (1);
				 }
			mysql_close(&mysql); //Cierra conexion SQL

			}
		}

	}

	if(strcmp(tipo, "cierre_factura") == 0){
	printf("FACTURA\n");

	//Es Factura
		fpt2 = fopen(FacturaConfig,"r");
		if(fpt2 == NULL){
			printf("\nERROR no se puede abrir el archivo de configuracion");
			imprimiendo = FALSE;
			return (1);
		}else{
			m=1;
			fac_nombre[0][0] = 0; //Inicializa el arreglo fac_nombre
			fac_nombre[0][1] = 0;
			fac_nombre[0][2] = 0;
			fac_nombre[0][3] = 0;
			while(fgets(c, 255, fpt2) != NULL){
				strcat(c," ");
				strcpy(cadconf,"");
				j=0;
				k=0;
				printf("%s",c);
				for(i=0;i<strlen(c);i++){
					if(c[i] != '#'){
						if(c[i] == ' '){
							j=0;
							if(k==0){
								//Guarda la posicion de las variables.
								if(strcmp(cadconf,"nombre") == 0) arr_nombre = m;
								else if(strcmp(cadconf,"direccion") == 0) arr_direccion = m;
								else if(strcmp(cadconf,"ciudad") == 0) 	arr_ciudad = m;
								else if(strcmp(cadconf,"rfc") == 0) arr_rfc = m;
								else if(strcmp(cadconf,"dia") == 0) arr_dia = m;
								else if(strcmp(cadconf,"mes") == 0) arr_mes = m;
								else if(strcmp(cadconf,"ano") == 0) arr_ano = m;
								else if(strcmp(cadconf,"cantidad_letra") == 0) arr_cantidad_letra = m;
								else if(strcmp(cadconf,"observaciones") == 0) arr_observaciones = m;
								else if(strcmp(cadconf,"subtotal") == 0) 	arr_subtotal = m;
								else if(strcmp(cadconf,"iva") == 0) arr_iva = m;
								else if(strcmp(cadconf,"total") == 0) arr_total = m;
								else if(strcmp(cadconf,"cantidad_inicio") == 0) {
									arr_cantidad_inicio = m;
									arr_cantidad_inicio_tmp = m;
								}
								else if(strcmp(cadconf,"descripcion_inicio") == 0){
									arr_descripcion_inicio = m;
									arr_descripcion_inicio_tmp = m;
								}
								else if(strcmp(cadconf,"p_unitario_inicio") == 0){
									arr_p_unitario_inicio = m;
									arr_p_unitario_inicio_tmp = m;
								}
								else if(strcmp(cadconf,"importe_inicio") == 0){
									arr_importe_inicio = m;
									arr_importe_inicio_tmp = m;
								}
								else if(strcmp(cadconf,"cantidad_fin") == 0) arr_cantidad_fin = m;
								else if(strcmp(cadconf,"descripcion_fin") == 0) arr_descripcion_fin = m;
								else if(strcmp(cadconf,"p_unitario_fin") == 0) arr_p_unitario_fin = m;
								else if(strcmp(cadconf,"importe_fin") == 0) arr_importe_fin = m;
								else if(strcmp(cadconf,"fin_archivo") == 0) arr_fin_archivo = m;
								else if(strcmp(cadconf,"num_factura") == 0) arr_num_factura = m;
								else if(strcmp(cadconf,"num_venta") == 0) arr_num_venta = m;
								else if(strcmp(cadconf,"num_cliente") == 0) arr_num_cliente = m;

								//printf("->nombre %s\n<-",cadconf);
							}else if(k==1){
								fac_nombre[m][0] = atoi(cadconf)-1;
								//printf("->Y %s\n<-",cadconf);
							}else if(k==2){
								fac_nombre[m][1] = atoi(cadconf)-1;
								//printf("->X1 %s\n<-",cadconf);
							}else if(k==3){
								fac_nombre[m][2] = atoi(cadconf)-1;
								//printf("->X2 %s\n<-",cadconf);
							}else if(k==4){
								fac_nombre[m][3] = atoi(cadconf);
								//printf("->ALINEACION %s\n<-",cadconf);
							}
							strcpy(cadconf,"");
							k++;
						}else{
							cadconf[j] = c[i];
							cadconf[j+1] = '\0';
							j++;
						}
					}else{
						break;
					}
				}

				fac_nombre[0][0] = 0;
				m++;

			}

			fclose(fpt2);
		}




//Buscar el Y mayor
			num_mayor_Y = fac_nombre[arr_nombre][0];
			if(fac_nombre[arr_direccion][0] > num_mayor_Y) num_mayor_Y = fac_nombre[arr_direccion][0];
			if(fac_nombre[arr_ciudad][0] > num_mayor_Y) num_mayor_Y = fac_nombre[arr_ciudad][0];
			if(fac_nombre[arr_rfc][0] > num_mayor_Y) num_mayor_Y = fac_nombre[arr_rfc][0];
			if(fac_nombre[arr_dia][0] > num_mayor_Y) num_mayor_Y = fac_nombre[arr_dia][0];
			if(fac_nombre[arr_mes][0] > num_mayor_Y) num_mayor_Y = fac_nombre[arr_mes][0];
			if(fac_nombre[arr_ano][0] > num_mayor_Y) num_mayor_Y = fac_nombre[arr_ano][0];
			if(fac_nombre[arr_cantidad_letra][0] > num_mayor_Y) num_mayor_Y = fac_nombre[arr_cantidad_letra][0];
			if(fac_nombre[arr_observaciones][0] > num_mayor_Y) num_mayor_Y = fac_nombre[arr_observaciones][0];
			if(fac_nombre[arr_subtotal][0] > num_mayor_Y) num_mayor_Y = fac_nombre[arr_subtotal][0];
			if(fac_nombre[arr_iva][0] > num_mayor_Y) num_mayor_Y = fac_nombre[arr_iva][0];
			if(fac_nombre[arr_total][0] > num_mayor_Y) num_mayor_Y = fac_nombre[arr_total][0];
			if(fac_nombre[arr_cantidad_inicio][0] > num_mayor_Y) num_mayor_Y = fac_nombre[arr_cantidad_inicio][0];
			if(fac_nombre[arr_descripcion_inicio][0] > num_mayor_Y) num_mayor_Y = fac_nombre[arr_descripcion_inicio][0];
			if(fac_nombre[arr_p_unitario_inicio][0] > num_mayor_Y) num_mayor_Y = fac_nombre[arr_p_unitario_inicio][0];
			if(fac_nombre[arr_importe_inicio][0] > num_mayor_Y) num_mayor_Y = fac_nombre[arr_importe_inicio][0];
			if(fac_nombre[arr_cantidad_fin][0] > num_mayor_Y) num_mayor_Y = fac_nombre[arr_cantidad_fin][0];
			if(fac_nombre[arr_descripcion_fin][0] > num_mayor_Y) num_mayor_Y = fac_nombre[arr_descripcion_fin][0];
			if(fac_nombre[arr_p_unitario_fin][0] > num_mayor_Y) num_mayor_Y = fac_nombre[arr_p_unitario_fin][0];
			if(fac_nombre[arr_importe_fin][0] > num_mayor_Y) num_mayor_Y = fac_nombre[arr_importe_fin][0];
			if(fac_nombre[arr_fin_archivo][0] > num_mayor_Y) num_mayor_Y = fac_nombre[arr_fin_archivo][0];
			if(fac_nombre[arr_num_factura][0] > num_mayor_Y) num_mayor_Y = fac_nombre[arr_num_factura][0];
			if(fac_nombre[arr_num_venta][0] > num_mayor_Y) num_mayor_Y = fac_nombre[arr_num_venta][0];
			if(fac_nombre[arr_num_cliente][0] > num_mayor_Y) num_mayor_Y = fac_nombre[arr_num_cliente][0];


			//Fin de buscar el Y mayor

			//Buscar el X mayor
			num_mayor_X = fac_nombre[arr_nombre][2];
			if(fac_nombre[arr_direccion][2] > num_mayor_X) num_mayor_X = fac_nombre[arr_direccion][2];
			if(fac_nombre[arr_ciudad][2] > num_mayor_X) num_mayor_X = fac_nombre[arr_ciudad][2];
			if(fac_nombre[arr_rfc][2] > num_mayor_X) num_mayor_X = fac_nombre[arr_rfc][2];
			if(fac_nombre[arr_dia][2] > num_mayor_X) num_mayor_X = fac_nombre[arr_dia][2];
			if(fac_nombre[arr_mes][2] > num_mayor_X) num_mayor_X = fac_nombre[arr_mes][2];
			if(fac_nombre[arr_ano][2] > num_mayor_X) num_mayor_X = fac_nombre[arr_ano][2];
			if(fac_nombre[arr_cantidad_letra][2] > num_mayor_X) num_mayor_X = fac_nombre[arr_cantidad_letra][2];
			if(fac_nombre[arr_observaciones][2] > num_mayor_X) num_mayor_X = fac_nombre[arr_observaciones][2];
			if(fac_nombre[arr_subtotal][2] > num_mayor_X) num_mayor_X = fac_nombre[arr_subtotal][2];
			if(fac_nombre[arr_iva][2] > num_mayor_X) num_mayor_X = fac_nombre[arr_iva][2];
			if(fac_nombre[arr_total][2] > num_mayor_X) num_mayor_X = fac_nombre[arr_total][2];
			if(fac_nombre[arr_cantidad_inicio][2] > num_mayor_X) num_mayor_X = fac_nombre[arr_cantidad_inicio][2];
			if(fac_nombre[arr_descripcion_inicio][2] > num_mayor_X) num_mayor_X = fac_nombre[arr_descripcion_inicio][2];
			if(fac_nombre[arr_p_unitario_inicio][2] > num_mayor_X) num_mayor_X = fac_nombre[arr_p_unitario_inicio][2];
			if(fac_nombre[arr_importe_inicio][2] > num_mayor_X) num_mayor_X = fac_nombre[arr_importe_inicio][2];
			if(fac_nombre[arr_cantidad_fin][2] > num_mayor_X) num_mayor_X = fac_nombre[arr_cantidad_fin][2];
			if(fac_nombre[arr_descripcion_fin][2] > num_mayor_X) num_mayor_X = fac_nombre[arr_descripcion_fin][2];
			if(fac_nombre[arr_p_unitario_fin][2] > num_mayor_X) num_mayor_X = fac_nombre[arr_p_unitario_fin][2];
			if(fac_nombre[arr_importe_fin][2] > num_mayor_X) num_mayor_X = fac_nombre[arr_importe_fin][2];
			if(fac_nombre[arr_fin_archivo][2] > num_mayor_X) num_mayor_X = fac_nombre[arr_fin_archivo][2];
			if(fac_nombre[arr_num_factura][2] > num_mayor_X) num_mayor_X = fac_nombre[arr_num_factura][2];
			if(fac_nombre[arr_num_venta][2] > num_mayor_X) num_mayor_X = fac_nombre[arr_num_venta][2];
			if(fac_nombre[arr_num_cliente][2] > num_mayor_X) num_mayor_X = fac_nombre[arr_num_cliente][2];

			//Fin de buscar el X mayor



			nfilas = num_mayor_X + 1;
			ncols = num_mayor_Y + 1;
			/*Reserva inicial de memoria */
			printf("X: %d\n",nfilas);
			printf("Y: %d\n",ncols);
			for(fila=0; fila<=nfilas; fila++){
				printf("Aqui si entra....\n");
				matrix[fila] = (char *) malloc (ncols * sizeof(char));
			}



			//mysql_init(&mysql);

			//if(!mysql_real_connect(&mysql, "192.168.0.3", "caja", "caja", "CarnesBecerra", 0, NULL, 0)){
			if(conecta_bd_2(&mysql) == -1){
				printf("\nNo me puedo conectar a la base de datos =(\n");
				imprimiendo = FALSE;
				return (1);
			}else{

				//Con ID FACTURA


//SELECT Venta_Factura.id_factura, Venta_Factura.fecha, Venta_Factura.observacion, Cliente.nombre, Cliente.domicilio, Cliente.ciudad_estado, Cliente.rfc, Venta_Factura_Relacion.num_factura FROM Venta_Factura, Venta, Cliente, Venta_Factura_Relacion WHERE Venta.id_venta = Venta_Factura_Relacion.id_venta AND Venta_Factura_Relacion.id_factura = Venta_Factura.id_factura AND Venta.id_cliente = Cliente.id_cliente AND Venta.id_venta = 1
				//strcpy(sql,"SELECT Venta_Factura.id_factura, Venta_Factura.fecha, Venta_Factura.observacion, Cliente.nombre, Cliente.domicilio, Cliente.ciudad_estado, Cliente.rfc, Venta_Factura_Relacion.num_factura, Venta_Factura_Relacion.id_venta, Cliente.id_cliente FROM Venta_Factura, Venta, Cliente, Venta_Factura_Relacion WHERE Venta.id_venta = Venta_Factura_Relacion.id_venta AND Venta_Factura_Relacion.id_factura = Venta_Factura.id_factura AND Venta.id_cliente = Cliente.id_cliente AND Venta.id_venta = ");
				//sprintf(sql,"SELECT Venta_Factura.id_factura, Venta_Factura.fecha, Venta_Factura.observacion, 'CLIENTES VARIOS', '', '', '', Venta_Factura_Relacion.num_factura, 'CIERRE', '' FROM Venta_Factura, Venta_Factura_Relacion WHERE Venta_Factura_Relacion.id_factura = Venta_Factura.id_factura AND Venta_Factura_Relacion.num_factura = %s GROUP BY Venta_Factura_Relacion.num_factura",id_venta);
				sprintf(sql,"SELECT Venta.fecha_factura, Venta.observacion, 'CLIENTES VARIOS', '', '', '', Venta.num_factura, 'CIERRE', '' FROM Venta WHERE Venta.num_factura = %s GROUP BY Venta.num_factura",id_venta);
				//strcat(sql, id_venta);

				printf("El id de la factura: %s\n", id_venta);
				printf("El SQL: %s\n", sql);
				if((err = mysql_query(&mysql, sql)) != 0){
					printf("Error al consultar los tipos de documentos: %s\n", mysql_error(&mysql));
					imprimiendo = FALSE;
					return (1);
				}else{
					resultado = mysql_store_result(&mysql); //Saca la consulta SQL
						if(resultado){
							if (mysql_num_rows(resultado) > 0){
								while((row = mysql_fetch_row(resultado)))
								{
									fpt = fopen(TicketImpresion,"w+");
									if(fpt == NULL){
									        printf("\nNo pudo imprimir la factura %s",row[0]);
										printf("\nERROR no se puede abrir el archivo a imprimir");
										imprimiendo = FALSE;
										return (1);
									}else{
										inicializar(); //Inicializa MATRIX
										for(i=0; i < 11;i++) //Guarda todo el arreglo en listapos
											listatipos[i] =  row[i];

										//strcpy(id_factura_interno,listatipos[0]);

										//Separamos el aÃ±o
										fechaTMP[0] = listatipos[0][0];
										fechaTMP[1] = listatipos[0][1];
										fechaTMP[2] = listatipos[0][2];
										fechaTMP[3] = listatipos[0][3];
										fechaTMP[4] = '\0';
										llenarMATRIX(arr_ano,fechaTMP); //DIA
										//Separamos el mes
										fechaTMP[0] = listatipos[0][5];
										fechaTMP[1] = listatipos[0][6];
										fechaTMP[2] = '\0';
										llenarMATRIX(arr_mes,fechaTMP); //MES
										//Separamos el dia
										fechaTMP[0] = listatipos[0][8];
										fechaTMP[1] = listatipos[0][9];
										fechaTMP[2] = '\0';
										llenarMATRIX(arr_dia,fechaTMP); //DIA

										llenarMATRIX(arr_observaciones, listatipos[1]); //Observaciones

										llenarMATRIX(arr_num_cliente, listatipos[8]); //ID cliente
										llenarMATRIX(arr_nombre, listatipos[2]); //Nombre cliente
										llenarMATRIX(arr_direccion, listatipos[3]); //Direccion cliente
										llenarMATRIX(arr_ciudad, listatipos[4]); //Ciudad y estado
										llenarMATRIX(arr_rfc, listatipos[5]); //RFC

										llenarMATRIX(arr_num_factura, listatipos[6]); //Numero de Factura
										llenarMATRIX(arr_num_venta, listatipos[7]); //Numero de la Venta

										//Con ID FACTURA



//							strcpy(sql,"SELECT /*FORMAT(*/Venta_Articulo.cantidad/*,3)*/, Articulo.nombre, /*FORMAT(*/Venta_Articulo.precio/*,2)*/ AS precio_IVA, /*FORMAT(*/Venta_Articulo.monto/*,2)*/ AS monto_IVA, /*FORMAT(*/Venta_Articulo.precio/*,2)*/ AS precio, /*FORMAT(*/Venta_Articulo.monto/*,2)*/ AS monto FROM Venta_Articulo, Articulo WHERE Venta_Articulo.id_articulo = Articulo.id_articulo AND Venta_Articulo.id_venta = ");

										//sprintf(sql, "SELECT FORMAT(Venta_Articulo.cantidad,3), Articulo.nombre, FORMAT(Venta_Articulo.precio,2) AS precio_IVA, FORMAT(Venta_Articulo.monto,2) AS monto_IVA, FORMAT(Venta_Articulo.precio,2) AS precio, FORMAT(Venta_Articulo.monto,2) AS monto FROM Venta_Articulo, Articulo WHERE Venta_Articulo.id_articulo = Articulo.id_articulo AND Venta_Articulo.id_venta = %s AND Venta_Articulo.id_factura = %s ORDER BY id_venta_articulo", id_venta, id_factura_interno);
										sprintf(sql, "SELECT '1', 'PRODUCTOS VARIOS', FORMAT(SUM(Venta.monto),2) AS precio_IVA, FORMAT(SUM(Venta.monto),2) AS monto_IVA, FORMAT(SUM(Venta.monto),2) AS precio, FORMAT(SUM(Venta.monto),2) AS monto FROM Venta WHERE Venta.num_factura = %s", id_venta);
										printf("%s\n",sql);
										err = mysql_query(&mysql, sql);
										if(err != 0){
											printf("Error al consultar la venta de factura: %s\n", mysql_error(&mysql));
											imprimiendo = FALSE;
											return (1);
										}else{
											resultado2 = mysql_store_result(&mysql); //Saca la consulta SQL
											if(resultado2){

												fac_subtotal_num = 0;
												fac_total_num = 0;
												while((row2 = mysql_fetch_row(resultado2))){
													printf("---%s\n",row[0]);
													/*for(i=0; i < 6;i++) //Guarda todo el arreglo en listapos
														listatipos[i] =  row2[i];*/


/*													num_numero = atof(listatipos[0]);
													sprintf(fac_valor_char, "%.3f", num_numero);*/
													printf("Cantidad: %s\n", row2[0]);
													sprintf(fac_valor_char, "%s", row2[0]);
													llenarMATRIX(arr_cantidad_inicio, fac_valor_char); //Cantidad
													//llenarMATRIX(arr_cantidad_inicio, listatipos[0]);
													llenarMATRIX(arr_descripcion_inicio, row2[1]); //Descripcion
													/*num_numero = atof(listatipos[2]);
													sprintf(fac_valor_char, "%.2f", num_numero);*/
													sprintf(fac_valor_char, "%s", row2[2]);
													llenarMATRIX(arr_p_unitario_inicio, fac_valor_char); //P.Unitario
													//llenarMATRIX(arr_p_unitario_inicio, listatipos[2]);
													/*num_numero = atof(listatipos[3]);
													sprintf(fac_valor_char, "%.2f", num_numero);*/
													sprintf(fac_valor_char, "%s", row2[3]);
													llenarMATRIX(arr_importe_inicio,fac_valor_char); //Importe
													//llenarMATRIX(arr_importe_inicio,listatipos[3]);
/*													fac_subtotal_num = fac_subtotal_num + atof(listatipos[3]);
													fac_total_num = fac_total_num + atof(listatipos[3]);*/

													//Incrementa las posiciones en Y

													fac_nombre[arr_cantidad_inicio][0] ++;
													fac_nombre[arr_descripcion_inicio][0] ++;
													fac_nombre[arr_p_unitario_inicio][0] ++;
													fac_nombre[arr_importe_inicio][0] ++;
													if(fac_nombre[arr_cantidad_inicio][0] >= fac_nombre[arr_cantidad_fin][0]){
														printf("\n");
														printf("\nERROR AL GUARDAR EN EL ARCHIVO, SE SUPERO EL TAMAï¿œ DE LA CADENA PERMITIDA");
														printf("\n");
														//return (1);
														break;
													}
												}
												//sprintf(sql, "SELECT FORMAT(SUM(Venta_Articulo.monto),2) as total, SUM(Venta_Articulo.monto) as total_letra FROM Venta_Articulo WHERE id_venta = %s AND id_factura = %s ORDER BY id_venta_articulo", id_venta, id_factura_interno);
												sprintf(sql, "SELECT FORMAT(SUM(Venta.monto),2) as total, SUM(Venta.monto) as total_letra FROM Venta WHERE Venta.num_factura = %s", id_venta);
												printf("%s\n",sql);
												err = mysql_query(&mysql, sql);
												if(err != 0){
													printf("Error al consultar la venta de factura: %s\n", mysql_error(&mysql));
													imprimiendo = FALSE;
													return (1);
												}else{
													resultado2 = mysql_store_result(&mysql); //Saca la consulta
													if(resultado2)
													{
														row = mysql_fetch_row(resultado2);
														//fac_iva_num = fac_total_num - fac_subtotal_num;
														fac_iva_num = 0;
														sprintf(fac_iva, "%.2f", fac_iva_num);
														sprintf(fac_subtotal, "%s", row[0]);
														sprintf(fac_total, "%s", row[0]);
														sprintf(num_tmp, "%s", row[1]); //Convierte a cadena
													}
												}
												arr_cantidad_inicio = arr_cantidad_inicio_tmp;
												arr_descripcion_inicio = arr_descripcion_inicio_tmp;
												arr_p_unitario_inicio = arr_p_unitario_inicio_tmp;
												arr_importe_inicio = arr_importe_inicio_tmp;

												//printf("\nTOTAL: %s",peso);
												llenarMATRIX(arr_subtotal, fac_subtotal); //Importe SUBTOTAL
												llenarMATRIX(arr_iva, fac_iva); //Importe IVA
												llenarMATRIX(arr_total, fac_total); //Importe TOTAL

												cantidad_letra_num = atof(num_tmp);

												traduce(cantidad_letra_num, retVar);
												printf("\nCANIDAD LETRA: %s ",retVar);
												llenarMATRIX(arr_cantidad_letra, retVar); //Importe Cantidad con letra

											}else{
												imprimiendo = FALSE;
												return (1);
											}
										}


									fputs(resetea, fpt);
									fputs(pt_pagina,fpt);
									fputs(pmargen_a,fpt);
									fputs(pcpi10,fpt);
									fputs(pcondensed,fpt);

									escribirsalida("");
									escribirsalida("archivo");

									fputs(psig_pag,fpt);
									fputs(resetea, fpt);

									//fputs(c,fpt);
									fclose(fpt);
									//FACTURA
									if((impresora_nombre[arr_impresora_factura][0]) == 2)
									{
										strcpy(cad_temporal,"lp.cups ");
										strcat(cad_temporal,impresora_factura);
										strcat(cad_temporal," ");
										strcat(cad_temporal,TicketImpresion);
									}
									else
									{
										strcpy(cad_temporal,"cat ");
										strcat(cad_temporal,TicketImpresion);
										strcat(cad_temporal,"> ");
										strcat(cad_temporal,impresora_factura);
									}
									//system(cad_temporal);
									manda_imprimir (TicketImpresion,"factura");
									printf ("TEMP = %s\n",cad_temporal);
									imprimiendo = FALSE;
									return (1);
								}

								}
							}else{
								printf("No se pudo la factura....\n");
								imprimiendo = FALSE;
								return(1);

							}
						}else{
							imprimiendo = FALSE;
							return (1);
						}


				mysql_close(&mysql); //Cierra conexion SQL

				}

			}





	}
	else if(strcmp(tipo, "listado_cierre") == 0){ //Es credito o contado

		fpt = fopen(TicketImpresion,"w");
		if(fpt == NULL){
			printf("\nERROR no se puede abrir el archivo a imprimir");
			imprimiendo = FALSE;
			return (1);
		}
		else
		{
		//Abre el encabezado de archivo
		fpt2 = fopen(TicketArriba,"r");
		if(fpt2 == NULL){
			printf("\nERROR no se puede abrir el archivo del encabezado");
			imprimiendo = FALSE;
			return (1);
		}else
		{
			imprimir(alinea_c, nX);
			imprimir(negrita_subraya,nX);
			imprimir("Cierre de Facturas",nX);
			imprimir(cancela,nX);
			imprimir(salto,nX);
			imprimir(salto,nX);
			imprimir(alinea_i, nX);
			while(fgets(c, 255, fpt2) != NULL)
			{
				imprimir(c,nX);
			}
			fclose(fpt2);
			imprimir(salto,nX);
			imprimir(alinea_c,nX);
			imprimir(negrita_grande,nX);
			imprimir("CARNES BECERRA",nX);
			imprimir(salto,nX);
			imprimir(salto,nX);
			imprimir(salto,nX);
			imprimir(salto,nX);
			imprimir(cancela,nX);
			imprimir(tamano1,nX);
			imprimir(alinea_i,nX);

			//sprintf(sql, "SELECT Venta.id_venta, DATE_FORMAT(Venta.fecha,\"%%d-%%m-%%Y\"), FORMAT(ROUND(Venta.monto,2),2), Venta_Factura_Relacion.id_venta, Venta.monto FROM Venta INNER JOIN Venta_Factura_Relacion ON Venta.id_venta = Venta_Factura_Relacion.id_venta WHERE Venta_Factura_Relacion.num_factura = %s AND Venta.cancelada = 'n' ORDER BY Venta.id_venta", id_venta);
			sprintf(sql, "SELECT Venta.id_venta, DATE_FORMAT(Venta.fecha,\"%%d-%%m-%%Y\"), FORMAT(ROUND(Venta.monto,2),2), Venta.monto FROM Venta WHERE Venta.num_factura = %s AND Venta.cancelada = 'n' ORDER BY Venta.id_venta", id_venta);
	
			printf("Consulta: %s\n", sql);

			if(conecta_bd() == 1)
			{
				err = mysql_query(&mysql, sql);
				if(err == 0)
				{
					resultado = mysql_store_result(&mysql);
					if(resultado)
					{
						strcpy(total_cierre_listado,"0.00");
						sprintf(c, "FECHA        FOLIO          MONTO");
						imprimir(alinea_d, nX);
						imprimir(subraya_s,nX);
						imprimir(c, nX);
						imprimir(salto,nX);
						imprimir(cancela, nX);
						imprimir(defecto, nX);
						imprimir(alinea_i, nX);
						while((row=mysql_fetch_row(resultado)))
						{
								strcpy(c, row[1]);
								strncat(c, temp3, 12-strlen(row[0]));
								strcat(c, row[0]);
								strncat(c, temp3, 16-strlen(row[2]));
								strcat(c, row[2]);
								imprimir(c,nX);
								imprimir(salto,nX);
								sprintf(total_cierre_listado, "%f", atof(total_cierre_listado)+atof(row[3]));
						}
						imprimir(salto,nX);
						sprintf(sql, "SELECT FORMAT(%s,2)",total_cierre_listado);
						err = mysql_query(&mysql, sql);
						if(err == 0)
						{
							resultado = mysql_store_result(&mysql);
							if(resultado)
							{
								if((row=mysql_fetch_row(resultado)))
								{
									strcpy(total_cierre_listado, row[0]);
									//imprimir(salto,nX);
									imprimir(alinea_d,nX);
									imprimir("---------------",nX);
									imprimir(salto,nX);
									imprimir(negrita_grande,nX);
									imprimir(total_cierre_listado,nX);
									imprimir(salto,nX);
									imprimir(resetea,nX);
								}
							}
						}
						else
						{
							sprintf(Errores,"%s",mysql_error(&mysql));
							Err_Info(Errores);
						}
						imprimir(salto,nX);
						imprimir(salto,nX);
					}
				}
				else
				{
					sprintf(Errores,"%s",mysql_error(&mysql));
					Err_Info(Errores);
				}
			}
			else
			{
				sprintf(Errores,"%s",mysql_error(&mysql));
				Err_Info(Errores);
			}
			mysql_close(&mysql);
		}
		imprimir(salto,nX);
		imprimir(alinea_d,nX);
		//imprimir(negrita1,nX);
		imprimir("Matica",nX);
		imprimir(salto,nX);
		imprimir(negrita_subraya1,nX);
		imprimir("software libre",nX);
		imprimir(salto,nX);
		imprimir(cancela,nX);
		imprimir(defecto,nX);
		imprimir(salto,nX);
		imprimir(salto,nX);
		imprimir(salto,nX);
		imprimir(salto,nX);
		imprimir(salto,nX);
		imprimir(salto,nX);
		printf("\n\n\n\n\n");
		imprimir(corta_papel,nX);

//		imprimir(c,nX);
		imprimir(cancela,nX);
		imprimir(defecto,nX);
		imprimir(resetea,nX);
		fclose(fpt);
		printf("\n\n\nIMPRIMIENDO....\n");
		//TICKET
		if((impresora_nombre[arr_impresora_contado][0]) == 2)
		{
			strcpy(cad_temporal,"lp.cups ");
			strcat(cad_temporal,impresora_contado);
			strcat(cad_temporal," ");
			strcat(cad_temporal,TicketImpresion);
		}
		else
		{
			strcpy(cad_temporal,"cat ");
			strcat(cad_temporal,TicketImpresion);
			strcat(cad_temporal,"> ");
			strcat(cad_temporal,impresora_contado);
		}
  		manda_imprimir (TicketImpresion,"corte_all_caja");
		printf ("TEMP = %s\n",cad_temporal);
  		}

	}
	else if(strcmp(tipo, "pedidos_paso1") == 0){ //Es credito o contado

		fpt = fopen(TicketImpresion,"w");
		if(fpt == NULL){
			printf("\nERROR no se puede abrir el archivo a imprimir");
			imprimiendo = FALSE;
			return (1);
		}
		else
		{
		//Abre el encabezado de archivo

			imprimir(alinea_c, nX);
			imprimir(negrita_subraya,nX);
			imprimir("CARNES BECERRA",nX);
			imprimir(cancela,nX);
			imprimir(salto,nX);
			imprimir(alinea_c, nX);
			imprimir(negrita_grande,nX);
			imprimir("PEDIDO",nX);
			imprimir(salto,nX);
			imprimir(cancela,nX);
			imprimir(tamano1,nX);
			imprimir(alinea_i,nX);

			sprintf(sql, "SELECT  DATE_FORMAT(Pedido.fecha,\"%%d/%%m/%%Y\"), Pedido.hora, Pedido.HoraEntrega, DATE_FORMAT(Pedido.FechaEntrega,\"%%d/%%m/%%Y\"), Cliente.nombre, Cliente.telefono, Usuario.nombre, Cliente.domicilio, Cliente.id_cliente, Cliente.entre_calles, Cliente.ciudad_estado, Cliente.colonia, Pedido.observacion_pedido, Pedido.servicio, FORMAT(Pedido.servicio,2), Lista.nombre FROM Pedido INNER JOIN Cliente ON Cliente.id_cliente = Pedido.id_cliente INNER JOIN Lista On Lista.id_lista = Cliente.id_lista INNER JOIN Usuario ON Pedido.id_usuario=Usuario.id_usuario WHERE Pedido.id_pedido=%s", id_venta);

			printf("\nConsulta: %s\n", sql);

			if(conecta_bd() == 1)
			{
				printf("\nTicket para el carnicero\n\n\n");
				err = mysql_query(&mysql, sql);
				if(err == 0)
				{
					resultado = mysql_store_result(&mysql);
					if(resultado)
					{
						if (mysql_num_rows(resultado) > 0){
							if((row=mysql_fetch_row(resultado)))
							{
								if (row[12] != NULL)
									observaciones_pedido=row[12];
								else
									observaciones_pedido = "";
								
								if (row[13] != NULL)
								{
									servicio = atof(row[13]);
									sprintf(servicio_formato,"%s",row[13]);
								}
								else
								{
									servicio = 0;
								}
								
								printf ("\n\n########### Observaciones = %s\n\n",observaciones_pedido);
								sprintf (sql,"SELECT ciudad_estado, domicilio, telefono FROM Cliente_Envio WHERE id_cliente = %s",row[8]);
								printf ("SQL 8 = %s ",sql);
								
								res_enviar_a = conecta_bd_3(sql);
								if (res_enviar_a)
								if (row_enviar_a = mysql_fetch_row (res_enviar_a))
								{
									printf ("\n\n\n ##########33DATOS DE ENVIO############\n\n\n");
									if (row_enviar_a[0])
										printf ("Ciudad Estado: %s\n",row_enviar_a[0]);
									
									if (row_enviar_a[1])
										printf ("Domicilio: %s\n",    row_enviar_a[1]);
									
									if (row_enviar_a[2])
									{
										printf ("Telefono: %s\n",     row_enviar_a[2]);
									}
								}
								
								strcpy(c,"Folio: ");
								strcat(c, id_venta);
								imprimir(c,nX);
								imprimir(salto,nX);
								imprimir(salto,nX);
								strcpy(c,"Capturado: ");
								imprimir(negrita,nX);
								imprimir(c,nX);

								strcpy(c, row[1]);
								strncat(c, temp3, 10-strlen(row[1]));
								strcat(c, row[0]);
								strncat(c, temp3, 12-strlen(row[0]));
								imprimir(c,nX);
								imprimir(salto,nX);
								imprimir(cancela,nX);
								imprimir(defecto,nX);
								strcpy(c,"Capturo:  ");
								strcat(c, row[6]);
								imprimir(c,nX);
								imprimir(salto,nX);
								strcpy(c,"Entregarlo:  ");
								imprimir(c,nX);
								//imprimir(salto,nX);
								strcpy(c, row[2]);
								strncat(c, temp3, 10-strlen(row[2]));
								strcat(c, row[3]);
								strncat(c, temp3, 12-strlen(row[3]));
								imprimir(c,nX);
								imprimir(salto,nX);
								
								if (strcmp(row[15],"MINISUPER") == 0)
								{
									imprimir(negrita,nX);
									imprimir(alinea_c,nX);
									strcpy(c, "+---------------+");
									imprimir(c,nX);
									imprimir(salto,nX);
									strcpy(c, "|   MINISUPER   |");
									imprimir(c,nX);
									imprimir(salto,nX);
									strcpy(c, "+---------------+");
									imprimir(c,nX);
									imprimir(salto,nX);
									imprimir(tamano1,nX);
									imprimir(alinea_i,nX);
								}

								strcpy(c,"Cliente:  ");
								imprimir(c,nX);
								strcpy(c, row[8]);
								imprimir(c,nX);
								imprimir(psalto_l,nX);
								strcpy(c, row[4]);
								imprimir(c,nX);
								imprimir(psalto_l,nX);
								
								/*strcpy(c,"Numero de cliente:  ");
								strcat(c, row[8]);
								imprimir(c,nX);
								imprimir(psalto_l,nX);*/
								
								sprintf (sql,"SELECT domicilio, entre_calles, ciudad_estado, colonia, telefono FROM Cliente_Envio WHERE id_cliente = %s",row[8]);
								printf ("\n### %s\n",sql);
								err = mysql_query(&mysql,sql);
								if (err == 0)
								{
									if ( (resultado2 = mysql_store_result (&mysql)) )
									{
										if (row2 = mysql_fetch_row (resultado2))
										{
											strcpy(domicilio_envio, row2[0]);
										}
									}
									else
										printf ("Error al obtener el domicilio de envio.");
								}
								else
									printf ("Error al obtener el domicilio de envio.");
								
								if  (strlen (domicilio_envio) > 0)
								{
									strcpy(c,"Domicilio:  ");
									strcat(c, domicilio_envio);
									imprimir(c,nX);
									imprimir(psalto_l,nX);
									
									strcpy(c,"Entre calles: ");
									strcat(c, row2[1]);
									imprimir(c,nX);
									
									imprimir(psalto_l,nX);
									
									strcpy(c,"Ciudad_Estado: ");
									strcat(c, row2[2]);
									imprimir(c,nX);
									imprimir(psalto_l,nX);
									
									strcpy(c,"Colonia: ");
									strcat(c, row2[3]);
									imprimir(c,nX);
									imprimir(psalto_l,nX);
									
									strcpy(c,"Telefono: ");
									strcat(c, row2[4]);
									imprimir(c,nX);
									imprimir(psalto_l,nX);
								}
								else
								{
									strcpy(c,"Domicilio:  ");
									strcat(c, row[7]);
									imprimir(c,nX);
									imprimir(psalto_l,nX);
									
									strcpy(c,"Entre Calles:  ");
									strcat(c, row[9]);
									imprimir(c,nX);
									
									imprimir(psalto_l,nX);
									
									strcpy(c,"Ciudad_Estado: ");
									strcat(c, row[10]);
									imprimir(c,nX);
									imprimir(psalto_l,nX);
									
									strcpy(c,"Colonia: ");
									strcat(c, row[11]);
									imprimir(c,nX);
									imprimir(psalto_l,nX);
									
									strcpy(c,"Telefono: ");
									strcat(c, row[5]);
									imprimir(c,nX);
									imprimir(psalto_l,nX);
								}
							}else{
								Err_Info("\nError al procesar  los pedidos");
							}
						}
						sprintf(sql, "SELECT CONCAT(Subproducto.codigo,Articulo.codigo), FORMAT(Pedido_Articulo.cantidad,3), Articulo.nombre, Articulo.tipo, Pedido_Articulo.observaciones FROM Pedido_Articulo INNER JOIN Articulo ON Articulo.id_articulo = Pedido_Articulo.id_articulo INNER JOIN Subproducto ON Articulo.id_subproducto = Subproducto.id_subproducto WHERE id_pedido = %s ORDER BY Pedido_Articulo.id_pedidos",id_venta);
						printf("\nConsulta2: %s\n", sql);
      						err = mysql_query(&mysql, sql);
						if(err == 0)
						{
							resultado = mysql_store_result(&mysql);
							if(resultado)
							{
								imprimir(alinea_c, nX);
								sprintf(c, "                                      ");
								imprimir(subraya_s1,nX);
								imprimir(c,nX);
								imprimir(salto,nX);
								imprimir(cancela,nX);
								imprimir(defecto,nX);
								if (mysql_num_rows(resultado) > 0){
									while((row=mysql_fetch_row(resultado)))
									{
										strcpy(c, "");
										if(strcmp(row[3], "peso")==0)
											strcpy(num_tmp,"kg");
										else
											strcpy(num_tmp,"  ");

										imprimir(alinea_i, nX);
										imprimir(negrita1,nX);
										strcpy(cad_temporal, row[0]);
										strncat(c, temp, 4-strlen(cad_temporal)); //ID Articulo
										strcat(c, cad_temporal);
										imprimir(c,nX);
										if(strlen(row[2]) > 27) //Nombre
										{
											imprimir(salto,nX);
											//imprimir(alinea_d, nX);
											imprimir(row[2], nX);
										}
										else
										{
											//imprimir(alinea_i, nX);
											sprintf(c, " %s", row[2]);
											strncat(c, temp3, 27-strlen(row[2]));
											imprimir(c,nX);
										}
										imprimir(salto,nX);
										imprimir(alinea_c, nX);
										imprimir(negrita,nX);
										strcpy(cad_temporal,"");
										strcpy(c,"");
										sprintf(cad_temporal, "%s%s", row[1],num_tmp);	//Cantidad
										strncat(c, temp3, 10-strlen(cad_temporal));
										strcat(c, cad_temporal);
										imprimir(c,nX);
										imprimir(salto,nX);
										imprimir(cancela,nX);
										imprimir(defecto,nX);
										imprimir(alinea_i, nX);
										sprintf(c,"%s",row[4]);
										imprimir(c,nX);
										imprimir(salto,nX);
										sprintf(c,"--------------------------------------");
										imprimir(c,nX);
										imprimir(salto,nX);
									}
									imprimir(alinea_c, nX);
									sprintf(c, "                                      ");
									imprimir(subraya_s1,nX);
									imprimir(c,nX);
									imprimir(salto,nX);
									imprimir(cancela,nX);
									imprimir(defecto,nX);
									imprimir(alinea_i, nX);
									num_numero_int = mysql_num_rows(resultado);
									sprintf(cambio, "%d",num_numero_int);
									sprintf(c, "TOTAL ARTICULOS: %s\n",cambio); imprimir(c,nX);

									//Calcula el Total de Cantidad
									sprintf(sql, "SELECT TRUNCATE(SUM(Pedido_Articulo.cantidad),3) FROM Pedido_Articulo INNER JOIN Articulo ON Articulo.id_articulo = Pedido_Articulo.id_articulo WHERE Pedido_Articulo.id_pedido = %s AND Articulo.tipo = 'peso'  GROUP BY Pedido_Articulo.id_pedido",id_venta);
									printf("\nConsulta3 (a): %s\n", sql);
									if(!(err = mysql_query(&mysql, sql)))
										if((resultado = mysql_store_result(&mysql)))
											if((row=mysql_fetch_row(resultado))){
												sprintf(c, "TOTAL KILOS:  %skg\n",row[0]); imprimir(c,nX);
											}else
												Err_Info("Error al calcular la cantidad de los articulos");
										else
											Err_Info("Error al calcular la cantidad de los articulos");
									else
										Err_Info("Error al calcular la cantidad de los articulos");

									//Comentarios de los productos
									/*sprintf(sql, "SELECT Articulo.codigo, Articulo.nombre, Pedido_Articulo.observaciones FROM Pedido_Articulo INNER JOIN Articulo ON Articulo.id_articulo = Pedido_Articulo.id_articulo where id_pedido = %s AND Pedido_Articulo.observaciones != '' ",id_venta);
									printf("\nConsulta4: %s\n", sql);
									if(!(err = mysql_query(&mysql, sql))){
										if((resultado = mysql_store_result(&mysql))){
											if (mysql_num_rows(resultado) > 0){
												imprimir(salto,nX);
												imprimir(subraya_s1,nX);
												strcpy(c,"OBSERVACIONES: ");
												imprimir(c,nX);
												imprimir(salto,nX);
												imprimir(salto,nX);
												imprimir(cancela,nX);
												imprimir(defecto,nX);
												while((row=mysql_fetch_row(resultado))){
													strcpy(c,"--");
													strcat(c,row[1]);
													strcat(c,"--");
													imprimir(negrita1,nX);
													imprimir(c,nX);
													imprimir(salto,nX);
													imprimir(cancela,nX);
													imprimir(defecto,nX);
													strcpy(c,row[2]);
													imprimir(c,nX);
													imprimir(salto,nX);
												}
											}
										}else
										Err_Info("Error al buscar las observaciones de los articulos");
									}else
										Err_Info("Error al buscar las observaciones de los articulos");*/
								}
							}
						}
						else
						{
							sprintf(Errores,"%s",mysql_error(&mysql));
							Err_Info(Errores);
						}
						imprimir(salto,nX);
					}
				}
				else
				{
					sprintf(Errores,"%s",mysql_error(&mysql));
					Err_Info(Errores);
				}
			}
			else
			{
				sprintf(Errores,"%s",mysql_error(&mysql));
				Err_Info(Errores);
			}
			mysql_close(&mysql);
			
			if((observaciones_pedido != ""))
			{
				imprimir(salto,nX);
				imprimir(observaciones_pedido,nX);
				imprimir(salto,nX);
				//printf ("\n### 2 Observaciones de Pedidos %s\n",observaciones_pedido);
			}
			if (servicio > 0)
			{
				imprimir(salto,nX);
				imprimir("Servicio: $ ",nX);
				imprimir(servicio_formato,nX);
				imprimir(salto,nX);
			}

		imprimir(salto,nX);
		imprimir(alinea_d,nX);
		//imprimir(negrita1,nX);
		imprimir("Matica",nX);
		imprimir(salto,nX);
		imprimir(negrita_subraya1,nX);
		imprimir("software libre",nX);
		imprimir(salto,nX);
		imprimir(salto,nX);
		
		if((observaciones_pedido != ""))
		{
			imprimir(salto,nX);
			imprimir(salto,nX);
			imprimir(negrita_media,nX);
			imprimir(alinea_i,nX);
			imprimir(salto,nX);
			//imprimir(negrita,nX);
			//imprimir(tamano2,nX);
			
			
			imprimir("[------------------------------]",33);
			imprimir(salto,33);
			
			imprimir(observaciones_pedido,33);
			imprimir(salto,33);
			
			imprimir("[------------------------------]",33);
			imprimir(salto,33);
			//printf ("\n### 2 Observaciones de Pedidos %s\n",observaciones_pedido);
		}
		
		imprimir(cancela,nX);
		imprimir(defecto,nX);
		imprimir(salto,nX);
		imprimir(salto,nX);
		imprimir(salto,nX);
		imprimir(salto,nX);
		imprimir(salto,nX);
		printf("\n\n\n\n\n");
		imprimir(corta_papel,nX);


//		imprimir(c,nX);
		imprimir(cancela,nX);
		imprimir(defecto,nX);
		imprimir(resetea,nX);
		fclose(fpt);
		printf("\n\n\nIMPRIMIENDO....\n");
		//TICKET

		if((impresora_nombre[arr_impresora_pedidos_paso1][0]) == 2)
		{
			strcpy(cad_temporal,"lp.cups ");
			strcat(cad_temporal,impresora_pedidos_paso1);
			strcat(cad_temporal," ");
			strcat(cad_temporal,TicketImpresion);
		}
		else
		{
			strcpy(cad_temporal,"cat ");
			strcat(cad_temporal,TicketImpresion);
			strcat(cad_temporal,"> ");
			strcat(cad_temporal,impresora_pedidos_paso1);
		}

		//system(cad_temporal);
		if (manda_imprimir (TicketImpresion,"pedidos_paso1") != 0)
		{
			Err_Info ("Error de impresion");
		}
		printf ("TEMP = %s\n",cad_temporal);
		imprimiendo = FALSE;
		return (1);
		}

	}
	printf("Antes de que termine la funcion de impresion\n");
	printf("Antes de que termine la funcion de impresion... un poquito después\n");
	//imprimiendo = FALSE;
	return (0);
}


int abrir_cajon (char puerto[50])
{
        char comando[6];
        FILE *impresora;
                                                                                                 
        comando[0]=27;
        comando[1]='p';
        comando[2]=0;
        comando[3]=25;
        comando[4]=250;
        comando[5]='\0';
        if((impresora = fopen(puerto,"w")))
	{
        	fputc (comando[0],impresora);
        	fputc (comando[1],impresora);
        	fputc (comando[2],impresora);
        	fputc (comando[3],impresora);
        	fputc (comando[4],impresora);
        	fputc (comando[5],impresora);
		fclose (impresora);
	}
                                                                                                 
        return 0;
}


int checar_puerto_serie(){
//char nombrearchivo[50] = "prueba.txt"; //Nombre del archivo
//char *consulta = "default"; //Nombre de la impresora
char impresora[100];

//char ImpresoraConfig[] = "impresion_conf/impresoras.conf.txt"; //Nombre del archivo de configuracion
char c[1000]; //Aqui se guarda las cadenas a imprimir
char cadconf[50];
int i, j, k, m;
int error = 0;
//Parametros.
int par1 = -1;
int par2 = -1;
char par3[50];
char par4[50];
FILE *fpt;
//printf("\nSacando informacion de la configuracion Serial");
/*CONFIGURACION DE LA IMPRESORA
*/
		fpt = fopen(ImpresoraConfig,"r");
		if(fpt == NULL){
			printf("\nERROR no se puede abrir el archivo de configuracion de las impresoras");
			return (1);
		}else{
			m=1;
			while(fgets(c, 255, fpt) != NULL){
				printf("M=%d\n",m);
				strcat(c," ");
				strcpy(cadconf,"");
				j=0;
				k=0;
				//printf("\n\nCADENA: %s",c);
				for(i=0;i<strlen(c);i++){
					if(c[i] != '#'){
						if(c[i] == ' ' && (strcmp(cadconf," ") != 0)){
							j=0;
							quitar_escape(cadconf);
							trim(cadconf);
							//printf("\n	Cadenas de conf: %s",cadconf);
							if(k==0){
								if(strcmp(cadconf, "sincajon") == 0)
									return(-1);
								else if(strcmp(cadconf, "contado") == 0)
									par1 = 0; //Indica que se metio
								else
									break; //Se sale del for
								//printf("->nombre %s\n<-",cadconf);
							}else if(k==1){
								if( (par2 = buscar_arreglo(cadconf))  == -1){
									printf("\nNo encuentro nada %s",cadconf);
									break;
								}
								if(strcmp(cadconf,"paralelo") == 0)
									return (-1);
								//printf("->Paralelo (0) o Serial (1) %s\n<-",cadconf);
							}else if(k==2){
								strcpy(par3,cadconf);
								//printf("->Puerto Serial %s\n<-",cadconf);
								printf ("\n ############# Serial ########### \n puerto = %d \n\n",atoi(cadconf));
								return (atoi(cadconf));
							}else if(k==3){
								//impresiones = atoi(cadconf);
								printf("Numero de copias: %s\n<-",cadconf);
							}else if(k==4){
								/*trim(cadconf);
								if(strlen(cadconf) > 0){
									printf("\nVarias impresiones\n\n");
									impresiones = atoi(cadconf);
								}*/
								strcpy(par4,cadconf);
							}
							strcpy(cadconf," ");
							k++;
						}else{
							cadconf[j] = c[i];
							cadconf[j+1] = '\0';
							j++;
						}
					}else{
						break;
					}
				}
				m++;
				if(error==1){ //Si existe un error se sale del while
					break;
				}
			}
			fclose(fpt);
		}
		/*if(par1 != -1){
			switch(par2){
				case 0: //Serial
					strcpy(impresora,"cat ");
					strcat(impresora,nombrearchivo);
					strcat(impresora," > /dev/");
					strcat(impresora,"ttyS");
					strcat(impresora,par3);
				break;
				case 1: //Paralela
					strcpy(impresora,"cat ");
					strcat(impresora,nombrearchivo);
					strcat(impresora," > /dev/");
					strcat(impresora,"lp");
					strcat(impresora,par3);
				break;
				case 2: //cups
					strcpy(impresora,"lp.cups -d ");
					strcat(impresora,par3);
					if (strcmp (consulta,"default") == 0)
					{
						strcat(impresora," -h ");
					}
					else
					{
						strcat(impresora," -o raw -h ");
					}
					strcat(impresora,par4);
				    	strcat(impresora,"  ");
					strcat(impresora,nombrearchivo);
				break;
				case 3: //print file 32
					strcpy(impresora,"prfile32 ");
					strcat(impresora,nombrearchivo);
				break;
				default:
					error = 1;
				break;
			}
		}*/
		
	/*if(error != 1){
		printf("\nCadena de la Impresion: ->%s<--",impresora);
		printf("\n");
		system(impresora);
		return(0);
	}else{
		printf("\nNo se puede imprimir, verifica el archivo de configuracion");
		printf("\n");
		return(-1);
	}*/
}
