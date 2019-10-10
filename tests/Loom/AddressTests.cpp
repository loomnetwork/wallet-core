// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Loom/Address.h"
#include "HexCoding.h"
#include "PrivateKey.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Loom;

TEST(LoomAddress, Invalid) {
    ASSERT_FALSE(Address::isValid("abc"));
    ASSERT_FALSE(Address::isValid("aaeb60f3e94c9b9a09f33669435e7ef1beaed"));
    ASSERT_FALSE(Address::isValid("fB6916095ca1df60bB79Ce92cE3Ea74c37c5d359"));
}

TEST(LoomAddress, EIP55) {
    ASSERT_EQ(
        Address("0x5aaeb6053f3e94c9b9a09f33669435e7ef1beaed").string(),
        "loom:0x5aaeb6053f3e94c9b9a09f33669435e7ef1beaed"
    );
    ASSERT_EQ(
        Address("0x5AAEB6053F3E94C9b9A09f33669435E7Ef1BEAED").string(),
        "loom:0x5aaeb6053f3e94c9b9a09f33669435e7ef1beaed"
    );
}

TEST(LoomAddress, String) {
    const auto address = Address("0x5aAeb6053F3E94C9b9A09f33669435E7Ef1BeAed");
    ASSERT_EQ(address.string(), "loom:0x5aaeb6053f3e94c9b9a09f33669435e7ef1beaed");
}

TEST(LoomAddress, FromPrivateKey) {
    const auto privateKey = PrivateKey(parse_hex("afeefca74d9a325cf1d6b6911d61a65c32afa8e02bd5e78e2e4ac2910bab45f5"));
    const auto publicKey = PublicKey(privateKey.getPublicKey(TWPublicKeyTypeSECP256k1Extended));
    const auto address = Address(publicKey);

    ASSERT_EQ(address.string(), "loom:0xac1ec44e4f0ca7d172b7803f6836de87fb72b309");

}

TEST(LoomAddress, IsValid) {
    ASSERT_FALSE(Address::isValid("abc"));
    ASSERT_TRUE(Address::isValid("0x5aAeb6053F3E94C9b9A09f33669435E7Ef1BeAed"));
}

