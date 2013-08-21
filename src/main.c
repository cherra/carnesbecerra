/*
 * Archivo main.c inicial generado por Glade. Edítelo como
 * lo necesite, Glade nunca sobreescribirá este archivo.
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "interface.h"
#include "support.h"

int
main (int argc, char *argv[])
{
  GtkWidget *Inicio_Sesion;

#ifdef ENABLE_NLS
  bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
  bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
  textdomain (GETTEXT_PACKAGE);
#endif

  gtk_set_locale ();
  gtk_init (&argc, &argv);

  add_pixmap_directory (PACKAGE_DATA_DIR "/" PACKAGE "/pixmaps");

  /*
   * The following code was added by Glade to create one of each component
   * (except popup menus), just so that you see something after building
   * the project. Delete any components that you don't want shown initially.
   */
/*  caja = create_caja ();
  gtk_widget_show (caja);*/
		
  Inicio_Sesion = create_Inicio_Sesion();
  gtk_widget_show(Inicio_Sesion);
  gtk_main ();
  return 0;
}

