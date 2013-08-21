/*
 * DO NOT EDIT THIS FILE - it is generated by Glade.
 */

/*#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"

#define GLADE_HOOKUP_OBJECT(component,widget,name) \
  g_object_set_data_full (G_OBJECT (component), name, \
    gtk_widget_ref (widget), (GDestroyNotify) gtk_widget_unref)

#define GLADE_HOOKUP_OBJECT_NO_REF(component,widget,name) \
  g_object_set_data (G_OBJECT (component), name, widget)
*/
GtkWidget*
create_conf_impresion (void)
{
  GtkWidget *conf_impresion;
  GtkWidget *scrolledwindow1;
  GtkWidget *layout1;
  GtkWidget *cmbImpresion;
  GList *cmbImpresion_items = NULL;
  GtkWidget *combo_entry_cmbImpresion;
  GtkWidget *txtPar1;
  GtkWidget *txtPar2;
  GtkWidget *cmbImpresora;
  GList *cmbImpresora_items = NULL;
  GtkWidget *combo_entry_cmbImpresora;
  GtkWidget *label1;
  GtkWidget *label2;
  GtkWidget *lblPar2;
  GtkWidget *lblPar1;
  GtkWidget *btnCancelar_conf_impresion;
  GtkWidget *image3;
  GtkWidget *btn_aceptar_conf_impresion;
  GtkWidget *alignment2;
  GtkWidget *hbox2;
  GtkWidget *image2;
  GtkWidget *label6;
  GtkWidget *label7;
  GtkWidget *label8;
  GtkAccelGroup *accel_group;
  GtkTooltips *tooltips;

  tooltips = gtk_tooltips_new ();

  accel_group = gtk_accel_group_new ();

  conf_impresion = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_widget_set_name (conf_impresion, "conf_impresion");
  gtk_widget_set_size_request (conf_impresion, 330, 210);
  gtk_window_set_title (GTK_WINDOW (conf_impresion), _("Configuraci\303\263n de impresoras"));
  gtk_window_set_position (GTK_WINDOW (conf_impresion), GTK_WIN_POS_CENTER);
  gtk_window_set_resizable (GTK_WINDOW (conf_impresion), FALSE);

  scrolledwindow1 = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_set_name (scrolledwindow1, "scrolledwindow1");
  gtk_widget_show (scrolledwindow1);
  gtk_container_add (GTK_CONTAINER (conf_impresion), scrolledwindow1);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow1), GTK_POLICY_NEVER, GTK_POLICY_NEVER);

  layout1 = gtk_layout_new (NULL, NULL);
  gtk_widget_set_name (layout1, "layout1");
  gtk_widget_show (layout1);
  gtk_container_add (GTK_CONTAINER (scrolledwindow1), layout1);
  gtk_layout_set_size (GTK_LAYOUT (layout1), 400, 400);
  GTK_ADJUSTMENT (GTK_LAYOUT (layout1)->hadjustment)->step_increment = 10;
  GTK_ADJUSTMENT (GTK_LAYOUT (layout1)->vadjustment)->step_increment = 10;

  cmbImpresion = gtk_combo_new ();
  g_object_set_data (G_OBJECT (GTK_COMBO (cmbImpresion)->popwin),
                     "GladeParentKey", cmbImpresion);
  gtk_widget_set_name (cmbImpresion, "cmbImpresion");
  gtk_widget_show (cmbImpresion);
  gtk_layout_put (GTK_LAYOUT (layout1), cmbImpresion, 130, 16);
  gtk_widget_set_size_request (cmbImpresion, 190, 25);
  gtk_combo_set_value_in_list (GTK_COMBO (cmbImpresion), TRUE, TRUE);
  cmbImpresion_items = g_list_append (cmbImpresion_items, (gpointer) _("Predeterminada"));
  cmbImpresion_items = g_list_append (cmbImpresion_items, (gpointer) _("Credito"));
  cmbImpresion_items = g_list_append (cmbImpresion_items, (gpointer) _("Contado"));
  cmbImpresion_items = g_list_append (cmbImpresion_items, (gpointer) _("Factura"));
  cmbImpresion_items = g_list_append (cmbImpresion_items, (gpointer) _("Corte de caja parcial"));
  cmbImpresion_items = g_list_append (cmbImpresion_items, (gpointer) _("Corte de caja final/global"));
  cmbImpresion_items = g_list_append (cmbImpresion_items, (gpointer) _("Deudores de credito"));
  cmbImpresion_items = g_list_append (cmbImpresion_items, (gpointer) _("Pedido para el carnicero"));
  gtk_combo_set_popdown_strings (GTK_COMBO (cmbImpresion), cmbImpresion_items);
  g_list_free (cmbImpresion_items);

  combo_entry_cmbImpresion = GTK_COMBO (cmbImpresion)->entry;
  gtk_widget_set_name (combo_entry_cmbImpresion, "combo_entry_cmbImpresion");
  gtk_widget_show (combo_entry_cmbImpresion);
  gtk_tooltips_set_tip (tooltips, combo_entry_cmbImpresion, _("Selecciona el tipo de impresi\303\263n"), NULL);
  gtk_editable_set_editable (GTK_EDITABLE (combo_entry_cmbImpresion), FALSE);

  txtPar1 = gtk_entry_new ();
  gtk_widget_set_name (txtPar1, "txtPar1");
  gtk_widget_show (txtPar1);
  gtk_layout_put (GTK_LAYOUT (layout1), txtPar1, 136, 80);
  gtk_widget_set_size_request (txtPar1, 136, 24);
  gtk_tooltips_set_tip (tooltips, txtPar1, _("No deben de existir espacios en blanco entre caracteres"), NULL);
  gtk_entry_set_max_length (GTK_ENTRY (txtPar1), 15);

  txtPar2 = gtk_entry_new ();
  gtk_widget_set_name (txtPar2, "txtPar2");
  gtk_widget_show (txtPar2);
  gtk_layout_put (GTK_LAYOUT (layout1), txtPar2, 136, 112);
  gtk_widget_set_size_request (txtPar2, 136, 24);
  gtk_tooltips_set_tip (tooltips, txtPar2, _("No deben de existir espacios en blanco entre caracteres"), NULL);

  cmbImpresora = gtk_combo_new ();
  g_object_set_data (G_OBJECT (GTK_COMBO (cmbImpresora)->popwin),
                     "GladeParentKey", cmbImpresora);
  gtk_widget_set_name (cmbImpresora, "cmbImpresora");
  gtk_widget_show (cmbImpresora);
  gtk_layout_put (GTK_LAYOUT (layout1), cmbImpresora, 130, 47);
  gtk_widget_set_size_request (cmbImpresora, 190, 25);
  cmbImpresora_items = g_list_append (cmbImpresora_items, (gpointer) _("CUPS"));
  cmbImpresora_items = g_list_append (cmbImpresora_items, (gpointer) _("Paralelo"));
  cmbImpresora_items = g_list_append (cmbImpresora_items, (gpointer) _("Serial"));
  cmbImpresora_items = g_list_append (cmbImpresora_items, (gpointer) _("Print File 32"));
  gtk_combo_set_popdown_strings (GTK_COMBO (cmbImpresora), cmbImpresora_items);
  g_list_free (cmbImpresora_items);

  combo_entry_cmbImpresora = GTK_COMBO (cmbImpresora)->entry;
  gtk_widget_set_name (combo_entry_cmbImpresora, "combo_entry_cmbImpresora");
  gtk_widget_show (combo_entry_cmbImpresora);
  gtk_tooltips_set_tip (tooltips, combo_entry_cmbImpresora, _("Selecciona el tipo de impresora"), NULL);
  gtk_editable_set_editable (GTK_EDITABLE (combo_entry_cmbImpresora), FALSE);

  label1 = gtk_label_new (_("Tipo de impresi\303\263n"));
  gtk_widget_set_name (label1, "label1");
  gtk_widget_show (label1);
  gtk_layout_put (GTK_LAYOUT (layout1), label1, 8, 16);
  gtk_widget_set_size_request (label1, 120, 24);
  gtk_label_set_use_markup (GTK_LABEL (label1), TRUE);
  gtk_label_set_justify (GTK_LABEL (label1), GTK_JUSTIFY_LEFT);

  label2 = gtk_label_new (_("Impresora"));
  gtk_widget_set_name (label2, "label2");
  gtk_widget_show (label2);
  gtk_layout_put (GTK_LAYOUT (layout1), label2, 48, 51);
  gtk_widget_set_size_request (label2, 80, 16);
  gtk_label_set_use_markup (GTK_LABEL (label2), TRUE);
  gtk_label_set_justify (GTK_LABEL (label2), GTK_JUSTIFY_LEFT);

  lblPar2 = gtk_label_new (_("Nombre del \ncontrolador"));
  gtk_widget_set_name (lblPar2, "lblPar2");
  gtk_widget_show (lblPar2);
  gtk_layout_put (GTK_LAYOUT (layout1), lblPar2, 36, 108);
  gtk_widget_set_size_request (lblPar2, 96, 32);
  gtk_label_set_use_markup (GTK_LABEL (lblPar2), TRUE);
  gtk_label_set_justify (GTK_LABEL (lblPar2), GTK_JUSTIFY_RIGHT);

  lblPar1 = gtk_label_new (_("Direcci\303\263n IP"));
  gtk_widget_set_name (lblPar1, "lblPar1");
  gtk_widget_show (lblPar1);
  gtk_layout_put (GTK_LAYOUT (layout1), lblPar1, 40, 83);
  gtk_widget_set_size_request (lblPar1, 88, 16);
  gtk_label_set_use_markup (GTK_LABEL (lblPar1), TRUE);
  gtk_label_set_justify (GTK_LABEL (lblPar1), GTK_JUSTIFY_LEFT);

  btnCancelar_conf_impresion = gtk_button_new ();
  gtk_widget_set_name (btnCancelar_conf_impresion, "btnCancelar_conf_impresion");
  gtk_widget_show (btnCancelar_conf_impresion);
  gtk_layout_put (GTK_LAYOUT (layout1), btnCancelar_conf_impresion, 176, 152);
  gtk_widget_set_size_request (btnCancelar_conf_impresion, 103, 35);
  GTK_WIDGET_SET_FLAGS (btnCancelar_conf_impresion, GTK_CAN_DEFAULT);
  gtk_widget_add_accelerator (btnCancelar_conf_impresion, "clicked", accel_group,
                              GDK_F5, 0,
                              GTK_ACCEL_VISIBLE);
  gtk_widget_add_accelerator (btnCancelar_conf_impresion, "clicked", accel_group,
                              GDK_Escape, 0,
                              GTK_ACCEL_VISIBLE);

  image3 = create_pixmap (conf_impresion, "carnesbecerra_cancelar.jpg");
  gtk_widget_set_name (image3, "image3");
  gtk_widget_show (image3);
  gtk_container_add (GTK_CONTAINER (btnCancelar_conf_impresion), image3);

  btn_aceptar_conf_impresion = gtk_button_new ();
  gtk_widget_set_name (btn_aceptar_conf_impresion, "btn_aceptar_conf_impresion");
  gtk_widget_show (btn_aceptar_conf_impresion);
  gtk_layout_put (GTK_LAYOUT (layout1), btn_aceptar_conf_impresion, 40, 152);
  gtk_widget_set_size_request (btn_aceptar_conf_impresion, 103, 35);
  gtk_widget_add_accelerator (btn_aceptar_conf_impresion, "clicked", accel_group,
                              GDK_F1, 0,
                              GTK_ACCEL_VISIBLE);

  alignment2 = gtk_alignment_new (0.5, 0.5, 0, 0);
  gtk_widget_set_name (alignment2, "alignment2");
  gtk_widget_show (alignment2);
  gtk_container_add (GTK_CONTAINER (btn_aceptar_conf_impresion), alignment2);

  hbox2 = gtk_hbox_new (FALSE, 2);
  gtk_widget_set_name (hbox2, "hbox2");
  gtk_widget_show (hbox2);
  gtk_container_add (GTK_CONTAINER (alignment2), hbox2);

  image2 = create_pixmap (conf_impresion, "carnesbecerra_aceptar.jpg");
  gtk_widget_set_name (image2, "image2");
  gtk_widget_show (image2);
  gtk_box_pack_start (GTK_BOX (hbox2), image2, FALSE, FALSE, 0);

  label6 = gtk_label_new_with_mnemonic ("");
  gtk_widget_set_name (label6, "label6");
  gtk_widget_show (label6);
  gtk_box_pack_start (GTK_BOX (hbox2), label6, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label6), GTK_JUSTIFY_LEFT);

  label7 = gtk_label_new (_("<small><b>(F1)</b></small>"));
  gtk_widget_set_name (label7, "label7");
  gtk_widget_show (label7);
  gtk_layout_put (GTK_LAYOUT (layout1), label7, 72, 187);
  gtk_widget_set_size_request (label7, 38, 17);
  gtk_label_set_use_markup (GTK_LABEL (label7), TRUE);
  gtk_label_set_justify (GTK_LABEL (label7), GTK_JUSTIFY_LEFT);

  label8 = gtk_label_new (_("<small><b>(F5)</b></small>"));
  gtk_widget_set_name (label8, "label8");
  gtk_widget_show (label8);
  gtk_layout_put (GTK_LAYOUT (layout1), label8, 208, 188);
  gtk_widget_set_size_request (label8, 38, 17);
  gtk_label_set_use_markup (GTK_LABEL (label8), TRUE);
  gtk_label_set_justify (GTK_LABEL (label8), GTK_JUSTIFY_LEFT);

  g_signal_connect ((gpointer) conf_impresion, "show",
                    G_CALLBACK (on_conf_impresion_show_conf_impresion),
                    NULL);
  g_signal_connect ((gpointer) combo_entry_cmbImpresion, "changed",
                    G_CALLBACK (on_combo_entry_cmbImpresion_changed_conf_impresion),
                    NULL);
  g_signal_connect ((gpointer) combo_entry_cmbImpresora, "changed",
                    G_CALLBACK (on_combo_entry_cmbImpresora_changed_conf_impresion),
                    NULL);
  g_signal_connect_swapped ((gpointer) btnCancelar_conf_impresion, "clicked",
                            G_CALLBACK (on_btnCancelar_conf_impresion_clicked_conf_impresion),
                            GTK_OBJECT (conf_impresion));
  g_signal_connect ((gpointer) btn_aceptar_conf_impresion, "clicked",
                    G_CALLBACK (on_btn_aceptar_conf_impresion_clicked_conf_impresion),
                    NULL);

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (conf_impresion, conf_impresion, "conf_impresion");
  GLADE_HOOKUP_OBJECT (conf_impresion, scrolledwindow1, "scrolledwindow1");
  GLADE_HOOKUP_OBJECT (conf_impresion, layout1, "layout1");
  GLADE_HOOKUP_OBJECT (conf_impresion, cmbImpresion, "cmbImpresion");
  GLADE_HOOKUP_OBJECT (conf_impresion, combo_entry_cmbImpresion, "combo_entry_cmbImpresion");
  GLADE_HOOKUP_OBJECT (conf_impresion, txtPar1, "txtPar1");
  GLADE_HOOKUP_OBJECT (conf_impresion, txtPar2, "txtPar2");
  GLADE_HOOKUP_OBJECT (conf_impresion, cmbImpresora, "cmbImpresora");
  GLADE_HOOKUP_OBJECT (conf_impresion, combo_entry_cmbImpresora, "combo_entry_cmbImpresora");
  GLADE_HOOKUP_OBJECT (conf_impresion, label1, "label1");
  GLADE_HOOKUP_OBJECT (conf_impresion, label2, "label2");
  GLADE_HOOKUP_OBJECT (conf_impresion, lblPar2, "lblPar2");
  GLADE_HOOKUP_OBJECT (conf_impresion, lblPar1, "lblPar1");
  GLADE_HOOKUP_OBJECT (conf_impresion, btnCancelar_conf_impresion, "btnCancelar_conf_impresion");
  GLADE_HOOKUP_OBJECT (conf_impresion, image3, "image3");
  GLADE_HOOKUP_OBJECT (conf_impresion, btn_aceptar_conf_impresion, "btn_aceptar_conf_impresion");
  GLADE_HOOKUP_OBJECT (conf_impresion, alignment2, "alignment2");
  GLADE_HOOKUP_OBJECT (conf_impresion, hbox2, "hbox2");
  GLADE_HOOKUP_OBJECT (conf_impresion, image2, "image2");
  GLADE_HOOKUP_OBJECT (conf_impresion, label6, "label6");
  GLADE_HOOKUP_OBJECT (conf_impresion, label7, "label7");
  GLADE_HOOKUP_OBJECT (conf_impresion, label8, "label8");
  GLADE_HOOKUP_OBJECT_NO_REF (conf_impresion, tooltips, "tooltips");

  gtk_widget_grab_default (btn_aceptar_conf_impresion);
  gtk_window_add_accel_group (GTK_WINDOW (conf_impresion), accel_group);

  return conf_impresion;
}

