// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
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

    const std::string fromHex = "9a1aC42a17AAD6Dbc6d21c162989d0f701074044";
    const std::string toHex = "3d7Fc003CD15B4c42C9300708673eA22b386AA2A";
    const std::string payload = "60fe47b100000000000000000000000000000000000000000000000000000000000003db";
    const std::string txBytes  = "0a780a74080212700a1f0a0764656661756c7412143d7fc003cd15b4c42c9300708673ea22b386aa2a121f0a0764656661756c7412149a1ac42a17aad6dbc6d21c162989d0f7010740441a2c0801122460fe47b100000000000000000000000000000000000000000000000000000000000003db1a020a00100212403211f0a8691c22d75031a51780c5eb888db6f294b8f87d572e811681d0f7e4703148be2382eaf4fcd9771395e393e479f8aa5fb2c710291bbdd04e1b1e26a2051a20f60c7e22684970ea51f6017c8b8add4fb614b515e816ae1b40afe4d2d03779e7";
    auto input = Proto::SigningInput();
    input.set_id(Proto::TXType::CALL);
    input.set_payload("payload");
    input.set_vm_type(Proto::VMType::EVM);
    input.set_sequence(1);
    auto privateKey = parse_hex("80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005");
    input.set_private_key(privateKey.data(), privateKey.size());
    auto from = Proto::Address();
    from.set_chain_id("loom");
    from.set_local(fromHex);
    auto to = Proto::Address();
    to.set_chain_id("loom");
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



