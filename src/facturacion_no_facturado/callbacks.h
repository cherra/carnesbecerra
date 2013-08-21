#include <gtk/gtk.h>


void
on_btn_agregar_win_no_facturado_clicked
                                        (GtkButton       *button,
                                        gpointer         user_data);

void
on_win_facturacion_no_facturado_show   (GtkWidget       *widget,
                                        gpointer         user_data);

void
on_btn_aceptar_win_no_facturado_clicked
                                        (GtkButton       *button,
                                        gpointer         user_data);

void
on_btn_quitar_win_no_facturado_clicked (GtkButton       *button,
                                        gpointer         user_data);

void
on_entry_limite_articulos_win_no_facturado_changed
                                        (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_entry_folio_win_no_facturado_changed
                                        (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_entry_factura_win_no_facturado_changed
                                        (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_entry_cliente_win_no_facturado_changed
                                        (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_btn_buscar_folio_win_no_facturado_clicked
                                        (GtkButton       *button,
                                        gpointer         user_data);

void
on_btn_buscar_cliente_win_no_facturado_clicked
                                        (GtkButton       *button,
                                        gpointer         user_data);
