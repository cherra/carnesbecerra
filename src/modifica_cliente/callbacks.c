#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <mysql/mysql.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"

//#include "dialogos2.h"
//#include "dialogos.h" //Libreria para mensajes
//#include "funciones.h"

MYSQL mysql;
MYSQL_RES *res, *res2;
MYSQL_ROW row, row2, row3;

gchar *criterio_bak;
gchar *lista2;

//int er = 0;

/*struct cliente
{
  gchar *id_cliente;
  gchar *nombre;
  gchar *domicilio;
  gchar *telefono;
  gchar *telefono2;
  gchar *telefono3;
  gchar *colonia;
  gchar *CPostal;
  gchar *ciudad;
  gchar *estado;
  gchar *RFC;
  gchar *contacto;
  gchar *nombre_de_lista;
  gchar *calles;
  gchar *pago;
  gchar *formato;
  gchar *vencimiento;
}modifica_cliente, alta_cliente;*/

/*** MODIFICAR CLIENTES ***/
//char id_cliente_modifica[10];
//char id_lista_modifica[10];

void obtiene_nombre_de_lista(gchar *id_del_cliente)
{
	char sqllista2[200] = "SELECT Lista.nombre FROM Lista INNER JOIN Cliente ON Cliente.id_lista =  Lista.id_lista WHERE Cliente.id_cliente = ";
	int er;


        strcat(sqllista2,id_del_cliente);
	/*if(conecta_bd() == -1)
    	{
      		printf("No se pudo conectar a la base de datos. Error: %s\n", mysql_error(&mysql2));
    	}
	else
	{*/
		er = mysql_query(&mysql,sqllista2);
		if(er == 0)
		{
			res2 = mysql_store_result(&mysql);
			if(res2)
			{
				
				while((row3=mysql_fetch_row(res2)))
				{
					lista2 = row3[0];
					printf("lista: %s\n",lista2);
				}
			}
		}
		else
			printf("Error: %s", mysql_error(&mysql));
//	}
	printf("desConecta en 2946\n");
//	mysql_close(&mysql2);
}


void
on_Modificar_cliente_show_actualizar   (GtkCombo       *combo_modificarclientelista,
                                        gpointer         user_data)
{

  GList *items = NULL;
  int error;

	if(conecta_bd() == -1)
    	{
      		printf("No se pudo conectar a la base de datos Error: %s\n", mysql_error(&mysql));
    	}
        else
	 {
		error = mysql_query(&mysql,"SELECT nombre FROM Lista");
		if(error == 0)
		{
			res = mysql_store_result(&mysql);
			if(res)
			
			{
			 while((row = mysql_fetch_row(res)))
				{
				        printf("Entra al while \n");
					items = g_list_append (items, row[0]);
					gtk_combo_set_popdown_strings (GTK_COMBO (combo_modificarclientelista), items);	
				}
			 }
			
			printf("SELECT FROM Lista\n");
		}
		else
			printf("Error: %s", mysql_error(&mysql));
	 }
		
	printf("desConecta en 2500\n");
  mysql_close(&mysql);
  
}


void
on_Modificar_cliente_show_lista        (GtkCList       *lista_clientes,
                                        gpointer         user_data)
{
	char sqlclientes[200] = "SELECT Cliente.id_cliente, Cliente.nombre, Lista.nombre FROM Cliente INNER JOIN Lista ON Cliente.id_lista =  Lista.id_lista";
	gchar *lista[3];
	int er;
	
	gtk_clist_clear(lista_clientes);

	if(conecta_bd() == -1)
    	{
      		printf("No se pudo conectar a la base de datos. Error: %s\n", mysql_error(&mysql));
    	}
	else
	{
		er = mysql_query(&mysql,sqlclientes);
		if(er == 0)
		{
			res = mysql_store_result(&mysql);
			if(res)
			{
				while((row=mysql_fetch_row(res)))
				{
					lista[0] = row[0];
					lista[1] = row[1];
					lista[2] = row[2];
					gtk_clist_append(lista_clientes, lista);
				}
			}
		}
		else
			printf("Error: %s", mysql_error(&mysql));
	}
	printf("DesConecta en 2253 \n");
	mysql_close(&mysql);
	
	gtk_clist_select_row(lista_clientes, 0, 0);
}



void
on_entry_modificarclientebuscar_activate
                                        (GtkCList        *lista_clientes,
                                        gpointer         user_data)
{
        char sqlclientes[400];
	gchar *lista[3];
	gchar *criterio;
	int er;

	GtkEntry *txtbuscarcliente = user_data;

	criterio = gtk_editable_get_chars(GTK_EDITABLE(txtbuscarcliente),0,-1);
	criterio_bak = criterio;
	sprintf(sqlclientes, "SELECT Cliente.id_cliente, Cliente.nombre, Lista.nombre FROM Cliente LEFT JOIN Lista ON Cliente.id_lista =  Lista.id_lista WHERE Cliente.nombre LIKE \"%%%s%%\" OR Cliente.contacto LIKE \"%%%s%%\" OR Cliente.id_cliente = \"%s\" ORDER BY Cliente.id_cliente",criterio,criterio,criterio);

	gtk_clist_clear(lista_clientes);

	if(conecta_bd() == -1)
    	{
      		printf("No se pudo conectar a la base de datos. Error: %s\n", mysql_error(&mysql));
    	}
	else
	{
		er = mysql_query(&mysql,sqlclientes);
		printf("%s\n",sqlclientes);
		if(er == 0)
		{
			res = mysql_store_result(&mysql);
			if(res)
			{
				while((row=mysql_fetch_row(res)))
				{
					lista[0] = row[0];
					lista[1] = row[1];
					lista[2] = row[2];
					gtk_clist_append(lista_clientes, lista);
				}
			}
		}
		else
			printf("Error: %s", mysql_error(&mysql));
	}
	mysql_close(&mysql);

	gtk_clist_select_row(lista_clientes, 0, 0);
	gtk_entry_set_text(txtbuscarcliente,""); 
}


void
on_btn_modificarcliente_clicked_nombre (GtkEntry       *entry_nombre,
                                        gpointer         user_data)
{
 modifica_cliente.nombre = gtk_editable_get_chars(GTK_EDITABLE(entry_nombre),0,-1);
}


void
on_btn_modificarcliente_clicked_domicilio
                                        (GtkEntry       *entry_domicilio,
                                        gpointer         user_data)
{
 modifica_cliente.domicilio = gtk_editable_get_chars(GTK_EDITABLE(entry_domicilio),0,-1);
}


void
on_btn_modificarcliente_clicked_colonia
                                        (GtkEntry       *entry_colonia,
                                        gpointer         user_data)
{
 modifica_cliente.colonia = gtk_editable_get_chars(GTK_EDITABLE(entry_colonia),0,-1);
}


void
on_btn_modificarcliente_clicked_ciudad_estado
                                        (GtkEntry       *entry_ciudad_estado,
                                        gpointer         user_data)
{
 modifica_cliente.ciudad = gtk_editable_get_chars(GTK_EDITABLE(entry_ciudad_estado),0,-1);
}


void
on_btn_modificarcliente_clicked_telefono
                                        (GtkEntry       *entry_telefono,
                                        gpointer         user_data)
{
 modifica_cliente.telefono = gtk_editable_get_chars(GTK_EDITABLE(entry_telefono),0,-1);
}


void
on_btn_modificarcliente_clicked_telefono2
                                        (GtkEntry       *entry_telefono2,
                                        gpointer         user_data)
{
 modifica_cliente.telefono2 = gtk_editable_get_chars(GTK_EDITABLE(entry_telefono2),0,-1);
}


void
on_btn_modificarcliente_clicked_telefono3
                                        (GtkEntry       *entry_telefono3,
                                        gpointer         user_data)
{
 modifica_cliente.telefono3 = gtk_editable_get_chars(GTK_EDITABLE(entry_telefono3),0,-1);
}


void
on_btn_modificarcliente_clicked_cp     (GtkEntry       *entry_cp,
                                        gpointer         user_data)
{
 modifica_cliente.CPostal = gtk_editable_get_chars(GTK_EDITABLE(entry_cp),0,-1);
}


void
on_btn_modificarcliente_clicked_rfc    (GtkEntry       *entry_rfc,
                                        gpointer         user_data)
{
 modifica_cliente.RFC = gtk_editable_get_chars(GTK_EDITABLE(entry_rfc),0,-1);
}


void
on_btn_modificarcliente_clicked_contacto
                                        (GtkEntry       *entry_contacto,
                                        gpointer         user_data)
{
 modifica_cliente.contacto = gtk_editable_get_chars(GTK_EDITABLE(entry_contacto),0,-1);
}


void
on_btn_modificarcliente_clicked_lista  (GtkEntry       *entry_lista,
                                        gpointer         user_data)
{
 modifica_cliente.nombre_de_lista = gtk_editable_get_chars(GTK_EDITABLE(entry_lista),0,-1);
}


void
on_btn_modificarcliente_clicked_pago   (GtkButton       *entry_pago,
                                        gpointer         user_data)
{
 modifica_cliente.pago = gtk_editable_get_chars(GTK_EDITABLE(entry_pago),0,-1);
}


void
on_btn_modificarcliente_clicked_formato
                                        (GtkEntry       *entry_formato,
                                        gpointer         user_data)
{
 modifica_cliente.formato = gtk_editable_get_chars(GTK_EDITABLE(entry_formato),0,-1);
}


void
on_btn_modificarcliente_clicked_calles (GtkEntry       *entry_calles,
                                        gpointer         user_data)
{
 modifica_cliente.calles = gtk_editable_get_chars(GTK_EDITABLE(entry_calles),0,-1);
}


void
on_btn_modificarcliente_clicked_vencimiento
                                        (GtkEntry       *entry_vencimiento,
                                        gpointer         user_data)
{
 modifica_cliente.vencimiento = gtk_editable_get_chars(GTK_EDITABLE(entry_vencimiento), 0, -1);
}

void
on_btn_modificarcliente_clicked        (GtkButton       *button,
                                        gpointer         user_data)
{
  GtkWidget *entry_modificarclientenombre;
  GtkWidget *entry_modificarclientedomicilio;
  GtkWidget *entry_modificarclientecolonia;
  GtkWidget *entry_modificarclienteciudad_estado;
  GtkWidget *entry_modificarclientecp;
  GtkWidget *entry_modificarclientetelefono;
  GtkWidget *entry_modificarclientetelefono2;
  GtkWidget *entry_modificarclientetelefono3;
  GtkWidget *entry_modificarclienterfc;
  GtkWidget *entry_modificarclienteentre_calles;
  GtkWidget *entry_modificarclientecontacto;
  GtkWidget *combo_entry_modificarclientelista;
  GtkWidget *combo_entry_modificarclientepago;
  GtkWidget *combo_entry_modificarclienteformato;
  GtkWidget *entry_modificarclientevencimiento;
  GtkWidget *clist_clientes;
  gchar *id_cliente, *nombre, *domicilio, *colonia, *ciudad_estado, *cp, *telefono, *telefono2, *telefono3, *rfc, *entre_calles, *contacto, *lista, *pago, *formato, *vencimiento;
  
  MYSQL_RES *res_cta_contable;
  MYSQL_ROW row_cta_contable;
  char sql_checa_contable[200];
  char sql_contable[]="SELECT MAX(cuenta_contable)+1 FROM Cliente";
  char cta_contable[20]="0";
  int bandera_rfc;

  char cadenilla[10000];

  entry_modificarclientenombre = lookup_widget(GTK_WIDGET(button), "entry_modificarclientenombre");
  entry_modificarclientedomicilio = lookup_widget(GTK_WIDGET(button), "entry_modificarclientedomicilio");
  entry_modificarclientecolonia = lookup_widget(GTK_WIDGET(button), "entry_modificarclientecolonia");
  entry_modificarclienteciudad_estado = lookup_widget(GTK_WIDGET(button), "entry_modificarclienteciudad_estado");
  entry_modificarclientecp = lookup_widget(GTK_WIDGET(button), "entry_modificarclientecp");
  entry_modificarclientetelefono = lookup_widget(GTK_WIDGET(button), "entry_modificarclientetelefono");
  entry_modificarclientetelefono2 = lookup_widget(GTK_WIDGET(button), "entry_modificarclientetelefono2");
  entry_modificarclientetelefono3 = lookup_widget(GTK_WIDGET(button), "entry_modificarclientetelefono3");
  entry_modificarclienterfc = lookup_widget(GTK_WIDGET(button), "entry_modificarclienterfc");
  entry_modificarclienteentre_calles = lookup_widget(GTK_WIDGET(button), "entry_modificarclienteentre_calles");
  entry_modificarclientecontacto = lookup_widget(GTK_WIDGET(button), "entry_modificarclientecontacto");
  combo_entry_modificarclientelista = lookup_widget(GTK_WIDGET(button), "combo_entry_modificarclientelista");
  combo_entry_modificarclientepago = lookup_widget(GTK_WIDGET(button), "combo_entry_modificarclientepago");
  combo_entry_modificarclienteformato = lookup_widget(GTK_WIDGET(button), "combo_entry_modificarclienteformato");
  entry_modificarclientevencimiento = lookup_widget(GTK_WIDGET(button), "entry_modificarclientevencimiento");
  clist_clientes                      = lookup_widget(GTK_WIDGET(button), "clist_modificarcliente");

  nombre        = gtk_editable_get_chars(GTK_EDITABLE(entry_modificarclientenombre), 0,-1);
  domicilio     = gtk_editable_get_chars(GTK_EDITABLE(entry_modificarclientedomicilio), 0,-1);
  colonia       = gtk_editable_get_chars(GTK_EDITABLE(entry_modificarclientecolonia), 0,-1);
  ciudad_estado = gtk_editable_get_chars(GTK_EDITABLE(entry_modificarclienteciudad_estado), 0,-1);
  cp            = gtk_editable_get_chars(GTK_EDITABLE(entry_modificarclientecp), 0,-1);
  telefono      = gtk_editable_get_chars(GTK_EDITABLE(entry_modificarclientetelefono), 0,-1);
  telefono2     = gtk_editable_get_chars(GTK_EDITABLE(entry_modificarclientetelefono2), 0,-1);
  telefono3     = gtk_editable_get_chars(GTK_EDITABLE(entry_modificarclientetelefono3), 0,-1);
  rfc           = gtk_editable_get_chars(GTK_EDITABLE(entry_modificarclienterfc), 0,-1);
  entre_calles  = gtk_editable_get_chars(GTK_EDITABLE(entry_modificarclienteentre_calles), 0,-1);
  contacto      = gtk_editable_get_chars(GTK_EDITABLE(entry_modificarclientecontacto), 0,-1);
  lista         = gtk_editable_get_chars(GTK_EDITABLE(combo_entry_modificarclientelista), 0,-1);
  pago          = gtk_editable_get_chars(GTK_EDITABLE(combo_entry_modificarclientepago), 0,-1);
  formato       = gtk_editable_get_chars(GTK_EDITABLE(combo_entry_modificarclienteformato), 0,-1);
  vencimiento   = gtk_editable_get_chars(GTK_EDITABLE(entry_modificarclientevencimiento), 0,-1);
  
  sprintf (sql_checa_contable,"SELECT cuenta_contable FROM Cliente Where id_cliente = %s",id_cliente_modifica);
  
  if (strlen(rfc) > 0)
	bandera_rfc = arreglar_rfc(rfc);
  
  
  if((strlen(nombre) == 0) || (strlen(telefono) == 0) ||(strlen(domicilio) == 0))
  	Err_Info("Faltan algunos datos");
  else if(bandera_rfc == 1 && strlen(rfc) > 0)
  {
	Err_Info("El RFC es incorrecto");
	gtk_widget_grab_focus(entry_modificarclienterfc);
  }
  else
  {
  	res_cta_contable = conecta_bd_3(sql_checa_contable);
  	if (res_cta_contable != NULL)
	{
		row_cta_contable = mysql_fetch_row (res_cta_contable);
		if(strcmp(row_cta_contable[0],"0") == 0 && bandera_rfc != 1 && strlen(rfc) > 0)
  		{
  			//Si no tiene cuenta contable genera una nueva
			res_cta_contable = conecta_bd_3(sql_contable);
			if (res_cta_contable != NULL)
			{
				row_cta_contable = mysql_fetch_row (res_cta_contable);
				printf ("%s \n",row_cta_contable[0]);
				strcpy(cta_contable,row_cta_contable[0]);
			}
  		}
	  	else
  		{
  			//Si tiene cuenta contable solo la reafirma
			printf ("%s \n",row_cta_contable[0]);
			strcpy(cta_contable,row_cta_contable[0]);
  		}

	  	sprintf(cadenilla, "UPDATE Cliente SET nombre=\"%s\", domicilio=\"%s\", colonia=\"%s\", ciudad_estado=\"%s\", cp=\"%s\", telefono=\"%s\", telefono2=\"%s\", telefono3=\"%s\", rfc=\"%s\", entre_calles=\"%s\", contacto=\"%s\", id_lista=%s, tipo_pago=\"%s\", tipo_impresion=\"%s\", vencimiento=%s, cuenta_contable = %s WHERE id_cliente=%s", nombre, domicilio, colonia, ciudad_estado, cp, telefono, telefono2, telefono3, rfc, entre_calles, contacto, id_lista_modifica, pago, formato, vencimiento,cta_contable, id_cliente_modifica);

  		printf ("MODIFICA = %s\n",cadenilla);
	  	if(conecta_bd() == -1)
  		{
	      	printf("No se pudo conectar a la base de datos Error: %s\n", mysql_error(&mysql));
  		}
  		else
  		{
	      		printf("Cadena: %s\n",cadenilla);
      			er = mysql_query(&mysql,cadenilla);
      			if (er!=0)
			{
				printf("Ocurrio un error al insertar los datos del cliente: %s\n", mysql_error(&mysql));
			}
			else
			{
				gtk_clist_set_text (GTK_CLIST (clist_clientes), obtener_row_seleccionada (clist_clientes,0), 1, nombre);
				gtk_clist_set_text (GTK_CLIST (clist_clientes), obtener_row_seleccionada (clist_clientes,0), 2, lista);
			}
	    	}
	    	mysql_close(&mysql);
	}
	else
	{
		Err_Info ("Ocurrio un error al momento de obtener la cuenta contable.");
	}
    }
}


void
on_btn_modificarcliente_lista          (GtkCList       *clist_clientes,
                                        gpointer         user_data)
{
        /*char sqlclientes[400] = "SELECT Cliente.id_cliente, Cliente.nombre, Lista.nombre FROM Cliente INNER JOIN Lista ON Cliente.id_lista =  Lista.id_lista WHERE Cliente.nombre LIKE \"%";
	gchar *lista[3];
	gchar *criterio;
	int er;

	criterio = criterio_bak;

	if(criterio)
	{
	strcat(sqlclientes, criterio);
	strcat(sqlclientes, "%\"");
	strcat(sqlclientes, " OR Cliente.id_cliente = \"");
	strcat(sqlclientes, criterio);
	}
	strcat(sqlclientes, "\" ORDER BY Cliente.id_cliente");

	gtk_clist_clear(clist_clientes);
	printf("Conecta en 2904\n");
	if(conecta_bd() == -1)
    	{
      		printf("No se pudo conectar a la base de datos. Error: %s\n", mysql_error(&mysql));
    	}
	else
	{
		er = mysql_query(&mysql,sqlclientes);
		printf("%s\n",sqlclientes);
		if(er == 0)
		{
			res = mysql_store_result(&mysql);
			if(res)
			{
				while((row=mysql_fetch_row(res)))
				{
					lista[0] = row[0];
					lista[1] = row[1];
					lista[2] = row[2];
					gtk_clist_append(clist_clientes, lista);
				}
			}
		}
		else
			printf("Error: %s", mysql_error(&mysql));
	}
	printf("Conecta en 2904\n");
	mysql_close(&mysql);*/

	//gtk_clist_select_row(clist_clientes, 0, 0);

}


void
on_clist_modificarcliente_select_row_codigo
                                        (GtkCList        *clist,
                                        gint             fila,
                                        gint             columna,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
  GtkWidget *entry_modificarclientenombre;
  GtkWidget *entry_modificarclientedomicilio;
  GtkWidget *entry_modificarclientecolonia;
  GtkWidget *entry_modificarclienteciudad_estado;
  GtkWidget *entry_modificarclientecp;
  GtkWidget *entry_modificarclientetelefono;
  GtkWidget *entry_modificarclientetelefono2;
  GtkWidget *entry_modificarclientetelefono3;
  GtkWidget *entry_modificarclienterfc;
  GtkWidget *entry_modificarclienteentre_calles;
  GtkWidget *entry_modificarclientecontacto;
  GtkWidget *combo_entry_modificarclientelista;
  GtkWidget *combo_entry_modificarclientepago;
  GtkWidget *combo_entry_modificarclienteformato;
  GtkWidget *entry_modificarclientevencimiento;
  
  GtkWidget *combo_listas;
  GList *lista_listas = NULL;
  
  gchar *id_cliente, *nombre, *domicilio, *colonia, *ciudad_estado, *cp, *telefono, *telefono2, *telefono3, *rfc, *entre_calles, *contacto, *lista, *pago, *formato, *vencimiento;
  gchar *id_cliente_lista;

  char sqllistas[400];

  entry_modificarclientenombre = lookup_widget(GTK_WIDGET(clist), "entry_modificarclientenombre");
  entry_modificarclientedomicilio = lookup_widget(GTK_WIDGET(clist), "entry_modificarclientedomicilio");
  entry_modificarclientecolonia = lookup_widget(GTK_WIDGET(clist), "entry_modificarclientecolonia");
  entry_modificarclienteciudad_estado = lookup_widget(GTK_WIDGET(clist), "entry_modificarclienteciudad_estado");
  entry_modificarclientecp = lookup_widget(GTK_WIDGET(clist), "entry_modificarclientecp");
  entry_modificarclientetelefono = lookup_widget(GTK_WIDGET(clist), "entry_modificarclientetelefono");
  entry_modificarclientetelefono2 = lookup_widget(GTK_WIDGET(clist), "entry_modificarclientetelefono2");
  entry_modificarclientetelefono3 = lookup_widget(GTK_WIDGET(clist), "entry_modificarclientetelefono3");
  entry_modificarclienterfc = lookup_widget(GTK_WIDGET(clist), "entry_modificarclienterfc");
  entry_modificarclienteentre_calles = lookup_widget(GTK_WIDGET(clist), "entry_modificarclienteentre_calles");
  entry_modificarclientecontacto = lookup_widget(GTK_WIDGET(clist), "entry_modificarclientecontacto");
  combo_entry_modificarclientelista = lookup_widget(GTK_WIDGET(clist), "combo_entry_modificarclientelista");
  combo_entry_modificarclientepago = lookup_widget(GTK_WIDGET(clist), "combo_entry_modificarclientepago");
  combo_entry_modificarclienteformato = lookup_widget(GTK_WIDGET(clist), "combo_entry_modificarclienteformato");
  entry_modificarclientevencimiento = lookup_widget(GTK_WIDGET(clist), "entry_modificarclientevencimiento");
  combo_listas = lookup_widget(GTK_WIDGET(clist), "combo_modificarclientelista");

  gtk_clist_get_text(clist,fila,0,&id_cliente_lista);

  strcpy(id_cliente_modifica, id_cliente_lista);

  sprintf(sqllistas, "SELECT Cliente.id_cliente, Cliente.nombre, Cliente.domicilio, Cliente.colonia, Cliente.ciudad_estado, Cliente.cp, Cliente.telefono, Cliente.telefono2, Cliente.telefono3, Cliente.rfc, Cliente.entre_calles, Cliente.contacto, Lista.nombre, Cliente.tipo_pago, Cliente.tipo_impresion, Cliente.vencimiento, Cliente.id_lista FROM Cliente INNER JOIN Lista USING (id_lista) WHERE id_cliente = %s", id_cliente_lista);

	if(conecta_bd() == -1)
    	{
      		printf("No se pudo conectar a la base de datos. Error: %s\n", mysql_error(&mysql));
    	}
	else
	{
		er = mysql_query(&mysql,sqllistas);
		if(er != 0)
			printf("Error en el query de listas: %s\n", mysql_error(&mysql));
		else
		{
			res = mysql_store_result(&mysql);
			if(res)
			{
				if((row=mysql_fetch_row(res)))
				{
					id_cliente = row[0];
					nombre = row[1];
					domicilio = row[2];
					colonia = row[3];
					ciudad_estado = row[4];
					cp = row[5];
					telefono = row[6];
					telefono2 = row[7];
					telefono3 = row[8];
					rfc = row[9];
					entre_calles = row[10];
					contacto = row[11];
					lista = row[12];
					pago = row[13];
					formato = row[14];
					vencimiento = row[15];
					strcpy(id_lista_modifica,row[16]);
				}
			}
		}
	}
 	mysql_close(&mysql);
	
	
	if(conecta_bd() == -1)
	{
		printf("No se pudo conectar a la base de datos. Error: %s\n", mysql_error(&mysql));
	}
	else
	{
		er = mysql_query(&mysql,"SELECT Lista.nombre From Lista");
		if(er != 0)
			printf("Error en el query de listas: %s\n", mysql_error(&mysql));
		else
		{
			res = mysql_store_result(&mysql);
			if(res)
			{
				while((row=mysql_fetch_row(res)))
				{
					lista_listas = g_list_append (lista_listas, row[0]);
				}
			}
		}
	}
	mysql_close(&mysql);

  	gtk_combo_set_popdown_strings (GTK_COMBO (combo_listas), lista_listas);

  gtk_entry_set_text(GTK_ENTRY(entry_modificarclientenombre), nombre);
  gtk_entry_set_text(GTK_ENTRY(entry_modificarclientedomicilio), domicilio);
  gtk_entry_set_text(GTK_ENTRY(entry_modificarclientecolonia), colonia);
  gtk_entry_set_text(GTK_ENTRY(entry_modificarclienteciudad_estado), ciudad_estado);
  gtk_entry_set_text(GTK_ENTRY(entry_modificarclientecp), cp);
  gtk_entry_set_text(GTK_ENTRY(entry_modificarclientetelefono), telefono);
  gtk_entry_set_text(GTK_ENTRY(entry_modificarclientetelefono2), telefono2);
  gtk_entry_set_text(GTK_ENTRY(entry_modificarclientetelefono3), telefono3);
  gtk_entry_set_text(GTK_ENTRY(entry_modificarclienterfc), rfc);
  gtk_entry_set_text(GTK_ENTRY(entry_modificarclienteentre_calles), entre_calles);
  gtk_entry_set_text(GTK_ENTRY(entry_modificarclientecontacto), contacto); 
  gtk_entry_set_text(GTK_ENTRY(combo_entry_modificarclientelista), lista);
  gtk_entry_set_text(GTK_ENTRY(combo_entry_modificarclientepago), pago);
  gtk_entry_set_text(GTK_ENTRY(combo_entry_modificarclienteformato), formato);
  gtk_entry_set_text(GTK_ENTRY(entry_modificarclientevencimiento), vencimiento);
}



void
on_clist_modificarcliente_select_row_nombre
                                        (GtkEntry        *entry_nombre,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
/*	gtk_entry_set_text(entry_nombre, "");
	gtk_entry_set_text(entry_nombre, modifica_cliente.nombre);
	printf("en clist_modif_del_row_nombre nombre: %s\n",modifica_cliente.nombre);*/
}


void
on_clist_modificarcliente_select_row_domicilio
                                        (GtkEntry        *entry_domicilio,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
 /*gtk_entry_set_text(entry_domicilio, "");
 gtk_entry_set_text(entry_domicilio, modifica_cliente.domicilio);*/
}


void
on_clist_modificarcliente_select_row_colonia
                                        (GtkEntry        *entry_colonia,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
 /*gtk_entry_set_text(entry_colonia, "");
 gtk_entry_set_text(entry_colonia, modifica_cliente.colonia);*/
}


void
on_clist_modificarcliente_select_row_ciudad_estado
                                        (GtkEntry        *entry_ciudad_estado,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
 /*gtk_entry_set_text(entry_ciudad_estado, "");
 gtk_entry_set_text(entry_ciudad_estado, modifia_cliente.ciudad);*/
}


void
on_clist_modificarcliente_select_row_cp
                                        (GtkEntry        *entry_cp,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
 /*gtk_entry_set_text(entry_cp, "");
 gtk_entry_set_text(entry_cp, modifica_cliente.CPostal);*/
}


void
on_clist_modificarcliente_select_row_telefono
                                        (GtkEntry        *entry_telefono,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
 /*gtk_entry_set_text(entry_telefono, "");
 gtk_entry_set_text(entry_telefono, modifica_cliente.telefono);*/
}


void
on_clist_modificarcliente_select_row_telefono2
                                        (GtkEntry        *entry_telefono2,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
 /*gtk_entry_set_text(entry_telefono2, "");
 gtk_entry_set_text(entry_telefono2, modifica_cliente.telefono2);*/
}


void
on_clist_modificarcliente_select_row_telefono3
                                        (GtkEntry        *entry_telefono3,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
 /*gtk_entry_set_text(entry_telefono3, "");
 gtk_entry_set_text(entry_telefono3, modifica_cliente.telefono3);*/
}


void
on_clist_modificarcliente_select_row_rfc
                                        (GtkEntry        *entry_rfc,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
 /*gtk_entry_set_text(entry_rfc, "");
 gtk_entry_set_text(entry_rfc, modifica_cliente.RFC);*/
}


void
on_clist_modificarcliente_select_row_entrecalles
                                        (GtkEntry        *entry_calles,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
 /*gtk_entry_set_text(entry_calles, "");
 gtk_entry_set_text(entry_calles, modifica_cliente.calles);*/
}


void
on_clist_modificarcliente_select_row_contacto
                                        (GtkEntry        *entry_contacto,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
 /*gtk_entry_set_text(entry_contacto, "");
 gtk_entry_set_text(entry_contacto, modifica_cliente.contacto);*/
}


void
on_clist_modificarcliente_select_row_pago
                                        (GtkEntry        *entry_pago,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
 /*gtk_entry_set_text(entry_pago, modifica_cliente.pago);*/
}


void
on_clist_modificarcliente_select_row_formato
                                        (GtkEntry        *entry_formato,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
 /*gtk_entry_set_text(entry_formato, modifica_cliente.formato);*/
}


void
on_clist_modificarcliente_select_row_lista
                                        (GtkEntry        *entry_lista,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
 /*gtk_entry_set_text(entry_lista, "");
 gtk_entry_set_text(entry_lista, lista2);*/
}


void
on_clist_modificarcliente_select_row_vencimiento
                                        (GtkEntry        *entry_vencimiento,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data)
{
 /*gtk_entry_set_text(entry_vencimiento, "");
 gtk_entry_set_text(entry_vencimiento, modifica_cliente.vencimiento);*/
 
}


void
on_combo_entry_modificarclientelista_changed
                                        (GtkEditable     *editable,
                                        gpointer         user_data)
{
	char sqllista[100];
	gchar *lista;

	lista = gtk_editable_get_chars(editable,0,-1);

	sprintf(sqllista, "SELECT id_lista FROM Lista WHERE nombre=\"%s\"", lista);

	if(conecta_bd() == -1)
  	{
      		printf("No se pudo conectar a la base de datos Error: %s\n", mysql_error(&mysql));
  	}
  	else
  	{
      		printf("Cadena: %s\n",sqllista);
      		er = mysql_query(&mysql,sqllista);
      		if (er!=0)
			printf("Ocurrió un error al insertar los datos del cliente: %s\n", mysql_error(&mysql));
		else
		{
			res = mysql_store_result(&mysql);
			if(res)
			{
				if((row = mysql_fetch_row(res)))
				{
					printf("ID Lista: %s\n", row[0]);
					strcpy(id_lista_modifica,row[0]);
				}
				else
					printf("Error: %s\n", mysql_error(&mysql));
			}
		}
    	}
	
    	mysql_close(&mysql);
}

