/**
 * Workspace.h
 * A class to manage the Uki workspace and its associated controls.
 *
 * @author Nathan Campos <nathan@innoveworkshop.com>
 */

#ifndef _WORKSPACE_H_
#define _WORKSPACE_H_

#include <gtk/gtk.h>
#include <stdbool.h>

// Row types.
#define ROW_TYPE_TITLE    0
#define ROW_TYPE_FOLDER   1
#define ROW_TYPE_ARTICLE  2
#define ROW_TYPE_TEMPLATE 3

// Tree view columns enum.
enum {
	COL_NAME = 0,
	COL_INDEX,
	COL_TYPE,
	NUM_COLS
};

// Initialization and destruction.
void initialize_workspace(GtkWidget *tview);
void close_workspace();
bool open_workspace(const char *wiki_root);

// TreeView Population.
void populate_workspace_treeview();

#endif /* _WORKSPACE_H_ */
