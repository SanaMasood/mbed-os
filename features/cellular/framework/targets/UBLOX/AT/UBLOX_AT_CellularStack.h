/*
 * Copyright (c) 2017, Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef UBLOX_AT_CELLULARSTACK_H_
#define UBLOX_AT_CELLULARSTACK_H_

#include "AT_CellularStack.h"
#include "CellularUtil.h"

//TO DO add target based
#define UBLOX_U201_SOCKET_MAX 7
#define UBLOX_U201_AT_COMMAND_BUFFER_SIZE 256
#define UBLOX_U201_MAX_PACKET_SIZE 1500

namespace mbed {

class UBLOX_AT_CellularStack : public AT_CellularStack
{
public:
    UBLOX_AT_CellularStack(ATHandler &atHandler, int cid, nsapi_ip_stack_t stack_type);
    virtual ~UBLOX_AT_CellularStack();

protected: // NetworkStack

    virtual nsapi_error_t socket_listen(nsapi_socket_t handle, int backlog);

    virtual nsapi_error_t socket_accept(nsapi_socket_t server,
                                        nsapi_socket_t *handle, SocketAddress *address=0);

protected: // AT_CellularStack

    virtual int get_max_socket_count();

    virtual int get_max_packet_size();

    virtual bool is_protocol_supported(nsapi_protocol_t protocol);

    virtual nsapi_error_t socket_close_impl(int sock_id);

    virtual nsapi_error_t create_socket_impl(CellularSocket *socket);

    virtual nsapi_size_or_error_t socket_sendto_impl(CellularSocket *socket, const SocketAddress &address,
            const void *data, nsapi_size_t size);

    virtual nsapi_size_or_error_t socket_recvfrom_impl(CellularSocket *socket, SocketAddress *address,
            void *buffer, nsapi_size_t size);

private:
    // URC handlers
    void UUSORD_URC();
    void UUSORF_URC();
    void UUSOCL_URC();
    void UUPSDD_URC();
};
} // namespace mbed
#endif /* UBLOX_AT_CELLULARSTACK_H_ */
