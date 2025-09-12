/*
 * Copyright (c) 2024 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#include "persistent_storage_secure.h"
#include "custom_storage.h"

#include <zephyr/settings/settings.h>

namespace
{
struct DeleteSubtreeEntry {
	const char *prefix;
	int result;
};

int DeleteSubtreeCallback(const char *name, size_t entrySize, settings_read_cb readCb, void *cbArg, void *param)
{
	DeleteSubtreeEntry &entry = *static_cast<DeleteSubtreeEntry *>(param);
	char fullKey[SETTINGS_MAX_NAME_LEN + 1];

	// name comes from Zephyr settings subsystem so it is guaranteed to fit in the buffer.
	(void)snprintf(fullKey, sizeof(fullKey), "%s/%s", entry.prefix, name);
	const int result = settings_delete(fullKey);

	// Return the first error, but continue removing remaining keys anyway.
	if (entry.result == 0) {
		entry.result = result;
	}

	return 0;
}
} // namespace

namespace Nrf
{

PSErrorCode PersistentStorageSecure::_SecureInit(PersistentStorageNode *rootNode)
{
	if (rootNode == nullptr) {
		return PSErrorCode::Failure;
	}

	mRootNode = rootNode;

	return settings_load() ? PSErrorCode::Failure : PSErrorCode::Success;
}

PSErrorCode PersistentStorageSecure::_SecureStore(PersistentStorageNode *node, const void *data, size_t dataSize)
{
	if (!data || !node) {
		return PSErrorCode::Failure;
	}

	char key[PersistentStorageNode::kMaxKeyNameLength];

	if (!node->GetKey(key)) {
		return PSErrorCode::Failure;
	}

	psa_status_t status = psa_custom_set(key, dataSize, data, PSA_STORAGE_FLAG_NONE);
	return (status ? PSErrorCode::Failure : PSErrorCode::Success);
}

PSErrorCode PersistentStorageSecure::_SecureLoad(PersistentStorageNode *node, void *data, size_t dataMaxSize,
						 size_t &outSize)
{
	if (!data || !node) {
		return PSErrorCode::Failure;
	}

	char key[PersistentStorageNode::kMaxKeyNameLength];

	if (!node->GetKey(key)) {
		return PSErrorCode::Failure;
	}

	psa_status_t status = psa_custom_get(key, 0, dataMaxSize, data, &outSize);
	return (status == PSA_SUCCESS ? PSErrorCode::Success : PSErrorCode::Failure);
}

PSErrorCode PersistentStorageSecure::_SecureHasEntry(PersistentStorageNode *node)
{
	if (!node) {
		return PSErrorCode::Failure;
	}

	char key[PersistentStorageNode::kMaxKeyNameLength];

	if (!node->GetKey(key)) {
		return PSErrorCode::Failure;
	}

	struct psa_storage_info_t info;
	psa_status_t status = psa_custom_get_info(key, &info);
	if (status != PSA_SUCCESS || info.size == 0) {
		return PSErrorCode::Failure;
	}

	return PSErrorCode::Success;
}

PSErrorCode PersistentStorageSecure::_SecureRemove(PersistentStorageNode *node)
{
	if (!node) {
		return PSErrorCode::Failure;
	}

	char key[PersistentStorageNode::kMaxKeyNameLength];

	if (!node->GetKey(key)) {
		return PSErrorCode::Failure;
	}

	psa_status_t status = psa_custom_remove(key);
	return (status == PSA_SUCCESS ? PSErrorCode::Success : PSErrorCode::Failure);
}

PSErrorCode PersistentStorageSecure::_SecureFactoryReset()
{
	char key[PersistentStorageNode::kMaxKeyNameLength];
	char path[SETTINGS_MAX_NAME_LEN + 1];

	if (!mRootNode->GetKey(key)) {
		return PSErrorCode::Failure;
	}

	int ret = snprintf(path, sizeof(path), "%s/%s", CUSTOM_STORAGE_PREFIX, key);
	if (ret < 0 || (size_t)ret >= sizeof(path)) {
		return PSErrorCode::Failure;
	}

	DeleteSubtreeEntry entry{ path, 0 };
	int result = settings_load_subtree_direct(path, DeleteSubtreeCallback, &entry);

	if (result == 0 && entry.result == 0) {
		return PSErrorCode::Success;
	}

	return PSErrorCode::Failure;
}

} /* namespace Nrf */
