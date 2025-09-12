/*
 * Copyright (c) 2023 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include <psa/internal_trusted_storage.h>
#include <psa/storage_common.h>
#include <psa/trusted_storage_backend.h>

#include <stdio.h>

#include "../storage_backend.h"
#include "../aead/aead_key.h"

#define INVALID_UID 0U

static psa_status_t get_path(void *arg, char *path, size_t path_size)
{
	return storage_create_filename_from_uid(path, path_size,
						CONFIG_PSA_INTERNAL_TRUSTED_STORAGE_PREFIX,
						*(psa_storage_uid_t *)arg);
}

static psa_status_t get_key(void *arg, uint8_t *key_buf, size_t key_length)
{
	return trusted_storage_get_key((const uint8_t *)arg, sizeof(psa_storage_uid_t), key_buf,
				       key_length);
}

psa_status_t psa_its_get_info(psa_storage_uid_t uid, struct psa_storage_info_t *p_info)
{
	if (uid == INVALID_UID) {
		return PSA_ERROR_INVALID_ARGUMENT;
	}

	return trusted_get_info(get_path, &uid, p_info);
}

psa_status_t psa_its_get(psa_storage_uid_t uid, size_t data_offset, size_t data_length,
			 void *p_data, size_t *p_data_length)
{
	if (uid == INVALID_UID) {
		return PSA_ERROR_INVALID_ARGUMENT;
	}

	return trusted_get(get_path, get_key, &uid, data_offset, data_length, p_data,
			   p_data_length);
}

psa_status_t psa_its_set(psa_storage_uid_t uid, size_t data_length, const void *p_data,
			 psa_storage_create_flags_t create_flags)
{
	if (uid == INVALID_UID) {
		return PSA_ERROR_INVALID_ARGUMENT;
	}

	return trusted_set(get_path, get_key, &uid, data_length, p_data, create_flags);
}

psa_status_t psa_its_remove(psa_storage_uid_t uid)
{
	if (uid == INVALID_UID) {
		return PSA_ERROR_INVALID_ARGUMENT;
	}

	return trusted_remove(get_path, &uid);
}
