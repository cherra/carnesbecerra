#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <mysql/mysql.h>
#include <stdlib.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"

/*#include "dialogos.h"
#include "dialogos2.h"
#include "funciones.h"
#include "impresion.h"*/
#include "libreria.h"


GtkWidget *entry_global;

/** HAY QUE COMENTAR ESTA FUNCION **/
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


void
on_win_facturacion_no_facturado_show   (GtkWidget       *widget,
                                        gpointer         user_data)
{
	GtkWidget *entry_folio;
	GtkWidget *entry_factura;
	GtkWidget *clist_folios;
	
	MYSQL_RES *res_ultima_factura;
	MYSQL_ROW row_ultima_factura;
	char sql_ultima_factura[]="SELECT num_factura + 1 FROM Venta_Factura_Relacion ORDER BY num_factura DESC LIMIT 1";
	
	entry_folio   = lookup_widget (GTK_WIDGET(widget),"entry_folio_win_no_facturado");
	entry_factura = lookup_widget (GTK_WIDGET(widget),"entry_factura_win_no_facturado");
	clist_folios  = lookup_widget (GTK_WIDGET(widget),"clist_folios_win_no_facturado");
	
	gtk_clist_set_column_justification  (GTK_CLIST (clist_folios), 0,GTK_JUSTIFY_RIGHT);
	gtk_clist_set_column_justification  (GTK_CLIST (clist_folios), 1,GTK_JUSTIFY_RIGHT);
	gtk_clist_set_column_justification  (GTK_CLIST (clist_folios), 3,GTK_JUSTIFY_RIGHT);
	gtk_clist_set_column_visibility     (GTK_CLIST (clist_folios), 4, FALSE);
	gtk_clist_set_column_visibility     (GTK_CLIST (clist_folios), 5, FALSE);
	gtk_clist_clear (GTK_CLIST (clist_folios));
	
	gtk_entry_set_text (GTK_ENTRY(entry_folio),"");
	
	gtk_widget_grab_focus (GTK_WIDGET (entry_folio));
	
	res_ultima_factura  = conecta_bd_3(sql_ultima_factura);
	
	if (mysql_num_rows (res_ultima_factura) > 0)
	{
		row_ultima_factura = mysql_fetch_row (res_ultima_factura);
		gtk_entry_set_text (GTK_ENTRY(entry_factura),row_ultima_factura[0]);
	}
	else
	{
		gtk_entry_set_text (GTK_ENTRY(entry_factura),"1");
	}
}

void
on_btn_agregar_win_no_facturado_clicked
                                        (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *entry_folio;
	GtkWidget *lbl_tot_articulos;
	GtkWidget *clist_folios;
	GtkWidget *btn_quitar;
	
	MYSQL_RES *res_facturado;
	
	gchar *folio;
	gchar *folio_clist;
	gchar *folio_clist2;
	const gchar *tot_articulos;
	char total_art_markup[100];
	char sql_folio[400];
	char sql_folio_repetido[400];
	
	MYSQL_RES *res_folio;
	MYSQL_ROW row_folio;
	
	int i;
	int j;
	int n_productos;
	gboolean folio_repetido = FALSE;
	gboolean repetido;
	
	
	entry_folio       = lookup_widget (GTK_WIDGET(button),"entry_folio_win_no_facturado"        );
	lbl_tot_articulos = lookup_widget (GTK_WIDGET(button),"label_tot_articulos_win_no_facturado");
	clist_folios      = lookup_widget (GTK_WIDGET(button),"clist_folios_win_no_facturado"       );
	btn_quitar        = lookup_widget (GTK_WIDGET(button),"btn_quitar_win_no_facturado"         );
	
	
	folio         = gtk_editable_get_chars (GTK_EDITABLE (entry_folio),0,-1);
	tot_articulos = gtk_label_get_text(GTK_LABEL(lbl_tot_articulos));
	
	sprintf (sql_folio_repetido,"SELECT Venta.id_venta FROM Venta INNER JOIN Venta_Factura_Relacion ON Venta.id_venta = Venta_Factura_Relacion.id_venta WHERE Venta.id_venta = '%s'",folio);
	
	res_facturado = conecta_bd_3(sql_folio_repetido);
	
	if (res_facturado == NULL)
	{
		Err_Info ("Error al validar el numero de folio.");
	}
	else if (mysql_num_rows (res_facturado) > 0)
	{
		Info ("Este folio ya ha sido facturado");
	}
	else
	{
		for (i=0; i<GTK_CLIST(clist_folios)->rows; i++)
		{
			gtk_clist_get_text (GTK_CLIST(clist_folios),i,0,&folio_clist);
			if (strcmp (folio_clist,folio) == 0)
			{
				Info ("No se puede agregar dos veces el mismo folio.");
				folio_repetido = TRUE;
				break;
			}
		}
	
		if (atoi(folio) <= 0)
		{
			Info ("Por favor escriba un numeor de folio valido.");
		}
		else
		if (! folio_repetido)
		{
			sprintf (sql_folio,"SELECT Venta.id_venta, Venta.fecha, Articulo.nombre, FORMAT(Venta.monto,2), Articulo.id_articulo, Venta.id_cliente FROM Venta INNER JOIN Venta_Articulo ON Venta.id_venta = Venta_Articulo.id_venta INNER JOIN Articulo ON Articulo.id_articulo = Venta_Articulo.id_articulo WHERE Venta.id_venta = %s /*GROUP BY Venta.id_venta*/",folio);
			
			res_folio = conecta_bd_3(sql_folio);
		
			if (res_folio != NULL)
			{
				if (mysql_num_rows (res_folio) > 0)
				{
					while (row_folio = mysql_fetch_row (res_folio))
					{
						gtk_clist_append (GTK_CLIST (clist_folios),row_folio);
					}
					n_productos = 0;
					
					for (i=0; i<GTK_CLIST(clist_folios)->rows; i++)
					{
						gtk_clist_get_text (GTK_CLIST(clist_folios),i,4,&folio_clist);
						repetido = FALSE;
						for (j=i+1; j<GTK_CLIST(clist_folios)->rows; j++)
						{
							if (i != j)
							{
								gtk_clist_get_text (GTK_CLIST(clist_folios),j,4,&folio_clist2);
								if (strcmp (folio_clist2,folio_clist) == 0)
								{
									repetido = TRUE;
								}
							}
						}
						if (!repetido)
						{
							n_productos ++;
						}
					}
					sprintf (total_art_markup , "<span color=\"#0000FF\">%d</span>",n_productos);
					gtk_label_set_markup(GTK_LABEL(lbl_tot_articulos),total_art_markup);
					gtk_widget_set_sensitive (btn_quitar,TRUE);
				}
				else
				{
					Info ("El folio no existe.");
				}
			}
		}
	}
		
	gtk_widget_grab_focus (GTK_WIDGET (entry_folio));
}


void
on_btn_aceptar_win_no_facturado_clicked
                                        (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *clist_folios;
	GtkWidget *lbl_tot_articulos;
	GtkWidget *txtv_obs;
	GtkWidget *entry_cliente;
	GtkWidget *entry_num_factura;
	GtkWidget *limite_articulos;
	GtkTextBuffer *buff;
	GtkTextIter start, end;
	
	MYSQL_RES *res_id;
	MYSQL_ROW row_id;
	MYSQL_RES *res_id_cli;
	MYSQL_ROW row_id_cli;
	MYSQL mysql;
	
	const gchar *tot_articulos;
	char sqlfactura[200];
	char sql_last_id[]="SELECT LAST_INSERT_ID()";
	char sqlVenta_Articulo[200];
	char sqlfactura_relacion[200];
	char cambia_id_cliente[100];
	char cliente_rfc[200];
	gchar *obs;
	gchar *num_factura;
	gchar *limite_art;
	gchar *id_cliente;
	gchar *id_venta;
	char rfc_cli[30]="";
	char c_id_cliente[20];
	char c_id_venta[20];
	gboolean exito_factura = FALSE;
	gboolean exito = TRUE;
	
	int i;
	gchar *folio;
	char folio2[20];
	char folios[800]="";
	char folios_tmp[800]="";
	
	entry_cliente     = lookup_widget (GTK_WIDGET(button),"entry_cliente_win_no_facturado");
	entry_num_factura = lookup_widget (GTK_WIDGET(button),"entry_factura_win_no_facturado");
	clist_folios      = lookup_widget (GTK_WIDGET(button),"clist_folios_win_no_facturado");
	txtv_obs          = lookup_widget (GTK_WIDGET(button),"txtv_obs_win_no_facturado");
	lbl_tot_articulos = lookup_widget (GTK_WIDGET(button),"label_tot_articulos_win_no_facturado");
	limite_articulos  = lookup_widget (GTK_WIDGET(button),"entry_limite_articulos_win_no_facturado");
	
	tot_articulos = gtk_label_get_text(GTK_LABEL(lbl_tot_articulos));
	limite_art = gtk_editable_get_chars (GTK_EDITABLE(limite_articulos),0,-1);
	
	buff = gtk_text_view_get_buffer (GTK_TEXT_VIEW (txtv_obs));
	gtk_text_buffer_get_iter_at_offset (buff, &start, 0);
	gtk_text_buffer_get_iter_at_offset (buff, &end, -1);
	
	obs = gtk_text_buffer_get_text(buff,&start,&end,TRUE);
	
	num_factura = gtk_editable_get_chars( GTK_EDITABLE(entry_num_factura), 0, -1);
	id_cliente  = gtk_editable_get_chars( GTK_EDITABLE(entry_cliente),     0, -1);
	strcpy (c_id_cliente,id_cliente);
	
	sprintf (cliente_rfc,"SELECT rfc from Cliente where id_cliente = %s",c_id_cliente);
	
	res_id_cli = conecta_bd_3(cliente_rfc);
	
	if (res_id_cli)
	{
		if ( (row_id_cli = mysql_fetch_row (res_id_cli)) )
			strcpy (rfc_cli,row_id_cli[0]);
	}
	
	if (GTK_CLIST(clist_folios)->rows <= 0)
	{
		Info ("Por favor agregue al menos un folio para realizar la factura.");
	}
	else
	if (atoi(num_factura) <= 0)
	{
		Info ("Por favor escriba un numero de factura valido.");
		gtk_widget_grab_focus (entry_num_factura);
	}
	else
	if (atoi(id_cliente) <= 0)
	{
		Info ("Por favor escriba o busque una clave de cliente valida.");
		gtk_widget_grab_focus (entry_cliente);
	}
	else
	if (strlen (rfc_cli) < 12)
	{
		Info ("El cliente no tiene RFC.");
		gtk_widget_grab_focus (entry_cliente);
	}
	else
	if (atof (tot_articulos) > atoi(limite_art))
	{
		Info ("El numero de articulos es mayor al imite de articulos por factura.");
	}
	else
	{
		//SE INTRODUCE UNA NUEVA FACTURA
		sprintf (sqlfactura         , "INSERT INTO Venta_Factura (id_factura, fecha, hora, observacion) VALUES(NULL,CURDATE(),CURTIME(),\"%s\")",obs);
		
		if (conecta_bd_2 (&mysql) != -1) 
		{
		
		if (mysql_query(&mysql,sqlfactura) == 0)
		{
			sprintf (sqlfactura,"OK");
		}
		else
		{
			Err_Info ("Error en la insercion de la nueva factura.");
		}
		
		if ( strcmp(sqlfactura,"OK") == 0)
		{
			//SE OBTIENE EL ULTIMO ID DE FACTURA QUE SE INTRODUJO
			if ( mysql_query (&mysql,sql_last_id) != 0)
			{
			}
			else
			{
				res_id = mysql_store_result (&mysql);
				if (res_id)
				{
					if (mysql_num_rows(res_id) <= 0)
					{
						Err_Info ("Error al momento de obtener el ultimo id de factura.");
					}
					else
					{
						//SE INSERTAN TODAS LAS RELACIONES CON LA TABLA VENTA_FACTURA_RELACION
						row_id = mysql_fetch_row (res_id);
						printf ("%s\n",sqlfactura);
						for (i=0; i<GTK_CLIST(clist_folios)->rows; i++)
						{
							gtk_clist_get_text (GTK_CLIST(clist_folios),i,0,&folio);
							if ( strcmp(folio2,folio) != 0)
							{
								strcpy (folio2,folio);
								sprintf (sqlfactura_relacion, "INSERT INTO Venta_Factura_Relacion (id_interno,id_venta, id_factura, num_factura) VALUES(NULL,%s,%s,%s)",folio2, row_id[0],num_factura);
								printf ("Factura Relacion = %s\n",sqlfactura_relacion);
								conecta_bd_3 (sqlfactura_relacion);
								if ( strcmp(sqlfactura_relacion,"OK") != 0)
								{
									Err_Info ("Error al crear la relacion en la tabla Venta_Factura_Relacion.");
									exito = FALSE;
								}
							}
						}
						
						
						//Se cambia el id el cliente para realizar la facturacion.
						
						gtk_clist_get_text (GTK_CLIST (clist_folios),0,0,&id_venta);
						strcpy (c_id_venta,id_venta);
						sprintf (cambia_id_cliente,"UPDATE Venta SET id_cliente = %s WHERE id_venta = %s",c_id_cliente, c_id_venta);
						
						conecta_bd_3 (cambia_id_cliente);
						
						if ( strcmp(cambia_id_cliente,"OK") != 0)
						{
							Err_Info ("Error al cambiar el id del cliente en la facturacion.");
						}
						
						//Si Todo va bien se actualiza el numero de factura en la tabla Venta_Articulo
						if (exito)
						{
							strcpy (folio2,"");
							for (i=0; i<GTK_CLIST(clist_folios)->rows; i++)
							{
								gtk_clist_get_text (GTK_CLIST(clist_folios),i,0,&folio);
								if ( strcmp(folio2,folio) != 0)
								{
									strcpy (folio2,folio);
									sprintf (sqlVenta_Articulo,"UPDATE Venta_Articulo SET id_factura = %s WHERE id_venta = %s", row_id[0], folio2);
									printf ("Venta_Articulo = %s\n",sqlVenta_Articulo);
									conecta_bd_3(sqlVenta_Articulo);
									if (strcmp(sqlVenta_Articulo,"OK") != 0)
									{
										Err_Info ("Error en la actualizacion de el id_factura.");
										break;
									}
									strcat (folios_tmp,folio2);
									strcat (folios_tmp," OR Venta_Articulo.id_venta = ");
								}
							}
		
							strncpy (folios,folios_tmp, strlen(folios_tmp)-30); //Para quitar el ultimo " OR Venta_Articulo.id_venta = "

							exito_factura = TRUE;
						}
					}
				}
			}
			
		}
		else
		{
			Err_Info ("Error al crear el registro de la factura en la tabla Venta_Factura.");
		}
		mysql_close (&mysql);
		}
		else
		{
			Err_Info ("No se pudo iniciar la conexion");
		}
		
	}
	
	if (exito_factura)
	{
		imprimirticket(folios, "factura", 0);
		
		gtk_clist_get_text (GTK_CLIST(clist_folios),0,5,&id_cliente);
		
		sprintf (cambia_id_cliente,"UPDATE Venta SET id_cliente = %s WHERE  id_venta = %s",id_cliente, c_id_venta);
		printf ("Cliente %s \n",cambia_id_cliente);
		conecta_bd_3(cambia_id_cliente);
		
		if (strcmp(cambia_id_cliente,"OK") != 0)
		{
			Err_Info ("Error el reresar el id de cliente a la venta.");
		}
		
		gtk_signal_emit_by_name ( GTK_OBJECT(gtk_widget_get_toplevel(GTK_WIDGET(button))) , "show");
	}
}

void
on_btn_quitar_win_no_facturado_clicked (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *clist_folios;
	GtkWidget *btn_quitar;
	GtkWidget *entry_folio;
	GtkWidget *lbl_tot_articulos;
	
	char strpregunta[80];
	char folio_tmp[20];
	char total_art_markup[100];
	gchar *folio;
	gchar *folio2;
	gchar *folio_clist;
	gchar *folio_clist2;
	
	gboolean repetido;
	
	int row;
	int i;
	int j;
	int n_productos;
	
	lbl_tot_articulos = lookup_widget (GTK_WIDGET(button),"label_tot_articulos_win_no_facturado");
	clist_folios = lookup_widget (GTK_WIDGET(button),"clist_folios_win_no_facturado");
	btn_quitar   = lookup_widget (GTK_WIDGET(button),"btn_quitar_win_no_facturado"  );
	entry_folio  = lookup_widget (GTK_WIDGET(button),"entry_folio_win_no_facturado" );
	
	row = obtener_row_seleccionada(clist_folios,0);
	
	if (row != -1)
	{
		gtk_clist_get_text (GTK_CLIST(clist_folios),GTK_CLIST(clist_folios)->focus_row,0,&folio);
		
		sprintf (strpregunta,"Esta seguro que desea borrar el folio %s",folio);
		if (pregunta(NULL,strpregunta))
		{
			strcpy (folio_tmp,folio);
			for (i=GTK_CLIST(clist_folios)->rows -1; i>=0; i--)
			{
				gtk_clist_get_text (GTK_CLIST(clist_folios),i,0,&folio2);
				if (strcmp(folio_tmp, folio2) == 0)
				{
					gtk_clist_remove(GTK_CLIST(clist_folios),i);
				}
			}
			if (GTK_CLIST(clist_folios)->rows <= 0)
			{
				gtk_widget_set_sensitive (btn_quitar,FALSE);
			}
			
			n_productos = 0;
			for (i=0; i<GTK_CLIST(clist_folios)->rows; i++)
			{
				gtk_clist_get_text (GTK_CLIST(clist_folios),i,4,&folio_clist);
				repetido = FALSE;
				for (j=i+1; j<GTK_CLIST(clist_folios)->rows; j++)
				{
					if (i != j)
					{
						gtk_clist_get_text (GTK_CLIST(clist_folios),j,4,&folio_clist2);
						if (strcmp (folio_clist2,folio_clist) == 0)
						{
							repetido = TRUE;
						}
					}
				}
				if (!repetido)
				{
					n_productos ++;
				}
			}
			
			sprintf (total_art_markup , "<span color=\"#0000FF\">%d</span>",n_productos);
			gtk_label_set_markup(GTK_LABEL(lbl_tot_articulos),total_art_markup);
		}
	}
	else
	{
		Info ("Por favor seleccione una fila de la lista.");
	}
	
	gtk_widget_grab_focus (entry_folio);
}


void
on_entry_limite_articulos_win_no_facturado_changed
                                        (GtkEditable     *editable,
                                        gpointer         user_data)
{
	es_signo_numeracion(editable,0);
}


void
on_entry_folio_win_no_facturado_changed
                                        (GtkEditable     *editable,
                                        gpointer         user_data)
{
	es_signo_numeracion(editable,0);
}


void
on_entry_factura_win_no_facturado_changed
                                        (GtkEditable     *editable,
                                        gpointer         user_data)
{
	es_signo_numeracion(editable,0);
}


void
on_entry_cliente_win_no_facturado_changed
                                        (GtkEditable     *editable,
                                        gpointer         user_data)
{
	es_signo_numeracion(editable,0);
}


void
on_btn_buscar_folio_win_no_facturado_clicked
                                        (GtkButton       *button,
                                        gpointer         user_data)
{
	
	GtkWidget *entry_folio;
	GtkWidget *btn_agregar_folio;
	
	entry_folio       = lookup_widget(GTK_WIDGET(button), "entry_folio_win_no_facturado");
	btn_agregar_folio = lookup_widget(GTK_WIDGET(button), "btn_agregar_win_no_facturado");
	
	entry_global = entry_folio;
	
	gtk_widget_show(create_Buscar_venta());
}


void
on_btn_buscar_cliente_win_no_facturado_clicked
                                        (GtkButton       *button,
                                        gpointer         user_data)
{
	GtkWidget *entry_facturarcliente;
	
	entry_facturarcliente = lookup_widget(GTK_WIDGET(button), "entry_cliente_win_no_facturado");
	
	entry_global = entry_facturarcliente;
	
	gtk_widget_show(create_Buscar_cliente());
}

