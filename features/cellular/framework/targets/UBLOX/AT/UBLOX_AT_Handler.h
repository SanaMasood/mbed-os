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

#ifndef UBLOX_AT_HANDLER_H_
#define UBLOX_AT_HANDLER_H_

#include "platform/mbed_retarget.h"

#include "EventQueue.h"
#include "PlatformMutex.h"
#include "nsapi_types.h"

#include "PlatformMutex.h"
#include "Callback.h"
#include "EventQueue.h"
#include "ATHandler.h"

namespace mbed {

class FileHandle;

/**
 * If application calls associated FileHandle only from single thread context
  * then locking between AT command and response is not needed. However,
  * note that many cellular functions are called indirectly, for example with the socket API.
  * If you are unsure, then AT_HANDLER_MUTEX must be defined.
  */
#define AT_HANDLER_MUTEX

extern const char *OK;
extern const char *CRLF;

#define UBLOX_BUFF_SIZE 21



/** Class ATHandler
 *
 *  Class for sending AT commands and parsing AT responses.
 */
class UBLOX_ATHandler : public ATHandler

{




public:
    /** Constructor
     *
     *  @param fh               file handle used for reading AT responses and writing AT commands
     *  @param queue            Event queue used to transfer sigio events to this thread
     *  @param timeout          Timeout when reading for AT response
     *  @param output_delimiter delimiter used when parsing at responses, "\r" should be used as output_delimiter
     *  @param send_delay       the minimum delay in ms between the end of last response and the beginning of a new command
     */
	UBLOX_ATHandler();
    virtual ~UBLOX_ATHandler();

    /**  This looks for necessary matches: prefix, OK, ERROR, URCs and sets the correct scope.
     *
     *  @param prefix string to be matched from receiving buffer. If not NULL and match succeeds, then scope
     *          will be set as information response(info_type)
     *  @param stop flag to indicate if we go to information response scope or not.
     *        (needed when nothing is expected to be received anymore after the prefix match:
     *         sms case: "> ", bc95 reboot case)
     */
    virtual void resp_start(const char *prefix = NULL, bool stop = false);

    /** Return used file handle.
     *
     *  @return used file handle
     */
};

} // namespace mbed

#endif //AT_HANDLER_H_
