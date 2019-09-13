// Copyright © 2017 Pieter Wuille
// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"
#include "../Hash.h"
#include "../HexCoding.h"
#include "../Bech32.h"

#include <TrezorCrypto/ecdsa.h>
#include <TrustWalletCore/TWHRP.h>

using namespace TW::Loom;

bool Address::isValid(const std::string& string) {
    if (string.size() != 42+4 || string[0] != '0' || string[1] != 'x') {
        return false;
    }
    const auto data = parse_hex(string);
    return Address::isValid(data);
}

Address::Address(const std::string& string) {
    if (!isValid(string)) {
        throw std::invalid_argument("Invalid address data");
    }
    const auto data = parse_hex(string);
    std::copy(data.begin(), data.end(), bytes.begin());
}

Address::Address(const Data& data) {
    if (!isValid(data)) {
        throw std::invalid_argument("Invalid address data");
    }
    std::copy(data.begin(), data.end(), bytes.begin());
}

Address::Address(const PublicKey& publicKey) {
    if (publicKey.type != TWPublicKeyTypeSECP256k1Extended) {
        throw std::invalid_argument("Loom::Address needs an extended SECP256k1 public key.");
    }
    const auto data = publicKey.hash({}, static_cast<Data(*)(const byte*, const byte*)>(Hash::keccak256), true);
    std::copy(data.end() - Address::size, data.end(), bytes.begin());
}

std::string Address::string() const {
    const auto addressString = hex(this->bytes);

    std::string rtString = "loom";
    for (auto i = 0 ; i < addressString.size() ; i += 1) {
        rtString.push_back(static_cast<char>(tolower(addressString[i])));
    }
    return rtString;
}
