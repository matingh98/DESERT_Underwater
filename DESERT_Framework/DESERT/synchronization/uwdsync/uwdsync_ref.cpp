//
// Copyright (c) 2019 Regents of the SIGNET lab, University of Padova.
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
 * @file   uwdsync_ref.cpp
 * @author Matin Ghalkhani
 * @version 1.0.0
 *
 * \brief Implementation of Uwpolling_SINK class
 *
 */

#include "uwdsync_ref.h"
#include "mmac.h"
#include "mac.h"
#include "uwcbr-module.h"
#include "mphy_pktheader.h"
#include "rng.h"
#include "uwdsync_cmn_hdr.h"

#include <algorithm>
#include <sstream>
#include <sys/time.h>

/**
 * Class that represents the binding with the tcl configuration script
 */
static class UwDSync_REF : public TclClass
{
public:
	/**
	 * Constructor of the class
	 */
	UwDSync_REF()
		: TclClass("Module/UW/DSYNC/REF")
	{
	}

	/**
	 * Creates the TCL object needed for the tcl language interpretation
	 * @return Pointer to an TclObject
	 */
	TclObject *
	create(int, const char *const *)
	{
		return (new UwDSync_ref());
	}
} class_module_uwdsync_ref;

UwDSync_ref::UwDSync_ref()
	: MMac()

{
}

UwDSync_ref::~UwDSync_ref()
{
}

int UwDSync_ref::command(int argc, const char *const *argv)
{
}

int UwDSync_ref::crLayCommand(ClMessage *m)
{
	switch (m->type())
	{
	default:
		return MMac::crLayCommand(m);
	}
}

void UwDSync_ref::stateIdle()
{
}

//void UwDSync_ref::Phy2MacStartRx(Packet *p)
//{
//}

// void UwDSync_ref::Phy2MacEndRx(Packet *p){}

void UwDSync_ref::Mac2PhyStartTx(Packet *p)
{
	MMac::Mac2PhyStartTx(p); // Send down the packet to phy layer
}

void UwDSync_ref::Phy2MacEndTx(const Packet *p)
{
	hdr_DSYNC *dysnc_hdr = HDR_DSYNC(p);

	// Increment DSYNC_uid_ each time a packet is received
	dysnc_hdr->DSYNC_uid_++; // Access DSYNC_uid_ as a variable

	// Update TIME_STAMP
	dysnc_hdr->TIME_STAMP = NOW;
	stateIdle();
}
