// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "../proto/Loom.pb.h"
#include "../PrivateKey.h"
#include <TrustWalletCore/TWCurve.h>
#include <iostream>

using namespace TW;
using namespace TW::Loom;

Proto::SigningOutput Signer::sign(const Proto::SigningInput &input) const noexcept {
    auto signedTx = buildTransaction(input);
std::cout << "piers Signer0\n";
    auto privateKey = PrivateKey(input.private_key());
    //auto privateKey = PublicKey(input.private_key(), TWPublicKeyTypeED25519);

    auto hash = Hash::sha256(signedTx.inner());

    auto signature = privateKey.sign(hash, TWCurveED25519);
    signedTx.set_signature(signature.data(), signature.size());
std::cout << "piers Signer1\n";
    const auto pubKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
    signedTx.set_public_key(pubKey.bytes.data(), pubKey.bytes.size());

    auto signedBytes = signedTx.SerializeAsString();
std::cout << "piers Signer2\n";
    auto output = Proto::SigningOutput();
    output.set_encoded(signedBytes.data(), signedBytes.size());
    return output;
}

Proto::SignedTx Signer::buildTransaction(const Proto::SigningInput &input) const noexcept {
    std::cout << "piers buildTransaction0\n";
    auto messageTx = Proto::MessageTx();
    if (Proto::TXType::CALL == input.id()) {
        auto callTx = Proto::CallTx();
        callTx.set_vm_type(input.vm_type());
        //auto value = input.value();
        //callTx.set_allocated_value(&value);
        callTx.set_input(input.payload().data(), input.payload().size());
        auto data = callTx.SerializeAsString();
        messageTx.set_data(data.data(), data.size());
        std::cout << "piers buildTransaction06\n" ;//<< data;
    } else {
        std::cout << "piers deokiy buildTransaction07\n";
        auto deployTx = Proto::DeployTx();
        deployTx.set_vm_type(input.vm_type());
        auto value = input.value();
        deployTx.set_allocated_value(&value);
        deployTx.set_code(input.payload().data(), input.payload().size());
        auto data = deployTx.SerializeAsString();
        messageTx.set_data(data.data(), data.size());
    }
std::cout << "piers buildTransaction3\n";
    auto to = input.to();
    messageTx.set_allocated_to(&to);
    auto from = input.from();
    messageTx.set_allocated_from(&from);
    std::string message;
    auto mesgData = messageTx.SerializeAsString();
std::cout << "piers buildTransaction§2\n";
    auto transactionTx = Proto::Transaction();
    transactionTx.set_id(input.id());
    transactionTx.set_data(mesgData.data(), mesgData.size());
    auto txData = transactionTx.SerializeAsString();
std::cout << "piers buildTransaction4\n";
    auto nonceTx = Proto::NonceTx();
    nonceTx.set_inner(txData.data(), txData.size());
    nonceTx.set_sequence(input.sequence());
    std::string nonce;
    auto nonceData = nonceTx.SerializeAsString();
std::cout << "piers buildTransaction5\n";
    auto signedTx = Proto::SignedTx();
    signedTx.set_inner(nonceData.data(), nonceData.size());
std::cout << "piers buildTransaction6\n";
    return signedTx;
}
