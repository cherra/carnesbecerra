#include <gtk/gtk.h>

//	DESCRIPCION DE LA FUNCION
// Dialogo de pregunta (SI, NO)
// Recibe la ventana de donde se envia el mensaje y el mensaje
// Regresa -1 si se presiona el boton Cancelar
// Regresa 1 si se presiona el boton OK

//	MODO DE USO
// if  ( pregunta (GTK_WINDOW (gtk_widget_get_toplevel (GTK_WIDGET(button))) , mensaje) )
// {
// 	g_print ("Aceptar. \n");
// }
// else
// {
//	g_print ("Cancelar.\n");
// }

gboolean
pregunta (GtkWindow *ventana, char *cad)
{
	int res;
	GtkWidget *diag;

	diag = gtk_message_dialog_new(ventana, GTK_DIALOG_MODAL , GTK_MESSAGE_QUESTION, GTK_BUTTONS_OK_CANCEL,"%s",cad);

	res = gtk_dialog_run (GTK_DIALOG (diag));

	gtk_widget_destroy (diag);

	if (res == -6)
	{
		return (FALSE);
	}
	else
	if (res == -5)
	{
		return (TRUE);
	}
}



//	DESCRIPCION DE LA FUNCION
// Dialogo de informaciòn
// Recibe la ventana de donde se envia el mensaje y el mensaje

void
Info2 (GtkWindow *ventana, char *cad)
{
	int res;
	GtkWidget *diag;

	diag = gtk_message_dialog_new(ventana, GTK_DIALOG_MODAL , GTK_MESSAGE_INFO, GTK_BUTTONS_OK,"%s",cad);

	res = gtk_dialog_run (GTK_DIALOG (diag));

	gtk_widget_destroy (diag);
}

//	DESCRIPCION DE LA FUNCION
// Dialogo de error
// Recibe la ventana de donde se envia el mensaje y el mensaje

void
Err_Info2 (GtkWindow *ventana, char *cad)
{
	int res;
	GtkWidget *diag;

	diag = gtk_message_dialog_new(ventana, GTK_DIALOG_MODAL ,  GTK_MESSAGE_ERROR, GTK_BUTTONS_CLOSE,"%s",cad);

	res = gtk_dialog_run (GTK_DIALOG (diag));

	gtk_widget_destroy (diag);
}
