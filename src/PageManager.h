/**
 * PageManager.h
 * A simple module to manage the page loading, editing, and saving.
 *
 * @author: Nathan Campos <hi@nathancampos.me>
 */

#ifndef _PAGEMANAGER_H_
#define _PAGEMANAGER_H_

#include <gtk/gtk.h>
#include <stdbool.h>

// Initialization.
void initialize_page_manager(GtkWidget **edit, GtkWidget **view);

// Loading content.
bool load_article(const gint index);
bool load_template(const gint index);
void refresh_page_viewer();

#endif /* _PAGEMANAGER_H_ */
