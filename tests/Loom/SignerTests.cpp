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

using namespace TW;
using namespace TW::Loom;



TEST(LoomSigner, SignTx) {
    const std::string fromHex = "12E8FE8B81ECC1F4F774EA6EC8DF267138B9F2D9";
    const std::string toHex = "12E8FE8B81ECC1F4F774EA6EC8DF267138B9F2D9";
    const std::string txBytes  = "";

    auto input = Proto::SigningInput();
    input.set_id(Proto::TXType::CALL);
    input.set_payload("");
    input.set_vm_type(Proto::VMType::EVM);
    input.set_sequence(1);
    input.set_private_key("80e81ea269e66a0a05b11236df7919fb7fbeedba87452d667489d7403a02f005");

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
    ASSERT_EQ(output.encoded(), txBytes);
}



