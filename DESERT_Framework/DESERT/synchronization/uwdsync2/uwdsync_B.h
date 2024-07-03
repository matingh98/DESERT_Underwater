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
 * @file   uwdsync_node.h
 * @author Matin Ghalkhani
 * @version 1.0.0
 *
 * \brief Class that represents a node of UWPOLLING
 *
 *
 */


#ifndef UWDSYNC_B_H
#define UWDSYNC_B_H

#include "uwdsync2_cmn_hdr.h"
#include "mmac.h"
#include <iostream>
#include <string>
#include <map>
#include <set>
#include <queue>
#include <fstream>
#include <chrono>
#include <random> // Include the random library

class UwDSync_B;

class UwDSync_B_Timer : public TimerHandler {

friend class BackOffTimer;
public:
    UwDSync_B_Timer (UwDSync_B *m)
        : start_time(0.0)
        , left_duration(0.0)
        , counter(0)
        , module(m)
    {
        assert(m != NULL);
    }

    virtual ~UwDSync_B_Timer() {}


    virtual void stop() {
        force_cancel();
    }

    virtual void schedule(double val) {
        start_time = NOW;
        left_duration = val;
        resched(val);
    }

    void resetCounter() {
        counter = 0;
    }

    void incrCounter() {
        ++counter;
    }

    int getCounter() {
        return counter;
    }

    double getDuration() {
        return left_duration;
    }
    int getRandomBackoffTime() {
        // Return a fixed backoff time of 2000 milliseconds (2 seconds)
        return 2000;
    }

    virtual void expire(Event *e);  

protected:
    double start_time;
    double left_duration;
    int counter;
    UwDSync_B *module;
    // UWDSYNC_A_TIMER_STATUS timer_status;

};



/**
 * Class used to represent the UWDSYNC MAC layer of a node.
 */
class UwDSync_B : public MMac
{
friend class UwDSync_B_Timer;

public:
    UwDSync_B();
    virtual ~UwDSync_B();

    virtual int command(int argc, const char *const *argv);
    virtual int crLayCommand(ClMessage *m);
    virtual void stateRxTrigger(Packet *p);
    virtual void stateIdle(Packet *p = nullptr);
    virtual std::vector<double> sendReceivedTimestamp(Packet* p);
    // virtual void Mac2PhyStartTx(Packet *p);
    virtual void Phy2MacStartRx(Packet *p);
    virtual void TransmittingToNodeA();

protected:
    int pktid;                  /**< Packet ID */
    int num_equations;          /**< Number of equations */
    double receivedTimeStamp[4]; // Array to hold four timestamps
    Packet *current_packet;


    /**< Type of the packet */
    enum UWDSYNC_B_PKT_TYPE {
        UWPOLLING_DATA_PKT = 1,
    };

    /**< Status of the timer */
    enum UWDSYNC_B_TIMER_STATUS {
        UWDSYNC_A_IDLE = 1,
        UWDSYNC_A_RUNNING,
    };
    UwDSync_B_Timer timer;



    static std::map<UwDSync_B::UWDSYNC_B_PKT_TYPE, std::string> pkt_type_info;
};



#endif // UWDSYNC_B_H
