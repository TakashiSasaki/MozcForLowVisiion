// Copyright 2010-2013, Google Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef MOZC_DICTIONARY_DICTIONARY_INTERFACE_H_
#define MOZC_DICTIONARY_DICTIONARY_INTERFACE_H_

#include <string>
#include <vector>

#include "base/port.h"
#include "base/string_piece.h"
#include "base/trie.h"

namespace mozc {

// These structures are defined in converter
struct Node;
class NodeAllocatorInterface;

class DictionaryInterface {
 public:
  virtual ~DictionaryInterface() {}

  // limitation for LookupPrefixWithLimit
  struct Limit {
    // For prefix lookup, to reduce redundant lookup for making lattice,
    // returns only nodes which key length is >= this limit.
    int key_len_lower_limit;
    // For predictive lookup,
    // returns only nodes which predicted key starts with the string in the trie
    // Example:
    //  predictive lookup key: 'conv'
    //  begin with list: 'er', 'in'
    //  In this case, we will get 'convert', converter', 'convince', etc.
    //  We will not get 'convention', etc.
    // This does not have the ownership
    const Trie<string> *begin_with_trie;

    // For predictive and prefix lookup, enables ambiguous search.
    bool kana_modifier_insensitive_lookup_enabled;
    Limit() :
        key_len_lower_limit(0),
        begin_with_trie(NULL),
        kana_modifier_insensitive_lookup_enabled(false) {
    }
  };

  // Returns true if the dictionary has an entry for the given value.
  virtual bool HasValue(const StringPiece value) const = 0;

  // For Lookup methods, dictionary does not manage the ownerships of the
  // returned Node objects.
  // If the |allocator| is specified, we will use it to make Node objects.

  virtual Node *LookupPredictiveWithLimit(
      const char *str, int size, const Limit &limit,
      NodeAllocatorInterface *allocator) const = 0;

  virtual Node *LookupPredictive(const char *str, int size,
                                 NodeAllocatorInterface *allocator) const = 0;

  virtual Node *LookupPrefixWithLimit(
      const char *str, int size,
      const Limit &limit,
      NodeAllocatorInterface *allocator) const = 0;

  virtual Node *LookupPrefix(const char *str, int size,
                             NodeAllocatorInterface *allocator) const = 0;

  virtual Node *LookupExact(const char *str, int size,
                            NodeAllocatorInterface *allocator) const = 0;

  // For reverse lookup, the reading is stored in Node::value and the word
  // is stored in Node::key.
  virtual Node *LookupReverse(const char *str, int size,
                              NodeAllocatorInterface *allocator) const = 0;

  // Looks up a user comment from a pair of key and value.  When (key, value)
  // doesn't exist in this dictionary or user comment is empty, bool is
  // returned and string is kept as-is.
  virtual bool LookupComment(StringPiece key, StringPiece value,
                             string *comment) const { return false; }

  virtual void PopulateReverseLookupCache(
      const char *str, int size, NodeAllocatorInterface *allocator) const {}
  virtual void ClearReverseLookupCache(
      NodeAllocatorInterface *allocator) const {}

  // Sync mutable dictionary data into local disk.
  virtual bool Sync() { return true; }

  // Reload dictionary data from local disk.
  virtual bool Reload() { return true; }

 protected:
  // Do not allow instantiation
  DictionaryInterface() {}
};

}  // namespace mozc

#endif  // MOZC_DICTIONARY_INTERFACE_H_
