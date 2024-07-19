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
UwSyncREF::UwSyncREF() : MMac(), pktid_(0), start_time(0), stop_time(1001)
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
    if (strcasecmp(argv[1], "transmit") == 0)
    {
        initPkt();
        return TCL_OK;
    }

    return MMac::command(argc, argv);
}

void UwSyncREF::initPkt()
{
    Packet *p = Packet::alloc();
    hdr_cmn *ch = hdr_cmn::access(p);
    hdr_SYNC *synch = HDR_SYNC(p);

    synch->ID() = pktid_;
    std::cout << "Initializing packet with ID: " << synch->ID() << std::endl;
    ch->size() = sizeof(hdr_SYNC);
    stateIdle(p);
}


void UwSyncREF::stateIdle(Packet *p)
{
    hdr_SYNC *synch = HDR_SYNC(p);
    int pktid_ = synch->ID();

    std::cout << "Entering stateIdle with pktid: " << pktid_ << std::endl;

    if (pktid_ == 0)
    {
        std::cout << "Calling TransmittingToNodeREG for pktid 0" << std::endl;
        TransmittingToNodeREG(p);
    }
    else if (pktid_ == 3)
    {
        std::cout << "Calling RxPacket for pktid 3" << std::endl;
        RxPacket(p);
    }
    else if (pktid_ == 4)
    {
        std::cout << "Calling TransmittingToNodeREG for pktid 4" << std::endl;
        TransmittingToNodeREG(p);
    }
    else
    {
        std::cout << "Not a valid situation, pktid: " << pktid_ << std::endl;
        Packet::free(p);
    }
}


void UwSyncREF::RxPacket(Packet *p)
{
    hdr_SYNC *synch = HDR_SYNC(p);
    int pktid_ = synch->ID();

    synch->ts_[3] = NOW;
    synch->ID() = 4;

    Transmitting(p);
}

void UwSyncREF::TransmittingToNodeREG(Packet *p)
{
    hdr_SYNC *synch = HDR_SYNC(p);
    std::cout << "Before transmit packet ID: " << synch->ID() << std::endl;
    if (synch->ID() == 0)
    {
        synch->ts_[0] = NOW;
        synch->ID() = 1;
        ts_[0] = synch->ts_[0];
    }
    else if (synch->ID() == 4)
    {
    }
    std::cout << "After packet ID: " << synch->ID() << std::endl;
    std::cout << "Packet before transmission: ";
    for (std::size_t i = 0; i < sizeof(hdr_SYNC); ++i)
    {
        std::cout << std::hex << (int)((unsigned char *)synch)[i] << " ";
    }
    std::cout << std::dec << std::endl;
    Transmitting(p);
}

void UwSyncREF::Transmitting(Packet *p)
{
    hdr_cmn *ch = hdr_cmn::access(p);
    if (ch == nullptr)
    {
        std::cerr << "Failed to access common header in Transmitting" << std::endl;
        Packet::free(p);
        return;
    }

    std::cout << "Transmitting packet of size: " << ch->size() << std::endl;
    MMac::Mac2PhyStartTx(p);
}

void UwSyncREF::recv(Packet *p)
{
    hdr_cmn *ch = hdr_cmn::access(p);
    hdr_SYNC *synch = HDR_SYNC(p);
    std::cout << "Receiving ID: " << synch->ID() << std::endl;
    std::cout << "Packet on reception: ";
    for (std::size_t i = 0; i < sizeof(hdr_SYNC); ++i)
    {
        std::cout << std::hex << (int)((unsigned char *)synch)[i] << " ";
    }
    std::cout << std::dec << std::endl;
    if (synch->ID() == 0)
    {
        Packet::free(p);
        initPkt();
    }
    else
    {
         if (synch->ID() == 3)
    {
        if (ch->direction() == hdr_cmn::UP || ch->direction() == hdr_cmn::DOWN)
        {
            std::cout << "Received from Node REG ";

            stateIdle(p);
        }
    }
    }
}
