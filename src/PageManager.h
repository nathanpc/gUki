/**
 * PageManager.h
 * A simple module to manage the page loading, editing, and saving.
 *
 * @author Nathan Campos <hi@nathancampos.me>
 */

#ifndef _PAGEMANAGER_H_
#define _PAGEMANAGER_H_

#include <gtk/gtk.h>
#include <stdbool.h>

// Initialization.
void initialize_page_manager(GtkWidget **edit, GtkWidget **view);

// Misc.
GtkTextBuffer* get_page_editor_buffer();
bool is_article_opened();
void set_page_unsaved_changes(bool state);
bool check_page_unsaved_changes();

// Loading content.
void clear_page_contents();
bool load_article(const gint index);
bool load_template(const gint index);
void refresh_page_viewer();

// Saving and creation.
bool save_current_page();
size_t new_article(const char *fpath);
size_t new_template(const char *fpath);

#endif /* _PAGEMANAGER_H_ */
