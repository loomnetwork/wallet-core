// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWLoomSigner.h>
#include <TrustWalletCore/TWLoomProto.h>

#include "../Loom/Signer.h"
#include "../proto/Loom.pb.h"
#include "../uint256.h"

using namespace TW;
using namespace TW::Loom;

TW_Loom_Proto_SigningOutput TWLoomSignerSign(TW_Loom_Proto_SigningInput data) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));

    auto signer = Signer();
    auto protoOutput = signer.sign(input);

    auto serialized = protoOutput.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
}
