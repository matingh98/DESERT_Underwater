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
 * @file   uwsync_REG.h
 * @author Matin Ghalkhani
 * @version 1.0.0
 *
 * \brief Class that represents a Regular node of UWSYNC
 *
 *
 */

#ifndef UWSYNC_REG
#define UWSYNC_REG

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
#include <cassert> // Include assert for proper usage

class UwSyncREG : public MMac
{
    friend class UwSyncREG_Timer;

public:
    UwSyncREG();
    virtual ~UwSyncREG();
    virtual void StateRxPacket(Packet *p);
    virtual int command(int argc, const char *const *argv);
    virtual int crLayCommand(ClMessage *m);
    virtual void TransmittingToNodeREF(Packet *p);
    virtual void stateIdle(Packet *p);
    virtual std::vector<double> sendReceivedTimestamp(Packet *p);
    virtual void recv(Packet *p);

    class UwSyncREG_Timer : public TimerHandler
    {
    public:
        UwSyncREG_Timer(UwSyncREG *m, Packet *pkt)
            : TimerHandler(), left_duration(0), BT(m), packet(pkt)
        {
            assert(m != NULL);
        }

        virtual ~UwSyncREG_Timer() {}

        virtual void stop()
        {
            force_cancel();
        }

        virtual void BackoffTimer()
        {
            left_duration = 10.0; // Set the backoff timer for 10 seconds
            resched(left_duration); // Schedule the timer to expire after 10 seconds
        }

        virtual void expire(Event *e)
        {
            BT->TransmittingToNodeREF(packet); // Call TransmittingToNodeA with the packet
        }

    protected:
        double left_duration;
        UwSyncREG *BT;
        Packet *packet; // Store the packet for later use
    };

private:
    int pktid; /**< Packet ID */
    double receivedTimeStamp[4];
    Packet *lastPacket;
};

#endif 
