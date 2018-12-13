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

#include <ctype.h>
#include <stdio.h>
#include <limits.h>
#include "UBLOX_AT_Handler.h"
#include "mbed_poll.h"
#include "FileHandle.h"
#include "mbed_wait_api.h"
#include "mbed_debug.h"
#include "rtos/Thread.h"
#include "Kernel.h"
#include "CellularUtil.h"

using namespace mbed;
using namespace events;
using namespace mbed_cellular_util;

#include "CellularLog.h"
// URCs should be handled fast, if you add debug traces within URC processing then you also need to increase this time


void UBLOX_ATHandler::resp_start(const char *prefix, bool stop)
{
	nsapi_error_t _last_err;

    if (_last_err) {
        return;
    }

    // Try get as much data as possible
    rewind_buffer();
    (void)fill_buffer(false);

    if (prefix) {
        MBED_ASSERT(strlen(prefix) < UBLOX_BUFF_SIZE);
        strcpy(_info_resp_prefix, prefix); // copy prefix so we can later use it without having to provide again for info_resp
    }

    set_scope(RespType);

    resp(prefix, true);

    if (!stop && prefix && _prefix_matched) {
        set_scope(InfoType);
    }
}

/*bool ATHandler::fill_buffer(bool wait_for_timeout)
{
    // Reset buffer when full
    if (sizeof(_recv_buff) == _recv_len) {
        tr_error("AT overflow");
        reset_buffer();
    }

    pollfh fhs;
    fhs.fh = _fileHandle;
    fhs.events = POLLIN;
    int count = poll(&fhs, 1, poll_timeout(wait_for_timeout));
    if (count > 0 && (fhs.revents & POLLIN)) {
        ssize_t len = _fileHandle->read(_recv_buff + _recv_len, sizeof(_recv_buff) - _recv_len);
        if (len > 0) {
            debug_print(_recv_buff + _recv_len, len);
            _recv_len += len;
            return true;
        }
    }

    return false;
}
void ATHandler::rewind_buffer()
{
    if (_recv_pos > 0 && _recv_len >= _recv_pos) {
        _recv_len -= _recv_pos;
        // move what is not read to beginning of buffer
        memmove(_recv_buff, _recv_buff + _recv_pos, _recv_len);
        _recv_pos = 0;
    }
}
void ATHandler::set_scope(ScopeType scope_type)
{
    if (_current_scope != scope_type) {
        _current_scope = scope_type;
        switch (_current_scope) {
            case RespType:
                _stop_tag = &_resp_stop;
                _stop_tag->found = false;
                break;
            case InfoType:
                _stop_tag = &_info_stop;
                _stop_tag->found = false;
                consume_char(' ');
                break;
            case ElemType:
                _stop_tag = &_elem_stop;
                _stop_tag->found = false;
                break;
            case NotSet:
                _stop_tag = NULL;
                return;
            default:
                break;
        }
    }
}

void ATHandler::set_tag(tag_t *tag_dst, const char *tag_seq)
{
    if (tag_seq) {
        size_t tag_len = strlen(tag_seq);
        set_string(tag_dst->tag, tag_seq, tag_len);
        tag_dst->len = tag_len;
        tag_dst->found = false;
    } else {
        _stop_tag = NULL;
    }
}

void ATHandler::resp(const char *prefix, bool check_urc)
{
    _prefix_matched = false;
    _urc_matched = false;
    _error_found = false;

    while (!get_last_error()) {

        match(CRLF, CRLF_LENGTH);

        if (match(OK, OK_LENGTH)) {
            set_scope(RespType);
            _stop_tag->found = true;
            return;
        }

        if (match_error()) {
            _error_found = true;
            return;
        }

        if (prefix && match(prefix, strlen(prefix))) {
            _prefix_matched = true;
            return;
        }

        if (check_urc && match_urc()) {
            _urc_matched = true;
        }

        // If no match found, look for CRLF and consume everything up to and including CRLF
        if (mem_str(_recv_buff, _recv_len, CRLF, CRLF_LENGTH)) {
            // If no prefix, return on CRLF - means data to read
            if (!prefix) {
                return;
            }
            consume_to_tag(CRLF, true);
        } else {
            // If no prefix, no CRLF and no more chance to match for OK, ERROR or URC(since max resp length is already in buffer)
            // return so data could be read
            if (!prefix && ((_recv_len - _recv_pos) >= _max_resp_length)) {
                return;
            }
            if (!fill_buffer()) {
                // if we don't get any match and no data within timeout, set an error to indicate need for recovery
                set_error(NSAPI_ERROR_DEVICE_ERROR);
            }
        }
    }

    return;
    // something went wrong so application need to recover and retry
}
*/
