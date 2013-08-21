#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <mysql/mysql.h>

#include "callbacks.h"
#include "interface.h"
//#include "support.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

/////////////////////////////////////////////////////////////////////////////
//ESTO SE BORRA SI YA EXISTE!
/////////////////////////////////////////////////////////////////////////////
/***********************************************/
//#include "dialogos.h" //Libreria para mensajes
//#include "funciones.h" //Libreria de funciones comunes

/*
# # # # # # # # # #   NOTA # # # # # # # # # # # # # # 

Hay que agregar las siguientes funciones a la libreria de funciones principal (si no estan):

void quitar_escape(char *cadconf)
int buscar_arreglo(char *cadconf)
void trim(char *cadena)
int obtener_row_seleccionada (GtkWidget *clist, int inicio)
double suma_clist  (GtkCList *clist, gint column )
void trim(char *cadena)
int existen_espacios(char *cadena)

*/

//FUNCION DE IMPRESION
#include "imprimir.c"

GtkWidget *ventana_conf_impresion; //Widget que hace referencia a la ventana pedidos_moto_llegada
//char *tipo[] = { "serial","paralelo","cups","printfile"};

/***********VARIABLES MYSQL***************/
MYSQL mysql;
MYSQL_RES *resultado, *resultado2;
MYSQL_RES *res;
MYSQL_ROW row,row2;

GtkWidget *entry_global; //BORRAME si ya existo.
int obtener_row_seleccionada (GtkWidget *clist, int inicio); //BORRAME SI YA ESTOY
//int er = 0; //Variable de error
char Errores[200];

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
		printf("Si hay archivo...\n");
		while(!feof(fconfiguracionbd))
		{
			tmp = fgetc(fconfiguracionbd);
			printf("%c", tmp);
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
		printf("IP: %s\n", ip);
		printf("Usuario: %s\n",usuario);
		printf("Contrasena: %s\n",contrasena);
		printf("BasedeDatos: %s\n",bd);

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

////////////////////////////////////////////////////////////////////////////
///HASTA AQUI SE PUEDE BORRAR



void conv_impresoras(char *entry_text){
	char consulta[100];
	//Convierte el texto de las cajas a algo entendible para el archivo de configuracion.
	if(strcmp(entry_text,"Predeterminada") == 0)
		strcpy(consulta,"default");
	else if(strcmp(entry_text,"Credito") == 0)
		strcpy(consulta,"credito");
	else if(strcmp(entry_text,"Contado") == 0)
		strcpy(consulta,"contado");
	else if(strcmp(entry_text,"Factura") == 0)
		strcpy(consulta,"factura");
	else if(strcmp(entry_text,"Corte de caja parcial") == 0)
		strcpy(consulta,"corte_caja");
	else if(strcmp(entry_text,"Deudores de credito") == 0)
		strcpy(consulta,"credito_abono");
	else if(strcmp(entry_text,"Corte de caja final/global") == 0)
		strcpy(consulta,"corte_all_caja");
	else if(strcmp(entry_text,"Pedido para el carnicero") == 0)
		strcpy(consulta,"pedidos_paso1");

	strcpy(entry_text,consulta);
}


void
on_combo_entry_cmbImpresion_changed_conf_impresion
                                        (GtkEditable     *editable,
                                        gpointer         user_data)
{
GtkWidget *cmbImpresora, *txtPar1, *txtPar2, *cmbImpresion;
gchar *entry_text;
//Nombre del archivo de configuracion
char consulta[50];
char ImpresoraConfig[] = "impresion_conf/impresoras.conf.txt"; //Nombre del archivo de configuracion
char c[1000]; //Aqui se guarda las cadenas a imprimir
char cadconf[100];
char conv_impresora_txt[100];
int i, j, k, m;
int error = 0;
//Parametros.
int par1 = -1;
int par2 = -1;
char par3[50];
char par4[50];
FILE *fpt;

txtPar1 = lookup_widget(GTK_WIDGET(editable),"txtPar1");
txtPar2 = lookup_widget(GTK_WIDGET(editable),"txtPar2");
cmbImpresora  = lookup_widget(ventana_conf_impresion,"combo_entry_cmbImpresora");
cmbImpresion  = lookup_widget(ventana_conf_impresion,"combo_entry_cmbImpresion");


entry_text = gtk_editable_get_chars(GTK_EDITABLE(cmbImpresion),0,-1);
strcpy(consulta,entry_text);
conv_impresoras(consulta);

printf("\n\nSacando informacion de la configuracion %s",consulta);

/*CONFIGURACION DE LA IMPRESORA
*/
fpt = fopen(ImpresoraConfig,"r");
if(fpt == NULL){
	printf("\nERROR no se puede abrir el archivo de configuracion de las impresoras");
	error = 1;
}else{
	m=1;
	while(fgets(c, 255, fpt) != NULL){
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
					//printf("\n	Cadenas de conf: %s",cadconf);
					if(k==0){
						if(strcmp(cadconf, consulta) == 0){
							par1 = 0; //Indica que se metio
						}else{
							//error = 1;
							break; //Se sale del for
						}
						//printf("->nombre %s\n<-",cadconf);
					}else if(k==1){
						//printf("\nCad par2: %s",cadconf);
						if((par2 = buscar_arreglo(cadconf)) == -1){
							printf("\nNo encuentro nada %s",cadconf);
							break;
						}
						//printf("->Paralelo (0) o Serial (1) %s\n<-",cadconf);
					}else if(k==2){
						//printf("\nCad par3: %s",cadconf);
						strcpy(par3,cadconf);
						//printf("->Puerto %s\n<-",cadconf);
					}else if(k==3){
						//printf("\nCad par3: %s",cadconf);
						strcpy(par4,cadconf);
						//printf("->Puerto %s\n<-",cadconf);
					}
					strcpy(cadconf," ");
					k++;
				}else{
				        if(c[i] != ' '){
						cadconf[j] = c[i];
						cadconf[j+1] = '\0';
						j++;
					}
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

	if(strcmp(tipo[par2],"serial") == 0)
		strcpy(conv_impresora_txt,"Serial");
	else if(strcmp(tipo[par2],"paralelo") == 0)
		strcpy(conv_impresora_txt,"Paralelo");
	else if(strcmp(tipo[par2],"cups") == 0)
		strcpy(conv_impresora_txt,"CUPS");
	else if(strcmp(tipo[par2],"printfile") == 0)
		strcpy(conv_impresora_txt,"Print File 32");
	else
		strcpy(conv_impresora_txt,"");
	printf("\nParametro 0: %s",conv_impresora_txt);
	switch(par2){
		case 0: //Serial
		case 1: //Paralela
			printf("\n Parametro1: %s",par3);
   			gtk_entry_set_text (GTK_ENTRY(cmbImpresora),conv_impresora_txt);
			gtk_entry_set_text (GTK_ENTRY(txtPar1),par3);
			gtk_entry_set_text (GTK_ENTRY(txtPar2),"");
		break;
		case 2: //cups
			printf("\n Parametro1: %s",par3);
			printf("\n Parametro2: %s",par4);
			printf("\nCups????: %s",tipo[par2]);
			gtk_entry_set_text (GTK_ENTRY(txtPar2),par3);
			gtk_entry_set_text (GTK_ENTRY(txtPar1	),par4);
			gtk_entry_set_text (GTK_ENTRY(cmbImpresora),conv_impresora_txt);
		break;
		case 3: //printfile
			gtk_entry_set_text (GTK_ENTRY(cmbImpresora),conv_impresora_txt);
			gtk_entry_set_text (GTK_ENTRY(txtPar1),"");
			gtk_entry_set_text (GTK_ENTRY(txtPar2),"");
		break;
		default:
			error = 1;
		break;
	}
}

printf("\n");
}


void
on_combo_entry_cmbImpresora_changed_conf_impresion
                                        (GtkEditable     *editable,
                                        gpointer         user_data)
{
GtkWidget *lblPar1, *txtPar1, *txtPar2, *lblPar2;
gchar *entry_text;
int index = -1;
int error = 0;

lblPar1 = lookup_widget(GTK_WIDGET(editable),"lblPar1");
txtPar1 = lookup_widget(GTK_WIDGET(editable),"txtPar1");
lblPar2 = lookup_widget(GTK_WIDGET(editable),"lblPar2");
txtPar2 = lookup_widget(GTK_WIDGET(editable),"txtPar2");


entry_text = gtk_editable_get_chars(GTK_EDITABLE(editable),0,-1);
printf("\nTexto: %s",entry_text);

if(strcmp(entry_text,"CUPS") == 0)
	index =  2;
else if(strcmp(entry_text,"Paralelo") == 0)
	index =  1;
else if(strcmp(entry_text,"Serial") == 0)
	index =  0;
else if(strcmp(entry_text,"Print File 32") == 0)
	index =  3;



	if(index != -1){
		switch(index){
			case 0: //Serial
			case 1: //Paralela
				gtk_widget_set_child_visible    (lblPar1,TRUE);
				gtk_widget_set_child_visible    (txtPar1,TRUE);
				gtk_label_set_markup              (GTK_LABEL(lblPar1),"Puerto" );
				gtk_widget_set_child_visible    (lblPar2,FALSE);
				gtk_widget_set_child_visible    (txtPar2,FALSE);
			break;
			case 2: //cups
				gtk_widget_set_child_visible    (lblPar1,TRUE);
				gtk_widget_set_child_visible    (txtPar1,TRUE);
				gtk_label_set_markup              (GTK_LABEL(lblPar1),"Dirección IP" );
				gtk_widget_set_child_visible    (lblPar2,TRUE);
				gtk_widget_set_child_visible    (txtPar2,TRUE);
			break;
			case 3: //printfile
				gtk_widget_set_child_visible    (lblPar1,FALSE);
				gtk_widget_set_child_visible    (txtPar1,FALSE);
				gtk_widget_set_child_visible    (lblPar2,FALSE);
				gtk_widget_set_child_visible    (txtPar2,FALSE);
			break;
			default:
				error = 1;
			break;
		}
	}

printf("\n");
}


void
on_conf_impresion_show_conf_impresion  (GtkWidget       *widget,
                                        gpointer         user_data)
{
GtkWidget *cmbImpresion ;
ventana_conf_impresion = widget;

cmbImpresion  = lookup_widget(ventana_conf_impresion,"combo_entry_cmbImpresion");
gtk_entry_set_text (GTK_ENTRY(cmbImpresion)," ");
gtk_entry_set_text (GTK_ENTRY(cmbImpresion),"Predeterminada");



printf("\nIniciando configuracion de la impresora");

printf("\n");
}


void
on_btn_aceptar_conf_impresion_clicked_conf_impresion
                                        (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *txtPar1, *txtPar2, *cmbImpresora, *cmbImpresion;
gchar *txtPar1_char, *txtPar2_char, *cmbImpresora_char, *cmbImpresion_char;

char ImpresoraConfig[] = "impresion_conf/impresoras.conf.txt"; //Nombre del archivo de
char ImpresoraConfigTMP[] = "impresion_conf/impresoras.conf_temporal"; //Nombre del archivo de
char cadSystem[100];
int index; //Indice de tipo de impresion
char consulta[50];
char c[1000]; //Aqui se guarda las cadenas a imprimir
char cTMP[1000]; //Aqui se guarda la cadena temporal
char cadconf[50];
int i, j, k, m;
int error = 0;
//Parametros.
int par1 = -1;
int par2 = -1;
char par3[50];
char par4[50];
FILE *fpt, *fptTMP;
int bandera = 0;


txtPar1 = lookup_widget(ventana_conf_impresion,"txtPar1");
txtPar2 = lookup_widget(ventana_conf_impresion,"txtPar2");
cmbImpresora  = lookup_widget(ventana_conf_impresion,"combo_entry_cmbImpresora");
cmbImpresion  = lookup_widget(ventana_conf_impresion,"combo_entry_cmbImpresion");

cmbImpresion_char = gtk_editable_get_chars(GTK_EDITABLE(cmbImpresion),0,-1);
cmbImpresora_char = gtk_editable_get_chars(GTK_EDITABLE(cmbImpresora),0,-1);
txtPar1_char = gtk_editable_get_chars(GTK_EDITABLE(txtPar1),0,-1);
txtPar2_char = gtk_editable_get_chars(GTK_EDITABLE(txtPar2),0,-1);
printf("\n	tipo impresion: %s",cmbImpresora_char);
	if(strcmp(cmbImpresora_char,"CUPS") == 0)
		index =  2;
	else if(strcmp(cmbImpresora_char,"Paralelo") == 0)
		index =  1;
	else if(strcmp(cmbImpresora_char,"Serial") == 0)
		index =  0;
	else if(strcmp(cmbImpresora_char,"Print File 32") == 0)
		index =  3;

	trim(txtPar1_char);
	trim(txtPar2_char);
	if(index == 2){
		if(strcmp(txtPar1_char, "") == 0 || strcmp(txtPar2_char, "") == 0 ){
			if(strcmp(txtPar1_char, "") == 0 )
				gtk_widget_grab_focus(txtPar1);
			else if(strcmp(txtPar2_char, "") == 0 )
				gtk_widget_grab_focus(txtPar2);
			Info("Completa los campos");
			error = 1;
		}
	}else if((index == 0) || (index==1))
	{
		if(strcmp(txtPar1_char, "") == 0 ){
			gtk_widget_grab_focus(txtPar1);
			Info("Completa los campos");
			error = 1;
		}
	}else if(index == 3) //Solo para verificar que se metio
	{
		error = 0;
	}else{
		Info("Error al seleccionar el tipo de impresora");
		error = 1;
	}

	if(index == 2){
		if( (existen_espacios(txtPar1_char) == 0) || existen_espacios(txtPar2_char) == 0){
			if(existen_espacios(txtPar1_char) == 0)
				gtk_widget_grab_focus(txtPar1);
			else if(existen_espacios(txtPar2_char) == 0)
				gtk_widget_grab_focus(txtPar2);

			Info("No deben de existir espacios en blanco entre caracteres");
			error = 1;
		}
	}else if((index == 0) || (index==1))
	{
		if (existen_espacios(txtPar1_char) == 0){
			gtk_widget_grab_focus(txtPar1);
		Info("No deben de existir espacios en blanco entre caracteres");
		error = 1;
		}
	}







	if (error != 1){
		if (!(fptTMP=fopen(ImpresoraConfigTMP,"w"))) /* controlamos si se produce un error */
		{
			printf("Error al abrir el archivo temporal");
			error = 1; /* abandonamos el programa */
		}
		else
		{
			//Realiza un respaldo del archivo de configuracion
			sprintf(cadSystem,"cp %s %s.bak", ImpresoraConfig, ImpresoraConfig);
			printf("\nComando: %s",cadSystem);
			system(cadSystem);

			fpt = fopen(ImpresoraConfig,"r");
			if(fpt == NULL){
				printf("\nERROR no se puede abrir el archivo de configuracion de las impresoras");
				error = 1;
			}else{
				//Convierte a formato entendible por el archivo de conf.
				conv_impresoras(cmbImpresion_char);
				strcpy(consulta,cmbImpresion_char);
				m=1;
				while(fgets(c, 255, fpt) != NULL){
					strcat(c,"");
					strcpy(cTMP,c);
					strcpy(cadconf,"");
					j=0;
					k=0;
					//printf("\n\nCADENA: %s",c);
					for(i=0;i<strlen(c);i++){
						if(c[i] != '#'){
							if(c[i] == ' ' && (strcmp(cadconf," ") != 0)){
								j=0;
								//quitar_escape(cadconf);
								//printf("\nCadenas de conf: %s",cadconf);
								if(k==0){
									printf("\n\nCADENA2: %s",cadconf);
									if(strcmp(cadconf, consulta) == 0){
										bandera = 1;
										printf("\nMe meti: %s",cadconf);
										strcpy(cTMP,consulta);
									}else{
										break; //Se sale del for
									}
									//printf("->nombre %s\n<-",cadconf);
								}else if(k==1){
										strcat(cTMP," ");
										strcat(cTMP,tipo[index]);
										printf("\nAqui guardo: %s",tipo[index]);
										if(index == 2){
											strcat(cTMP,"  ");
											strcat(cTMP,txtPar2_char);
											strcat(cTMP,"   ");
											strcat(cTMP,txtPar1_char);
											strcat(cTMP,"\n");
										}else if((index == 0) || (index==1))
										{
											strcat(cTMP," ");
											strcat(cTMP,txtPar1_char);
											strcat(cTMP,"\n");
										}else if(index == 3)
										{
											strcat(cTMP," ");
											strcat(cTMP,"\n");
										}

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
					printf("\n\nCADENA: %s",cTMP);
					fprintf(fptTMP,"%s",cTMP);
				}
				fclose(fpt);
			}
			if(bandera == 0){ //significa que nunca se metio a guardar algo.
				strcpy(cTMP,consulta);
				strcat(cTMP," ");
				strcat(cTMP,tipo[index]);
				if(index == 2){
					strcat(cTMP,"  ");
					strcat(cTMP,txtPar2_char);
					strcat(cTMP,"   ");
					strcat(cTMP,txtPar1_char);
					strcat(cTMP,"\n");
				}else if((index == 0) || (index==1))
				{
					strcat(cTMP," ");
					strcat(cTMP,txtPar1_char);
					strcat(cTMP,"\n");
				}else if(index == 3)
				{
					strcat(cTMP," ");
					strcat(cTMP,"\n");
				}
				fprintf(fptTMP,"%s",cTMP);
			}
			fclose(fptTMP);
			//Copia el archivo Temporal con el original
			sprintf(cadSystem,"mv %s %s", ImpresoraConfigTMP, ImpresoraConfig);
			printf("\nComando: %s",cadSystem);
			system(cadSystem);
			Info("Se guardaron los cambios");
			gtk_widget_grab_focus(cmbImpresion);
		}
	}
printf("\n");

//if ( manda_imprimir("imprimir.c","default") == 0)

}


void
on_btnCancelar_conf_impresion_clicked_conf_impresion
                                        (GtkButton       *button,
                                        gpointer         user_data)
{
printf("\nMe destruyen configuracion de la impresora");
gtk_widget_destroy(GTK_WIDGET(button));
printf("\n");
}

