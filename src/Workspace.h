/**
 * Workspace.h
 * A class to manage the Uki workspace and its associated controls.
 *
 * @author: Nathan Campos <nathan@innoveworkshop.com>
 */

#ifndef _WORKSPACE_H_
#define _WORKSPACE_H_

#include <gtk/gtk.h>
#include <stdbool.h>

// Initialization.
void initialize_workspace(GtkWidget *tview);

// TreeView Population.
void populate_workspace_treeview();

#endif /* _WORKSPACE_H_ */
