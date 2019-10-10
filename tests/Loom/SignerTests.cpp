// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full§ Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Loom/Signer.h"
#include "Loom/Address.h"
#include "proto/Loom.pb.h"
#include "HexCoding.h"

#include <gtest/gtest.h>

#include <iostream>

using namespace TW;
using namespace TW::Loom;



TEST(LoomSigner, SignTx) {

    const std::string fromHex = "ba665d0403f1ffedf16b85b373f2ca601ce59ed1";
    const std::string toHex = "3d7Fc003CD15B4c42C9300708673eA22b386AA2A";
    const std::string payload = "60fe47b100000000000000000000000000000000000000000000000000000000000003db";
    const std::string txBytes  = "0a740a700802126c0a1b0a0365746812143d7fc003cd15b4c42c9300708673ea22b386aa2a121f0a0764656661756c741214ba665d0403f1ffedf16b85b373f2ca601ce59ed11a2c0801122460fe47b100000000000000000000000000000000000000000000000000000000000003db1a020a001002124200a374f5cb54f10aee15957e5f21b0f761b94e48f6b9e041d092c147101208df0116b70e06207aa79c111e8a321fd63619b3a76f8f548756331ea0a76a8e93eb971b1a410457286ec3f37d33557bbbaa000b27744ac9023aa9967cae75a181d1ff91fa9dc55a3dc9049894b1abfaa17b1645fd3928bbfd62de779d71269ba7e307c665d302";
    auto input = Proto::SigningInput();
    input.set_id(Proto::TXType::CALL);
    input.set_payload(payload);
    input.set_vm_type(Proto::VMType::EVM);
    input.set_sequence(1);
    auto privateKey = parse_hex("80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005");
    input.set_private_key(privateKey.data(), privateKey.size());
    auto from = Proto::Address();
    from.set_chain_id("default");
    from.set_local(fromHex);
    auto to = Proto::Address();
    to.set_chain_id("eth");
    to.set_local(toHex);
    auto value = Proto::BigUInt();
    value.set_value("");
    input.set_allocated_to(&from);
    input.set_allocated_from(&to);
    input.set_allocated_value(&value);
    auto signer = Signer();
    auto output = signer.sign(input);
    input.release_value();
    input.release_to();
    input.release_from();

    ASSERT_EQ(hex(output.encoded()), txBytes);
}



