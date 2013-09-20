#include <gtk/gtk.h>
#include <string.h>
#include <gdk/gdkkeysyms.h>

#define GLADE_HOOKUP_OBJECT(component,widget,name) \
  gtk_object_set_data_full (GTK_OBJECT (component), name, \
    gtk_widget_ref (widget), (GtkDestroyNotify) gtk_widget_unref)

#define GLADE_HOOKUP_OBJECT_NO_REF(component,widget,name) \
  gtk_object_set_data (GTK_OBJECT (component), name, widget)



/////////Deficicion de las funciones
GtkWidget* create_dialog_Aviso (void);
GtkWidget* create_dialog_Error (void);
GtkWidget* create_dialog_Confirma(void);


//Variable global para poner los mensajes en el label de cada ventana de dialogo
char mensaje[512];


//////////////////////////////////Funcioes  para mensajes de informacion

//Esta function abre la ventana de dialogo
void Info (char *informacion)
{
  GtkWidget *dialogo;
  gint result;

  strcpy(mensaje, informacion);
  dialogo = create_dialog_Aviso();
//  gtk_widget_show (create_dialog_Aviso());
  result = gtk_dialog_run (GTK_DIALOG (dialogo));
  gtk_widget_destroy(dialogo);
//  return (result);
}


gint Confirma(char *informacion)
{
  GtkWidget *dialogo;
  gint result;

  strcpy(mensaje, informacion);
  dialogo = create_dialog_Confirma();
  //gtk_widget_show(dialogo);
  result = gtk_dialog_run (GTK_DIALOG (dialogo));
  gtk_widget_destroy(dialogo);
  return (result);
}


//Esta funcion pone el mensaje correspondiente en el label del mensaje
void
on_dialog_Aviso_show                   (GtkLabel       *label,
                                        gpointer         user_data)
{
  gtk_label_set_text (label,mensaje);
}

void
on_dialog_Confirma_show                   (GtkLabel       *label,
                                        gpointer         user_data)
{
  gtk_label_set_text(label, mensaje);
}


//Esta funcion destruye la ventana de dialogo
void
on_okbutton2_clicked                   (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy (GTK_WIDGET (button));
}




//////////////////////////////Funciones para mensaje de error

//Esta function abre la ventana de dialogo
void Err_Info (char *informacion)
{
  strcpy(mensaje, informacion);
  gtk_widget_show (create_dialog_Error());
}



//Esta funcion pone el mensaje de error en el label
void
on_dialog_Error_show                   (GtkLabel       *label,
                                        gpointer         user_data)
{
  gtk_label_set_text (label,mensaje);
}



/////////			Esta funcoin destruye la ventana de error
void
on_okbutton3_clicked                   (GtkButton       *button,
                                        gpointer         user_data)
{
  gtk_widget_destroy (GTK_WIDGET (button));
}




////////////////////Funcione para crear la ventana de dialogo de Error
GtkWidget*
create_dialog_Error (void)
{
  GtkWidget *dialog_Error;
  GtkWidget *dialog_vbox5;
  GtkWidget *table19;
  GtkWidget *image61;
  GtkWidget *lblError;
  GtkWidget *dialog_action_area4;
  GtkWidget *okbutton3;

  dialog_Error = gtk_dialog_new ();
  gtk_widget_set_name (dialog_Error, "dialog_Error");
  gtk_window_set_title (GTK_WINDOW (dialog_Error), _("Error"));
  gtk_window_set_resizable (GTK_WINDOW (dialog_Error), FALSE);
  gtk_window_set_position (GTK_WINDOW (dialog_Error), GTK_WIN_POS_CENTER);

  dialog_vbox5 = GTK_DIALOG (dialog_Error)->vbox;
  gtk_widget_set_name (dialog_vbox5, "dialog_vbox5");
  gtk_widget_show (dialog_vbox5);

  table19 = gtk_table_new (1, 2, FALSE);
  gtk_widget_set_name (table19, "table19");
  gtk_widget_show (table19);
  gtk_box_pack_start (GTK_BOX (dialog_vbox5), table19, TRUE, TRUE, 0);

  image61 = gtk_image_new_from_stock ("gtk-dialog-error", GTK_ICON_SIZE_DIALOG);
  gtk_widget_set_name (image61, "image61");
  gtk_widget_show (image61);
  gtk_table_attach (GTK_TABLE (table19), image61, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL), 0, 0);

  lblError = gtk_label_new (_("label136"));
  gtk_widget_set_name (lblError, "lblError");
  gtk_widget_show (lblError);
  gtk_table_attach (GTK_TABLE (table19), lblError, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (lblError), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (lblError), 0, 0.5);

  dialog_action_area4 = GTK_DIALOG (dialog_Error)->action_area;
  gtk_widget_set_name (dialog_action_area4, "dialog_action_area4");
  gtk_widget_show (dialog_action_area4);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area4), GTK_BUTTONBOX_EDGE);

  okbutton3 = gtk_button_new_from_stock ("gtk-ok");
  gtk_widget_set_name (okbutton3, "okbutton3");
  gtk_widget_show (okbutton3);
  gtk_dialog_add_action_widget (GTK_DIALOG (dialog_Error), okbutton3, GTK_RESPONSE_OK);
  GTK_WIDGET_SET_FLAGS (okbutton3, GTK_CAN_DEFAULT);

  gtk_signal_connect_object (GTK_OBJECT (dialog_Error), "show",
                             GTK_SIGNAL_FUNC (on_dialog_Error_show),
                             GTK_OBJECT (lblError));
  gtk_signal_connect_object (GTK_OBJECT (okbutton3), "clicked",
                             GTK_SIGNAL_FUNC (on_okbutton3_clicked),
                             GTK_OBJECT (dialog_Error));

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (dialog_Error, dialog_Error, "dialog_Error");
  GLADE_HOOKUP_OBJECT_NO_REF (dialog_Error, dialog_vbox5, "dialog_vbox5");
  GLADE_HOOKUP_OBJECT (dialog_Error, table19, "table19");
  GLADE_HOOKUP_OBJECT (dialog_Error, image61, "image61");
  GLADE_HOOKUP_OBJECT (dialog_Error, lblError, "lblError");
  GLADE_HOOKUP_OBJECT_NO_REF (dialog_Error, dialog_action_area4, "dialog_action_area4");
  GLADE_HOOKUP_OBJECT (dialog_Error, okbutton3, "okbutton3");

  return dialog_Error;
}




///////////////////Funcion para crear la ventana de Aviso
GtkWidget*
create_dialog_Aviso (void)
{
  GtkWidget *dialog_Aviso;
  GtkWidget *dialog_vbox4;
  GtkWidget *table18;
  GtkWidget *image60;
  GtkWidget *lblMensaje;
  GtkWidget *dialog_action_area3;
  GtkWidget *okbutton2;

  dialog_Aviso = gtk_dialog_new ();
  gtk_widget_set_name (dialog_Aviso, "dialog_Aviso");
  gtk_window_set_title (GTK_WINDOW (dialog_Aviso), _("Informaci\303\262n"));
  gtk_window_set_resizable (GTK_WINDOW (dialog_Aviso), FALSE);
  gtk_window_set_position (GTK_WINDOW (dialog_Aviso), GTK_WIN_POS_CENTER);

  dialog_vbox4 = GTK_DIALOG (dialog_Aviso)->vbox;
  gtk_widget_set_name (dialog_vbox4, "dialog_vbox4");
  gtk_widget_show (dialog_vbox4);

  table18 = gtk_table_new (1, 2, FALSE);
  gtk_widget_set_name (table18, "table18");
  gtk_widget_show (table18);
  gtk_box_pack_start (GTK_BOX (dialog_vbox4), table18, TRUE, TRUE, 0);

  image60 = gtk_image_new_from_stock ("gtk-dialog-info", GTK_ICON_SIZE_DIALOG);
  gtk_widget_set_name (image60, "image60");
  gtk_widget_show (image60);
  gtk_table_attach (GTK_TABLE (table18), image60, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL), 0, 0);

  lblMensaje = gtk_label_new (_("Info"));
  gtk_widget_set_name (lblMensaje, "lblMensaje");
  gtk_widget_show (lblMensaje);
  gtk_table_attach (GTK_TABLE (table18), lblMensaje, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (lblMensaje), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (lblMensaje), 0, 0.5);

  dialog_action_area3 = GTK_DIALOG (dialog_Aviso)->action_area;
  gtk_widget_set_name (dialog_action_area3, "dialog_action_area3");
  gtk_widget_show (dialog_action_area3);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area3), GTK_BUTTONBOX_EDGE);

  okbutton2 = gtk_button_new_from_stock ("gtk-ok");
  gtk_widget_set_name (okbutton2, "okbutton2");
  gtk_widget_show (okbutton2);
  gtk_dialog_add_action_widget (GTK_DIALOG (dialog_Aviso), okbutton2, GTK_RESPONSE_OK);
  GTK_WIDGET_SET_FLAGS (okbutton2, GTK_CAN_DEFAULT);

  gtk_signal_connect_object (GTK_OBJECT (dialog_Aviso), "show",
                             GTK_SIGNAL_FUNC (on_dialog_Aviso_show),
                             GTK_OBJECT (lblMensaje));
  /*gtk_signal_connect_object (GTK_OBJECT (okbutton2), "clicked",
                             GTK_SIGNAL_FUNC (on_okbutton2_clicked),
                             GTK_OBJECT (dialog_Aviso));*/

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (dialog_Aviso, dialog_Aviso, "dialog_Aviso");
  GLADE_HOOKUP_OBJECT_NO_REF (dialog_Aviso, dialog_vbox4, "dialog_vbox4");
  GLADE_HOOKUP_OBJECT (dialog_Aviso, table18, "table18");
  GLADE_HOOKUP_OBJECT (dialog_Aviso, image60, "image60");
  GLADE_HOOKUP_OBJECT (dialog_Aviso, lblMensaje, "lblMensaje");
  GLADE_HOOKUP_OBJECT_NO_REF (dialog_Aviso, dialog_action_area3, "dialog_action_area3");
  GLADE_HOOKUP_OBJECT (dialog_Aviso, okbutton2, "okbutton2");

  return dialog_Aviso;
}

GtkWidget*
create_dialog_Confirma (void)
{
  GtkWidget *dialog_Confirma;
  GtkWidget *dialog_vbox4;
  GtkWidget *table18;
  GtkWidget *image60;
  GtkWidget *lblMensaje;
  GtkWidget *dialog_action_area3;
  GtkWidget *okbutton2;
  GtkWidget *okbutton;
  GtkWidget *image86;
  GtkWidget *image85;

  dialog_Confirma = gtk_dialog_new ();
  gtk_widget_set_name (dialog_Confirma, "dialog_Confirma");
  gtk_window_set_title (GTK_WINDOW (dialog_Confirma), _("Confirmación"));
  gtk_window_set_resizable (GTK_WINDOW (dialog_Confirma), FALSE);
  gtk_window_set_position (GTK_WINDOW (dialog_Confirma), GTK_WIN_POS_CENTER);

  dialog_vbox4 = GTK_DIALOG (dialog_Confirma)->vbox;
  gtk_widget_set_name (dialog_vbox4, "dialog_vbox4");
  gtk_widget_show (dialog_vbox4);

  table18 = gtk_table_new (1, 2, FALSE);
  gtk_widget_set_name (table18, "table18");
  gtk_widget_show (table18);
  gtk_box_pack_start (GTK_BOX (dialog_vbox4), table18, TRUE, TRUE, 0);

  image60 = gtk_image_new_from_stock ("gtk-dialog-question", GTK_ICON_SIZE_DIALOG);
  gtk_widget_set_name (image60, "image60");
  gtk_widget_show (image60);
  gtk_table_attach (GTK_TABLE (table18), image60, 0, 1, 0, 1,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL), 0, 0);

  lblMensaje = gtk_label_new (_("Info"));
  gtk_widget_set_name (lblMensaje, "lblMensaje");
  gtk_widget_show (lblMensaje);
  gtk_table_attach (GTK_TABLE (table18), lblMensaje, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_label_set_justify (GTK_LABEL (lblMensaje), GTK_JUSTIFY_LEFT);
  gtk_misc_set_alignment (GTK_MISC (lblMensaje), 0, 0.5);

  dialog_action_area3 = GTK_DIALOG (dialog_Confirma)->action_area;
  gtk_widget_set_name (dialog_action_area3, "dialog_action_area3");
  gtk_widget_show (dialog_action_area3);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area3), GTK_BUTTONBOX_EDGE);

  okbutton = gtk_button_new();
  gtk_widget_set_name (okbutton, "okbutton");
  gtk_widget_show (okbutton);
  gtk_dialog_add_action_widget (GTK_DIALOG (dialog_Confirma), okbutton, GTK_RESPONSE_REJECT);
  gtk_widget_set_size_request (okbutton, 103, 35);
  GTK_WIDGET_SET_FLAGS (okbutton, GTK_CAN_DEFAULT);
  gtk_button_set_relief (GTK_BUTTON (okbutton), GTK_RELIEF_NONE);

  image85 = create_pixmap (dialog_Confirma, "carnesbecerra_cancelar.jpg");
  gtk_widget_set_name (image85, "image85");
  gtk_widget_show (image85);
  gtk_container_add (GTK_CONTAINER (okbutton), image85);

//  okbutton2 = gtk_button_new_from_stock ("gtk-ok");
  okbutton2 = gtk_button_new ();
  gtk_widget_set_name (okbutton2, "okbutton2");
  gtk_widget_show (okbutton2);
  gtk_dialog_add_action_widget (GTK_DIALOG (dialog_Confirma), okbutton2, GTK_RESPONSE_ACCEPT);
  gtk_widget_set_size_request (okbutton2, 103, 35);
  GTK_WIDGET_SET_FLAGS (okbutton2, GTK_CAN_DEFAULT);
  gtk_button_set_relief (GTK_BUTTON (okbutton2), GTK_RELIEF_NONE);

  image86 = create_pixmap (dialog_Confirma, "carnesbecerra_aceptar.jpg");
  gtk_widget_set_name (image86, "image86");
  gtk_widget_show (image86);
  gtk_container_add (GTK_CONTAINER (okbutton2), image86);

  gtk_signal_connect_object (GTK_OBJECT (dialog_Confirma), "show",
                             GTK_SIGNAL_FUNC (on_dialog_Confirma_show),
                             GTK_OBJECT (lblMensaje));
/*  gtk_signal_connect_object (GTK_OBJECT (okbutton2), "clicked",
                             GTK_SIGNAL_FUNC (on_okbutton2_clicked),
                             GTK_OBJECT (dialog_Confirma));*/

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (dialog_Confirma, dialog_Confirma, "dialog_Confirma");
  GLADE_HOOKUP_OBJECT_NO_REF (dialog_Confirma, dialog_vbox4, "dialog_vbox4");
  GLADE_HOOKUP_OBJECT (dialog_Confirma, table18, "table18");
  GLADE_HOOKUP_OBJECT (dialog_Confirma, image60, "image60");
  GLADE_HOOKUP_OBJECT (dialog_Confirma, lblMensaje, "lblMensaje");
  GLADE_HOOKUP_OBJECT_NO_REF (dialog_Confirma, dialog_action_area3, "dialog_action_area3");
  GLADE_HOOKUP_OBJECT (dialog_Confirma, okbutton2, "okbutton2");

  return dialog_Confirma;	
}
