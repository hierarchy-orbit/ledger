#pragma once
//------------------------------------------------------------------------------
//
//   Copyright 2018-2020 Fetch.AI Limited
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.
//
//------------------------------------------------------------------------------

#include "chain/tx_declaration.hpp"
#include "core/digest.hpp"

#include <memory>
#include <unordered_map>

namespace fetch {
namespace chain {

/**
 * The transaction class
 */
class Transaction;

using TransactionPtr = std::shared_ptr<Transaction>;

using TransactionIndex = DigestMap<TransactionPtr>;

class TransactionLayout;

}  // namespace chain
}  // namespace fetch
