// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWEthereumAddress.h>

#include "../Data.h"
#include "../Loom/Address.h"

#include <TrustWalletCore/TWHash.h>
#include <TrustWalletCore/TWPublicKey.h>

#include <TrezorCrypto/ecdsa.h>
#include <memory>
#include <string>

using namespace TW;
using namespace TW::Loom;

bool TWLoomAddressEqual(struct TWEthereumAddress *_Nonnull lhs, struct TWEthereumAddress *_Nonnull rhs) {
    return lhs->impl == rhs->impl;
}

bool TWLoomAddressIsValidString(TWString *_Nonnull string) {
    auto s = reinterpret_cast<const std::string*>(string);
    return Address::isValid(*s);
}

struct TWLoomAddress *_Nullable TWLoomAddressCreateWithString(TWString *_Nonnull string) {
auto s = reinterpret_cast<const std::string*>(string);
if (!Address::isValid(*s)) {
return nullptr;
}
return new TWEthereumAddress{ Address(*s) };
}

struct TWLoomAddress *_Nullable TWLoommAddressCreateWithKeyHash(TWData *_Nonnull keyHash) {
auto d = reinterpret_cast<const Data*>(keyHash);
if (!Address::isValid(*d)) {
return nullptr;
}
return new TWEthereumAddress{ Address(*d) };
}

struct TWLoomAddress *_Nonnull TWLoomAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey) {
    return new TWLoomAddress{ Address(publicKey->impl) };
}

void TWLoomAddressDelete(struct TWLoomAddress *_Nonnull address) {
    delete address;
}

TWString *_Nonnull TWLoomAddressDescription(struct TWLoomAddress *_Nonnull address) {
    const auto string = address->impl.string();
    return TWStringCreateWithUTF8Bytes(string.c_str());
}

TWData *_Nonnull TWLoomAddressKeyHash(struct TWLoomAddress *_Nonnull address) {
    return TWDataCreateWithBytes(address->impl.bytes.data(), address->impl.bytes.size());
}
