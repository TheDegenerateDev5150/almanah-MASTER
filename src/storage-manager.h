/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */
/*
 * Almanah
 * Copyright (C) Philip Withnall 2008 <philip@tecnocode.co.uk>
 * 
 * Almanah is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Almanah is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Almanah.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ALMANAH_STORAGE_MANAGER_H
#define ALMANAH_STORAGE_MANAGER_H

#include <glib.h>
#include <glib-object.h>

#include "entry.h"
#include "link.h"

G_BEGIN_DECLS

#define ALMANAH_TYPE_STORAGE_MANAGER		(almanah_storage_manager_get_type ())
#define ALMANAH_STORAGE_MANAGER_ERROR		(almanah_storage_manager_error_quark ())
#define ALMANAH_STORAGE_MANAGER(o)		(G_TYPE_CHECK_INSTANCE_CAST ((o), ALMANAH_TYPE_STORAGE_MANAGER, AlmanahStorageManager))
#define ALMANAH_STORAGE_MANAGER_CLASS(k)	(G_TYPE_CHECK_CLASS_CAST((k), ALMANAH_TYPE_STORAGE_MANAGER, AlmanahStorageManagerClass))
#define ALMANAH_IS_STORAGE_MANAGER(o)		(G_TYPE_CHECK_INSTANCE_TYPE ((o), ALMANAH_TYPE_STORAGE_MANAGER))
#define ALMANAH_IS_STORAGE_MANAGER_CLASS(k)	(G_TYPE_CHECK_CLASS_TYPE ((k), ALMANAH_TYPE_STORAGE_MANAGER))
#define ALMANAH_STORAGE_MANAGER_GET_CLASS(o)	(G_TYPE_INSTANCE_GET_CLASS ((o), ALMANAH_TYPE_STORAGE_MANAGER, AlmanahStorageManagerClass))

typedef struct _AlmanahStorageManagerPrivate	AlmanahStorageManagerPrivate;

typedef struct {
	GObject parent;
	AlmanahStorageManagerPrivate *priv;
} AlmanahStorageManager;

typedef struct {
	GObjectClass parent;
} AlmanahStorageManagerClass;

typedef enum {
	ALMANAH_STORAGE_MANAGER_ERROR_UNSUPPORTED,
	ALMANAH_STORAGE_MANAGER_ERROR_OPENING_FILE,
	ALMANAH_STORAGE_MANAGER_ERROR_CREATING_CONTEXT,
	ALMANAH_STORAGE_MANAGER_ERROR_DECRYPTING,
	ALMANAH_STORAGE_MANAGER_ERROR_ENCRYPTING,
	ALMANAH_STORAGE_MANAGER_ERROR_GETTING_KEY,
	ALMANAH_STORAGE_MANAGER_ERROR_RUNNING_QUERY
} AlmanahStorageManagerError;

typedef gint (*AlmanahQueryCallback) (gpointer user_data, gint columns, gchar **data, gchar **column_names);

typedef struct {
	gchar **data;
	gint rows;
	gint columns;
} AlmanahQueryResults;

GType almanah_storage_manager_get_type (void);
GQuark almanah_storage_manager_error_quark (void);
AlmanahStorageManager *almanah_storage_manager_new (const gchar *filename);

gboolean almanah_storage_manager_connect (AlmanahStorageManager *self, GError **error);
gboolean almanah_storage_manager_disconnect (AlmanahStorageManager *self, GError **error);

AlmanahQueryResults *almanah_storage_manager_query (AlmanahStorageManager *self, const gchar *query, GError **error, ...);
void almanah_storage_manager_free_results (AlmanahQueryResults *results);
gboolean almanah_storage_manager_query_async (AlmanahStorageManager *self, const gchar *query, const AlmanahQueryCallback callback, gpointer user_data, GError **error, ...);

gboolean almanah_storage_manager_get_statistics (AlmanahStorageManager *self, guint *entry_count, guint *link_count, guint *character_count);

gboolean almanah_storage_manager_entry_exists (AlmanahStorageManager *self, GDate *date);
AlmanahEntry *almanah_storage_manager_get_entry (AlmanahStorageManager *self, GDate *date);
gboolean almanah_storage_manager_set_entry (AlmanahStorageManager *self, AlmanahEntry *entry);
guint almanah_storage_manager_search_entries (AlmanahStorageManager *self, const gchar *search_string, GDate *matches[]);

gboolean *almanah_storage_manager_get_month_marked_days (AlmanahStorageManager *self, GDateYear year, GDateMonth month);

AlmanahLink **almanah_storage_manager_get_entry_links (AlmanahStorageManager *self, GDate *date);
gboolean almanah_storage_manager_add_entry_link (AlmanahStorageManager *self, GDate *date, AlmanahLink *link);
gboolean almanah_storage_manager_remove_entry_link (AlmanahStorageManager *self, GDate *date, const gchar *link_type_id);

G_END_DECLS

#endif /* !ALMANAH_STORAGE_MANAGER_H */
