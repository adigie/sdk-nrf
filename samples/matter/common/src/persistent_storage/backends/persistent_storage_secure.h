/*
 * Copyright (c) 2024 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

#pragma once

#include "../persistent_storage_common.h"

namespace Nrf
{
class PersistentStorageSecure {
protected:
	PSErrorCode _NonSecureInit(PersistentStorageNode *rootNode);
	PSErrorCode _NonSecureStore(PersistentStorageNode *node, const void *data, size_t dataSize);
	PSErrorCode _NonSecureLoad(PersistentStorageNode *node, void *data, size_t dataMaxSize, size_t &outSize);
	PSErrorCode _NonSecureHasEntry(PersistentStorageNode *node);
	PSErrorCode _NonSecureRemove(PersistentStorageNode *node);
	PSErrorCode _NonSecureFactoryReset();

	PSErrorCode _SecureInit(PersistentStorageNode *rootNode);
	PSErrorCode _SecureStore(PersistentStorageNode *node, const void *data, size_t dataSize);
	PSErrorCode _SecureLoad(PersistentStorageNode *node, void *data, size_t dataMaxSize, size_t &outSize);
	PSErrorCode _SecureHasEntry(PersistentStorageNode *node);
	PSErrorCode _SecureRemove(PersistentStorageNode *node);
	PSErrorCode _SecureFactoryReset();

private:
	PersistentStorageNode *mRootNode{ nullptr };
};

inline PSErrorCode PersistentStorageSecure::_NonSecureInit(PersistentStorageNode *rootNode)
{
	return PSErrorCode::NotSupported;
};

inline PSErrorCode PersistentStorageSecure::_NonSecureStore(PersistentStorageNode *node, const void *data,
							    size_t dataSize)
{
	return PSErrorCode::NotSupported;
}

inline PSErrorCode PersistentStorageSecure::_NonSecureLoad(PersistentStorageNode *node, void *data, size_t dataMaxSize,
							   size_t &outSize)
{
	return PSErrorCode::NotSupported;
}

inline PSErrorCode PersistentStorageSecure::_NonSecureHasEntry(PersistentStorageNode *node)
{
	return PSErrorCode::NotSupported;
}

inline PSErrorCode PersistentStorageSecure::_NonSecureRemove(PersistentStorageNode *node)
{
	return PSErrorCode::NotSupported;
}

inline PSErrorCode PersistentStorageSecure::_NonSecureFactoryReset()
{
	return PSErrorCode::NotSupported;
}

} /* namespace Nrf */
