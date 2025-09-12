/*
 * Copyright (c) 2025 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */
#pragma once

#include <stddef.h>

#include "psa/error.h"
#include "psa/storage_common.h"

#define CUSTOM_STORAGE_PREFIX "psc"

#ifdef __cplusplus
extern "C" {
#endif

psa_status_t psa_custom_set(const char *key, size_t data_length, const void *p_data,
			    psa_storage_create_flags_t create_flags);

psa_status_t psa_custom_get(const char *key, size_t data_offset, size_t data_size, void *p_data, size_t *p_data_length);

psa_status_t psa_custom_get_info(const char *key, struct psa_storage_info_t *p_info);

psa_status_t psa_custom_remove(const char *key);

#ifdef __cplusplus
}
#endif
