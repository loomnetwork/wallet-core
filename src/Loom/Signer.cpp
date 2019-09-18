// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "../proto/Loom.pb.h"
#include "../PrivateKey.h"
#include <TrustWalletCore/TWCurve.h>

using namespace TW;
using namespace TW::Loom;

Proto::SigningOutput Signer::sign(const Proto::SigningInput &input) const noexcept {
    auto signedTx = buildTransaction(input);

    auto privateKey = PrivateKey(input.private_key());
    //auto privateKey = PublicKey(input.private_key(), TWPublicKeyTypeED25519);

    auto hash = Hash::sha256(signedTx.inner());

    auto signature = privateKey.sign(hash, TWCurveED25519);
    signedTx.set_signature(signature.data(), signature.size());

    const auto pubKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
    signedTx.set_public_key(pubKey.bytes.data(), pubKey.bytes.size());

    std::string signedBytes;
    signedTx.SerializeToString(&signedBytes);

    auto output = Proto::SigningOutput();
    output.set_encoded(signedBytes);
    return output;
}

Proto::SignedTx Signer::buildTransaction(const Proto::SigningInput &input) const noexcept {
    std::string data;
    if (Proto::TXType::DEPLOY == input.id()) {
        auto callTx = Proto::CallTx();
        callTx.set_vm_type(input.vm_type());
        auto value = input.value();
        callTx.set_allocated_value(&value);
        callTx.set_input(input.payload());
        callTx.SerializeToString(&data);
    } else {
        auto deployTx = Proto::DeployTx();
        deployTx.set_vm_type(input.vm_type());
        auto value = input.value();
        deployTx.set_allocated_value(&value);
        deployTx.set_code(input.payload());
        deployTx.SerializeToString(&data);
    }

    auto messageTx = Proto::MessageTx();
    messageTx.set_data(data);
    auto to = input.to();
    messageTx.set_allocated_to(&to);
    auto from = input.to();
    messageTx.set_allocated_from(&from);
    std::string message;
    messageTx.SerializeToString(&message);

    auto transactionTx = Proto::Transaction();
    transactionTx.set_id(input.id());
    transactionTx.set_data(message);
    std::string transaction;
    transactionTx.SerializeToString(&transaction);

    auto nonceTx = Proto::NonceTx();
    nonceTx.set_inner(transaction);
    nonceTx.set_sequence(input.sequence());
    std::string nonce;
    nonceTx.SerializeToString(&nonce);

    auto signedTx = Proto::SignedTx();
    signedTx.set_inner(nonce);

    return signedTx;
}
