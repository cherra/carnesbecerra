#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif 

#include <gtk/gtk.h>

#include "string.h"


/** VALIDA SI ES UN NUMERO (1-9) 0 "." ò "-" **/
// Si int_float = 0, solo checa si es entero positivo
// Si int_float = 1, checa si es entero positivo o negativo
// Si int_float = 2, solo checa si es flotante positivo
// Si int_float = 3, checa si es flotante positivo o negativo

gboolean es_signo_numeracion (GtkEditable *editable, int int_float);

gboolean es_signo_numeracion (GtkEditable *editable, int int_float)
{
	gint posicion;
	gchar *caracters;
	char caracter;
	
	int flag_punto;
	int flag_negativo;
	int i;

	posicion=gtk_editable_get_position(editable);

	caracters = gtk_editable_get_chars(editable, posicion, posicion+1);
	caracter = caracters[0];
	caracters = gtk_editable_get_chars(editable, 0, -1);

	// Se valida
	
	if (int_float == 0) // Si es entero positivo
	{
		if( (caracter >= 48) && (caracter <= 57))
			return TRUE;
	}
	else if (int_float == 1) // Si es entero positivo o negativo
	{
		if(  ((caracter >= 48) && (caracter <= 57)) || (caracter == 45) )
		{
			flag_negativo = 0;
			for (i=0; i<strlen(caracters); i++)
				if (caracters[i] == '-' && i != posicion && caracter == '-')
					flag_negativo = 1;
					
			if (flag_negativo == 0 && posicion != 0 && caracter == '-')
				flag_negativo = 1;
			
			if (flag_negativo == 0)
				return TRUE;
		}
	}
	else if (int_float == 2) // Si es Flotante positivo
	{
		if(  ((caracter >= 48) && (caracter <= 57)) || (caracter == 46) )
		{
			flag_punto = 0;
			for (i=0; i<strlen(caracters); i++)
				if (caracters[i] == '.' && i != posicion && caracter == '.')
					flag_punto = 1;
			if (flag_punto == 0)
				return TRUE;
		}
	}
	else if (int_float == 3) // Si es flotante positivo o negativo
	{
		if(  ((caracter >= 48) && (caracter <= 57)) || (caracter == 45) || (caracter == 46))
		{
			flag_punto = 0;
			for (i=0; i<strlen(caracters); i++)
				if (caracters[i] == '.' && i != posicion && caracter == '.')
					flag_punto = 1;
					
			flag_negativo = 0;
			for (i=0; i<strlen(caracters); i++)
				if (caracters[i] == '-' && i != posicion && caracter == '-')
					flag_negativo = 1;
					
			if (flag_negativo == 0 && posicion != 0 && caracter == '-')
				flag_negativo = 1;
			
			if (flag_punto == 0 && flag_negativo == 0)
				return TRUE;
		}
	}
	
	gtk_editable_delete_text(editable, posicion, posicion+1);
	return FALSE;
}

/** FUNCION PARA CAMBIAR EL FOCUS DE UN ENTRY A OTRO **/

void cambiar_focus_entry (GtkWidget *editable_focus, char * nom_nuevo_focus);

void cambiar_focus_entry (GtkWidget *editable_focus, char * nom_nuevo_focus)
{
	GtkWidget *entry_nuevo_focus;
	int len_chars_entry;
	
	entry_nuevo_focus = lookup_widget (editable_focus,nom_nuevo_focus);
	
	len_chars_entry = gtk_entry_get_max_length ( GTK_ENTRY (editable_focus));
	
	if (gtk_editable_get_position (GTK_EDITABLE (editable_focus)) >= len_chars_entry - 1)
		gtk_widget_grab_focus (entry_nuevo_focus);

}
