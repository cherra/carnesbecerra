#include <gtk/gtk.h>


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
on_btn_modificarcliente_clicked_rfc    (GtkEntry       *entry_rfc,
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
on_btn_modificarcliente_clicked_calles (GtkEntry       *entry_calles,
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

void
on_clist_modificarcliente_select_row_codigo
                                        (GtkCList        *clist,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_clist_modificarcliente_select_row_nombre
                                        (GtkEntry        *entry_nombre,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_clist_modificarcliente_select_row_domicilio
                                        (GtkEntry        *entry_domicilio,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_clist_modificarcliente_select_row_colonia
                                        (GtkEntry        *entry_colonia,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_clist_modificarcliente_select_row_ciudad_estado
                                        (GtkEntry        *entry_ciudad_estado,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_clist_modificarcliente_select_row_cp
                                        (GtkEntry        *entry_cp,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_clist_modificarcliente_select_row_telefono
                                        (GtkEntry        *entry_telefono,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_clist_modificarcliente_select_row_telefono2
                                        (GtkEntry        *entry_telefono2,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_clist_modificarcliente_select_row_telefono3
                                        (GtkEntry        *entry_telefono3,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_clist_modificarcliente_select_row_rfc
                                        (GtkEntry        *entry_rfc,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_clist_modificarcliente_select_row_entrecalles
                                        (GtkEntry        *entry_calles,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_clist_modificarcliente_select_row_contacto
                                        (GtkEntry        *entry_contacto,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_clist_modificarcliente_select_row_lista
                                        (GtkEntry        *entry_lista,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
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
on_clist_modificarcliente_select_row_vencimiento
                                        (GtkEntry        *entry_vencimiento,
                                        gint             row,
                                        gint             column,
                                        GdkEvent        *event,
                                        gpointer         user_data);

void
on_combo_entry_modificarclientelista_changed
                                        (GtkEditable     *editable,
                                        gpointer         user_data);
