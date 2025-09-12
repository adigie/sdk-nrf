/*
 * Copyright (c) 2023 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#ifndef __TRUSTED_STORAGE_BACKEND_H_
#define __TRUSTED_STORAGE_BACKEND_H_

#include <psa/error.h>
#include <psa/storage_common.h>

typedef psa_status_t (*trusted_path_func)(void *arg, char *path, size_t path_size);

typedef psa_status_t (*trusted_key_func)(void *arg, uint8_t *key_buf, size_t key_length);

psa_status_t trusted_get_info(trusted_path_func get_path_func, void *func_arg,
			      struct psa_storage_info_t *p_info);

psa_status_t trusted_get(trusted_path_func get_path_func, trusted_key_func get_key_func,
			 void *func_arg, size_t data_offset, size_t data_length, void *p_data,
			 size_t *p_data_length);

psa_status_t trusted_set(trusted_path_func get_path_func, trusted_key_func get_key_func,
			 void *func_arg, size_t data_length, const void *p_data,
			 psa_storage_create_flags_t create_flags);

psa_status_t trusted_remove(trusted_path_func get_path_func, void *func_arg);

uint32_t trusted_get_support(void);

psa_status_t trusted_create(trusted_path_func get_path_func, trusted_key_func get_key_func,
			    void *func_arg, size_t capacity,
			    psa_storage_create_flags_t create_flags);

psa_status_t trusted_set_extended(trusted_path_func get_path_func, trusted_key_func get_key_func,
				  void *func_arg, size_t data_offset, size_t data_length,
				  const void *p_data);

#endif /* __TRUSTED_STORAGE_BACKEND_H_*/
