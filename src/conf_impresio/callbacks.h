#include <gtk/gtk.h>


void
on_combo_entry_cmbImpresion_changed_conf_impresion
                                        (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_combo_entry_cmbImpresora_changed_conf_impresion
                                        (GtkEditable     *editable,
                                        gpointer         user_data);

void
on_conf_impresion_show_conf_impresion  (GtkWidget       *widget,
                                        gpointer         user_data);

void
on_btn_aceptar_conf_impresion_clicked_conf_impresion
                                        (GtkButton       *button,
                                        gpointer         user_data);


void
on_btnCancelar_conf_impresion_clicked_conf_impresion
                                        (GtkButton       *button,
                                        gpointer         user_data);
