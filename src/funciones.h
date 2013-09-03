#ifndef _FUNCIONES_H_
#define _FUNCIONES_H_
#include <stdio.h>
#include <mysql/mysql.h>
#include <string.h>
#include <stdlib.h>


//#include "dialogos2.h"

char *tipo[] = { "serial","paralelo","cups","printfile"};

/*Variable para poner error */
char myerr[220];
enum banderas{true,false} leerconf=true;
char ip[50]="";
char usuario[50]="";
char contrasena[50]="";
char bd[50]="";
int puerto=3306;

/*Conecta con la base de datos usando la informacion del archivo
de configuracion*/
int conecta_bd_2(MYSQL *);
MYSQL_RES *conecta_bd_3(char *SQL);

/*Ejecutar sentencias de Mysql y determinar si hubo error=-1 
o no error=0*/
int ejecutar(MYSQL *,char *);

/*Funcion que pone el error en la variables externa myerr
Si regresa -1, quiere decir que no se pudo determinar el 
error, regrsa diferente de -1 si el tipo de error fue 
regresado por el servidor*/
int error_mysql(MYSQL *);

//Funciones para eliminar espacios al principio y al final de una cadena
char* rtrim(char* string, char junk);
char* ltrim(char* string, char junk);
char tipo_de_interface();

int conecta_bd_2(MYSQL *conexion)
{
	FILE *fconfiguracionbd;
	int i=0;
	int j=0;
	char tipo[50]="";
	char dato[50]="";
	char tmp;
	char *error ;
	if(leerconf == true)
	{
		if((fconfiguracionbd = fopen(file_db_config,"r")))
		{
			printf("Si hay archivo...  \n");
			while(!feof(fconfiguracionbd))
			{
				tmp = fgetc(fconfiguracionbd);
				//printf("%c", tmp);
				if(tmp != ' ')
				{
					tipo[i] = tmp;
					i++;
				}else
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
					if(strcmp(tipo,"puerto") == 0)
						puerto = atoi(dato);
					strcpy(tipo,"");
					for(i=0;i<30;i++)
						tipo[i]='\0';
					i=0;
				}
			}
			fclose(fconfiguracionbd);
			leerconf=false;
		}else
		{
			printf("No se pudo abrir el archivo....");
				return -1;
		}
	}
/*	printf("IP: %s\n", ip);
	printf("Usuario: %s\n",usuario);
	printf("Contrasena: %s\n",contrasena);
	printf("BasedeDatos: %s\n",bd);*/
	mysql_init(conexion);
	if(conexion == NULL){
		printf("Error de memoria para MYSQL");
		return -1;
	}
	
	if(!(mysql_real_connect(conexion, ip, usuario, contrasena, bd, puerto, NULL, 0)))
		return -1;
	else
		return 1;
}
int error_mysql(MYSQL *conexion)
{
	//extern char myerr[220];
	if(conexion != NULL && mysql_error(conexion)){
		sprintf(myerr,"Error: %u %s\n",mysql_errno(conexion),mysql_error(conexion));
		return -1;
	}
	return 0;
}
int ejecutar(MYSQL *conexion, char *peticion){
	if(mysql_query(conexion,peticion) != 0){
		return -1;
	}else{
		return 0;
	}
}

int arreglar_rfc(char *rfc)
{
	int i,j,mal=0;
	char temp[14];
	char temp2[20];
	char elbueno[16];
	
	strcpy(temp2,rfc);
	
	j=0;
	for (i=0; i<strlen(temp2); i++)
	{
		temp2[i] = toupper (temp2[i]);
		if(    ((temp2[i] >= 65) && (temp2[i] <= 90) )     ||     ((temp2[i] >= 48) && (temp2[i] <= 57))   )
		{
			rfc[j] = temp2[i];
			j++;
		}
	}
	rfc[j]='\0';
	
	if((strlen(rfc) > 13) ||(strlen(rfc) < 12))
		mal = 1;
	else
	{
		if(strlen(rfc) == 12)
		{
			temp[0] = 32;
			for(i=0;i<12;i++)			//SE ARRGLA EL RFC PARA QUE MIDA 13
				temp[i+1] = rfc[i];
			temp[13] = '\0';
		}
		else
			strcpy(temp, rfc);

		if(temp[0] == 32)
			for(i=1;i<4;i++)
			{
				if((temp[i] < 65) || (temp[i] > 90))
					mal = 1;
			}
		else								//SE CHECAN LAS INICIALES
			for(i=0;i<4;i++)
			{
				if((temp[i] < 65) || (temp[i] > 90))
					mal = 1;
			}

		for(i=4;i<10;i++)						//SE CHECAN LOS NUMEROS
		{
			if((temp[i] < 48) || (temp[i] > 57))
				mal = 1;
		}

		for(i=10;i<13;i++)						//SE CHECA LA HOMOCLAVE
		{
			if(((temp[i] < 48) || (temp[i] > 57)) && ((temp[i] < 65) || (temp[i] > 90)))
				mal = 1;
		}

		if(mal == 0)
		{
			for(i=0;i<4;i++)
				elbueno[i]=temp[i];
			elbueno[4] = '-';
			for(i=5;i<11;i++)
				elbueno[i]=temp[i-1];
			elbueno[11] = '-';
			for(i=12;i<15;i++)
				elbueno[i]=temp[i-2];
			elbueno[15]='\0';

			strcpy(rfc, elbueno);
			printf("El RFC Bueno: %s\n", elbueno);
		}
	}
	return(mal);
}

#endif


MYSQL_RES *conecta_bd_3(char *SQL)
{
	MYSQL mysql;
	MYSQL_RES *res;
	
	FILE *fconfiguracionbd;
	int i=0;
	int j=0;
	char tipo[50]="";
	char dato[50]="";
	char tmp;
	char *error ;
	
	if(leerconf == true)
	{
		if((fconfiguracionbd = fopen(file_db_config,"r")))
		{
			printf("Si hay archivo...  \n");
			while(!feof(fconfiguracionbd))
			{
				tmp = fgetc(fconfiguracionbd);
				//printf("%c", tmp);
				if(tmp != ' ')
				{
					tipo[i] = tmp;
					i++;
				}else
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
			leerconf=false;
		}else
		{
			printf("No se pudo abrir el archivo....");
				return NULL;
		}
	}
	/*printf("IP: %s\n", ip);
	printf("Usuario: %s\n",usuario);
	printf("Contrasena: %s\n",contrasena);
	printf("BasedeDatos: %s\n",bd);*/
	mysql_init(&mysql);
	if(&mysql == NULL)
	{
		//Err_Info2(NULL,"Error de memoria para MYSQL");
		printf("Error de memoria para MYSQL");
		return NULL;
	}
	
	if(!(mysql_real_connect(&mysql, ip, usuario, contrasena, bd, puerto, NULL, 0)))
	{
		mysql_close (&mysql);
		//Err_Info2(NULL,"Error al iniciar la conexion.");
		return NULL;
	}
		
	if(mysql_query(&mysql,SQL) != 0){
		//Err_Info2(NULL,"Error en la cadena SQL.");
		//Err_Info2(NULL,mysql_error (&mysql));
		printf ("MySQL Error : %s",mysql_error (&mysql));
		mysql_close (&mysql);
		return NULL;
	}else{
		res = mysql_store_result (&mysql);
		printf ("Se envia el resultado\n");
		mysql_close (&mysql);
		sprintf (SQL,"OK");
		return res;
	}
}


//Funcion TRIM
//Jose Oviedo - 4 Julio 2003
void trim(char *cadena){
	char cadTMP[500];
	char cadTMP2[500];
	int i, k, j, tam=0;
	int bandera=0;
	strcpy(cadTMP2,"");
	strcpy(cadTMP,cadena);
	tam = strlen(cadTMP);

	printf("\nCadena1: %s",cadena);
	for(i=0;i<tam;i++){
		if(cadTMP[i] != ' '){
			k=0;
			for(j=i;j<tam;j++){
				cadTMP2[k] = cadTMP[j];
				cadTMP2[k+1] = '\0';
				k++;
			}
			break;
		}
	}
	strcpy(cadTMP,cadTMP2);

	strcpy(cadTMP2,"");
	tam = strlen(cadTMP) - 1;
	printf("\ntam:->%d<-\n	",tam);
	for(i=tam;i>=0;i--){
		if(cadTMP[i] != ' '){
			k=0;
			//printf("\nCadena3:->%c<-",cadTMP[i]);
			for(j=0;j<=i;j++){
				//printf("\nCadena4:->%c<-",cadTMP[j]);
				cadTMP2[k] = cadTMP[j];
				cadTMP2[k+1] = '\0';
				k++;
			}
			break;
		}
	}
	strcpy(cadena,cadTMP2);
	printf("\nCadena2:->%s<-",cadena);

}

//Busca un valor de una cadena en un arreglo
int buscar_arreglo(char *cadconf){
	//En tipo se dice que arreglo se va a comparar.
	//tipo =  ----> tipo
	int num_elements;
	int salir = 0;
	int i, regresa;
	regresa = -1; //Valor que regresa en el return
	num_elements=sizeof(tipo)/sizeof(char*); //Calcula el tamao del arreglo
	i=0;
	//printf("\nTotal arreglo: %d",num_elements);
	
	while (salir == 0 && i < num_elements){
		printf("\n### Comparar: %s arreglo: %s I = %d",cadconf, tipo[i],i);
		if(strcmp(cadconf, tipo[i]) == 0){
			printf("\nEsto encontre: comparar: %s arreglo: %s I = %d",cadconf, tipo[i], i);
			regresa = i;
			salir=1;
		}
		i++;
	}
	if(salir == 1){
		i--;
	}
	return (regresa);
}

//Funcion que nos dice si existen espacios en blanco
int existen_espacios(char *cadena){
	int i=0;
	int tam;
	int bandera = -1;
	tam = strlen(cadena);
	for(i=0;i<tam;i++){
		if(cadena[i] == ' '){
			bandera = 0;
			break;
		}
	}
	return(bandera);
}

void quitar_escape(char *cadconf){
	//Jose Oviedo Uribe 2 /Julio / 2003
	//Elimina el caracter de salto de linea \n de una cadena.
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
}

int obtener_row_seleccionada (GtkWidget *clist, int inicio);

int obtener_row_seleccionada (GtkWidget *clist, int inicio)
{
	GList *row_sel;
	int i;
	gboolean b=FALSE;
	
	row_sel = GTK_CLIST(clist)->row_list;

	// Ciclo para localizar la row de donde se va a comezar a leer.	
	//if (i<inicio)
	for (i=0 ; i<inicio; i++)
		row_sel = g_list_next (row_sel);
	
	for (i=0; row_sel; row_sel = g_list_next (row_sel), i++)
		if ((GTK_CLIST_ROW (row_sel)->state == GTK_STATE_SELECTED))
		{
			b=TRUE;
			break;
		}
	
	if (b)
		return (i);
	else
		return (-1);
}

int obtener_row_por_valor(GtkWidget *clist, gchar *valorBuscado, int columna);

int obtener_row_por_valor(GtkWidget *clist, gchar *valorBuscado, int columna)
{
	gchar *valorActual;
	int i;
	gboolean b=FALSE;
	
	for (i=0; i < GTK_CLIST(clist)->rows; i++)
	{
		gtk_clist_get_text(GTK_CLIST(clist) ,i,columna,&valorActual);
		if (strcmp(valorBuscado,valorActual) == 0)
		//if ((GTK_CLIST_ROW (row_sel)->state == GTK_STATE_SELECTED))
		{
			b=TRUE;
			break;
		}
	}
	
	if (b)
		return (i);
	else
		return (-1);
}


void formatea_cadena(char *cadena);

void formatea_cadena(char *cadena)
{
	int i,j;
	char cadena_tmp[50]="",cadena_tmp2[50];
	char decimales[4];
	char tres_digitos[4];
	gboolean tiene_punto=FALSE;
	int cont_coma=0;
	int cont_decimales=0;
	
	//printf ("\n####INICIA LA CONVERSION DE %s.\n",cadena);
	
	for (i=0; i<strlen(cadena); i++)
	{
		if (cadena[i] == '.')
		{
			tiene_punto = TRUE;
			cont_decimales=0;
		}
		cont_decimales ++;
	}
	
	//printf ("CONT. DEC = %d\n",cont_decimales);
	
	if (! tiene_punto)
	{
		strcat (cadena,".00");
	}
	else
	{
		if (cont_decimales == 2)
		{
			strcat (cadena,"0");
		}
	}
	
	//se ponen el . y los dos decimales.
	decimales[2] = cadena[strlen(cadena)-1];
	decimales[1] = cadena[strlen(cadena)-2];
	decimales[0] = cadena[strlen(cadena)-3];
	decimales[3] = '\0';
	
	//printf ("### DECIMALES = %s\n",decimales);
	
	cont_coma=0;
	for (i=strlen(cadena)-4; i>=0; i--)
	{
		tres_digitos[2-cont_coma] = cadena[i];
		cont_coma++;
		//printf ("DIGITOP = %c\n",cadena[i]);
		if (cont_coma == 3)
		{
			tres_digitos[3]='\0';
			//printf ("TRIO = %s\n",tres_digitos);
			sprintf (cadena_tmp2,",%s%s",tres_digitos,cadena_tmp);
			strcpy (cadena_tmp,cadena_tmp2);
			cont_coma = 0;
		}
	}
	
	
	//Se acomoda la cadena del tri final rellenandola de espacios.
	if (cont_coma == 0)
	{
		cadena_tmp[0] = ' ';
		tres_digitos[0] = ' ';
		tres_digitos[1] = ' ';
		tres_digitos[2] = ' ';
	}
	if (cont_coma == 1)
	{
		tres_digitos[0] = ' ';
		tres_digitos[1] = ' ';
	}
	if (cont_coma == 2)
	{
		tres_digitos[0] = ' ';
	}
	tres_digitos[3] = '\0';
		
	//printf ("DIGITOS FINALES = %s\n",tres_digitos);
	sprintf (cadena,"%s%s%s",tres_digitos,cadena_tmp,decimales);
	
	//printf ("TERMINA LA CONVERSION.\n");
	//printf ("FORMATEADAD = %s #####\n",cadena);
}

//FUNCION PARA RELLENAR UNA CADENA CON UN CARACTER DADO
//Con  0 rellena por la izquierda
//si no rellena por la derecha

void rellenar_cadena(char cadena[100], char *caracter, int len, int alig)
{
	char tmp[100]="";
	int i;

	if (alig == 0)
	{
		for (i=0; i<(len-strlen(cadena)); i++)
		{
			strcat (tmp, caracter);
		}
		
		strcat (tmp, cadena);
		strcpy (cadena, tmp);
	}
	else
	{
		for (i=strlen(cadena); i<len; i++)
		{
			strcat (cadena, caracter);
		}
	}
}

char* rtrim(char* string, char junk)
{
    char* original = string + strlen(string);
    while(*--original == junk);
    *(original + 1) = '\0';
    return string;
}

char* ltrim(char *string, char junk)
{
    char* original = string;
    char *p = original;
    int trimmed = 0;
    do
    {
        if (*original != junk || trimmed)
        {
            trimmed = 1;
            *p++ = *original;
        }
    }
    while (*original++ != '\0');
    return string;
}


char tipo_de_interface()
{
	FILE *fconfiguracionbd;
	char tmp;
	char tipo[30]="";
	char dato[50]="";
	char interface='1';
	int i=0;
	int j=0;

        printf("El archivo de configuracion de conexión: %s \n", file_db_config);
	if((fconfiguracionbd = fopen(file_db_config,"r")))
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
				
				printf("El tipo: %s\n",tipo);
				if(strcmp(tipo,"tipo") == 0){
					if(strcmp(dato,"produccion")==0)
						interface = '2';
					else
						interface = '1';
					//interface = tipo;
				}
				strcpy(tipo,"");
				for(i=0;i<30;i++)
					tipo[i]='\0';
				i=0;
			}
		}
		fclose(fconfiguracionbd);
	}
	else
		printf("No se pudo obtener el tipo de interface, error al abrir el archivo de configuracion\n");

	if(interface == '2')
	{
		gtk_widget_hide(GTK_WIDGET(data.menubar));
		/*gtk_widget_hide(GTK_WIDGET(data.fixed22));
		gtk_widget_hide(GTK_WIDGET(data.radiobtncaja));
		gtk_widget_hide(GTK_WIDGET(data.radiobtncajap));
		gtk_widget_hide(GTK_WIDGET(data.radiobtncajafactura));
		gtk_widget_hide(GTK_WIDGET(data.radiobtncajapcredito));
		gtk_widget_hide(GTK_WIDGET(data.chkcajapedido));
		gtk_widget_hide(GTK_WIDGET(data.label1294));
		gtk_widget_hide(GTK_WIDGET(data.label1296));
		gtk_widget_hide(GTK_WIDGET(data.label1298));
		gtk_widget_hide(GTK_WIDGET(data.lbliva0));
		gtk_widget_hide(GTK_WIDGET(data.lblsubtotal));
		gtk_widget_hide(GTK_WIDGET(data.lbliva15));
		gtk_widget_hide(GTK_WIDGET(data.lblantesdetotal));*/
		gtk_widget_hide(GTK_WIDGET(data.lbltotal));
		/*gtk_widget_hide(GTK_WIDGET(data.label1096));
		gtk_widget_hide(GTK_WIDGET(data.lbldescuento));
		gtk_widget_hide(GTK_WIDGET(data.btncaja_descuento));
		gtk_widget_hide(GTK_WIDGET(data.label1167));
		gtk_widget_hide(GTK_WIDGET(data.label187));
		gtk_widget_hide(GTK_WIDGET(data.label1048));
		gtk_widget_hide(GTK_WIDGET(data.lblkilos));
		gtk_widget_hide(GTK_WIDGET(data.lblpiezas));*/

		gtk_widget_set_sensitive(GTK_WIDGET(data.menubar),FALSE);
		gtk_widget_set_sensitive(GTK_WIDGET(data.fixed22),FALSE);
		/*gtk_widget_set_sensitive(GTK_WIDGET(data.radiobtncaja),FALSE);
		gtk_widget_set_sensitive(GTK_WIDGET(data.radiobtncajap),FALSE);
		gtk_widget_set_sensitive(GTK_WIDGET(data.radiobtncajafactura),FALSE);
		gtk_widget_set_sensitive(GTK_WIDGET(data.radiobtncajapcredito),FALSE);*/
		gtk_widget_set_sensitive(GTK_WIDGET(data.chkcajapedido),FALSE);
		/*
		gtk_widget_hide(GTK_WIDGET(data.label1294));
		gtk_widget_hide(GTK_WIDGET(data.label1296));
		gtk_widget_hide(GTK_WIDGET(data.label1298));
		gtk_widget_hide(GTK_WIDGET(data.lbliva0));
		gtk_widget_hide(GTK_WIDGET(data.lblsubtotal));
		gtk_widget_hide(GTK_WIDGET(data.lbliva15));
		gtk_widget_hide(GTK_WIDGET(data.lblantesdetotal));
		gtk_widget_hide(GTK_WIDGET(data.lbltotal));
		gtk_widget_hide(GTK_WIDGET(data.label1096));
		gtk_widget_hide(GTK_WIDGET(data.lbldescuento));
		gtk_widget_hide(GTK_WIDGET(data.lblpedido));
		//gtk_widget_set_sensitive(GTK_WIDGET(data.fixed25),FALSE);*/
		gtk_widget_hide(GTK_WIDGET(data.txttelefono));
		gtk_widget_hide(GTK_WIDGET(data.label21));
		gtk_widget_hide(GTK_WIDGET(data.btn_cajanuevocliente));
		gtk_widget_set_sensitive(GTK_WIDGET(data.radioTipoVentaVenta),FALSE);
		gtk_widget_set_sensitive(GTK_WIDGET(data.radioTipoVentaPrueba),FALSE);
		gtk_widget_set_sensitive(GTK_WIDGET(data.radioTipoVentaConsumo),FALSE);

		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(data.radiobtncaja),TRUE);
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(data.radiobtncajap),TRUE);
		gtk_widget_set_sensitive(data.radiobtncaja, FALSE);
		gtk_widget_set_sensitive(data.radiobtncajap, FALSE);
		gtk_widget_set_sensitive(data.radiobtncajafactura, FALSE);
		gtk_widget_set_sensitive(data.radiobtncajapcredito, FALSE);
		
		
		//gtk_container_remove(GTK_CONTAINER(data.table1), data.table2);
		//gtk_table_attach_defaults(GTK_TABLE(data.table1),GTK_WIDGET(data.table2),0,2,2,4);
		//gtk_widget_show(data.table2);
		gtk_container_remove(GTK_CONTAINER(data.table2), data.scrolledwindow3);
		gtk_table_attach_defaults(GTK_TABLE(data.table2),GTK_WIDGET(data.scrolledwindow3),0,4,1,13);
		gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (data.scrolledwindow3), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
		gtk_widget_show(data.scrolledwindow3);
		
		gtk_widget_set_size_request (data.frame3, 280, 205);
		gtk_widget_set_size_request (data.scrolledConceptoVale, 185, 190);

		gtk_signal_emit_by_name(GTK_OBJECT(data.radioTipoVentaVale),  "toggled");
	}
	return (interface);
}

/*
 * Create a new string with [substr] being replaced by [replacement] in [string]
 * Returns the new string, or NULL if out of memory.
 * The caller is responsible for freeing this new string.
 */
char *
str_replace( const char *string, const char *substr, const char *replacement ){
  char *tok = NULL;
  char *newstr = NULL;

  tok = strstr( string, substr );
  if( tok == NULL ) return strdup( string );
  newstr = malloc( strlen( string ) - strlen( substr ) + strlen( replacement ) + 1 );
  if( newstr == NULL ) return NULL;
  memcpy( newstr, string, tok - string );
  memcpy( newstr + (tok - string), replacement, strlen( replacement ) );
  memcpy( newstr + (tok - string) + strlen( replacement ), tok + strlen( substr ), strlen( string ) - strlen( substr ) - ( tok - string ) );
  memset( newstr + strlen( string ) - strlen( substr ) + strlen( replacement ), 0, 1 );
  return newstr;
}
