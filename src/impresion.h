/* Funcion que imprime Ticket ya sean de contado o credito
*
* SINTAXIS:
*                               imprimirticket(ID_DE_LA_VENTA , TIPO_TICKET , PAGO);
*  Donde TIPO_TICKET:
*            "credito"  ----------> Ticket para credito y en pago se envia 0 o lo que sea
*            "contado" ---------> Ticket para contado y en pago se envia lo que se pago
*	      "factura" ---------->  Impresion de la Factura y en pago se envia 0
*				0 Normal  - Primer valor es numero venta
*				1 CIERRE FACTURA - Credito primer valor es numero factura
*				2 CIERRE FACTURA - Contado
*	      "corte_caja" ------> Corte de Caja y en pago se envia 0, solo para la caja especifica
*	      "credito_abono" --> Lista de los deudores de credito y cuanto han abonado y se envia la fecha de hoy
*					   el formato es AAAAMMDD
*	      "corte_all_caja" --> Corte de Caja y en pago se envia 0, sirve para todas las cajas
*					Reporte detallado del corte de caja del dia con formato AAAAMMDD
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
#include<string.h>
#include <mysql/mysql.h>

#define MAXFIL 9999

MYSQL mysql;
MYSQL_RES *resultado, *resultado2;
MYSQL_ROW row, row2;
FILE *fpt, *fpt2; //Declaracion del archivo
char TicketImpresion[] = "impresion/impresiones-tmp.txt";
char TicketArriba[] = "impresion/ticket-arriba.txt";
char TicketAbajo[100] = "";
char TicketAbajoCredito[] = "impresion/ticket-abajo-credito.txt";
char TicketAbajoContado[] = "impresion/ticket-abajo-contado.txt";
char FacturaConfig[] = "impresion/factura.conf.txt";
char CobranzaConfig[] = "impresion/cobranza.conf.txt";
char ImpresoraConfig[] = "impresion/impresoras.conf.txt";
char c[1000]; //Aqui se guarda las cadenas a imprimir
int fac_nombre[99][4];
int impresora_nombre[99][4];

char Errores[200];

//Variables del ARRAY
int fila, nfilas, ncols;
char *matrix[MAXFIL];
char retVar[500]; //Cadena con la cantidad con letra

/*int conecta_bd()
{
	FILE *fconfiguracionbd;
	int i=0;
	int j=0;
	char tipo[50]="";
	char dato[50]="";
	char tmp;
	char ip[20]="";
	char usuario[20]="";
	char contrasena[20]="";
	char bd[30]="";

	if((fconfiguracionbd = fopen("configuracionbd.dat","r")))
	{
		while(!feof(fconfiguracionbd))
		{
			tmp = fgetc(fconfiguracionbd);
			if(tmp != ' ')
			{
				tipo[i] = tmp;
				i++;
			}
			else
			{
				while(tmp != '\n')
				{
					tmp = fgetc(fconfiguracionbd);
					dato[j]=tmp;
					j++;
				}
				dato[j-1] = '\0';
				j=0;
				if(strcmp(tipo,"ip") == 0)
					strcpy(ip, dato);
				if(strcmp(tipo,"usuario") == 0)
					strcpy(usuario, dato);
				if(strcmp(tipo,"contrasena") == 0)
					strcpy(contrasena, dato);
				if(strcmp(tipo,"bd") == 0)
					strcpy(bd, dato);
				strcpy(tipo,"");
				for(i=0;i<30;i++)
					tipo[i]='\0';
				i=0;
			}
		}
		fclose(fconfiguracionbd);

		mysql_init(&mysql);
		if(!(mysql_real_connect(&mysql, ip, usuario, contrasena, bd, 0, NULL, 0)))
			return -1;
		else
			return 1;
	}
	else
	{
		printf("No se pudo abrir el archivo....\n");
		return -1;
	}
}*/


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
	int grande; //Tamaño de la cadena
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

	unidad[0] = "CERO";
	unidad[1] = "UN";
	unidad[2] = "DOS";
	unidad[3] = "TRES";
	unidad[4] = "CUATRO";
	unidad[5] = "CINCO";
	unidad[6] = "SEIS";
	unidad[7] = "SIETE";
	unidad[8] = "OCHO";
	unidad[9] = "NUEVE";

	decena[1] = "DIEZ";
	decena[2] = "VEINTE";
	decena[3] = "TREINTA";
	decena[4] = "CUARENTA";
	decena[5] = "CINCUENTA";
	decena[6] = "SESENTA";
	decena[7] = "SETENTA";
	decena[8] = "OCHENTA";
	decena[9] = "NOVENTA";

	centena[1] = "CIENTO";
	centena[2] = "DOSCIENTOS";
	centena[3] = "TRESCIENTOS";
	centena[4] = "CUATROCIENTOS";
	centena[5] = "QUINIENTOS";
	centena[6] = "SEISCIENTOS";
	centena[7] = "SETECIENTOS";
	centena[8] = "OCHOCIENTOS";
	centena[9] = "NOVECIENTOS";

	millar[1] = "MIL";
	millar[2] = "DOS MIL";
	millar[3] = "TRES MIL";
	millar[4] = "CUATRO MIL";
	millar[5] = "CINCO MIL";
	millar[6] = "SEIS MIL";
	millar[7] = "SIETE MIL";
	millar[8] = "OCHO MIL";
	millar[9] = "NUEVE MIL";

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
				for(i=1;i<2;i++){
					num_tmp[i-1] = num[j];
					num_tmp[i] = '\0';
					j++;
				}
				num_num2 = atoi(num_tmp);
				j=3;
				for(i=2;i<3;i++){
					num_tmp[i-1] = num[j];
					num_tmp[i] = '\0';
					j++;
				}
				num_num3 = atoi(num_tmp);
				
				if(num_num == 1 && num_num2 > 0 && num_num3 > 0){
					sprintf(num_tmp, "%c", num[largo-3]); //Convierte a cadena
					num_num = atoi(num_tmp);
					strcpy(centt,centena[num_num]);
					strcat(centt," ");
				}


				//if (parseInt(num.substr( largo - 3, 1)) == 1 && (parseInt(num.substr(2,num.length))) == 0 )
				//	centt = "cien";

				sprintf(num_tmp, "%c", num[largo-3]); //Primera parte del IF
				num_num = atoi(num_tmp);

				if(num_num == 1 && num_num2 == 0 && num_num3 == 0)
				{
					strcpy(centt,"CIEN");
				}
				else
				{
					strcpy(ciecentt,centena[1]);
					strcat (ciecentt," ");
				}

			}

		}


		if (largo > 3 ) {

			sprintf(num_tmp, "%c", num[largo-4]); //Convierte a cadena
			num_num = atoi(num_tmp);
			if(num_num > 1 ){
				strcpy(millt,unidad[num_num]);
				strcat(millt," MIL ");

			} else {
				if(num_num == 1 ){
					strcpy(millt,"UN MIL ");
				}
				if(num_num == 0 ){
					strcpy(millt," MIL ");
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
				for(i=2;i<3;i++){
					num_tmp[i-1] = num[j];
					num_tmp[i] = '\0';
					j++;
				}
				num_num3 = atoi(num_tmp);
				
				if(num_num == 1 && num_num2 > 0){
					strcpy(ciecentt,centena[num_num]);
					strcat(ciecentt," ");
				}
				if(num_num == 1 && num_num2 == 0 && num_num3 == 0){
					strcpy(ciecentt,"CIEN ");
				}
				else
				{
					strcpy(ciecentt,centena[1]);
					strcat (ciecentt," ");
				}
			}
		}
		
		if (strcmp(decet,"DIEZ Y ") == 0) {
			sprintf(num_tmp, "%c", num[largo-1]); //Convierte a cadena
			num_num = atoi(num_tmp);
		
			switch ( num_num ){
			case 1:
				strcpy(veintes,"ONCE");
				break;
			case 2:
				strcpy(veintes,"DOCE");
				break;
			case 3:
				strcpy(veintes,"TRECE");
				break;
			case 4:
				strcpy(veintes,"CATORCE");
				break;
			case 5:
				strcpy(veintes,"QUINCE");
				break;
			case 6:
				strcpy(veintes,"DIECISEIS");
				break;
			case 7:
				strcpy(veintes,"DIECISIETE");
				break;
			case 8:
				strcpy(veintes,"DIECIOCHO");
				break;
			case 9:
				strcpy(veintes,"DIECINUEVE");
				break;
			case 0:
				strcpy(veintes,"DIEZ");

				break;
   			}
			strcpy(decet,veintes);
			strcpy(unidt,"");
		}


	if (strcmp(decet,"VEINTE Y ") == 0) {
		sprintf(num_tmp, "%c", num[largo-1]); //Convierte a cadena
		num_num = atoi(num_tmp);

		switch ( num_num ){
		case 1:
			strcpy(veintes,"VEINTIUN");
			break;
		case 2:
			strcpy(veintes,"VEINTIDOS");
			break;
		case 3:
			strcpy(veintes,"VEINTITRES");
			break;
		case 4:
			strcpy(veintes,"VEINTICUATRO");
			break;
		case 5:
			strcpy(veintes,"VEINTICINCO");
			break;
		case 6:
			strcpy(veintes,"VEINTISEIS");
			break;
		case 7:
			strcpy(veintes,"VEINTISIETE");
			break;
		case 8:
			strcpy(veintes,"VEINTIOCHO");
			break;
		case 9:
			strcpy(veintes,"VEINTINUEVE");
			break;
		case 0:
			strcpy(veintes,"VEINTE");
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
				strcpy(veintes,"ONCE MIL");
				break;
			case 2:
				strcpy(veintes,"DOCE MIL");
				break;
			case 3:
				strcpy(veintes,"TRECE MIL");
				break;
			case 4:
				strcpy(veintes,"CATORCE MIL");
				break;
			case 5:
				strcpy(veintes,"QUINCE MIL");
				break;
			case 6:
				strcpy(veintes,"DIECISEIS MIL");
				break;
			case 7:
				strcpy(veintes,"DIECISIETE MIL");
				break;
			case 8:
				strcpy(veintes,"DIECIOCHO MIL");
				break;
			case 9:
				strcpy(veintes,"DIECINUEVE MIL");
				break;
			case 0:
				strcpy(veintes,"DIEZ MIL");
				break;
		}
		strcpy(decmillt,veintes);
		strcpy(millt,"");
		}

	if (strcmp(decet,"VEINTE Y ") == 0) {
		sprintf(num_tmp, "%c", num[largo-4]); //Convierte a cadena
		num_num = atoi(num_tmp);

		switch ( num_num ){
		case 1:
			strcpy(veintes,"VEINTIUN MIL");
			break;
		case 2:
			strcpy(veintes,"VEINTIDOS MIL");
			break;
		case 3:
			strcpy(veintes,"VEINTITRES MIL");
			break;
		case 4:
			strcpy(veintes,"VEINTICUATRO MIL");
			break;
		case 5:
			strcpy(veintes,"VEINTICINCO MIL");
			break;
		case 6:
			strcpy(veintes,"VEINTISEIS MIL");
			break;
		case 7:
			strcpy(veintes,"VEINTISIETE MIL");
			break;
		case 8:
			strcpy(veintes,"VEINTIOCHO MIL");
			break;
		case 9:
			strcpy(veintes,"VEINTINUEVE MIL");
			break;
		case 0:
			strcpy(veintes,"VEINTE");
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

				if (strcmp(num_tmp,"UN") == 0) {
					strcpy(textt,ciecentt); //Se concatenan todas las variables
					strcat(textt,decmillt);
					strcat(textt,millt);
					strcat(textt,centt);
					strcat(textt,decet);
					strcat(textt,unidt);
					strcat(textt,"MILLON");
				} else {
					strcpy(textt,ciecentt); //Se concatenan todas las variables
					strcat(textt,decmillt);
					strcat(textt,millt);
					strcat(textt,centt);
					strcat(textt,decet);
					strcat(textt,unidt);
					strcat(textt,"MILLONES");
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
		strcpy(pesos," PESO ");
	} else {
		strcpy(pesos," PESOS ");
	}


	num_num = atoi(respnu);
		strcpy(retVar,"");
		//strcpy(retVar,textt);
		//strcat(retVar,centavos);
		//strcat(retVar,"/100 M.N. ");

	if (num_num >= 1000000  && (num_num % 1000000)==0){
		strcpy(retVar,textt);
		strcat(retVar," DE PESOS ");
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
        char cTMP[255];
        strcpy(cTMP,""); //Inicializacion
        i=0;
        j=1;
        while(i <= strlen(c)){

               cTMP[i] = c[i];
                if(j == nX){
                        printf("\n");
                        fputs("\n",fpt);
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

int imprimirticket(char *id_venta_char, char tipo[20], double pago_num){
	int id_venta_num;
	
        int nX; //TamaÃ±o del papel de largo
/*        int numVenta; //Numero de venta*/
	int i, j, k,m;
	double peso_num=0;
	double cambio_num=0;
	double cambio_num_total=0;
	int num_articulos_int=0;
	double piezas_int=0;
	char temp[7] = "       ";
	char temp2[200] = "                                                                                                                                                                                                        ";
	char temp3[200] = "                                                                                                                                                                                                        ";

        char sql[1000], monto[10];
	char id_venta[20], cambio[10], pago[10], num_articulos[300], peso[300], pieza[300], totales[20], total_cierre_listado[40];
	char fechaTMP[10]; //Variable Temporal para separar las fechas

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
	char cad_temporal_fechas[1];

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
	int arr_direccion = 0;
	int arr_ciudad = 0;
	int arr_rfc = 0;
	int arr_dia = 0;
	int arr_mes = 0;
	int arr_ano = 0;
	int arr_cantidad_letra = 0;
	int arr_observaciones = 0 ;
	int arr_subtotal = 0;
	int arr_iva = 0;
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
	
	char domicilio_envio[100]="";


//	int tam_seek;

	gchar *listatipos[8]; //Se guarda la consulta SQL
//MINIPRINTER EPSON
	int num_articulos_venta=0;

	static char salto[4];
	static char defecto[4],cancela[4];
	static char subraya_s[4],negrita_grande[4],negrita_subraya[4],negrita_subraya1[4],negrita[4],negrita1[4],subraya_s1[4];
	static char alinea_d[4],alinea_i[4],alinea_c[4];
	static char tamano16[4],tamano1[4];
	static char resetea[3],corta_papel[3];
//IMPRESORA LX300 ESC/P2
	static char psalto[2], pcarro[2], psig_pag[2], psalto_l[3];
	static char pt_pagina[4], pmargen_a[4];
	static char pmargen_i[4], pmargen_d[4];
	static char pnegrita[3], pc_negrita[3];
	static char ptabulador[2];
	static char pcpi15[3],pcpi12[3],pcpi10[3];
	static char pcondensed[2], pc_condensed[2];
	static char pespaciado[4];
	
	id_venta_num = atoi (id_venta_char);
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
	pespaciado[0]=27;
	pespaciado[1]='A';
	pespaciado[2]=18;
	pespaciado[3]='\0';

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
	pmargen_i[2]=5;
	pmargen_i[3]='\0';
	pmargen_d[0]=27;
	pmargen_d[1]='Q';
	pmargen_d[2]=5;
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

	salto[0]=27;
	salto[1]=100;
	salto[2]=1;
	salto[3]='\0';

	corta_papel[0]=27;
	corta_papel[1]=109;
	corta_papel[2]='\0';

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
	negrita_subraya1[0]=27;
	negrita_subraya1[1]=33;
	negrita_subraya1[2]=137;
	negrita_subraya1[3]='\0';
	subraya_s1[0]=27;
	subraya_s1[1]=33;
	subraya_s1[2]=129;
	subraya_s1[3]='\0';

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

	nX = 200; //TamaÃ±o del ticket de largo


	// SE ACOMODA EL PAGO CON EL FORMATO DE MILES
	if(conecta_bd() == -1)
		printf("Error al conectarme a la base de datos: %s\n", mysql_error(&mysql));
	else
	{
		sprintf(sql, "SELECT FORMAT(%f, 2)", pago_num);
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
			printf("Error: %s\n", mysql_error(&mysql));
	}
	mysql_close(&mysql);

	sprintf(id_venta, "%d", id_venta_num);
	//sprintf(pago, "%.2f", pago_num);



/*CONFIGURACION DE LA IMPRESORA
*/
		fpt2 = fopen(ImpresoraConfig,"r");
		if(fpt2 == NULL){
			printf("\nERROR no se puede abrir el archivo de configuracion de las impresoras");
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
						if(c[i] == ' '){
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

								//printf("->nombre %s\n<-",cadconf);
							}else if(k==1){
								if(strcmp(cadconf,"serial") == 0) impresora_nombre[m][0] = 1;  //Serial
								else if(strcmp(cadconf,"paralelo") == 0) impresora_nombre[m][0] = 0; //Paralela
								else if(strcmp(cadconf,"remoto") == 0) impresora_nombre[m][0] = 2; //Red
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
	//IMPRESORA CREDITO
	strcpy(impresora_credito,"/dev/");
	if((impresora_nombre[arr_impresora_credito][0]) == 0)
		strcat(impresora_credito,"lp");
	else if((impresora_nombre[arr_impresora_credito][0]) == 1)
		strcat(impresora_credito,"ttyS");
	else
		strcpy(impresora_credito,"-d miniprinter -o raw -h 192.168.0.");
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
		strcpy(impresora_contado,"-d miniprinter -o raw -h 192.168.0.");
	sprintf(cad_temporal,"%d",impresora_nombre[arr_impresora_contado][1]);
	strcat(impresora_contado,cad_temporal);

	//IMPRESORA FACTURA
	strcpy(impresora_factura,"/dev/");
	if((impresora_nombre[arr_impresora_factura][0]) == 0)
		strcat(impresora_factura,"lp");
	else if((impresora_nombre[arr_impresora_factura][0]) == 1)
		strcat(impresora_factura,"ttyS");
	else
		strcpy(impresora_factura,"-d miniprinter -o raw -h 192.168.0.");
	sprintf(cad_temporal,"%d",impresora_nombre[arr_impresora_factura][1]);
	strcat(impresora_factura,cad_temporal);

	//IMPRESORA CORTE CAJA
	strcpy(impresora_corte_caja,"/dev/");
	if((impresora_nombre[arr_impresora_corte_caja][0]) == 0)
		strcat(impresora_corte_caja,"lp");
	else if((impresora_nombre[arr_impresora_corte_caja][0]) == 1)
		strcat(impresora_corte_caja,"ttyS");
	else
		strcpy(impresora_corte_caja,"-d miniprinter -o raw -h 192.168.0.");
	sprintf(cad_temporal,"%d",impresora_nombre[arr_impresora_corte_caja][1]);
	strcat(impresora_corte_caja,cad_temporal);

	//IMPRESORA CREDITO ABONO
	strcpy(impresora_credito_abono,"/dev/");
	if((impresora_nombre[arr_impresora_credito_abono][0]) == 0)
		strcat(impresora_credito_abono,"lp");
	else if((impresora_nombre[arr_impresora_credito_abono][0]) == 1)
		strcat(impresora_credito_abono,"ttyS");
	else
		strcpy(impresora_credito_abono,"-d miniprinter -o raw -h 192.168.0.");
	sprintf(cad_temporal,"%d",impresora_nombre[arr_impresora_credito_abono][1]);
	strcat(impresora_credito_abono,cad_temporal);

	//IMPRESORA CORTE ALL CAJA
	strcpy(impresora_corte_all_caja,"/dev/");
	if((impresora_nombre[arr_impresora_corte_all_caja][0]) == 0)
		strcat(impresora_corte_all_caja,"lp");
	else if((impresora_nombre[arr_impresora_corte_all_caja][0]) == 1)
		strcat(impresora_corte_all_caja,"ttyS");
	else
		strcpy(impresora_corte_all_caja,"-d miniprinter -o raw -h 192.168.0.");
	sprintf(cad_temporal,"%d",impresora_nombre[arr_impresora_corte_all_caja][1]);
	strcat(impresora_corte_all_caja,cad_temporal);

	//IMPRESORA pedidos_paso1 Para el carnicero

	strcpy(impresora_pedidos_paso1,"/dev/");
	if((impresora_nombre[arr_impresora_pedidos_paso1][0]) == 0)
		strcat(impresora_pedidos_paso1,"lp");
	else if((impresora_nombre[arr_impresora_pedidos_paso1][0]) == 1)
		strcat(impresora_pedidos_paso1,"ttyS");
	else
		strcpy(impresora_pedidos_paso1,"-d miniprinter -o raw -h 192.168.0.");
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
			if(conecta_bd() == -1){
				printf("\nNo me puedo conectar a la base de datos =(\n");
				return (1);
			}else{

				//Con ID FACTURA


//SELECT Venta_Factura.id_factura, Venta_Factura.fecha, Venta_Factura.observacion, Cliente.nombre, Cliente.domicilio, Cliente.ciudad_estado, Cliente.rfc, Venta_Factura_Relacion.num_factura FROM Venta_Factura, Venta, Cliente, Venta_Factura_Relacion WHERE Venta.id_venta = Venta_Factura_Relacion.id_venta AND Venta_Factura_Relacion.id_factura = Venta_Factura.id_factura AND Venta.id_cliente = Cliente.id_cliente AND Venta.id_venta = 1
				//strcpy(sql,"SELECT Venta_Factura.id_factura, Venta_Factura.fecha, Venta_Factura.observacion, Cliente.nombre, Cliente.domicilio, Cliente.ciudad_estado, Cliente.rfc, Venta_Factura_Relacion.num_factura, Venta_Factura_Relacion.id_venta, Cliente.id_cliente FROM Venta_Factura, Venta, Cliente, Venta_Factura_Relacion WHERE Venta.id_venta = Venta_Factura_Relacion.id_venta AND Venta_Factura_Relacion.id_factura = Venta_Factura.id_factura AND Venta.id_cliente = Cliente.id_cliente AND Venta.id_venta = ");
				strcpy(sql,"SELECT Venta_Factura.id_factura, Venta_Factura.fecha, Venta_Factura.observacion, Cliente.nombre, Cliente.domicilio, Cliente.ciudad_estado, Cliente.rfc, Venta_Factura_Relacion.num_factura, Venta_Factura_Relacion.id_venta, Cliente.id_cliente FROM Venta_Factura, Venta, Cliente, Venta_Factura_Relacion, Venta_Articulo WHERE Venta.id_venta = Venta_Factura_Relacion.id_venta AND Venta_Factura_Relacion.id_factura = Venta_Factura.id_factura AND Venta.id_cliente = Cliente.id_cliente AND Venta_Articulo.id_venta = Venta.id_venta AND (Venta_Articulo.id_venta = ");
				strcat(sql, id_venta_char);
				strcat(sql, ")");

				printf("El id de la venta: %s\n", id_venta_char);
				printf("El SQL: %s\n", sql);
				if((err = mysql_query(&mysql, sql)) != 0){
					printf("Error al consultar los tipos de documentos: %s\n", mysql_error(&mysql));
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
										return (1);
									}else{
										inicializar(); //Inicializa MATRIX
										for(i=0; i < 11;i++) //Guarda todo el arreglo en listapos
											listatipos[i] =  row[i];

										strcpy(id_factura_interno,listatipos[0]);

										//Separamos el año
										fechaTMP[0] = listatipos[1][0];
										fechaTMP[1] = listatipos[1][1];
										fechaTMP[2] = listatipos[1][2];
										fechaTMP[3] = listatipos[1][3];
										fechaTMP[4] = '\0';
										llenarMATRIX(arr_ano,fechaTMP); //DIA
										//Separamos el mes
										fechaTMP[0] = listatipos[1][5];
										fechaTMP[1] = listatipos[1][6];
										fechaTMP[2] = '\0';
										llenarMATRIX(arr_mes,fechaTMP); //MES
										//Separamos el dia
										fechaTMP[0] = listatipos[1][8];
										fechaTMP[1] = listatipos[1][9];
										fechaTMP[2] = '\0';
										llenarMATRIX(arr_dia,fechaTMP); //DIA

										llenarMATRIX(arr_observaciones, listatipos[2]); //Observaciones

										llenarMATRIX(arr_num_cliente, listatipos[9]); //ID cliente
										llenarMATRIX(arr_nombre, listatipos[3]); //Nombre cliente
										llenarMATRIX(arr_direccion, listatipos[4]); //Direccion cliente
										llenarMATRIX(arr_ciudad, listatipos[5]); //Ciudad y estado
										llenarMATRIX(arr_rfc, listatipos[6]); //RFC

										llenarMATRIX(arr_num_factura, listatipos[7]); //Numero de Factura
										llenarMATRIX(arr_num_venta, listatipos[8]); //Numero de la Venta

										//Con ID FACTURA



//							strcpy(sql,"SELECT /*FORMAT(*/Venta_Articulo.cantidad/*,3)*/, Articulo.nombre, /*FORMAT(*/Venta_Articulo.precio/*,2)*/ AS precio_IVA, /*FORMAT(*/Venta_Articulo.monto/*,2)*/ AS monto_IVA, /*FORMAT(*/Venta_Articulo.precio/*,2)*/ AS precio, /*FORMAT(*/Venta_Articulo.monto/*,2)*/ AS monto FROM Venta_Articulo, Articulo WHERE Venta_Articulo.id_articulo = Articulo.id_articulo AND Venta_Articulo.id_venta = ");

										//sprintf(sql, "SELECT FORMAT(Venta_Articulo.cantidad,3), Articulo.nombre, FORMAT(Venta_Articulo.precio,2) AS precio_IVA, FORMAT(Venta_Articulo.monto,2) AS monto_IVA, FORMAT(Venta_Articulo.precio,2) AS precio, FORMAT(Venta_Articulo.monto,2) AS monto FROM Venta_Articulo, Articulo WHERE Venta_Articulo.id_articulo = Articulo.id_articulo AND Venta_Articulo.id_venta = %s AND Venta_Articulo.id_factura = %s ORDER BY id_venta_articulo", id_venta, id_factura_interno);
										sprintf (sql,  "SELECT FORMAT(SUM(Venta_Articulo.cantidad),3), Articulo.nombre, FORMAT(Venta_Articulo.precio,2) AS precio_IVA, FORMAT(SUM(Venta_Articulo.monto),2) AS monto_IVA, FORMAT(Venta_Articulo.precio,2) AS precio, FORMAT(SUM(Venta_Articulo.monto),2) AS monto FROM Venta_Articulo INNER JOIN  Articulo ON Venta_Articulo.id_articulo = Articulo.id_articulo WHERE (Venta_Articulo.id_venta = %s) AND Venta_Articulo.id_factura = %s GROUP BY Venta_Articulo.id_articulo ORDER BY id_venta_articulo", id_venta_char, id_factura_interno);
										printf("%s\n",sql);
										err = mysql_query(&mysql, sql);
										if(err != 0){
											printf("Error al consultar la venta de factura: %s\n", mysql_error(&mysql));
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
														printf("\nERROR AL GUARDAR EN EL ARCHIVO, SE SUPERO EL TAMA� DE LA CADENA PERMITIDA");
														printf("\n");
														//return (1);
														break;
													}
												}
												sprintf(sql, "SELECT FORMAT(SUM(Venta_Articulo.monto),2) as total, SUM(Venta_Articulo.monto) as total_letra FROM Venta_Articulo WHERE (Venta_Articulo.id_venta = %s) AND id_factura = %s ORDER BY id_venta_articulo", id_venta_char, id_factura_interno);
												//sprintf (sql,  "SELECT FORMAT(SUM(Venta_Articulo.cantidad),3), Articulo.nombre, FORMAT(Venta_Articulo.precio,2) AS precio_IVA, FORMAT(SUM(Venta_Articulo.monto),2) AS monto_IVA, FORMAT(Venta_Articulo.precio,2) AS precio, FORMAT(SUM(Venta_Articulo.monto),2) AS monto FROM Venta_Articulo INNER JOIN  Articulo ON Venta_Articulo.id_articulo = Articulo.id_articulo WHERE (Venta_Articulo.id_venta = %s) AND Venta_Articulo.id_factura = %s GROUP BY Venta_Articulo.id_articulo ORDER BY id_venta_articulo", id_venta_char, id_factura_interno);
												printf("%s\n",sql);
												err = mysql_query(&mysql, sql);
												if(err != 0){
													printf("Error al consultar la venta de factura: %s\n", mysql_error(&mysql));
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
												return (1);
											}
										}



									escribirsalida("");
									escribirsalida("archivo");
									c[0] = 27;
									c[1] = 109;
									c[2] = '\0';
									fputs(c,fpt);
									fclose(fpt);
									//system("lpr -#1 impresion/impresiones-tmp.txt");
									//FACTURA
									//system("lpr -Pminiprinter impresion/impresiones-tmp.txt");
									strcpy(cad_temporal,"cat ");
									strcat(cad_temporal,TicketImpresion);
									strcat(cad_temporal,"> ");
									strcat(cad_temporal,impresora_factura);
									system(cad_temporal);
return (1);

								}

								}
							}else{
								printf("No se pudo la factura....\n");
								return(1);

							}
						}else{
							return (1);
						}


				mysql_close(&mysql); //Cierra conexion SQL

				}

			}





	}else if((strcmp(tipo, "credito") == 0) || (strcmp(tipo, "contado") == 0)){ //Es credito o contado
		if(strcmp(tipo, "credito") == 0){
			strcpy(TicketAbajo,TicketAbajoCredito);
		}
		else if (strcmp(tipo, "contado") == 0){ //es contado
			strcpy(TicketAbajo,TicketAbajoContado);
		}


		fpt = fopen(TicketImpresion,"w");
		if(fpt == NULL){
			printf("\nERROR no se puede abrir el archivo a imprimir");
			return (1);
		}else
		{
		//Abre el encabezado de archivo
		fpt2 = fopen(TicketArriba,"r");
		if(fpt2 == NULL){
			printf("\nERROR no se puede abrir el archivo del encabezado");
			return (1);
		}else{
			imprimir(resetea,nX);
			imprimir(pt_pagina,nX);
			imprimir(pmargen_a,nX);
			imprimir(pmargen_i,nX);
			imprimir(pmargen_d,nX);
			imprimir(pcpi10,nX);
			imprimir(pcondensed,nX);
			while(fgets(c, 255, fpt2) != NULL){
				imprimir(c,nX);
			}
			fclose(fpt2);
			imprimir(psalto_l,nX);
			imprimir(ptabulador,nX);
			imprimir(ptabulador,nX);
			imprimir(ptabulador,nX);
			imprimir(ptabulador,nX);
			imprimir(ptabulador,nX);
			imprimir(ptabulador,nX);
			imprimir(pnegrita,nX);
			imprimir("MATERIALES MONTEJANO",nX);
			imprimir(psalto_l,nX);
			imprimir(psalto_l,nX);
			imprimir(pc_negrita,nX);

			//Inserta la informacion del ticket
			if(conecta_bd() == -1){
				g_print("\nNo me puedo conectar a la base de datos\n");
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
							sprintf(sql, "SELECT Cliente_Envio.domicilio FROM Cliente_Envio INNER JOIN Cliente ON Cliente.id_cliente=Cliente_Envio.id_cliente INNER JOIN Venta ON Venta.id_cliente=Cliente.id_cliente WHERE Venta.id_venta = %s", id_venta);
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
												sprintf(sql,"SELECT Venta.id_venta, Venta.fecha, Venta.hora, FORMAT(Venta.monto,2), LEFT(CONCAT( Usuario.nombre,  ' ', Usuario.apellido ),35)  AS cajero, LEFT(Cliente.nombre,80) AS cliente, Cliente_Envio.domicilio, Cliente_Envio.colonia, Cliente_Envio.entre_calles, Cliente.id_cliente, Cliente_Envio.ciudad_estado, FORMAT(%f - Venta.monto,2) FROM Venta, Usuario, Cliente, Cliente_Envio, Empleado WHERE Usuario.id_usuario = Venta.id_usuario AND Cliente.id_cliente = Venta.id_cliente AND Cliente.id_cliente = Cliente_Envio.id_cliente AND Venta.id_venta = %s GROUP BY Venta.id_venta", pago_num, id_venta);
											else
												sprintf(sql,"SELECT Venta.id_venta, Venta.fecha, Venta.hora, FORMAT(Venta.monto,2), LEFT(CONCAT( Usuario.nombre,  ' ', Usuario.apellido ),35)  AS cajero, LEFT(Cliente.nombre,80) AS cliente, Cliente.domicilio, Cliente.colonia, Cliente.entre_calles, Cliente.id_cliente, Cliente.ciudad_estado, FORMAT(%f - Venta.monto,2) FROM Venta, Usuario, Cliente, Empleado WHERE Usuario.id_usuario = Venta.id_usuario AND Cliente.id_cliente = Venta.id_cliente AND Venta.id_venta = %s GROUP BY Venta.id_venta", pago_num, id_venta);

										}
									}
									else
									{
										sprintf(sql,"SELECT Venta.id_venta, Venta.fecha, Venta.hora, FORMAT(Venta.monto,2), LEFT(CONCAT( Usuario.nombre,  ' ', Usuario.apellido ),35)  AS cajero, LEFT(Cliente.nombre,80) AS cliente, Cliente.domicilio, Cliente.colonia, Cliente.entre_calles, Cliente.id_cliente, Cliente.ciudad_estado, FORMAT(%f - Venta.monto,2) FROM Venta, Usuario, Cliente, Empleado WHERE Usuario.id_usuario = Venta.id_usuario AND Cliente.id_cliente = Venta.id_cliente AND Venta.id_venta = %s GROUP BY Venta.id_venta", pago_num, id_venta);
									}
								}
							}
							else
								printf("Error: %s", mysql_error(&mysql));
						}
						else
							sprintf(sql,"SELECT Venta.id_venta, Venta.fecha, Venta.hora, FORMAT(Venta.monto,2), LEFT(CONCAT( Usuario.nombre,  ' ', Usuario.apellido ),35)  AS cajero, LEFT(Cliente.nombre,80) AS cliente, Cliente.domicilio, Cliente.colonia, Cliente.entre_calles, Cliente.id_cliente, Cliente.ciudad_estado, FORMAT(%f - Venta.monto,2) FROM Venta, Usuario, Cliente, Empleado WHERE Usuario.id_usuario = Venta.id_usuario AND Cliente.id_cliente = Venta.id_cliente AND Venta.id_venta = %s GROUP BY Venta.id_venta", pago_num, id_venta);

					}
				}

				printf("%s\n", sql);
				err = mysql_query(&mysql, sql);

				if(err != 0){
					printf("Error al consultar los tipos de documentos: %s\n", mysql_error(&mysql));
					return (1);
				}else{
					resultado = mysql_store_result(&mysql);
					if(resultado){
						if((row = mysql_fetch_row(resultado))){
							if (mysql_num_rows(resultado) > 0){
								for(i=0; i < 12;i++) //Guarda todo el arreglo en listapos
									listatipos[i] =  row[i];
								sprintf(monto, "%s", listatipos[3]);
								sprintf(cambio, "%s", listatipos[11]);

								sprintf(c, "Fecha: %c%c/%c%c/%c%c%c%c\tHora: %s\t\t",listatipos[1][8],listatipos[1][9],listatipos[1][5],listatipos[1][6],listatipos[1][0],listatipos[1][1],listatipos[1][2],listatipos[1][3],listatipos[2]);
								imprimir(c,nX);
								imprimir(pnegrita,nX);
								sprintf(c, "Folio: %s", listatipos[0]);
								imprimir(c,nX);
								imprimir(pc_negrita,nX);
								imprimir(ptabulador,nX);
								sprintf(c, "Atendio: %s", listatipos[4]);
								imprimir(c,nX);
								imprimir(psalto_l,nX);

								sprintf(c, "Cliente: %s",listatipos[9]);
								imprimir(c,nX);
								imprimir(ptabulador,nX);
								sprintf(c, "%s", listatipos[5]);
								imprimir(c,nX);
								imprimir(psalto_l,nX);
								if(strlen(listatipos[6]) != 0 ){ //Domicilio
									sprintf(c, "Domicilio: %s", listatipos[6]);
									imprimir(c,nX);
									imprimir(ptabulador, nX);
								}
								if(strlen(listatipos[7]) != 0 ){ //Colonia
									sprintf(c, "Colonia: %s", listatipos[7]);
									imprimir(c,nX);
									imprimir(ptabulador, nX);
								}
								if(strlen(listatipos[10]) != 0 ){ //Ciudad Estado
									sprintf(c, "Ciudad: %s", listatipos[10]);
									imprimir(c,nX);
									imprimir(psalto_l,nX);
								}
								if(strlen(listatipos[8]) != 0 ){ //Entre calles
									sprintf(c, "Referencia: %s", listatipos[8]);
									imprimir(c,nX);
									imprimir(psalto_l,nX);
								}
							}else{
								return (1);
							}
						}else{
							return (1);
						}
					}else{
						return (1);
					}
				}
				imprimir(psalto_l,nX);
				sprintf(sql,"SELECT LEFT(Articulo.nombre,39), FORMAT(Venta_Articulo.cantidad,2), FORMAT((Venta_Articulo.precio*1.15),2), FORMAT(Venta_Articulo.monto,2), Articulo.tipo, Venta_Articulo.cantidad FROM Venta_Articulo, Articulo WHERE Articulo.id_articulo = Venta_Articulo.id_articulo AND Venta_Articulo.id_venta = %s", id_venta);
				err = mysql_query(&mysql, sql);
				if(err != 0){
					printf("Error al consultar los tipos de documentos: %s\n", mysql_error(&mysql));
					return (1);
				}else{
					resultado = mysql_store_result(&mysql);
					if(resultado){
							num_articulos_venta = 10;
							if (mysql_num_rows(resultado) > 0){

								sprintf(c, "Articulo\t\t\t\t\t\t\t     Cantidad\t      Precio\t\t Monto");
								imprimir(c,nX);
								imprimir(psalto_l,nX);
								sprintf(c, "---------------------------------------------------------------------------------------------------------------");
								imprimir(c,nX);
								imprimir(psalto_l,nX);
								peso_num = 0; //Se inicializa el peso o cantidad de cada articulo
								while((row = mysql_fetch_row(resultado))){
									strcpy(c,"");

									if(strcmp(row[4], "peso")==0)
									{
										num_articulos_int = num_articulos_int + 1;
										strcpy(num_tmp," kg");
										peso_num = peso_num + atof(row[5]);
									}
									else
									{
										piezas_int = piezas_int + atof(row[5]);
										num_articulos_int = num_articulos_int + 1;// + atof(row[5]);
										strcpy(num_tmp,"   ");
									}
									//Nombre Producto
									sprintf(c, "%s", row[0]);
									strncat(c, temp3, 60-strlen(row[0]));
									c[60]='\0';
									imprimir(c,nX);

									//Cantidad
									strncpy(c,temp3,15-strlen(row[1]));
									c[15-strlen(row[1])]='\0';
									imprimir(c,nX);
									sprintf(c, "%s%s", row[1],num_tmp);
									imprimir(c,nX);
									//strncpy(c, temp3, 10-strlen(row[1]));
									//c[10-strlen(row[1])]='\0';
									//imprimir(c,nX);

									//Precio
									strncpy(c, temp3, 14-strlen(row[2]));
									c[14-strlen(row[2])]='\0';
									imprimir(c,nX);
									sprintf(c,"%s",row[2]);
									imprimir(c,nX);
									/*strncpy(c, temp3, 10-strlen(row[2]));
									c[10-strlen(row[2])]='\0';
									imprimir(c,nX);*/

									//Monto
									strncpy(c, temp3, 20-strlen(row[3]));
									c[20-strlen(row[3])]='\0';
									imprimir(c,nX);
									sprintf(c,"%s",row[3]);
									imprimir(c,nX);

									num_articulos_venta--;
									imprimir(psalto_l,nX);
								}
							}else{
								return (1);
							}
					}else{
						return (1);
					}
				}
				for(i=0;i<12;i++)
					imprimir(ptabulador,nX);
				imprimir("-----------------",nX);
				imprimir(psalto_l,nX);
				strcpy(temp2,"");
				strcpy(c,"");

				sprintf(c, "TOTAL :$ %s", monto);
				for(i=0;i<12;i++)
					imprimir(ptabulador,nX);
				imprimir(pnegrita,nX);
				imprimir(c,nX);
				imprimir(pc_negrita,nX);
				imprimir(psalto_l,nX);

				for(i=0;i<=num_articulos_venta;i++)
					imprimir(psalto_l,nX);

				if(strcmp(tipo, "contado") == 0){
					sprintf(c, "\tEFECTIVO : $ %s", pago);
					imprimir(c,nX);
					imprimir(psalto_l,nX);

					//Calcula el cambio
					sprintf(c, "\tSU CAMBIO :    $ %s", cambio);
					imprimir(c,nX);
				}

				imprimir(ptabulador,nX);
				imprimir(ptabulador,nX);
				imprimir(ptabulador,nX);
				imprimir(ptabulador,nX);
				
				//Se imprime el total de articulos
				strcpy(temp2,"");
				strcpy(c,"");
				strcpy(temp2,"Articulos: ");
				strncat(c,temp3,18-strlen(temp2));
				strcat(c, temp2);
				sprintf(num_articulos, "%d", num_articulos_int);

				strncat(c,temp3,4-strlen(num_articulos));
				strcat(c,num_articulos );

				imprimir(c,nX);
				imprimir(ptabulador,nX);
				
				
				//Se imprime el total de los kilos
				strcpy(temp2,"");
				strcpy(c,"");
				strcpy(temp2,"Peso: ");
				strncat(c,temp3,18-strlen(temp2));
				strcat(c, temp2);

				sprintf(peso, "%.2f kg", peso_num);
				strncat(c,temp3,11-strlen(peso));
				strcat(c,peso );
				imprimir(c,nX);
				imprimir(ptabulador,nX);
				
				
				//Se imprime el total de piezas
				strcpy(temp2,"");
				strcpy(c,"");
				strcpy(temp2,"Piezas: ");
				strncat(c,temp3,18-strlen(temp2));
				strcat(c, temp2);
				
				sprintf(pieza, "%.2f", piezas_int);
				strncat(c,temp3,11-strlen(pieza));
				strcat(c,pieza);
				imprimir(c,nX);
				
				imprimir(psalto_l,nX);
				
				mysql_close(&mysql); //Cierra conexion SQL

				strcpy(c,"");
				imprimir(psalto_l,nX);
			}
			//Fin de la insercion del ticket
			//Abre el pie de pagina

			imprimir("maticaLX",nX);
			imprimir(ptabulador,nX);
			imprimir(ptabulador,nX);
			imprimir("software libre",nX);
			imprimir(psig_pag,nX);
			fpt2 = NULL;
			fpt2 = fopen(TicketAbajo,"r");
			if(fpt2 == NULL){
				printf("\nERROR no se puede abrir el archivo del pie de pagina");
				return (1);
			}else{
				fseek(fpt,0,SEEK_END);
				while(fgets(c, 40, fpt2) != NULL)
				{
					if(strcmp(tipo,"contado") == 0)
					{
						//imprimir(negrita,nX);
						//imprimir(alinea_c,nX);
					}
					else
						imprimir(c,nX); //Funcion para imprimir y separa las letras
				}
				fclose(fpt2);
				if(strcmp(tipo,"credito") == 0)
					imprimir(psig_pag,nX);
			}
			//printf("\n\n<------------------>\nSe guarda en el archivo\n");
			imprimir(resetea,nX);

		}

		fclose(fpt);
		printf("\n\n\nIMPRIMIENDO....\n");
		//TICKET
		if(strcmp(tipo, "credito") == 0)
		{
			if((impresora_nombre[arr_impresora_credito][0]) == 2)
			{
				strcpy(cad_temporal,"lp.cups ");
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
		system(cad_temporal);
		}

	}
	else if(strcmp(tipo, "corte_caja") == 0){ //Corte Caja

		//mysql_init(&mysql);
		//if(!mysql_real_connect(&mysql, "192.168.0.3", "caja", "caja", "CarnesBecerra", 0, NULL, 0)){
		if(conecta_bd() == -1){
			g_print("\nNo me puedo conectar a la base de datos =(\n");
			return (1);
		}else{
			//Informacion del corte de caja
			strcpy(sql,"SELECT Corte_Caja.id_caja, Corte_Caja.fecha, Corte_Caja.hora, CONCAT_WS(  ' ', Usuario.nombre, Usuario.apellido )  AS usuario FROM Corte_Caja INNER  JOIN Usuario ON Usuario.id_usuario = Corte_Caja.id_usuario WHERE Corte_Caja.id_corte_caja = ");
			strcat(sql, id_venta);
			printf("%s\n",sql);
			if((err = mysql_query(&mysql, sql)) != 0){
				printf("Error al consultar los tipos de documentos: %s\n", mysql_error(&mysql));
				return (1);
			}else{
				if((resultado = mysql_store_result(&mysql))){
					if((row = mysql_fetch_row(resultado))){
						if (mysql_num_rows(resultado) > 0){

							fpt = fopen(TicketImpresion,"w");
							if(fpt == NULL){
								printf("\nERROR no se puede abrir el archivo a imprimir");
								return (1);	
							}else{
								for(i=0; i < 5;i++) //Guarda todo el arreglo en listapos
									listatipos[i] =  row[i];

								corte_dinero_caja_num = 0;
								corte_contado_num = 0;
								corte_retiro_num =0;
								imprimir(resetea,nX);
								imprimir(pt_pagina,nX);
								imprimir(pmargen_a,nX);
								imprimir(pmargen_i,nX);
								imprimir(pmargen_d,nX);
								imprimir(pcpi10,nX);
								//imprimir(pcondensed,nX);
								imprimir(ptabulador, nX);
								imprimir(ptabulador, nX);
								imprimir(ptabulador, nX);
								imprimir(pnegrita,nX);
								imprimir("MATERIALES MONTEJANO",nX);
								imprimir(pc_negrita,nX);
								imprimir(psalto_l,nX);
								imprimir(ptabulador, nX);
								imprimir(ptabulador, nX);
								imprimir(ptabulador, nX);
								imprimir(ptabulador, nX);
								imprimir(pnegrita,nX);
								imprimir("CORTE DE CAJA",nX);
								imprimir(pc_negrita,nX);
								imprimir(psalto_l,nX);
								imprimir(psalto_l,nX);

								sprintf(c, "Caja: %s\n", listatipos[0]); imprimir(c,nX);
								sprintf(c, "Fecha: %c%c/%c%c/%c%c%c%c \tHora: %s",listatipos[1][8],listatipos[1][9],listatipos[1][5],listatipos[1][6],listatipos[1][0],listatipos[1][1],listatipos[1][2],listatipos[1][3],listatipos[2]);
								imprimir(c,nX);
								imprimir(psalto_l,nX);
								//imprimir(resetea,nX);
								sprintf(c, "Cajero(a):  ");
								imprimir(c,nX);
								sprintf(c,"%s",listatipos[3]);
								imprimir(c,nX);
								imprimir(psalto_l,nX);

								sprintf(c, "_________________");
								imprimir(c,nX);
								imprimir(psalto_l,nX);


								//Credito y contado TOTAL e IVA
								sprintf(sql,"SELECT Venta.tipo, FORMAT(SUM( Venta_Articulo.monto ),2)  AS monto_IVA, FORMAT(SUM( Venta_Articulo.monto )  / 1.15,2) AS monto, SUM(Venta_Articulo.monto) FROM Corte_Caja, Venta, Venta_Articulo, Articulo WHERE Corte_Caja.id_caja = Venta.id_caja AND Venta.id_venta BETWEEN Corte_Caja.id_venta_inicio AND Corte_Caja.id_venta_fin AND Venta_Articulo.id_venta = Venta.id_venta AND Articulo.id_articulo = Venta_Articulo.id_articulo AND Venta.cancelada =  'n' AND Corte_Caja.id_corte_caja = %s GROUP BY Venta.tipo", id_venta);
								//printf("\n%s",sql);
								if((err = mysql_query(&mysql, sql)) != 0){
									printf("Error al consultar los tipos de documentos de Venta Total: %s\n", mysql_error(&mysql));
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
											sprintf(c, "VENTA TOTAL:");
											imprimir(c,nX);
											imprimir(psalto_l,nX);
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
													imprimir(ptabulador, nX);
													imprimir(ptabulador, nX);
													imprimir(c,nX);
													imprimir(psalto_l,nX);


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
													imprimir(ptabulador, nX);
													imprimir(ptabulador, nX);
													imprimir(c,nX);
													imprimir(psalto_l,nX);
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
												sprintf(sql,"SELECT Venta.tipo, FORMAT(SUM( Venta_Articulo.monto ),2)  AS monto_IVA, FORMAT(SUM( Venta_Articulo.monto )  / 1.15,2) AS monto FROM Corte_Caja, Venta, Venta_Articulo, Articulo WHERE Corte_Caja.id_caja = Venta.id_caja AND Venta.id_venta BETWEEN Corte_Caja.id_venta_inicio AND Corte_Caja.id_venta_fin AND Venta_Articulo.id_venta = Venta.id_venta AND Articulo.id_articulo = Venta_Articulo.id_articulo AND Venta.cancelada =  'n' AND Corte_Caja.id_corte_caja = %s GROUP BY Corte_Caja.id_corte_caja", id_venta);
												if((err = mysql_query(&mysql, sql)) != 0)
												{
													printf("Error al consultar los tipos de documentos de Venta Total: %s\n", mysql_error(&mysql));
													return (1);
												}
												else
												{
													if((resultado = mysql_store_result(&mysql)))
													{
														row = mysql_fetch_row(resultado);
														sprintf(cambio, row[1]);
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
												imprimir(ptabulador, nX);
												imprimir(ptabulador, nX);
												imprimir(c,nX);
												imprimir(psalto_l,nX);
											}
										}//Fin del > 0
									}else{
										return (1);
									}
								}


								//FACTURAS ya sea de credito o contado
								sprintf(sql,"SELECT Venta.tipo, FORMAT(SUM(Venta.monto),2) AS Monto_IVA, FORMAT((SUM(Venta.monto)  / 1.15),2) AS Monto, SUM(Venta.monto) FROM Corte_Caja INNER  JOIN Venta ON Corte_Caja.id_caja = Venta.id_caja INNER  JOIN Venta_Factura_Relacion ON Venta_Factura_Relacion.id_venta = Venta.id_venta INNER  JOIN Venta_Factura ON Venta_Factura.id_factura = Venta_Factura_Relacion.id_factura WHERE Venta.id_venta BETWEEN Corte_Caja.id_venta_inicio AND Corte_Caja.id_venta_fin AND Venta.cancelada =  'n' AND Corte_Caja.id_corte_caja = %s GROUP  BY Venta.tipo", id_venta);
								printf("\n%s\n",sql);
								if((err = mysql_query(&mysql, sql)) != 0){
									printf("Error al consultar los tipos de documentos: %s\n", mysql_error(&mysql));
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
											sprintf(c, "\nVENTA FACTURADA:\n"); imprimir(c,nX);
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
														imprimir(ptabulador, nX);
														imprimir(ptabulador, nX);
           													imprimir(c,nX);
														imprimir(psalto_l,nX);
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
														imprimir(ptabulador, nX);
														imprimir(ptabulador, nX);
														imprimir(c,nX);
														imprimir(psalto_l,nX);
														bandera ++;
													}
												}
											}

											//Verifica si existe Credito y contado para mostrar la suma de los 2
											if(bandera == 2)
											{
												sprintf(sql,"SELECT FORMAT(SUM(Venta.monto),2) AS Monto_IVA, FORMAT((SUM(Venta.monto)  / 1.15),2) AS Monto, SUM(Venta.monto) FROM Corte_Caja INNER  JOIN Venta ON Corte_Caja.id_caja = Venta.id_caja INNER  JOIN Venta_Factura_Relacion ON Venta_Factura_Relacion.id_venta = Venta.id_venta INNER  JOIN Venta_Factura ON Venta_Factura.id_factura = Venta_Factura_Relacion.id_factura WHERE Venta.id_venta BETWEEN Corte_Caja.id_venta_inicio AND Corte_Caja.id_venta_fin AND Venta.cancelada = 'n' AND Corte_Caja.id_corte_caja = %s GROUP  BY Corte_Caja.id_corte_caja", id_venta);
												if((err = mysql_query(&mysql, sql)) != 0)
												{
													printf("Error al consultar los tipos de documentos de Venta Total: %s\n", mysql_error(&mysql));
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
												imprimir(ptabulador, nX);
												imprimir(ptabulador, nX);
												imprimir(c,nX);
												imprimir(psalto_l,nX);
											}
										}
									}else{
										return (1);
									}
								}


								imprimir(psalto_l,nX);
								sprintf(c, "VENTA TICKETS:");
								imprimir(c,nX);
								imprimir(psalto_l,nX);

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
										sprintf(Errores,mysql_error(&mysql));
										Err_Info(Errores);
									}
								strncat(c,temp3,11-strlen(cambio));
								strcat(c, cambio);
								imprimir(ptabulador, nX);
								imprimir(ptabulador, nX);
        							imprimir(c,nX);
								imprimir(psalto_l,nX);

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
										sprintf(Errores,mysql_error(&mysql));
										Err_Info(Errores);
									}
								strncat(c,temp3,11-strlen(cambio));
								strcat(c, cambio);
								imprimir(ptabulador, nX);
								imprimir(ptabulador, nX);
        							imprimir(c,nX);
								imprimir(psalto_l,nX);


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
										sprintf(Errores,mysql_error(&mysql));
										Err_Info(Errores);
									}
								strncat(c,temp3,11-strlen(cambio));
								strcat(c, cambio);
								imprimir(ptabulador, nX);
								imprimir(ptabulador, nX);
								imprimir(c,nX);
								imprimir(psalto_l,nX);
								imprimir(psalto_l,nX);


								//Cancelaciones
								strcpy(sql,"SELECT Venta_Cancelada.id_venta, Venta_Cancelada.observaciones, Cliente.nombre FROM Venta_Cancelada INNER  JOIN Venta ON Venta_Cancelada.id_venta = Venta.id_venta INNER  JOIN Cliente ON Cliente.id_cliente = Venta.id_cliente INNER  JOIN Corte_Caja ON Venta.id_venta BETWEEN Corte_Caja.id_venta_inicio AND Corte_Caja.id_venta_fin AND Corte_Caja.id_caja = Venta.id_caja WHERE Venta.cancelada =  's' AND Corte_Caja.id_corte_caja = ");
								strcat(sql, id_venta);

								if((err = mysql_query(&mysql, sql)) != 0){
									printf("Error al consultar los tipos de documentos: %s\n", mysql_error(&mysql));
									return (1);
								}else{
									if((resultado = mysql_store_result(&mysql))){

										if (mysql_num_rows(resultado) > 0){
											sprintf(c, "_________________\n"); imprimir(c,nX);
											sprintf(c, "CANCELACIONES:\n"); imprimir(c,nX);
											while((row = mysql_fetch_row(resultado))){
												for(i=0; i < 3;i++) //Guarda todo el arreglo en listapos
													listatipos[i] =  row[i];

												//Cancelaciones FOLIO
												sprintf(c, "    Folio: %s\t",listatipos[0]); imprimir(c,nX);
												//Cancelaciones Cliente
												sprintf(c, "    Cliente: "); imprimir(c,nX);
												sprintf(c,"%s\n",listatipos[2]); imprimir(c,nX);
												sprintf(c, "    Observaciones:\n"); imprimir(c,nX);
												sprintf(c,"%s",listatipos[1]); imprimir(c,nX);
												imprimir(psalto_l,nX);
												imprimir(psalto_l,nX);
											}
										}else{
											//return (1);
										}
									}else{
										return (1);
									}
								}

								//Retiro Efectivo
								strcpy(sql,"SELECT Retiro.fecha, Retiro.hora, /*FORMAT(*/Retiro.monto/*,2)*/, CONCAT_WS(  '  ', Cajero.nombre, Cajero.apellido )  AS cajero, CONCAT_WS(  '  ', UsuarioRetira.nombre, UsuarioRetira.apellido )  AS UsuarioRetira FROM Usuario Cajero, Usuario UsuarioRetira, Corte_Caja INNER  JOIN Retiro ON Retiro.id_retiro BETWEEN Corte_Caja.id_retiro_inicio AND Corte_Caja.id_retiro_fin AND Corte_Caja.id_caja = Retiro.id_caja WHERE Cajero.id_usuario = Retiro.id_usuario_cajero AND UsuarioRetira.id_usuario = Retiro.id_usuario_retira AND Corte_Caja.id_corte_caja = ");
								strcat(sql, id_venta);

								if((err = mysql_query(&mysql, sql)) != 0){
									printf("Error al consultar los tipos de documentos: %s\n", mysql_error(&mysql));
									return (1);
								}else{
									if((resultado = mysql_store_result(&mysql))){

										if (mysql_num_rows(resultado) > 0){
											sprintf(c, "_________________\n"); imprimir(c,nX);
											sprintf(c, "RETIRO DE EFECTIVO:\n"); imprimir(c,nX);
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
														sprintf(Errores,mysql_error(&mysql));
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
													sprintf(Errores,mysql_error(&mysql));
													Err_Info(Errores);
												}
											strncat(c,temp3,11-strlen(cambio));
											strcat(c, cambio);
											imprimir(c,nX);
											imprimir(psalto_l,nX);
											imprimir(psalto_l,nX);

										}else{
											//return (1);
										}
									}else{
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
									sprintf(Errores,mysql_error(&mysql));
									Err_Info(Errores);
								}
							strncat(c,temp3,16-strlen(cambio));
							strcat(c, cambio);
							imprimir(c,nX);
							imprimir(psalto_l,nX);


							//Numero de Clientes

							strcpy(sql,"SELECT COUNT(*) FROM Corte_Caja, Venta WHERE Corte_Caja.id_caja = Venta.id_caja AND Venta.id_venta BETWEEN Corte_Caja.id_venta_inicio AND Corte_Caja.id_venta_fin  AND Corte_Caja.id_corte_caja = ");
							strcat(sql, id_venta);
							strcat(sql," GROUP  BY Corte_Caja.id_corte_caja");

							if((err = mysql_query(&mysql, sql)) != 0){
								printf("Error al consultar el total de clientes: %s\n", mysql_error(&mysql));
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
									return (1);
								}
							}



							//Cantidad vendida de kilos
							sprintf(sql,"SELECT Venta_Articulo.id_articulo, /*FORMAT(*/SUM( Venta_Articulo.monto )/*,2)*/  AS monto_IVA, /*FORMAT(*/(SUM( Venta_Articulo.monto )  / 1.15)/*,2)*/ AS monto, SUM( Venta_Articulo.cantidad )  AS cantidad, Articulo.nombre, Articulo.tipo AS tipo FROM Corte_Caja, Venta, Venta_Articulo, Articulo WHERE Corte_Caja.id_caja = Venta.id_caja AND Venta.id_venta BETWEEN Corte_Caja.id_venta_inicio AND Corte_Caja.id_venta_fin AND Venta_Articulo.id_venta = Venta.id_venta AND Articulo.id_articulo = Venta_Articulo.id_articulo AND Venta.cancelada =  'n' AND Corte_Caja.id_corte_caja = %s GROUP  BY Venta_Articulo.id_articulo ORDER BY tipo,cantidad DESC", id_venta);

							if((err = mysql_query(&mysql, sql)) != 0){
								printf("Error al consultar los tipos de documentos: %s\n", mysql_error(&mysql));
								return (1);
							}else{
								if((resultado = mysql_store_result(&mysql))){
									if (mysql_num_rows(resultado) > 0){
	 									sprintf(c, "_________________");
										imprimir(c,nX);
										imprimir(psalto_l,nX);
										sprintf(c, "ARTICULOS VENDIDOS:");
										imprimir(c,nX);
										imprimir(psalto_l,nX);
										sprintf(c, "Articulo\t\t\t\t\t\tCantidad");
										imprimir(c,nX);
										imprimir(psalto_l,nX);
										sprintf(c, "----------\t\t\t\t\t\t----------");
										imprimir(c,nX);
										imprimir(psalto_l,nX);
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
											for(i=0;i<60;i++){
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
											imprimir(psalto_l,nX);
										}
										//Monto dle retiro
										strcpy(cambio,"");
										sprintf(cambio, "%.3f",corte_peso);
										sprintf(c, "TOTAL KILOS:  %s",cambio);
										imprimir(psalto_l,nX);
										imprimir(c,nX);
										imprimir(psalto_l,nX);
										strcpy(cambio,"");
										sprintf(cambio, "%.2f",corte_articulos);
										sprintf(c, "TOTAL PIEZAS: %s",cambio);
										imprimir(c,nX);
										imprimir(psalto_l,nX);
									}else{
										//return (1);
									}
								}else{
									return (1);
								}
							}

							imprimir(psig_pag,nX);
							imprimir(resetea,nX);
							//imprimir(corta_papel,nX);
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
							system(cad_temporal);
							}
						}else{
							return (1);
						}
					}else{
						return (1);
					}

				}else{
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
		nX = num_mayor_X + 2; //TAMAÑO MAXIMO DE X

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
							system(cad_temporal);
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
				return (1);
			}else{
				if((resultado = mysql_store_result(&mysql))){
						if (mysql_num_rows(resultado) > 0)
						{
							fpt = fopen(TicketImpresion,"w");
							if(fpt == NULL){
								printf("\nERROR no se puede abrir el archivo a imprimir");
								return (1);
							}else{
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
								strcpy(sql,"SELECT Venta.tipo, /*FORMAT(*/SUM( Venta.monto )/*,2)*/  AS monto_IVA, /*FORMAT(*/(SUM( Venta.monto )  / 1.15)/*,2)*/ AS monto FROM Corte_Caja, Venta WHERE Venta.id_venta BETWEEN Corte_Caja.id_venta_inicio AND Corte_Caja.id_venta_fin AND Venta.cancelada =  'n' AND Venta.id_caja = Corte_Caja.id_caja AND Corte_Caja.fecha = '");
								strcat(sql, id_venta);
								strcat(sql,"' GROUP  BY Venta.tipo");
								//printf("\n%s",sql);
								if((err = mysql_query(&mysql, sql)) != 0){
									printf("Error al consultar los tipos de documentos de Venta Total: %s\n", mysql_error(&mysql));
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
											sprintf(c, "VENTA TOTAL:\n"); imprimir(c,nX);
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
										return (1);
									}
								}


								//FACTURAS ya sea de credito o contado
								strcpy(sql,"SELECT Venta.tipo,  /*FORMAT(*/Venta.monto/*,2)*/ AS Monto_IVA, /*FORMAT(*/(Venta.monto  / 1.15)/*,2)*/ AS Monto FROM Corte_Caja INNER  JOIN Venta ON Corte_Caja.id_caja = Venta.id_caja INNER  JOIN Venta_Factura_Relacion ON Venta_Factura_Relacion.id_venta = Venta.id_venta INNER  JOIN Venta_Factura ON Venta_Factura.id_factura = Venta_Factura_Relacion.id_factura WHERE Venta.id_venta BETWEEN Corte_Caja.id_venta_inicio AND Corte_Caja.id_venta_fin AND Venta.cancelada =  'n' AND Corte_Caja.fecha = '");
								strcat(sql, id_venta);
								strcat(sql,"' GROUP  BY Venta_Factura_Relacion.id_venta");
								//printf("\n%s",sql);
								if((err = mysql_query(&mysql, sql)) != 0){
									printf("Error al consultar los tipos de documentos: %s\n", mysql_error(&mysql));
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
											sprintf(c, "\nVENTA FACTURADA:\n"); imprimir(c,nX);
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
										return (1);
									}
								}


								sprintf(c, "\nVENTA TICKETS:\n"); imprimir(c,nX);

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
								strcpy(sql,"SELECT Venta_Cancelada.id_venta, Venta_Cancelada.observaciones, Cliente.nombre FROM Venta_Cancelada INNER  JOIN Venta ON Venta_Cancelada.id_venta = Venta.id_venta INNER  JOIN Cliente ON Cliente.id_cliente = Venta.id_cliente INNER  JOIN Corte_Caja ON Venta.id_venta BETWEEN Corte_Caja.id_venta_inicio AND Corte_Caja.id_venta_fin AND Corte_Caja.id_caja = Venta.id_caja WHERE Venta.cancelada =  's' AND Corte_Caja.fecha = '");
								strcat(sql, id_venta);
								strcat(sql,"'");
								//printf("\n%s",sql);
								if((err = mysql_query(&mysql, sql)) != 0){
									printf("Error al consultar los tipos de documentos: %s\n", mysql_error(&mysql));
									return (1);
								}else{
									if((resultado = mysql_store_result(&mysql))){

										if (mysql_num_rows(resultado) > 0){
											sprintf(c, "_________________\n"); imprimir(c,nX);
											sprintf(c, "CANCELACIONES:\n"); imprimir(c,nX);
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
										return (1);
									}
								}

								//Retiro Efectivo
								strcpy(sql,"SELECT Retiro.fecha, Retiro.hora, /*FORMAT(*/Retiro.monto/*,2)*/, CONCAT_WS(  '  ', Cajero.nombre, Cajero.apellido )  AS cajero, CONCAT_WS(  '  ', UsuarioRetira.nombre, UsuarioRetira.apellido )  AS UsuarioRetira FROM Usuario Cajero, Usuario UsuarioRetira, Corte_Caja INNER  JOIN Retiro ON Retiro.id_retiro BETWEEN Corte_Caja.id_retiro_inicio AND Corte_Caja.id_retiro_fin AND Corte_Caja.id_caja = Retiro.id_caja WHERE Cajero.id_usuario = Retiro.id_usuario_cajero AND UsuarioRetira.id_usuario = Retiro.id_usuario_retira AND Corte_Caja.fecha = '");
								strcat(sql, id_venta);
								strcat(sql,"'");
								//printf("\n%s",sql);
								if((err = mysql_query(&mysql, sql)) != 0){
									printf("Error al consultar los tipos de documentos: %s\n", mysql_error(&mysql));
									return (1);
								}else{
									if((resultado = mysql_store_result(&mysql))){

										if (mysql_num_rows(resultado) > 0){
											sprintf(c, "_________________\n"); imprimir(c,nX);
											sprintf(c, "RETIRO DE EFECTIVO:\n"); imprimir(c,nX);
											while((row = mysql_fetch_row(resultado))){
												for(i=0; i < 5;i++) //Guarda todo el arreglo en listapos
													listatipos[i] =  row[i];
												//Fecha Y Hora
												sprintf(c, "Fecha: %c%c-%c%c-%c%c%c%c Hora: %s\n",listatipos[0][8],listatipos[0][9],listatipos[0][5],listatipos[0][6],listatipos[0][0],listatipos[0][1],listatipos[0][2],listatipos[0][3],listatipos[1]); imprimir(c,nX);
												//Cajero
												sprintf(c, "Cajero(a): %s\n",listatipos[3]); imprimir(c,nX);
												//Cancelaciones Cliente
												sprintf(c, "RetirÃ?: %s\n",listatipos[4]); imprimir(c,nX);
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
							imprimir(c,nX);


							//Numero de Clientes

//							strcpy(sql,"SELECT Venta.id_cliente FROM Corte_Caja, Venta WHERE Corte_Caja.id_caja = Venta.id_caja AND Venta.id_venta BETWEEN Corte_Caja.id_venta_inicio AND Corte_Caja.id_venta_fin AND Venta.cancelada =  'n' AND Corte_Caja.fecha =  '");
							strcpy(sql,"SELECT Venta.id_venta FROM Venta WHERE Venta.cancelada = 'n' AND Venta.fecha = '");
							strcat(sql, id_venta);
							strcat(sql,"'");
							printf("%s\n",sql);
							if((err = mysql_query(&mysql, sql)) != 0){
								printf("Error al consultar el total de clientes: %s\n", mysql_error(&mysql));
								return (1);
							}else{
								if((resultado = mysql_store_result(&mysql))){
									if (mysql_num_rows(resultado) > 0){

	 						//			sprintf(c, "_____________________________________\n"); imprimir(c,nX);
										num_numero_int =mysql_num_rows(resultado);
										sprintf(cambio, "%d",num_numero_int);
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
									return (1);
								}
							}



							//Cantidad vendida de kilos
							strcpy(sql,"SELECT Venta_Articulo.id_articulo, /*FORMAT(*/SUM( Venta_Articulo.monto )/*,2)*/  AS monto_IVA, /*FORMAT(*/(SUM( Venta_Articulo.monto )  / 1.15)/*,2)*/ AS monto, SUM( Venta_Articulo.cantidad )  AS cantidad, Articulo.nombre, Linea.nombre AS linea, Linea.id_linea FROM Corte_Caja, Venta, Venta_Articulo, Articulo INNER  JOIN Linea ON Articulo.id_linea = Linea.id_linea WHERE Corte_Caja.id_caja = Venta.id_caja AND Venta.id_venta BETWEEN Corte_Caja.id_venta_inicio AND Corte_Caja.id_venta_fin AND Venta_Articulo.id_venta = Venta.id_venta AND Articulo.id_articulo = Venta_Articulo.id_articulo AND Venta.cancelada =  'n' AND Corte_Caja.fecha =  '");
							strcat(sql, id_venta);
							strcat(sql,"' GROUP  BY Venta_Articulo.id_articulo ORDER  BY Linea.nombre, cantidad");
							printf("%s",sql);
							if((err = mysql_query(&mysql, sql)) != 0){
								printf("Error al consultar los tipos de documentos: %s\n", mysql_error(&mysql));
								return (1);
							}else{
								if((resultado = mysql_store_result(&mysql))){
									if (mysql_num_rows(resultado) > 0){
	 									sprintf(c, "_________________\n"); imprimir(c,nX);
										sprintf(c, "ARTICULOS VENDIDOS:\n"); imprimir(c,nX);
										sprintf(c, "Articulo                     Cantidad\n");
										imprimir(c,nX);
										sprintf(c, "----------                ----------\n");
										imprimir(c,nX);
										corte_dinero_caja_num = 0;
          									bandera=0;
										strcpy(cad_temporal,"");
										while((row = mysql_fetch_row(resultado))){
											for(i=0; i < 6;i++) //Guarda todo el arreglo en listapos
												listatipos[i] =  row[i];


          										if(strcmp(cad_temporal,row[6]) != 0){ //Primera vez que se mete a inicializar la variable
            											sprintf(cad_temporal, "%s",row[6]);
												bandera=1;
											}

											if(bandera==1){
												sprintf(c, "\n* %s\n",row[5]);
												imprimir(c,nX);
												bandera=2;
											}

											//Monto dle retiro
											fac_subtotal_num = atof(listatipos[3]);
											sprintf(cambio, "%.2f", fac_subtotal_num);
											corte_dinero_caja_num = corte_dinero_caja_num + atof(cambio);
											strcpy(temp2,"");
											strcpy(c,"");
											for(i=0;i<26;i++){
												if( i >=strlen(listatipos[4]))
													temp2[i] = ' ';
												else
													temp2[i] = listatipos[4][i];
												temp2[i+1] = '\0';
											}
											strcat(c, temp2);
											strncat(c,temp3,11-strlen(cambio));
											strcat(c, cambio);
											strcat(c, "\n");
											imprimir(c,nX);


										}
										//Monto dle retiro
										strcpy(cambio,"");
										sprintf(cambio, "%.2f",corte_dinero_caja_num);
										sprintf(c, "\nTOTAL CANTIDAD:  %s\n",cambio); imprimir(c,nX);
										strcpy(cambio,"");
										num_numero_int =mysql_num_rows(resultado);
										sprintf(cambio, "%d",num_numero_int);
										sprintf(c, "TOTAL ARTICULOS: %s\n",cambio); imprimir(c,nX);
									}else{
										//return (1);
									}
								}else{
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
							imprimir(corta_papel,nX);
							fputs(c,fpt);
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
							system(cad_temporal);
							}
						}
						else
						{
							return (1);
						}


				}else{
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
			if(conecta_bd() == -1){
				printf("\nNo me puedo conectar a la base de datos =(\n");
				return (1);
			}else{

				//Con ID FACTURA


//SELECT Venta_Factura.id_factura, Venta_Factura.fecha, Venta_Factura.observacion, Cliente.nombre, Cliente.domicilio, Cliente.ciudad_estado, Cliente.rfc, Venta_Factura_Relacion.num_factura FROM Venta_Factura, Venta, Cliente, Venta_Factura_Relacion WHERE Venta.id_venta = Venta_Factura_Relacion.id_venta AND Venta_Factura_Relacion.id_factura = Venta_Factura.id_factura AND Venta.id_cliente = Cliente.id_cliente AND Venta.id_venta = 1
				//strcpy(sql,"SELECT Venta_Factura.id_factura, Venta_Factura.fecha, Venta_Factura.observacion, Cliente.nombre, Cliente.domicilio, Cliente.ciudad_estado, Cliente.rfc, Venta_Factura_Relacion.num_factura, Venta_Factura_Relacion.id_venta, Cliente.id_cliente FROM Venta_Factura, Venta, Cliente, Venta_Factura_Relacion WHERE Venta.id_venta = Venta_Factura_Relacion.id_venta AND Venta_Factura_Relacion.id_factura = Venta_Factura.id_factura AND Venta.id_cliente = Cliente.id_cliente AND Venta.id_venta = ");
				sprintf(sql,"SELECT Venta_Factura.id_factura, Venta_Factura.fecha, Venta_Factura.observacion, 'CLIENTES VARIOS', '', '', '', Venta_Factura_Relacion.num_factura, 'CIERRE', '' FROM Venta_Factura, Venta_Factura_Relacion WHERE Venta_Factura_Relacion.id_factura = Venta_Factura.id_factura AND Venta_Factura_Relacion.num_factura = %s GROUP BY Venta_Factura_Relacion.num_factura",id_venta);
				//strcat(sql, id_venta);

				printf("El id de la factura: %s\n", id_venta);
				printf("El SQL: %s\n", sql);
				if((err = mysql_query(&mysql, sql)) != 0){
					printf("Error al consultar los tipos de documentos: %s\n", mysql_error(&mysql));
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
										return (1);
									}else{
										inicializar(); //Inicializa MATRIX
										for(i=0; i < 11;i++) //Guarda todo el arreglo en listapos
											listatipos[i] =  row[i];

										strcpy(id_factura_interno,listatipos[0]);

										//Separamos el aÃ±o
										fechaTMP[0] = listatipos[1][0];
										fechaTMP[1] = listatipos[1][1];
										fechaTMP[2] = listatipos[1][2];
										fechaTMP[3] = listatipos[1][3];
										fechaTMP[4] = '\0';
										llenarMATRIX(arr_ano,fechaTMP); //DIA
										//Separamos el mes
										fechaTMP[0] = listatipos[1][5];
										fechaTMP[1] = listatipos[1][6];
										fechaTMP[2] = '\0';
										llenarMATRIX(arr_mes,fechaTMP); //MES
										//Separamos el dia
										fechaTMP[0] = listatipos[1][8];
										fechaTMP[1] = listatipos[1][9];
										fechaTMP[2] = '\0';
										llenarMATRIX(arr_dia,fechaTMP); //DIA

										llenarMATRIX(arr_observaciones, listatipos[2]); //Observaciones

										llenarMATRIX(arr_num_cliente, listatipos[9]); //ID cliente
										llenarMATRIX(arr_nombre, listatipos[3]); //Nombre cliente
										llenarMATRIX(arr_direccion, listatipos[4]); //Direccion cliente
										llenarMATRIX(arr_ciudad, listatipos[5]); //Ciudad y estado
										llenarMATRIX(arr_rfc, listatipos[6]); //RFC

										llenarMATRIX(arr_num_factura, listatipos[7]); //Numero de Factura
										llenarMATRIX(arr_num_venta, listatipos[8]); //Numero de la Venta

										//Con ID FACTURA



//							strcpy(sql,"SELECT /*FORMAT(*/Venta_Articulo.cantidad/*,3)*/, Articulo.nombre, /*FORMAT(*/Venta_Articulo.precio/*,2)*/ AS precio_IVA, /*FORMAT(*/Venta_Articulo.monto/*,2)*/ AS monto_IVA, /*FORMAT(*/Venta_Articulo.precio/*,2)*/ AS precio, /*FORMAT(*/Venta_Articulo.monto/*,2)*/ AS monto FROM Venta_Articulo, Articulo WHERE Venta_Articulo.id_articulo = Articulo.id_articulo AND Venta_Articulo.id_venta = ");

										//sprintf(sql, "SELECT FORMAT(Venta_Articulo.cantidad,3), Articulo.nombre, FORMAT(Venta_Articulo.precio,2) AS precio_IVA, FORMAT(Venta_Articulo.monto,2) AS monto_IVA, FORMAT(Venta_Articulo.precio,2) AS precio, FORMAT(Venta_Articulo.monto,2) AS monto FROM Venta_Articulo, Articulo WHERE Venta_Articulo.id_articulo = Articulo.id_articulo AND Venta_Articulo.id_venta = %s AND Venta_Articulo.id_factura = %s ORDER BY id_venta_articulo", id_venta, id_factura_interno);
										sprintf(sql, "SELECT '1', 'PRODUCTOS VARIOS', FORMAT(SUM(Venta.monto),2) AS precio_IVA, FORMAT(SUM(Venta.monto),2) AS monto_IVA, FORMAT(SUM(Venta.monto),2) AS precio, FORMAT(SUM(Venta.monto),2) AS monto FROM Venta INNER JOIN Venta_Factura_Relacion ON Venta.id_venta = Venta_Factura_Relacion.id_venta where Venta_Factura_Relacion.num_factura = %s", id_venta);
										printf("%s\n",sql);
										err = mysql_query(&mysql, sql);
										if(err != 0){
											printf("Error al consultar la venta de factura: %s\n", mysql_error(&mysql));
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
														printf("\nERROR AL GUARDAR EN EL ARCHIVO, SE SUPERO EL TAMAÑO DE LA CADENA PERMITIDA");
														printf("\n");
														//return (1);
														break;
													}
												}
												//sprintf(sql, "SELECT FORMAT(SUM(Venta_Articulo.monto),2) as total, SUM(Venta_Articulo.monto) as total_letra FROM Venta_Articulo WHERE id_venta = %s AND id_factura = %s ORDER BY id_venta_articulo", id_venta, id_factura_interno);
												sprintf(sql, "SELECT FORMAT(SUM(Venta.monto),2) as total, SUM(Venta.monto) as total_letra FROM Venta INNER JOIN Venta_Factura_Relacion ON Venta.id_venta=Venta_Factura_Relacion.id_venta WHERE Venta_Factura_Relacion.id_factura = %s", id_factura_interno);
												printf("%s\n",sql);
												err = mysql_query(&mysql, sql);
												if(err != 0){
													printf("Error al consultar la venta de factura: %s\n", mysql_error(&mysql));
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
									system(cad_temporal);
									return (1);
								}

								}
							}else{
								printf("No se pudo la factura....\n");
								return(1);

							}
						}else{
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
			return (1);
		}
		else
		{
		//Abre el encabezado de archivo
		fpt2 = fopen(TicketArriba,"r");
		if(fpt2 == NULL){
			printf("\nERROR no se puede abrir el archivo del encabezado");
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

			sprintf(sql, "SELECT Venta.id_venta, DATE_FORMAT(Venta.fecha,\"%%d-%%m-%%Y\"), FORMAT(ROUND(Venta.monto,2),2), Venta_Factura_Relacion.id_venta, Venta.monto FROM Venta INNER JOIN Venta_Factura_Relacion ON Venta.id_venta = Venta_Factura_Relacion.id_venta WHERE Venta_Factura_Relacion.num_factura = %s AND Venta.cancelada = 'n' ORDER BY Venta.id_venta", id_venta);
	
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
								sprintf(total_cierre_listado, "%f", atof(total_cierre_listado)+atof(row[4]));
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
							sprintf(Errores,mysql_error(&mysql));
							Err_Info(Errores);
						}
						imprimir(salto,nX);
						imprimir(salto,nX);
					}
				}
				else
				{
					sprintf(Errores,mysql_error(&mysql));
					Err_Info(Errores);
				}
			}
			else
			{
				sprintf(Errores,mysql_error(&mysql));
				Err_Info(Errores);
			}
			mysql_close(&mysql);
		}
		imprimir(salto,nX);
		imprimir(alinea_d,nX);
		//imprimir(negrita1,nX);
		imprimir("maticaLX",nX);
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
  		system(cad_temporal);
  		}

	}
	else if(strcmp(tipo, "pedidos_paso1") == 0){ //Es credito o contado

		fpt = fopen(TicketImpresion,"w");
		if(fpt == NULL){
			printf("\nERROR no se puede abrir el archivo a imprimir");
			return (1);
		}
		else
		{
		//Abre el encabezado de archivo
			imprimir(resetea,nX);
			imprimir(pt_pagina,nX);
			imprimir(pmargen_a,nX);
			imprimir(pmargen_i,nX);
			imprimir(pmargen_d,nX);
			imprimir(pcpi10,nX);
			imprimir(pcondensed,nX);
			imprimir(pnegrita,nX);
			imprimir(ptabulador, nX);
			imprimir(ptabulador, nX);
			imprimir(ptabulador, nX);
			imprimir(ptabulador, nX);
			imprimir(ptabulador, nX);
			imprimir("MATERIALES MONTEJANO",nX);
			imprimir(psalto_l,nX);
			imprimir(ptabulador, nX);
			imprimir(ptabulador, nX);
			imprimir(ptabulador, nX);
			imprimir(ptabulador, nX);
			imprimir(ptabulador, nX);
			imprimir("PEDIDO",nX);
			imprimir(psalto_l,nX);
			imprimir(pc_negrita,nX);

			sprintf(sql, "SELECT  DATE_FORMAT(Pedido.fecha,\"%%d/%%m/%%Y\"), Pedido.hora, Pedido.HoraEntrega, DATE_FORMAT(Pedido.FechaEntrega,\"%%d/%%m/%%Y\"), Cliente.nombre, Cliente.telefono, Usuario.nombre, Cliente.domicilio, Cliente.id_cliente, Cliente.entre_calles, Cliente.ciudad_estado, Cliente.colonia FROM Pedido INNER JOIN Cliente ON Cliente.id_cliente = Pedido.id_cliente INNER JOIN Usuario ON Pedido.id_usuario=Usuario.id_usuario WHERE Pedido.id_pedido=%s", id_venta);

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
								strcpy(c,"Folio: ");
								strcat(c, id_venta);
								imprimir(c,nX);
								imprimir(psalto_l,nX);
								imprimir(psalto_l,nX);
								strcpy(c,"Capturado: ");
								imprimir(pnegrita,nX);
								imprimir(c,nX);

								strcpy(c, row[1]);
								strncat(c, temp3, 10-strlen(row[1]));
								strcat(c, row[0]);
								strncat(c, temp3, 12-strlen(row[0]));
								imprimir(c,nX);
								imprimir(ptabulador,nX);
								//imprimir(ptabulador,nX);
								strcpy(c,"Entregarlo:  ");
								imprimir(c,nX);
								//imprimir(salto,nX);
								strcpy(c, row[2]);
								strncat(c, temp3, 10-strlen(row[2]));
								strcat(c, row[3]);
								strncat(c, temp3, 12-strlen(row[3]));
								imprimir(c,nX);
								imprimir(pc_negrita,nX);
								imprimir(psalto_l,nX);
								strcpy(c,"Capturo:  ");
								strcat(c, row[6]);
								imprimir(c,nX);
								imprimir(psalto_l,nX);
								
								strcpy(c,"Cliente:  ");
								strcat(c, row[4]);
								imprimir(c,nX);
								imprimir(ptabulador,nX);
								imprimir(ptabulador,nX);
								
								strcpy(c,"Numero:  ");
								strcat(c, row[8]);
								imprimir(c,nX);
								imprimir(psalto_l,nX);
								
								sprintf (sql,"SELECT domicilio, entre_calles, ciudad_estado, colonia, telefono FROM Cliente_Envio WHERE id_cliente = %s",row[8]);
								
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
									imprimir(ptabulador,nX);
									imprimir(ptabulador,nX);
								
									strcpy(c,"Entre calles: ");
									strcat(c, row2[1]);
									imprimir(c,nX);
									
									imprimir(psalto_l,nX);
									
									strcpy(c,"Ciudad_Estado: ");
									strcat(c, row2[2]);
									imprimir(c,nX);
									imprimir(ptabulador,nX);
									imprimir(ptabulador,nX);
									
									strcpy(c,"Colonia: ");
									strcat(c, row2[3]);
									imprimir(c,nX);
									imprimir(ptabulador,nX);
									imprimir(ptabulador,nX);
									
									strcpy(c,"Telefono: ");
									strcat(c, row2[4]);
									imprimir(c,nX);
									imprimir(ptabulador,nX);
									imprimir(ptabulador,nX);
								}
								else
								{
									strcpy(c,"Domicilio:  ");
									strcat(c, row[7]);
									imprimir(c,nX);
									imprimir(ptabulador,nX);
									imprimir(ptabulador,nX);
									
									strcpy(c,"Entre Calles:  ");
									strcat(c, row[9]);
									imprimir(c,nX);
									
									imprimir(psalto_l,nX);
									
									strcpy(c,"Ciudad_Estado: ");
									strcat(c, row[10]);
									imprimir(c,nX);
									imprimir(ptabulador,nX);
									imprimir(ptabulador,nX);
									
									strcpy(c,"Colonia: ");
									strcat(c, row[11]);
									imprimir(c,nX);
									imprimir(ptabulador,nX);
									imprimir(ptabulador,nX);
									
									strcpy(c,"Telefono: ");
									strcat(c, row[5]);
									imprimir(c,nX);
									imprimir(psalto_l,nX);
								}
							}else
							{
								Err_Info("\nError al procesar  los pedidos");
							}
						}
						sprintf(sql, "SELECT Articulo.codigo, FORMAT(Pedido_Articulo.cantidad,2), Articulo.nombre, Articulo.tipo, Pedido_Articulo.observaciones FROM Pedido_Articulo INNER JOIN Articulo ON Articulo.id_articulo = Pedido_Articulo.id_articulo WHERE id_pedido = %s ORDER BY Pedido_Articulo.id_pedidos",id_venta);
						printf("\nConsulta2: %s\n", sql);
      						err = mysql_query(&mysql, sql);
						if(err == 0)
						{
							resultado = mysql_store_result(&mysql);
							if(resultado)
							{
								sprintf(c, "------------------------------------------------------------------------------------------------------------------------");
								imprimir(c,nX);
								imprimir(psalto_l,nX);
								sprintf(c, "CODIGO      ARTICULO                            CANTIDAD               OBSERVACIONES");
								imprimir(c,nX);
								imprimir(psalto_l,nX);
								sprintf(c, "------------------------------------------------------------------------------------------------------------------------");
								imprimir(c,nX);
								imprimir(psalto_l,nX);
								if (mysql_num_rows(resultado) > 0){
									num_articulos_venta = 10;
									while((row=mysql_fetch_row(resultado)))
									{
										strcpy(c, "");
										if(strcmp(row[3], "peso")==0)
											strcpy(num_tmp," kg");
										else
											strcpy(num_tmp,"   ");

										//imprimir(pnegrita,nX);
										strcpy(cad_temporal, row[0]);
										strncat(c, temp, 4-strlen(cad_temporal)); //ID Articulo
										strcat(c, cad_temporal);
										imprimir(c,nX);
										sprintf(c, "  %s", row[2]);
										strncat(c, temp3, 40-strlen(row[2]));
										imprimir(c,nX);

										strcpy(cad_temporal,"");
										strcpy(c,"");
										sprintf(cad_temporal, "%s%s", row[1],num_tmp);	//Cantidad
										strncat(c, temp3, 10-strlen(cad_temporal));
										strcat(c, cad_temporal);
										imprimir(c,nX);

										strcpy(cad_temporal,"");
										strcpy(c,"");
										sprintf(cad_temporal, "\t%s", row[4]);	//Observaciones
										//strncat(c, temp3, 60-strlen(cad_temporal));
										sprintf(c, "%s", cad_temporal);
										imprimir(c,nX);
										imprimir(psalto_l,nX);
										num_articulos_venta--;
									}
									//imprimir(pc_negrita,nX);
									sprintf(c, "------------------------------------------------------------------------------------------------------------------------");
									imprimir(c,nX);
									imprimir(psalto_l,nX);
									for(i=0;i<=num_articulos_venta;i++)
										imprimir(psalto_l,nX);

									strcpy(cad_temporal,"");
									sprintf(sql, "SELECT TRUNCATE(SUM(Pedido_Articulo.cantidad),2) FROM Pedido_Articulo INNER JOIN Articulo ON Articulo.id_articulo = Pedido_Articulo.id_articulo WHERE Pedido_Articulo.id_pedido = %s AND Articulo.tipo = 'pieza' GROUP BY Pedido_Articulo.id_pedido",id_venta);
									printf("\nConsulta peiza: %s\n", sql);
									if( (err = mysql_query(&mysql, sql)) != -1 )
									{
										if((resultado = mysql_store_result(&mysql)))
										{
											if(mysql_num_rows(resultado) > 0)
											{
												if((row=mysql_fetch_row(resultado)))
												{
													sprintf(cad_temporal, "TOTAL PIEZAS:     %s",row[0]);
													sprintf(c, "TOTAL PIEZAS:     %s",row[0]);
													imprimir(c,nX);
												}
											}
											else
												printf("Error al calcular la cantidad de los articulos 1");
										}
										else
											printf("Error al calcular la cantidad de los articulos 2");
									}
									else
										printf("Error al calcular la cantidad de los articulos 3");

									strcpy(c,"");
									strncat(c, temp3, 60-strlen(cad_temporal));
									imprimir(c,nX);
									sprintf(c, "____________________________________");
									imprimir(c,nX);
									imprimir(psalto_l,nX);
									strcpy(cad_temporal, "");

									//Calcula el Total de Cantidad
									sprintf(sql, "SELECT TRUNCATE(SUM(Pedido_Articulo.cantidad),2) FROM Pedido_Articulo INNER JOIN Articulo ON Articulo.id_articulo = Pedido_Articulo.id_articulo WHERE Pedido_Articulo.id_pedido = %s AND Articulo.tipo = 'peso' GROUP BY Pedido_Articulo.id_pedido",id_venta);
									printf("\nConsulta peso: %s\n", sql);
									if(!(err = mysql_query(&mysql, sql)))
									{
										if((resultado = mysql_store_result(&mysql)))
										{
											if(mysql_num_rows(resultado) > 0)
											{
												if((row=mysql_fetch_row(resultado)))
												{
													sprintf(cad_temporal, "TOTAL KILOS:      %s kg",row[0]);
													sprintf(c, "TOTAL KILOS:      %s kg",row[0]);
													imprimir(c,nX);
												}
												else
													printf("Error al calcular la cantidad de los articulos");
											}
										}
										else
											printf("Error al calcular la cantidad de los articulos");
									}
									else
										printf("Error al calcular la cantidad de los articulos");

									strcpy(c,"");
									strncat(c, temp3, 60-strlen(cad_temporal));
									imprimir(c,nX);
									sprintf(c, "            RECIBI");
									imprimir(c,nX);
									imprimir(psalto_l,nX);

									//Comentarios de los productos
									/*sprintf(sql, " select Articulo.codigo, Articulo.nombre, Pedido_Articulo.observaciones FROM Pedido_Articulo INNER JOIN Articulo ON Articulo.id_articulo = Pedido_Articulo.id_articulo where id_pedido = %s AND Pedido_Articulo.observaciones != '' ",id_venta);
									printf("\nConsulta4: %s\n", sql);
									if(!(err = mysql_query(&mysql, sql))){
										if((resultado = mysql_store_result(&mysql))){
											if (mysql_num_rows(resultado) > 0){
												imprimir(psalto_l,nX);
												strcpy(c,"OBSERVACIONES: ");
												imprimir(c,nX);
												imprimir(psalto_l,nX);
												imprimir(psalto_l,nX);
												while((row=mysql_fetch_row(resultado))){
													strcpy(c,"--");
													strcat(c,row[1]);
													strcat(c,"--");
													imprimir(pnegrita,nX);
													imprimir(c,nX);
													imprimir(psalto_l,nX);
													strcpy(c,row[2]);
													imprimir(c,nX);
													imprimir(pc_negrita,nX);
													imprimir(psalto_l,nX);
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
							sprintf(Errores,mysql_error(&mysql));
							Err_Info(Errores);
						}
						imprimir(salto,nX);
					}
				}
				else
				{
					sprintf(Errores,mysql_error(&mysql));
					Err_Info(Errores);
				}
			}
			else
			{
				sprintf(Errores,mysql_error(&mysql));
				Err_Info(Errores);
			}
		mysql_close(&mysql);

		imprimir(psalto_l,nX);
		//imprimir(negrita1,nX);
		imprimir("maticaLX",nX);
		imprimir(ptabulador,nX);
		imprimir(ptabulador,nX);
		imprimir("software libre",nX);
		imprimir(psig_pag,nX);

		printf("\n\n\n\n\n");

//		imprimir(c,nX);
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

		system(cad_temporal);
		return (1);
		}

	}
	printf("\n");
	return (0);
}
