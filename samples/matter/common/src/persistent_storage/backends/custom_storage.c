/*
 * Copyright (c) 2025 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include "custom_storage.h"
#include <psa/trusted_storage_backend.h>

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <hw_unique_key.h>
#include <zephyr/init.h>

#ifndef CONFIG_TRUSTED_STORAGE_STORAGE_BACKEND_SETTINGS
#error "CONFIG_TRUSTED_STORAGE_STORAGE_BACKEND_SETTINGS is not set"
#endif

#define AEAD_KEY_SIZE (32)

#ifdef HUK_HAS_KMU
#define KEYSLOT HUK_KEYSLOT_MKEK
#else
#define KEYSLOT HUK_KEYSLOT_KDR
#endif

#define TRUSTED_STORAGE_CUSTOM_STORAGE_FILENAME_PATTERN "%s/%s"

static psa_status_t get_path(void *arg, char *path, size_t path_size)
{
	int ret;

	ret = snprintf(path, path_size, TRUSTED_STORAGE_CUSTOM_STORAGE_FILENAME_PATTERN, CUSTOM_STORAGE_PREFIX,
		       (const char *)arg);

	/* snprintf doc:
	 * Notice that only when this returned value is non-negative and less than n, the string has
	 * been completely written
	 */
	if (ret < 0 || ret >= path_size) {
		return PSA_ERROR_STORAGE_FAILURE;
	}

	return PSA_SUCCESS;
}

static psa_status_t get_key(void *arg, uint8_t *key_buf, size_t key_length)
{
	int result;

	if (key_length < AEAD_KEY_SIZE) {
		return PSA_ERROR_INVALID_ARGUMENT;
	}

	if (!hw_unique_key_are_any_written()) {
		return PSA_ERROR_BAD_STATE;
	}

	result = hw_unique_key_derive_key(KEYSLOT, NULL, 0, (const uint8_t *)arg, strlen((const char *)arg), key_buf,
					  key_length);
	if (result != HW_UNIQUE_KEY_SUCCESS) {
		return PSA_ERROR_BAD_STATE;
	}

	return PSA_SUCCESS;
}

psa_status_t psa_custom_get_info(const char *key, struct psa_storage_info_t *p_info)
{
	if (key == NULL) {
		return PSA_ERROR_INVALID_ARGUMENT;
	}

	return trusted_get_info(get_path, (void *)key, p_info);
}

psa_status_t psa_custom_get(const char *key, size_t data_offset, size_t data_length, void *p_data,
			    size_t *p_data_length)
{
	if (key == NULL) {
		return PSA_ERROR_INVALID_ARGUMENT;
	}

	return trusted_get(get_path, get_key, (void *)key, data_offset, data_length, p_data, p_data_length);
}

psa_status_t psa_custom_set(const char *key, size_t data_length, const void *p_data,
			    psa_storage_create_flags_t create_flags)
{
	if (key == NULL) {
		return PSA_ERROR_INVALID_ARGUMENT;
	}
	return trusted_set(get_path, get_key, (void *)key, data_length, p_data, create_flags);
}

psa_status_t psa_custom_remove(const char *key)
{
	if (key == NULL) {
		return PSA_ERROR_INVALID_ARGUMENT;
	}

	return trusted_remove(get_path, (void *)key);
}
