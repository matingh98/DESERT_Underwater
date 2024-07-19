// Copyright (c) 2017 Regents of the SIGNET lab, University of Padova.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. Neither the name of the University of Padova (SIGNET lab) nor the
//    names of its contributors may be used to endorse or promote products
//    derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
// TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//

/**
 * @file   uwsync_REF.h
 * @author Matin Ghalkhani
 * @version 1.0.0
 *
 * \brief Class that represents a node of UWSYNC
 *
 *
 */

#include <sync_header.h>
#include "mmac.h"
#include <iostream>
#include <string>
#include <map>
#include <set>
#include <queue>
#include <fstream>
#include <chrono>
#include <random>

class UwSyncREF : public MMac
{
public:
	UwSyncREF();
	virtual ~UwSyncREF();
	virtual int crLayCommand(ClMessage *m);
	virtual int command(int argc, const char *const *argv);
	virtual void initPkt();
	virtual void RxPacket(Packet *p);
	virtual void TransmittingToNodeREG(Packet *p);
	virtual void Transmitting(Packet *p);
	virtual void stateIdle(Packet *p);
	virtual void recv(Packet *p);
	inline int get_timestamp_ts1()
	{
		std::cout << "Object address in get_timestamp_ts1: " << this << std::endl;
		return ts_[0];
	}

private:
    int pktid_;
    double start_time;
    double stop_time;
    double ts_[4];
};
