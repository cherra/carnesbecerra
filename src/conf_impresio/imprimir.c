/*
Jose Oviedo Uribe.

Funcion para imprimir

Se envia como parametro nombrearchivo
y consulta

Asi que se borraria de la funcion main la declaracion de variables y se le cambiaria de nombre al main agregando como parametro los 2 campos anteriores

char nombrearchivo[50] = "prueba.txt"; //Nombre del archivo
char *consulta = "default"; //Nombre de la impresora


*/

#include <stdio.h>
#include <stdlib.h>
#include<string.h>
//char *tipo[] = { "serial","paralelo","cups","printfile"};

/*void quitar_escape(char *cadconf){
	//Funcion que quita los saltos de linea
	int i,tam;
	char cadtmp[50];
	tam = strlen(cadconf);
	for(i=0;i<tam;i++){
		if(cadconf[i] == '\n')
			break;
		else{
			cadtmp[i] = cadconf[i];
			cadtmp[i+1] = '\0';
		}
	}
	strcpy(cadconf,cadtmp);
}*/

/*int buscar_arreglo(char *cadconf){
	//En tipo se dice que arreglo se va a comparar.
	//tipo =  ----> tipo
	int num_elements;
	int salir = 0;
	int i, regresa;
	regresa = -1; //Valor que regresa en el return
	num_elements=sizeof(tipo)/sizeof(char*); //Calcula el tamao del arreglo
	i=-1;
	//printf("\nTotal arreglo: %d",num_elements);
	while (salir == 0 && i < num_elements){
		if(strcmp(cadconf, tipo[i]) == 0){
			//printf("\nEsto encontre: comparar: %s arreglo: %s",cadconf, nomb[i]);
			regresa = i;
			salir=1;
		}
		i++;
	}
	if(salir == 1){
		i--;
	}
	return (regresa);
}*/

int manda_imprimir(char nombrearchivo[50], char *consulta){
//char nombrearchivo[50] = "prueba.txt"; //Nombre del archivo
//char *consulta = "default"; //Nombre de la impresora
char impresora[100];

char ImpresoraConfig[] = "impresion_conf/impresoras.conf.txt"; //Nombre del archivo de configuracion
char c[1000]; //Aqui se guarda las cadenas a imprimir
char cadconf[50];
char *cadena_despues_de_trim;
int i, j, k, m;
int error = 0;
int impresiones = 1;
//Parametros.
int par1 = -1;
int par2 = -1;
char par3[50];
char par4[50];
FILE *fpt;
printf("\nSacando informacion de la configuracion %s",consulta);
/*CONFIGURACION DE LA IMPRESORA
*/
		fpt = fopen(ImpresoraConfig,"r");
		if(fpt == NULL){
			printf("\nERROR no se puede abrir el archivo de configuracion de las impresoras");
			return (1);
		}else{
			m=1;
			while(fgets(c, 255, fpt) != NULL){
				strcat(c," ");
				strcpy(cadconf,"");
				j=0;
				k=0;
				printf("\nCADENA: %s",c);
				for(i=0;i<strlen(c);i++){
					if(c[i] != '#'){
						if((c[i] == ' ' || c[i] == '\t') && (strcmp(cadconf," ") != 0)){
							j=0;
							quitar_escape(cadconf);
							//printf("\nAntes de las funciones trim: %s\n", cadconf);
							cadena_despues_de_trim = ltrim(rtrim(cadconf, ' '), ' ');
							sprintf(cadconf,"%s",cadena_despues_de_trim);
							cadena_despues_de_trim = ltrim(rtrim(cadconf, '\t'), '\t');
							sprintf(cadconf,"%s",cadena_despues_de_trim);
							//printf("\nDespues de las funciones trim: %s\n", cadconf);
							
							//printf("\n	Cadenas de conf: %s",cadconf);
							if(k==0){
								if(strcmp(cadconf, "dobleimpresion") == 0){
									impresiones = 2; //Doble impresion para impresora termica
								}else if(strcmp(cadconf, consulta) == 0){
									par1 = 0; //Indica que se metio
								}else{
									//error = 1;
									break; //Se sale del for
								}
								//printf("->nombre %s\n<-",cadconf);
							}else if(k==1){
								if( (par2 = buscar_arreglo(cadconf))  == -1){
									printf("\nNo encuentro nada %s",cadconf);
									break;
								}
								//printf("->Paralelo (0) o Serial (1) %s\n<-",cadconf);
							}else if(k==2){
								strcpy(par3,cadconf);
								//printf("->Puerto %s\n<-",cadconf);
							}else if(k==3){
								impresiones = atoi(cadconf);
								//printf("Numero de copias: %s\n<-",cadconf);
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
							//printf("\n----------> cadconf = %s",cadconf);
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
		if(par1 != -1){
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
		}
	if(error != 1){
		printf("\nCadena de la Impresion: ->%s<--",impresora);
		printf("\n");
		for(i=0;i<impresiones;i++)
			system(impresora);
		//system(impresora);   //DOBLE IMPRESION
		return(0);
	}else{
		printf("\nNo se puede imprimir, verifica el archivo de configuracion");
		printf("\n");
		return(-1);
	}
}
