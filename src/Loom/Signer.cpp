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
#include "HexCoding.h"

using namespace TW;
using namespace TW::Loom;

Proto::SigningOutput Signer::sign(const Proto::SigningInput &input) const noexcept {
    auto signedTx = buildTransaction(input);
    auto privateKey = PrivateKey(input.private_key());
    std::cout << "signedTx.inner() " << hex(signedTx.inner()) << "\n";
    auto hash = Hash::sha256(signedTx.inner());
    std::cout << "Hash::sha256(signedTx.inner()) " << hex(hash) << "\n";
    auto signature = privateKey.sign(hash, TWCurveSECP256k1);
    std::cout << "signature " << hex(signature) << "\n";
    signedTx.set_signature(signature.data(), signature.size());
    const auto pubKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);

    signedTx.set_public_key(pubKey.bytes.data(), pubKey.bytes.size());
    auto signedBytes = signedTx.SerializeAsString();

    auto output = Proto::SigningOutput();
    output.set_encoded(signedBytes.data(), signedBytes.size());
    return output;
}

Proto::SignedTx Signer::buildTransaction(const Proto::SigningInput &input) const noexcept {
    auto messageTx = Proto::MessageTx();
    if (Proto::TXType::CALL == input.id()) {
        auto callTx = Proto::CallTx();
        callTx.set_vm_type(input.vm_type());
        //auto value = input.value();
        //callTx.set_allocated_value(&value);
        callTx.set_input(input.payload().data(), input.payload().size());
std::cout << "input.payload() " << (input.payload()) << "\n";
        auto data = callTx.SerializeAsString();
        std::cout << "callTx.data() " << (data) << "\n" << "input " << (input.payload()) << "\n";
         std::cout << "callTx.value() " << "vm_type " << (input.vm_type()) << "\n";
        messageTx.set_data(data.data(), data.size());
        callTx.release_value();
    } else {
        auto deployTx = Proto::DeployTx();
        deployTx.set_vm_type(input.vm_type());
        auto value = input.value();
        deployTx.set_allocated_value(&value);
        deployTx.set_code(input.payload().data(), input.payload().size());
        auto data = deployTx.SerializeAsString();
        messageTx.set_data(data.data(), data.size());
        deployTx.release_value();
    }
    auto to = input.to();
    messageTx.set_allocated_to(&to);
    auto from = input.from();
    messageTx.set_allocated_from(&from);
    auto mesgData = messageTx.SerializeAsString();
std::cout << "messageTx.data() " << (messageTx.data()) << "\n" << "messageTx.from " << (messageTx.from().local()) << "\n";
    messageTx.release_to();
    messageTx.release_from();

    auto transactionTx = Proto::Transaction();
    transactionTx.set_id(input.id());
    transactionTx.set_data(mesgData.data(), mesgData.size());
    auto txData = transactionTx.SerializeAsString();

    auto nonceTx = Proto::NonceTx();
    nonceTx.set_inner(txData.data(), txData.size());
    nonceTx.set_sequence(input.sequence());\
    auto nonceData = nonceTx.SerializeAsString();

    auto signedTx = Proto::SignedTx();
    signedTx.set_inner(nonceData.data(), nonceData.size());
    return signedTx;
}


