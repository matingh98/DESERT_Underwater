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
 * @file   uwdsync_A.h
 * @author Matin Ghalkhani
 * @version 1.0.0
 *
 * \brief Class that represents a node of UWDSYNC
 *
 *
 */
#ifndef UWDSYNC_A_H
#define UWDSYNC_A_H

#include "uwdsync2_cmn_hdr.h"
#include "mmac.h"
#include "packet.h"
#include "rng.h"
#include "config.h" // Include the config.h header for NS-2

#include <iostream>
#include <string>
#include <map>
#include <set>
#include <queue>
#include <fstream>
#include <list>
#include <chrono>

// Forward declaration of UwDSync_A (used in UwDSync_A_Timer)
class UwDSync_A;

/**
 * Timer handler for UwDSync_A
 */
class UwDSync_A_Timer : public TimerHandler {
public:
    UwDSync_A_Timer(UwDSync_A *m, double *start, double *stop)
        : start_time(start)
        , stop_time(stop)
        , left_duration(0.0)
        , counter(0)
        , module(m)
    {
        assert(m != NULL);
    }

    virtual ~UwDSync_A_Timer() {}

    virtual void initialTimer();

    virtual void stop() {
        force_cancel();
    }

    virtual void schedule(double val) {
        if (start_time) {
            *start_time = NOW;
        }
        left_duration = val;
        resched(val);
    }

    void resetCounter() {
        counter = 0;
    }

    void incrCounter() {
        ++counter;
    }

    int getCounter() const {
        return counter;
    }

    double getDuration() const {
        return left_duration;
    }

    virtual void expire(Event *e);

protected:
    double *start_time;
    double *stop_time;
    double left_duration;
    int counter;
    UwDSync_A *module;
};

/**
 * Class used to represent the UWDSYNC MAC layer of a node.
 */
class UwDSync_A : public MMac {
public:
    UwDSync_A();
    virtual ~UwDSync_A();
    virtual void start();
    virtual void stop();
    virtual int command(int argc, const char *const *argv);
    virtual int crLayCommand(ClMessage *m);
    virtual void stateTxData();
    virtual void initPkt(Packet *p);
    virtual void stateRxTrigger(Packet *p);
    virtual void stateIdle(Packet *p = nullptr);
    virtual void Phy2MacEndRx(Packet *p);

    double getStartTime() const {
        return start_time;
    }

    double getStopTime() const {
        return stop_time;
    }

protected:
    int pktid;                  /**< Packet ID */
    int num_equations;          /**< Number of equations */
    double receivedTimeStamp[4]; // Array to hold four timestamps
    double start_time;
    double stop_time;
    UwDSync_A_Timer timer;      /**< Timer object */

    /**< Type of the packet */
    enum UWDSYNC_A_PKT_TYPE {
        UWPOLLING_DATA_PKT = 1,
    };

    /**< Status of the timer */
    enum UWDSYNC_A_TIMER_STATUS {
        UWDSYNC_A_IDLE = 1,
        UWDSYNC_A_RUNNING,
    };

    static std::map<UwDSync_A::UWDSYNC_A_PKT_TYPE, std::string> pkt_type_info;
};

#endif // UWDSYNC_A_H
