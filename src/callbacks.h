#include <gtk/gtk.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/signal.h>
#include <sys/types.h>

/*Variables Globales*/
struct termios options, last;
int sport; /*descriptor del Serial*/

unsigned int pow16(int coef);
unsigned short int gethex(unsigned char *caracter,int nibbles,int *estado);
void convert_muestra(char dato);
int open_port(int port);
int initport(int puerto);

void SIG_INTERRUPT();
void signal_handler_IO(int );

void llenar_servicios(GtkWidget *widget, char *ordenar);




/***********************************************************************************************
					JUAN
***************************************************************************************************/






void
mete_valores_en_lista		       ();  					/////JUAN


void 
mete_valores_entabla_producto	       ();  					/////JUAN

void 
mete_valores_entabla_usuario           ();  					/////JUAN

void
mete_valores_entabla_empleado          ();  					/////JUAN


void
on_btn_aceptaralta_clicked             (GtkWidget       *Ventana_altas,  	/////JUAN
                                        gpointer         user_data);


void
on_nuevo_articulo_activate             (GtkMenuItem     *menuitem,     //////JORGE
                                        gpointer         user_data);


void
on_btn_Altaempleadoaceptar_clicked_nombre
                                        (GtkEntry       *entry_nombre,  	/////JUAN
                                        gpointer         user_data);

void
on_btn_Altaempleadoaceptar_clicked_apellido
                                        (GtkEntry       *entry_apellido,  	/////JUAN
                                        gpointer         user_data);

void
on_btn_Altaempleadoaceptar_clicked_domicilio
                                        (GtkEntry       *entry_domicilio,  	/////JUAN
                                        gpointer         user_data);

void
on_btn_Altaempleadoaceptar_clicked_colonia
                                        (GtkEntry       *entry_colonia,  	/////JUAN
                                        gpointer         user_data);

void
on_btn_Altaempleadoaceptar_clicked_cp  (GtkEntry       *entry_cp,  	/////JUAN
                                        gpointer         user_data);

void
on_btn_Altaempleadoaceptar_clicked_ciudad
                                        (GtkEntry       *entry_ciudad,  	/////JUAN
                                        gpointer         user_data);

void
on_btn_Altaempleadoaceptar_clicked_estado
                                        (GtkEntry       *entry_estado,  	/////JUAN
                                        gpointer         user_data);

void
on_btn_Altaempleadoaceptar_clicked_telefono
                                        (GtkEntry       *entry_telefono,  	/////JUAN
                                        gpointer         user_data);

void
on_btn_Altaempleadoaceptar_clicked_puesto
                                        (GtkEntry       *entry_puesto,  	/////JUAN
                                        gpointer         user_data);

void
on_btn_Altaempleadoaceptar_clicked_seguro
                                        (GtkEntry       *entry_seguro,  	/////JUAN
                                        gpointer         user_data);

void
on_btn_Altaempleadoaceptar_clicked_sueldo
                                        (GtkEntry       *entry_sueldo,  	/////JUAN
                                        gpointer         user_data);

void
on_btn_Altaempleadoaceptar_clicked     (GtkWidget       *widget_ventana,  	/////JUAN
                                        gpointer         user_data);


/*void
on_clist_modificarcliente_select_row_codigo
                                        (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_clist_modificarcliente_select_row_nombre
                                        (GtkEntry        *entry_nombre,                /////JUAN
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_clist_modificarcliente_select_row_domicilio
                                        (GtkEntry        *entry_domicilio,                /////JUAN
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_clist_modificarcliente_select_row_colonia
                                        (GtkEntry        *entry_colonia,                /////JUAN
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_clist_modificarcliente_select_row_ciudad_estado
                                        (GtkEntry        *entry_ciudad_estado,                /////JUAN
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_clist_modificarcliente_select_row_cp
                                        (GtkEntry        *entry_cp,                /////JUAN
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_clist_modificarcliente_select_row_telefono
                                        (GtkEntry        *entry_telefono,                /////JUAN
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_clist_modificarcliente_select_row_telefono2
                                        (GtkEntry        *entry_telefono2,                /////JUAN
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_clist_modificarcliente_select_row_telefono3
                                        (GtkEntry        *entry_telefono3,		/////JUAN       
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_clist_modificarcliente_select_row_rfc
                                        (GtkEntry        *entry_rfc,              /////JUAN
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_clist_modificarcliente_select_row_entrecalles
                                        (GtkEntry        *entry_calles,                /////JUAN
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_clist_modificarcliente_select_row_contacto
                                        (GtkEntry        *entry_contacto,                /////JUAN
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_clist_modificarcliente_select_row_lista
                                        (GtkEntry        *entry_lista,               /////JUAN
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);


void
on_clist_modificarcliente_select_row_vencimiento
                                        (GtkEntry        *entry_vencimiento,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);



void
on_Modificar_cliente_show_actualizar   (GtkCombo       *combo_modificarclientelista,
                                        gpointer         user_data);



void
on_Modificar_cliente_show_lista        (GtkCList       *lista_clientes,
                                        gpointer         user_data);

void
on_entry_modificarclientebuscar_activate
                                        (GtkCList        *lista_clientes,
                                        gpointer         user_data);

void
on_clist_modificarcliente_select_row_pago
                                        (GtkEntry        *entry_pago,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_clist_modificarcliente_select_row_formato
                                        (GtkEntry        *entry_formato,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_btn_modificarcliente_clicked_nombre (GtkEntry       *entry_nombre,
                                        gpointer         user_data);

void
on_btn_modificarcliente_clicked_domicilio
                                        (GtkEntry       *entry_domicilio,
                                        gpointer         user_data);

void
on_btn_modificarcliente_clicked_colonia
                                        (GtkEntry       *entry_colonia,
                                        gpointer         user_data);

void
on_btn_modificarcliente_clicked_ciudad_estado
                                        (GtkEntry       *entry_ciudad_estado,
                                        gpointer         user_data);

void
on_btn_modificarcliente_clicked_telefono
                                        (GtkEntry       *entry_telefono,
                                        gpointer         user_data);

void
on_btn_modificarcliente_clicked_telefono2
                                       (GtkEntry       *entry_telefono2,
                                        gpointer         user_data);

void
on_btn_modificarcliente_clicked_telefono3
                                        (GtkEntry       *entry_telefono3,
                                        gpointer         user_data);

void
on_btn_modificarcliente_clicked_cp     (GtkEntry       *entry_cp,
                                        gpointer         user_data);

void
on_btn_modificarcliente_clicked_rfc   (GtkEntry       *entry_rfc,
                                        gpointer         user_data);

void
on_btn_modificarcliente_clicked_contacto
                                       (GtkEntry       *entry_contacto,
                                        gpointer         user_data);

void
on_btn_modificarcliente_clicked_lista  (GtkEntry       *entry_lista,
                                        gpointer         user_data);

void
on_btn_modificarcliente_clicked_pago   (GtkButton       *entry_pago,
                                        gpointer         user_data);

void
on_btn_modificarcliente_clicked_formato
                                       
                                        (GtkEntry       *entry_formato,
                                        gpointer         user_data);

void
on_btn_modificarcliente_clicked_calles  (GtkEntry       *entry_calles,
                                        gpointer         user_data);

void
on_btn_modificarcliente_clicked_vencimiento
                                        (GtkEntry       *entry_vencimiento,
                                        gpointer         user_data);


void
on_btn_modificarcliente_clicked        (GtkButton       *button,
                                        gpointer         user_data);

void
on_btn_modificarcliente_lista          (GtkCList       *clist_clientes,
                                        gpointer         user_data);


void obtiene_nombre_de_lista(gchar *id_del_cliente);

*/







/************************************************************************************
						JORGE
**************************************************************************************/




int checar_corte();

int fin_de_sesion();


void
on_txttelefono_activate                (GtkCList        *listaclientes,
                                        gpointer         user_data);		///////JORGE

/*void
on_btn_cancelaralta_clicked            (GtkButton       *btn_cancelaralta,
                                        gpointer         user_data);

void
on_closebutton1_clicked                (GtkButton       *button,
                                        gpointer         user_data);*/

void
on_nuevo_cliente_activate              (GtkMenuItem     *menuitem,
                                        gpointer         user_data);		///////JORGE

void
on_txtbarcode_activate                 (GtkCList        *listaarticulos,
                                        gpointer         user_data);		///////JORGE

void
on_txtbuscarcliente_activate           (GtkCList        *listaclientes,
                                        gpointer         user_data);		///////JORGE

void
on_caja_show                           (GtkCList       *listaclientes,
                                        gpointer         user_data);		///////JORGE


void
on_caja_show_la                        (GtkCList       *listaarticulos,
                                        gpointer         user_data);


void
on_txtbarcode_activate_cliente         (GtkCList        *listaclientes,
                                        gpointer         user_data);		///////JORGE

void
on_listaclientes_select_row            (GtkCList        *listaclientes,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);	///////JORGE

void
on_txtbuscarcliente_activate_focus_barcode
                                        (GtkEntry        *txtbarcode,
                                        gpointer         user_data);	///////JORGE


void
on_listaclientes_unselect_row          (GtkCList        *listaclientes,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);		///////JORGE

void
on_radiobtncajafactura_toggled         (GtkToggleButton *togglebutton,
                                        gpointer         user_data);



void
on_Datos_cliente_show                  (GtkCList       *lista_datos_cliente,
                                        gpointer         user_data);		///////JORGE


void
on_entry_seguridadusuario_activate     (GtkEntry        *entry,
                                        gpointer         user_data);	///////JORGE

void
on_btn_seguridadaceptar_clicked        (GtkWidget       *Seguridad,
                                        gpointer         user_data);	///////JORGE


gboolean
on_Datos_cliente_delete_event          (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);	///////JORGE

void
on_btncajacancelar_clicked_listaarticulos
                                        (GtkCList       *listaarticulos,
                                        gpointer         user_data);		///////JORGE

void
on_nuevo_empleado_activate             (GtkMenuItem     *menuitem,
                                        gpointer         user_data);		////JORGE


void
on_listaarticulos_select_row           (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);		///////JORGE

void
on_btnquitar_clicked_lista             (GtkButton       *button,
                                        gpointer         user_data);		///////JORGE

void
on_listaarticulos_unselect_row         (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);		///////JORGE



void
on_modificar_datos_cliente_activate    (GtkMenuItem     *menuitem,
                                        gpointer         user_data);



void
on_Alta_clientes_caja_show             (GtkCombo       *CB_lista,
                                        gpointer         user_data);




void sacarfecha(char cfecha[11], char hora[10]);


void
on_btnloginok_activate_usuario         (GtkEntry       *entry_usuario,
                                        gpointer         user_data);

void
on_btnloginok_activate_contrasena      (GtkEntry       *entry_contrasena,
                                        gpointer         user_data);

void
on_btnloginok_activate_ok              (GtkWindow       *Inicio_Sesion,
                                        gpointer         user_data);

gboolean
on_caja_delete_event                   (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_btncajacancelar_clicked_total       (GtkLabel       *lbltotal,
                                        gpointer         user_data);		///////JORGE


void
on_btncajacancelar_clicked_kilos       (GtkLabel       *lblkilos,
                                        gpointer         user_data);


void
on_btnquitar_clicked_total             (GtkLabel       *lbltotal,
                                        gpointer         user_data);		///////JORGE

void
on_btnquitar_clicked_total_kilos       (GtkLabel       *lblkilos,
                                        gpointer         user_data);


void
on_caja_show_lbltotal                  (GtkLabel       *lbltotal,
                                        gpointer         user_data);



void
on_base_de_datos_activate              (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_btnbdaceptar_clicked_ip             (GtkEntry       *entry_IP,
                                        gpointer         user_data);

void
on_btnbdaceptar_clicked_usuario        (GtkEntry       *entry_Usuario,
                                        gpointer         user_data);

void
on_btnbdaceptar_clicked_contrasena     (GtkEntry       *entry_Contrasena,
                                        gpointer         user_data);

void
on_btnbdaceptar_clicked_bd             (GtkEntry       *entry_BD,
                                        gpointer         user_data);

void
on_btnbdaceptar_clicked_caja           (GtkEntry       *combo_entry_Caja,
                                        gpointer         user_data);

void
on_btnbdaceptar_clicked                (GtkWindow       *Configuracion_BD,
                                        gpointer         user_data);

int conecta_bd();


void
on_Configuracion_BD_show_ip            (GtkEntry       *entry_IP,
                                        gpointer         user_data);

void
on_Configuracion_BD_show_usuario       (GtkEntry       *entry_Usuario,
                                        gpointer         user_data);

void
on_Configuracion_BD_show_Contrasena    (GtkEntry       *entry_Contrasena,
                                        gpointer         user_data);

void
on_Configuracion_BD_show_bd            (GtkEntry       *entry_BD,
                                        gpointer         user_data);

void
on_Configuracion_BD_show_caja          (GtkCombo       *combo_Caja,
                                        gpointer         user_data);

void
on_Configuracion_BD_show_caja_cual     (GtkEntry       *combo_entry_Caja,
                                        gpointer         user_data);


int Obtiene_caja();


void
on_btncajaok_clicked                   (GtkButton       *button,
                                        gpointer         user_data);


void
on_Confirmar_venta_show                (GtkLabel       *lblcambio,
                                        gpointer         user_data);


void
on_btnconfirmaraceptar_clicked         (GtkButton       *button,
                                        gpointer         user_data);

void
on_entry_Efectivo_change               (GtkLabel     *lblcambio,
                                        gpointer         entry_Efectivo);


void
on_facturar_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_btnfacturarok_clicked_ok            (GtkButton       *button,
                                        gpointer         user_data);


void
on_okbtn_facturar_clicked              (GtkButton       *button,
                                        gpointer         user_data);


void
on_final_activate                      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);


void
on_reimprimir_ticket_activate          (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_btn_reimpbuscar_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
on_btn_pedidobuscar_clicked            (GtkButton       *button,
                                        gpointer         user_data);


void
on_btn_reimpok_clicked                 (GtkEntry       *entry_reimpfolio,
                                        gpointer         user_data);


void
on_Ventana_Articulos_show_lineas       (GtkCList       *clist_linea,
                                        gpointer         user_data);


void
on_clist_linea_select_row              (GtkCList        *clist_ListadoDeArticulos,
                                        gint             fila,
                                        gint             columna,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_clist_linea_unselect_row            (GtkCList        *clist_ListadoDeArticulos,
                                        gint             fila,
                                        gint             columna,
                                        GdkEvent        *event,
                                        gpointer         user_data);


void
checar_permisos                        (GtkWidget       *widget,
                                        gpointer         user_data);


void
on_parcial_activate                    (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_global_activate                     (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_entry_dia_corte_global_changed      (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_entry_ano_corte_blobal_changed      (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_entry_mes_corte_global_changed      (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_btn_corte_global_clicked            (GtkButton       *button,
                                        gpointer         user_data);


void
on_llenar_pedido_activate              (GtkMenuItem     *menuitem,
                                        gpointer         user_data);


void
on_cierre_de_facturas_activate         (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_entry_cierre_dedia_changed          (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_entry_cierre_demes_changed          (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_entry_cierre_deano_changed          (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_entry_cierre_hastadia_changed       (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_entry_cierre_hastames_changed       (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_entry_cierre_hastaano_changed       (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_btn_cierre_contado_toggled          (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_btn_cierre_credito_toggled          (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_btn_cierre_revisar_clicked          (GtkButton       *button,
                                        gpointer         user_data);


void sacar_ventas_cierre(char *tipo);


void
on_Cierre_facturas_show                (GtkWidget       *widget,
                                        gpointer         user_data);

void
on_btn_cierre_facturar_clicked         (GtkButton       *button,
                                        gpointer         user_data);


void
on_btnlistado_clicked                  (GtkButton       *button,
                                        gpointer         user_data);

void
on_Facturar_show                       (GtkWidget       *widget,
                                        gpointer         user_data);

void
on_entry_Nofolio_activate              (GtkEntry        *entry,
                                        gpointer         user_data);


void
on_btnfacturarbuscar_clicked           (GtkButton       *button,
                                        gpointer         user_data);

void
on_btnfacturarbuscarc_clicked          (GtkButton       *button,
                                        gpointer         user_data);


void
on_entry_buscarventa_cliente_changed   (GtkEditable     *editable,
                                        gpointer         user_data);


void
on_calendario_buscarventa_day_selected (GtkCalendar     *calendar,
                                        gpointer         user_data);

void
on_clist_buscarventa_select_row        (GtkCList        *clist,
                                        gint             fila,
                                        gint             columna,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_btn_buscarventaaceptar_clicked      (GtkButton       *button,
                                        gpointer         user_data);

void
on_Buscar_venta_show                   (GtkWidget       *widget,
                                        gpointer         user_data);

void
on_btn_cancelaventabuscar_clicked      (GtkButton       *button,
                                        gpointer         user_data);

void
on_btn_cajabuscararticulo_clicked      (GtkButton       *button,
                                        gpointer         user_data);

void
on_Buscar_articulo_show                (GtkWidget       *widget,
                                        gpointer         user_data);

void
on_entry_buscararticulonombre_changed  (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_clist_buscararticulo_select_row     (GtkCList        *clist,
                                        gint             fila,
                                        gint             columna,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_btn_buscararticuloaceptar_clicked   (GtkButton       *button,
                                        gpointer         user_data);


void
on_Buscar_cliente_show                 (GtkWidget       *widget,
                                        gpointer         user_data);

void
on_entry_buscarclientenombre_changed   (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_clist_buscarcliente_select_row      (GtkCList        *clist,
                                        gint             fila,
                                        gint             columna,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_btn_buscarcliente_aceptar_clicked   (GtkButton       *button,
                                        gpointer         user_data);


void
on_btn_pedidobuscararticulo_clicked    (GtkButton       *button,
                                        gpointer         user_data);


void buscar_venta();

void buscar_servicio();

void buscar_articulo();

void buscar_cliente();


void
on_btn_buscarcliente_nuevo_clicked     (GtkButton       *button,
                                        gpointer         user_data);

void
on_btn_buscarventafecha_clicked        (GtkButton       *button,
                                        gpointer         user_data);

void
on_reimprimir_pedido_activate          (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_btn_reimppedidook_clicked           (GtkButton       *button,
                                        gpointer         user_data);


////DESCUENTOS
void
on_btncaja_descuento_clicked           (GtkButton       *button,
                                        gpointer         user_data);

void
on_btn_descuentoaceptar_clicked        (GtkButton       *button,
                                        gpointer         user_data);


/////TERMINA DESCUENTOS






/****************************************************************************************************
						EDER
******************************************************************************************************/



void
on_nuevo_articulo_activate             (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_Articulos_show                      (GtkWidget       *widget,
                                        gpointer         user_data);

void
on_Ventana_Articulos_show              (GtkCList       *listaArticulos,
                                        gpointer         user_data);

void
on_clist_ListadoDeArticulos_select_row (GtkCList        *lista_articulos,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_clist_ListadoDeArticulos_select_row_codigo
                                        (GtkEntry        *entry_codigo,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_clist_ListadoDeArticulos_select_row_nombre
                                        (GtkEntry        *entry_nombre,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_clist_ListadoDeArticulos_select_row_tipo
                                        (GtkEntry        *entry_tipo,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);


void
on_btn_guardar_clicked_codigo          (GtkEntry       *codigo,
                                        gpointer         user_data);

void
on_btn_guardar_clicked_nombre          (GtkEntry       *nombre,
                                        gpointer         user_data);

void
on_btn_guardar_clicked_tipo            (GtkEntry       *tipo,
                                        gpointer         user_data);

/*void
on_btn_guardar_clicked_guarda_actualiza
                                        (GtkButton       *button,
                                        gpointer         user_data);*/

void
on_btn_nuevo_clicked                   (GtkEntry       *codigo,
                                        gpointer         user_data);
void
on_btn_nuevo_clicked_tipo              (GtkEntry       *tipo,
                                        gpointer         user_data);

void
on_btn_nuevo_clicked_nombre            (GtkEntry       *nombre,
                                        gpointer         user_data);

void
on_btn_cancelar_clicked_codigo         (GtkEntry       *codigo,
                                        gpointer         user_data);

void
on_btn_cancelar_clicked_nombre         (GtkEntry       *nombre,
                                        gpointer         user_data);

void
on_btn_cancelar_clicked_tipo           (GtkEntry       *tipo,
                                        gpointer         user_data);



void Cancela_Venta();


void
on_Articulos_show                      (GtkWidget       *widget,
                                        gpointer         user_data);

void
on_Ventana_Articulos_show              (GtkCList       *listaArticulos,
                                        gpointer         user_data);

void
on_clist_ListadoDeArticulos_select_row (GtkCList        *lista_articulos,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_clist_ListadoDeArticulos_select_row_codigo
                                        (GtkEntry        *entry_codigo,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_clist_ListadoDeArticulos_select_row_nombre
                                        (GtkEntry        *entry_nombre,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_clist_ListadoDeArticulos_select_row_tipo
                                        (GtkEntry        *entry_tipo,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_btn_guardar_clicked_codigo          (GtkEntry       *codigo,
                                        gpointer         user_data);

void
on_btn_guardar_clicked_nombre          (GtkEntry       *nombre,
                                        gpointer         user_data);

void
on_btn_guardar_clicked_tipo            (GtkEntry       *tipo,
                                        gpointer         user_data);

void
on_btn_guardar_clicked_guarda_actualiza
                                        (GtkCList       *lista_articulo,
                                        gpointer         user_data);

void
on_btn_nuevo_clicked                   (GtkEntry       *codigo,
                                        gpointer         user_data);
void
on_btn_nuevo_clicked_tipo              (GtkEntry       *tipo,
                                        gpointer         user_data);

void
on_btn_nuevo_clicked_nombre            (GtkEntry       *nombre,
                                        gpointer         user_data);

void
on_btn_cancelar_clicked_codigo         (GtkEntry       *codigo,
                                        gpointer         user_data);

void
on_btn_cancelar_clicked_nombre         (GtkEntry       *nombre,
                                        gpointer         user_data);

void
on_btn_cancelar_clicked_tipo           (GtkEntry       *tipo,
                                        gpointer         user_data);

void
on_entry_BusquedaArticulo_activate     (GtkEntry       *entry,
                                        gpointer         user_data);

void
on_entry_BusquedaArticulo_activate_busca
                                        (GtkCList        *listaArticulos,
                                        gpointer         user_data);

void
on_btn_borrar_clicked                  (GtkCList     *lista,
                                        gpointer         user_data);

void
on_dialog_Articulo_Eder_show           (GtkLabel 	*label,
                                        gpointer         user_data);

void
on_btn_Cancel_clicked                  (GtkButton       *button,
                                        gpointer         user_data);

void
on_btn_OK_clicked                      (GtkButton       *button,
                                        gpointer         user_data);

void
on_dialog_Aviso_show                   (GtkLabel       *label,
                                        gpointer         user_data);

void
on_okbutton2_clicked                   (GtkButton       *button,
                                        gpointer         user_data);


void
on_dialog_Aviso_show                   (GtkLabel       *label,
                                        gpointer         user_data);

void
on_okbutton2_clicked                   (GtkButton       *button,
                                        gpointer         user_data);

void
on_btn_borrar_clicked_codigo           (GtkEntry       *codigo,
                                        gpointer         user_data);

void
on_btn_borrar_clicked_nombre           (GtkEntry       *nombre,
                                        gpointer         user_data);

void
on_btn_borrar_clicked_tipo             (GtkEntry       *tipo,
                                        gpointer         user_data);

void
on_dialog_Error_show                   (GtkLabel       *label,
                                        gpointer         user_data);

void
on_okbutton3_clicked                   (GtkButton       *button,
                                        gpointer         user_data);

void
on_entry_folio_activate_desc           (GtkScrolledWindow        *scroll,
                                        gpointer         user_data);

void
on_entry_folio_activate_observacion    (GtkScrolledWindow        *scroll,
                                        gpointer         user_data);

void
on_btn_cancela_venta_clicked_cancela   (GtkButton       *button,
                                        gpointer         user_data);

void
on_cancelacion_venta_show              (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_entry_folio_activate_lista          (GtkCList        *clist,
                                        gpointer         user_data);

void
on_btn_cancela_venta_clicked_desc      (GtkScrolledWindow       *scroll,
                                        gpointer         user_data);

void
on_btn_cancela_venta_clicked_obs       (GtkScrolledWindow       *scroll,
                                        gpointer         user_data);


void
on_btn_cancela_venta_clicked_cancelar  (GtkTextView       *textview,
                                        gpointer         user_data);


void
on_entry_folio_activate_id             (GtkEntry        *entry,
                                        gpointer         user_data);


void
on_entry_folio_activate_total          (GtkLabel        *total,
                                        gpointer         user_data);

void
on_cancelbutton_cancela_venta_clicked  (GtkButton       *button,
                                        gpointer         user_data);

void
on_okbutton_cancela_venta_clicked      (GtkButton       *button,
                                        gpointer         user_data);


void
on_cancelar_activate                   (GtkMenuItem     *menuitem,
                                        gpointer         user_data);



/**************************************************************
OVIEDO
***************************************************************/






void
on_retiro_de_efectivo_activate         (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

gboolean
on_Retiro_Efectivo_delete_event        (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_txtRetiro_Password_activate         (GtkEntry        *entry,
                                        gpointer         user_data);


void
on_txtRetiro_Password_activate         (GtkEntry        *txtRetiro_Usuario,
                                        gpointer         user_data);

void
on_txtRetiro_Password_activate_t       (GtkEntry        *txtRetiro_Cantidad,
                                        gpointer         user_data);

void
on_txtRetiro_Password_activate_l       (GtkEntry        *lblCantidad,
                                        gpointer         user_data);

void
on_txtRetiro_Password_activate_b       (GtkEntry        *btnRetiro_Aceptar,
                                        gpointer         user_data);

void
on_btnRetiro_Aceptar_clicked           (GtkEntry       *txtRetiro_Cantidad,
                                        gpointer         user_data);

void
on_acerca_de_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_radiobtncajapcredito_toggled        (GtkToggleButton *togglebutton,
                                        gpointer         user_data);


void
on_entry_codigo_activate_actualiza_lista
                                        (GtkCList        *clist,
                                        gpointer         user_data);

void
on_entry_cantidad_activate_actualiza_lista
                                        (GtkCList        *clist,
                                        gpointer         user_data);

void
on_btn_pedidobuscar_clicked            (GtkButton       *button,
                                        gpointer         user_data);

void
on_btn_pedidosenviar_clicked           (GtkButton       *button,
                                        gpointer         user_data);

void
on_Domicilio_envio_show                (GtkWidget       *widget,
                                        gpointer         user_data);

void
on_btn_envioborrar_clicked             (GtkButton       *button,
                                        gpointer         user_data);


void
on_btn_envioaceptar_clicked            (GtkButton       *button,
                                        gpointer         user_data);



void
on_btn_aceptar_clicked_guarda_pedido   (GtkCList       *clist,
                                        gpointer         user_data);

void
on_btn_cancelar_clicked_cierra_ventana (GtkButton       *button,
                                        gpointer         user_data);

void
on_V_pedidos_show_llena_lista          (GtkCList       *clist,
                                        gpointer         user_data);

void
on_btn_observaciones_aceptar_clicked   (GtkTextView       *text,
                                        gpointer         user_data);

void
on_btn_observaciones_cancelar_clicked  (GtkButton       *button,
                                        gpointer         user_data);

void
on_clist_articulos_select_row_guarda_row
                                        (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_btn_quitar_clicked                  (GtkButton       *button,
                                        gpointer         user_data);

void
gtk_widget_show_ventana                (GtkWidget       *widget,
                                        gpointer         user_data);

gboolean
on_V_observaciones_delete_event        (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_clist_cliente_select_row_guarda_row (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);


void
gtk_widget_show_V_pedidos              (GtkWidget       *clist,
                                        gpointer         user_data);

void
on_clist_articulos_unselect_row_V_observaciones
                                        (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

/*void
on_entry_buscarclientenombre_changed   (GtkEntry        *entry,
                                        gpointer         user_data);*/

void
on_facturar_folios_no_facturados_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_facturar_varios_folio_activate      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_impresoras_activate                 (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_chk_ver_precio_buscar_articulo_toggled
                                        (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_win_bascula_show                    (GtkWidget       *widget,
                                        gpointer         user_data);

void
on_btn_aceptar_bascula_clicked         (GtkButton       *button,
                                        gpointer         user_data);

gboolean
on_win_bascula_delete_event            (GtkWidget       *widget,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_btn_aplicar_cambios_observaciones_pedidos_clicked
                                        (GtkButton       *button,
                                        gpointer         user_data);

void
on_btn_eliminar_observaciones_pedidos_clicked
                                        (GtkButton       *button,
                                        gpointer         user_data);

void
on_btn_guardar_observaciones_pedido_clicked
                                        (GtkButton       *button,
                                        gpointer         user_data);
void
on_txtbuffer_changed
                                        (GtkTextBuffer    *txtbuffer,
                                        gpointer         user_data);

void
on_entryconfirma_vendedor_changed      (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_win_servicios_show                  (GtkWidget       *widget,
                                        gpointer         user_data);

void
on_entryNumeroRepartidor_Servicios_activate
                                        (GtkEntry        *entry,
                                        gpointer         user_data);

/*void
on_entryBuscaPedido_Servicios_activate (GtkEntry        *entry,
                                        gpointer         user_data);

void
on_btnBuscaPedido_Servicios_clicked    (GtkButton       *button,
                                        gpointer         user_data);*/

void
on_btnBuscarRepartidor_Servicios_clicked
                                        (GtkButton       *button,
                                        gpointer         user_data);

void
on_entryBuscaVenta_Servicios_activate  (GtkEntry        *entry,
                                        gpointer         user_data);

void
on_btnBuscaVenta_Servicios_clicked     (GtkButton       *button,
                                        gpointer         user_data);

void
on_servicios_de_vendedores_activate    (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_btnAceptar_Servicios_clicked        (GtkButton       *button,
                                        gpointer         user_data);

void
on_clistVentas_Servicios_click_column  (GtkCList        *clist,
                                        gint             column,
                                        gpointer         user_data);

void
on_cierre_pruebas_activate             (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_cancelar_pruebas1_activate          (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_entry_folio_activate                (GtkEntry        *entry,
                                        gpointer         user_data);

void
on_btn_cancelaventaPruebaBuscar_clicked
                                        (GtkButton       *button,
                                        gpointer         user_data);

void
on_btnAceptar_cancelar_venta_prueba_clicked
                                        (GtkButton       *button,
                                        gpointer         user_data);

void
on_btn_busca_pedido_cancelacion_clicked
                                        (GtkButton       *button,
                                        gpointer         user_data);

void
on_btn_aceptar_cancelacion_pedidos_clicked
                                        (GtkButton       *button,
                                        gpointer         user_data);

void
on_cancelar_pedido_activate            (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_entry_folio_pedido_cancelacion_activate
                                        (GtkEntry        *entry,
                                        gpointer         user_data);

void
on_entry_servicio_activate             (GtkEntry        *entry,
                                        gpointer         user_data);

void
on_btn_buscar_servicio_clicked         (GtkButton       *button,
                                        gpointer         user_data);

void
on_cancelar_servicio_domicilio_activate
                                        (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_btnAceptarCancelarServicio_clicked  (GtkButton       *button,
                                        gpointer         user_data);

void
on_nuevo_servicio_domicilio_activate   (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_entry_folio_nuevo_servicio_activate (GtkEntry        *entry,
                                        gpointer         user_data);

void
on_btnAceptaNuevoServicio_clicked      (GtkButton       *button,
                                        gpointer         user_data);

void
on_entry_monto_servicio_changed        (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_entryconfirma_vendedor_activate     (GtkEntry        *entry,
                                        gpointer         user_data);

void
on_btn_buscarserviciofecha_clicked     (GtkButton       *button,
                                        gpointer         user_data);

void
on_Buscar_Servicio_show                (GtkWidget       *widget,
                                        gpointer         user_data);

void
on_clist_servicios_select_row          (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_btnAceptar_clicked                  (GtkButton       *button,
                                        gpointer         user_data);

void
on_btnCancelar_clicked                 (GtkButton       *button,
                                        gpointer         user_data);

void
on_calendar_servicios_day_selected     (GtkCalendar     *calendar,
                                        gpointer         user_data);

void
on_entry_cliente_changed               (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_entry_cliente_activate              (GtkEntry        *entry,
                                        gpointer         user_data);

void
on_descuentos1_activate                (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_pagar_ventas_credito1_activate      (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_entry_folio_pagar_venta_activate    (GtkEntry        *entry,
                                        gpointer         user_data);

void
on_btn_buscar_venta_pagar_clicked      (GtkButton       *button,
                                        gpointer         user_data);

void
on_btn_aceptar_pagar_venta_clicked     (GtkButton       *button,
                                        gpointer         user_data);


void
on_radio_efectivo_toggled              (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_radio_tarjeta_toggled               (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_radio_cheque_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_Corte_global_show                   (GtkWidget       *widget,
                                        gpointer         user_data);

void
on_radioTipoVentaVales_toggled         (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void 
on_clistConceptoVale_select_row		(GtkCList 	*clistConceptoVale,
					gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_cancelar_vales1_activate            (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_entry_folio_vale_activate           (GtkEntry        *entry,
                                        gpointer         user_data);

void
on_btn_cancelaventaVale_clicked        (GtkButton       *button,
                                        gpointer         user_data);

void
on_btnAceptar_cancelar_vale_clicked    (GtkButton       *button,
                                        gpointer         user_data);

void
on_win_asignar_cheques_show            (GtkWidget       *widget,
                                        gpointer         user_data);

void
on_pagos_con_cheque1_activate          (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_entry_folio_asigna_cheque_activate  (GtkEntry        *entry,
                                        gpointer         user_data);

void
on_btn_buscar_asigna_cheque_clicked    (GtkButton       *button,
                                        gpointer         user_data);

void
on_btn_aplicar_asigna_cheque_clicked   (GtkButton       *button,
                                        gpointer         user_data);

void
on_win_paquetes_show                   (GtkWidget       *widget,
                                        gpointer         user_data);

void
on_paquetes1_activate                  (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_clist_paquetes_select_row           (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_entry_buscar_articulo_activate      (GtkEntry        *entry,
                                        gpointer         user_data);

void
on_entry_cantidad_paquete_activate     (GtkEntry        *entry,
                                        gpointer         user_data);

void
on_entry_busca_paquete_activate        (GtkEntry        *entry,
                                        gpointer         user_data);

void
on_btn_buscar_paquete_clicked          (GtkButton       *button,
                                        gpointer         user_data);

void
on_btn_buscar_articulo_clicked         (GtkButton       *button,
                                        gpointer         user_data);

void
on_btn_agrega_articulo_paquete_clicked (GtkButton       *button,
                                        gpointer         user_data);

void
on_btn_quita_articulo_paquete_clicked  (GtkButton       *button,
                                        gpointer         user_data);

void
on_clist_paquetes_unselect_row         (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_btn_aplicar_cantidad_paquete_clicked
                                        (GtkButton       *button,
                                        gpointer         user_data);

void
on_clist_articulos_paquete_select_row  (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_clist_articulos_paquete_unselect_row
                                        (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_btn_reimpbuscar_vale_clicked        (GtkButton       *button,
                                        gpointer         user_data);

void
on_btn_reimpok_vale_clicked            (GtkButton       *button,
                                        gpointer         user_data);

void
on_reimpriir_vale1_activate            (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_entry_reimpfolio_vale_activate      (GtkEntry        *entry,
                                        gpointer         user_data);

void
on_entry_monto_cheque_changed          (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_clist_ventas_select_row             (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_chk_chece_protegido_toggled         (GtkToggleButton *togglebutton,
                                        gpointer         user_data);

void
on_reimprimir_consumo1_activate        (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_btn_reimpok_consumo_clicked         (GtkButton       *button,
                                        gpointer         user_data);

void
on_btn_reimpbuscar_consumo_clicked     (GtkButton       *button,
                                        gpointer         user_data);


void
on_entry_consumo_folio_activate        (GtkEntry        *entry,
                                        gpointer         user_data);

void
on_btn_cancelaventaConsumoBuscar_clicked
                                        (GtkButton       *button,
                                        gpointer         user_data);

void
on_btnAceptar_cancelar_venta_consumo_clicked
                                        (GtkButton       *button,
                                        gpointer         user_data);

void
on_cancela_venta_consumo1_activate     (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_reimprimir_prueba1_activate         (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_btn_reimpbuscar_prueba_clicked      (GtkButton       *button,
                                        gpointer         user_data);

void
on_btn_reimpok_prueba_clicked          (GtkButton       *button,
                                        gpointer         user_data);

void
on_winAsignarPedidos_show              (GtkWidget       *widget,
                                        gpointer         user_data);

void
on_asignar_pedidos1_activate           (GtkMenuItem     *menuitem,
                                        gpointer         user_data);

void
on_btn_AsignaPedidos_clicked           (GtkButton       *button,
                                        gpointer         user_data);

void
on_entryBuscaPedido_activate           (GtkEntry        *entry,
                                        gpointer         user_data);

void
on_btnBuscaPedido_clicked              (GtkButton       *button,
                                        gpointer         user_data);

void
on_btn_UpdatePedidos_clicked           (GtkButton       *button,
                                        gpointer         user_data);

void
on_entryBuscaEmpleado_activate         (GtkEntry        *entry,
                                        gpointer         user_data);

void
on_btnBuscaEmpleado_clicked            (GtkButton       *button,
                                        gpointer         user_data);
