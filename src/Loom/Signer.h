// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../proto/Loom.pb.h"
#include "../Hash.h"

namespace TW::Loom {

/// Helper class that performs Loom transaction signing.
class Signer {
public:
    /// Initializes a transaction signer.
    Signer() {}

    /// Signs a Proto::SigningInput transaction
    Proto::SigningOutput sign(const Proto::SigningInput& input) const noexcept;
private:
    Data buildTransaction(const TW::Loom::Proto::SigningInput &input) const noexcept;

};

} // namespace

/// Wrapper for C interface.
struct TWLoomSigner {
    TW::Loom::Signer impl;
};
