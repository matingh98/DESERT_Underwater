//
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
friend class UwSyncREF_Timer;
public:
	UwSyncREF();
	virtual ~UwSyncREF();
	virtual void start();
	virtual void stop();
	virtual int command(int argc, const char *const *argv);
	virtual int crLayCommand(ClMessage *m);
	virtual void initPkt(Packet *p);
	virtual void RxPacket(Packet *p);
	virtual void stateIdle(Packet *p);
	virtual void recv(Packet *p);

	class UwSyncREF_Timer : public TimerHandler
	{
		
	public:
		UwSyncREF_Timer(UwSyncREF *m)
			: TimerHandler(), left_duration(0), module(m)
		{
			assert(m != NULL);
		}

		virtual ~UwSyncREF_Timer() {}

		virtual void initialTimer();

		virtual void stop()
		{
			force_cancel();
		}

	 	virtual void schedule(double val)
        {
            if (module->start_time)
            {
                module->start_time = NOW;
            }
            left_duration = val;
            resched(val);
        }
		virtual void expire(Event *e);

	protected:
		double left_duration;
		UwSyncREF *module;
	};

private:
	int pktid;
	double start_time;
	double stop_time;
	UwSyncREF_Timer timer;
};