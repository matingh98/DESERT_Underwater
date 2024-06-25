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
 * @file   uwdsync_node.cpp
 * @author Matin Ghalkhani
 * @version 1.0.0
 *
 * \brief Implementation of Uwpolling_NODE class
 *
 **/

#include "uwdsync2_cmn_hdr.h"
#include "uwdsync_B.h"
#include "mac.h"
#include "mmac.h"
#include "uwcbr-module.h"
#include "mphy_pktheader.h"
#include "rng.h"

#include <sstream>
#include <sys/time.h>
#include <iostream>
#include <random> // Include the random library

static class UwDSync_B_Class : public TclClass
{
public:
    UwDSync_B_Class() : TclClass("Module/UW/DSYNC/B") {}
    TclObject *create(int, const char *const *) {
        return (new UwDSync_B());
    }
} class_module_UwDSync_b;

UwDSync_B::UwDSync_B() : MMac(), timer(this) {}

UwDSync_B::~UwDSync_B() {}


int UwDSync_B::command(int argc, const char *const *argv) {
    Tcl &tcl = Tcl::instance();
    if (argc == 2) {
        if (strcasecmp(argv[1], "get_timestamp") == 0) {
            std::stringstream ss;
            for (int i = 0; i < 4; i++) {
                ss << receivedTimeStamp[i] << " ";
            }
            tcl.resultf("%s", ss.str().c_str());
            return TCL_OK;
        }
    }
    return MMac::command(argc, argv);
}

int UwDSync_B::crLayCommand(ClMessage *m) {
    switch (m->type()) {
    default:
        return MMac::crLayCommand(m);
    }
}

void UwDSync_B::Phy2MacStartRx(Packet *p) {
    std::cout << "Data received from PHY, forwarding to MAC" << std::endl;
    stateRxTrigger(p);
}

void UwDSync_B::stateRxTrigger(Packet *p) {
    hdr_DATA *datah = HDR_DATA(p);  
    int pktid = datah->ID();        

    if (pktid == 1) {
        std::cout << "Packet ID 1 received from node A" << std::endl;
        hdr_cmn *ch = HDR_CMN(p);   
        ch->size() += sizeof(double); 

        datah->ts_[1] = NOW;          
        datah->ID() = 2;            
        timer.schedule(timer.getRandomBackoffTime());

        hdr_cmn *ch2 = HDR_CMN(p);   
        ch2->size() += sizeof(double);

        datah->ts_[2] = NOW;          
        datah->ID() = 3;

        Mac2PhyStartTx(p);          
        stateIdle(p);
    }
    else if (pktid == 4) {
        std::cout << "Packet ID 4 received from node A" << std::endl;
        sendReceivedTimestamp(p);
        stateIdle(p);
    }
    else {
        std::cerr << "Not a valid situation" << std::endl;
    }
}

void UwDSync_B::stateIdle(Packet *p) {
    if (p != nullptr) {
        hdr_DATA *datah = HDR_DATA(p);
        int pktid = datah->ID();

        if (pktid == 1) {
            stateRxTrigger(p);
        } else if (pktid == 4 && num_equations < 5) {
            stateRxTrigger(p);
        } else {
            std::cerr << "Not a valid situation" << std::endl;
            Packet::free(p);
        }
    } else {
        if (pktid == 0) {
        }
    }
}

double UwDSync_B::sendReceivedTimestamp(Packet *p) {
    std::cout << "Sending received timestamps" << std::endl;
    hdr_DATA *datah = HDR_DATA(p);
    for (int i = 0; i < 4; i++) {
        receivedTimeStamp[i] = datah->ts_[i]; 
    }
    return receivedTimeStamp[0]; 
}

int UwDSync_B::BackOffTimer::getRandomBackoffTime() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(1, 10);
    return distr(gen);
}

void UwDSync_B::BackOffTimer::expire(Event *e) {
    if (module->timer.getCounter() < 5) {
        module->timer.incrCounter();
        module->timer.schedule(getRandomBackoffTime());
    } else {
        module->stateIdle();
    }
}