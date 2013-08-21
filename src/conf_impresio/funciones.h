/*
   Creada por Eder y modificada por Oviedo.
   Se le envia el Widget de la clist y el contador de inicio de donde empezara la cuenta
   Regresa el numero de columna en donde este seleccionada, en caso contrario regresaria un -1

*/
char *tipo[] = { "serial","paralelo","cups","printfile"};


int obtener_row_seleccionada (GtkWidget *clist, int inicio)
{
	//Funcion que indica en donde se tiene un row seleccionado de una lista
	GList *row_sel;
	int i, row_total;
	gboolean b=FALSE;

	row_sel = GTK_CLIST(clist)->row_list;

	// Ciclo para localizar la row de donde se va a comezar a leer.

	for (i=0 ; i<inicio; i++)
		row_sel = g_list_next (row_sel);

	for (i=inicio ; row_sel; row_sel = g_list_next (row_sel), i++)
		if ((GTK_CLIST_ROW (row_sel)->state == GTK_STATE_SELECTED))
		{
			b=TRUE;
			break;
		}

	if(b)
		return (i);
	else
		return (-1);

}


double
suma_clist                  (GtkCList        *clist,
                                        gint             column
                                        )
{
//Creada por Oviedo. 23/Jun/2003
//Suma la columna seleccionada de un clist, regresa el valor de la suma de las columas seleccionadas.
//Se envia la Clist y la columna a sumar.

int row_seleccionada;
gchar  *g_monto;
char monto_char[20];
double cantidad_pedido_sumatoria;

row_seleccionada = 0;
cantidad_pedido_sumatoria = 0;
while(row_seleccionada != -1){
	row_seleccionada = obtener_row_seleccionada (GTK_WIDGET(clist), row_seleccionada); //Busca los row seleccionados
	printf("\nROW_SELECCIONADA_1	: %d",row_seleccionada);
	if(row_seleccionada != -1){
		//Se guarda en la base de datos la informacion.
		gtk_clist_get_text(clist,row_seleccionada,column,&g_monto);
		strcpy(monto_char,g_monto);
		printf("\nMonto: %s", monto_char);
		cantidad_pedido_sumatoria = cantidad_pedido_sumatoria + atof(monto_char);
		row_seleccionada++; //Aumenta el contador del row_seleciconada para la siguiente busqueda

	}
}
return (cantidad_pedido_sumatoria); //Regresa el resultado de la suma.
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


//Busca un valor de una cadena en un arreglo
int buscar_arreglo(char *cadconf){
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

