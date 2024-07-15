//
// Copyright (c) 2019 Regents of the SIGNET lab, University of Padova.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions wSyncREFand the following disclaimer.
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
 * @file   uwsync_ref.cpp
 * @author Matin Ghalkhani
 * @version 1.0.0
 *
 * \brief Implementation of uwsync_ref class
 *
 */

#include <node_REF.h>
#include <sync_header.h>
#include "mac.h"
#include "mmac.h"
#include "mphy_pktheader.h"
#include <sstream>
#include <sys/time.h>
#include <iostream>
#include <random>

static class UwSyncREF_Class : public TclClass
{
public:
    UwSyncREF_Class() : TclClass("Module/UW/SYNC/REF") {}
    TclObject *create(int, const char *const *)
    {
        return (new UwSyncREF());
    }
} class_module_uwsync_ref;
UwSyncREF::UwSyncREF() : MMac(), pktid(0), start_time(0), stop_time(1001)
{
    std::cout << "UwDSync_A constructor called" << std::endl;
    bind("start_time_", (double *)&start_time);
    bind("stop_time_", (double *)&stop_time);
    std::fill(std::begin(ts_), std::end(ts_), 0.0); // Initialize ts_ to 0
}

UwSyncREF::~UwSyncREF(){};

int UwSyncREF::crLayCommand(ClMessage *m)
{
    switch (m->type())
    {
    default:
        return MMac::crLayCommand(m);
    }
}



int UwSyncREF::command(int argc, const char *const *argv)
{
    Tcl &tcl = Tcl::instance();
    std::cout << "Command method called. Object address: " << this << std::endl; // Print object address

    if (strcasecmp(argv[1], "get_timestamp_ts1") == 0) {
			tcl.resultf("%d", get_timestamp_ts1());
			return TCL_OK;}
    else if (strcasecmp(argv[1], "transmit") == 0)
    {
        initPkt();
        return TCL_OK;
    }

    return MMac::command(argc, argv);
}

void UwSyncREF::initPkt()
{
    Packet *p = Packet::alloc();

    if (p == nullptr) {
        std::cerr << "Packet allocation failed" << std::endl;
        return;
    }

    hdr_cmn *ch = hdr_cmn::access(p);
    if (ch == nullptr) {
        std::cerr << "Failed to access common header" << std::endl;
        Packet::free(p);
        return;
    }

    hdr_SYNC *synch = HDR_SYNC(p);
    if (synch == nullptr) {
        std::cerr << "Failed to access SYNC header" << std::endl;
        Packet::free(p);
        return;
    }

    synch->ID() = pktid;
    std::cout << "Packet initialized with pktid: " << synch->ID() << std::endl;

    // Ensure size is set correctly
    ch->size() = sizeof(hdr_SYNC);

    std::cout << "Packet size set to: " << ch->size() << std::endl;
    stateIdle(p);
}



void UwSyncREF::RxPacket(Packet *p)
{
    hdr_SYNC *synch = HDR_SYNC(p);
    int pktid = synch->ID();

    synch->ts_[3] = NOW;
    synch->ID() = 4;

    Transmitting(p);
    stateIdle(p);
}

void UwSyncREF::stateIdle(Packet *p)
{
    hdr_SYNC *synch = HDR_SYNC(p);
    int pktid = synch->ID();

    std::cout << "Entering stateIdle with pktid: " << pktid << std::endl;

    if (pktid == 0)
    {
        std::cout << "Calling TransmittingToNodeREG for pktid 0" << std::endl;
        TransmittingToNodeREG(p);
    }
    else if (pktid == 3)
    {
        std::cout << "Calling RxPacket for pktid 3" << std::endl;
        RxPacket(p);
    }
    else if (pktid == 4)
    {
        std::cout << "Calling TransmittingToNodeREG for pktid 4" << std::endl;
        TransmittingToNodeREG(p);
    }
    else
    {
        std::cout << "Not a valid situation, pktid: " << pktid << std::endl;
        Packet::free(p);
    }
}

void UwSyncREF::TransmittingToNodeREG(Packet *p)
{
    hdr_SYNC *synch = HDR_SYNC(p);
    int pktid = synch->ID();
    std::cout << "TransmittingToNodeREG called with pktid: " << pktid << std::endl;
    std::cout << "Object address in TransmittingToNodeREG: " << this << std::endl;

    if (pktid == 0)
    {
        synch->ts_[0] = 11;
        synch->ID() = 1;
        ts_[0] = 11;  // Also update the object's ts_
        std::cout << "ts_[0] set to " << synch->ts_[0] << " and pktid set to " << synch->ID() << std::endl;
    }
    else if (pktid == 4)
    {
        synch->ts_[3] = NOW;
        synch->ID() = 4;
        std::cout << "ts_[3] set to NOW, pktid set to 4" << std::endl;
    }
    Transmitting(p);
}


void UwSyncREF::Transmitting(Packet *p)
{
    hdr_cmn *ch = hdr_cmn::access(p);
    if (ch == nullptr) {
        std::cerr << "Failed to access common header in Transmitting" << std::endl;
        Packet::free(p);
        return;
    }

    std::cout << "Transmitting packet of size: " << ch->size() << std::endl;
    MMac::Mac2PhyStartTx(p);
}

void UwSyncREF::recv(Packet *p)
{
    std::cout << "Receiving packet" << std::endl;

    hdr_cmn *ch = hdr_cmn::access(p);
    if (ch == nullptr) {
        std::cerr << "Failed to access common header" << std::endl;
        return;
    }

    hdr_SYNC *synch = HDR_SYNC(p);
    if (synch == nullptr) {
        std::cerr << "Failed to access SYNC header" << std::endl;
        return;
    }

    int pktid = synch->ID();
    std::cout << "Packet received with direction: " << ch->direction() << " and pktid: " << pktid << std::endl;
    std::cout << "Current ts_[0] in recv: " << synch->ts_[0] << std::endl;

    if (pktid == 10)
    {
        std::cout << "pktid is 10, initializing packet" << std::endl;
        initPkt();
    }
    else
    {
        if (ch->direction() == hdr_cmn::UP)
        {
            std::cout << "Packet direction is UP, calling stateIdle" << std::endl;
            stateIdle(p);
        }
        else if (ch->direction() == hdr_cmn::DOWN)
        {
            std::cout << "Packet direction is DOWN, calling stateIdle" << std::endl;
            stateIdle(p);
        }
    }
}