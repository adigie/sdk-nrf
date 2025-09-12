/*
 * Copyright (c) 2023 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef __STORAGE_BACKEND_H_
#define __STORAGE_BACKEND_H_

#include <psa/error.h>
#include <psa/storage_common.h>

/* Function for getting a file path from an argument */
typedef psa_status_t (*storage_path_func)(void *arg, char *path, size_t path_size);

/* Creates a filename path from an UID */
psa_status_t storage_create_filename_from_uid(char *filename, const size_t filename_size,
					      const char *prefix, const psa_storage_uid_t uid);

/* Gets an object up to object_size size */
psa_status_t storage_get_object(storage_path_func get_path_func, void *arg, void *object_data,
				const size_t object_size, size_t *object_length);

/* Writes an object */
psa_status_t storage_set_object(storage_path_func get_path_func, void *arg, const void *object_data,
				const size_t object_size);

/* Deletes an object */
psa_status_t storage_remove_object(storage_path_func get_path_func, void *arg);

#endif /* __STORAGE_BACKEND_H_*/
