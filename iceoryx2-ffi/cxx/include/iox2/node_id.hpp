// Copyright (c) 2024 Contributors to the Eclipse Foundation
//
// See the NOTICE file(s) distributed with this work for additional
// information regarding copyright ownership.
//
// This program and the accompanying materials are made available under the
// terms of the Apache Software License 2.0 which is available at
// https://www.apache.org/licenses/LICENSE-2.0, or the MIT license
// which is available at https://opensource.org/licenses/MIT.
//
// SPDX-License-Identifier: Apache-2.0 OR MIT

#ifndef IOX2_NODE_ID_HPP
#define IOX2_NODE_ID_HPP

#include "iox2/iceoryx2.h"
#include "iox2/service_type.hpp"

#include <cstdint>
#include <ctime>
#include <iostream>

namespace iox2 {
class NodeId {
  public:
    NodeId(const NodeId& rhs);
    NodeId(NodeId&& rhs) noexcept;
    auto operator=(const NodeId& rhs) -> NodeId&;
    auto operator=(NodeId&& rhs) noexcept -> NodeId&;
    ~NodeId();

    /// Returns high bits of the underlying value of the [`NodeId`].
    auto value_high() const -> uint64_t;

    /// Returns low bits of the underlying value of the [`NodeId`].
    auto value_low() const -> uint64_t;

    /// Returns the [`ProcessId`] of the process that owns the [`Node`].
    auto pid() const -> int32_t;

    /// Returns the time the [`Node`] was created.
    auto creation_time() const -> timespec;

  private:
    template <ServiceType>
    friend class Node;
    template <ServiceType>
    friend class DeadNodeView;
    template <ServiceType>
    friend auto list_callback(iox2_node_state_e,
                              iox2_node_id_ptr,
                              const char*,
                              iox2_node_name_ptr,
                              iox2_config_ptr,
                              iox2_callback_context) -> iox2_callback_progression_e;


    explicit NodeId(iox2_node_id_h handle);
    void drop();

    iox2_node_id_h m_handle = nullptr;
};

auto operator<<(std::ostream& stream, const NodeId& node) -> std::ostream&;
auto operator==(const NodeId& lhs, const NodeId& rhs) -> bool;
auto operator!=(const NodeId& lhs, const NodeId& rhs) -> bool;
} // namespace iox2

#endif
