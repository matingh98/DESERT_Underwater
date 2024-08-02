## Copyright (c) 2015 Regents of the SIGNET lab, University of Padova.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 3. Neither the name of the University of Padova (SIGNET lab) nor the
#    names of its contributors may be used to endorse or promote products
#    derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
# TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
# PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
# OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
# ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# This script is used to test UW-SYNC protocol
# Here the complete stack used for each node in the simulation
#
# N.B.: UnderwaterChannel and UW/SYNC are used for doing Time Synchronization between two nodes
#
# Authors: Matin Ghalkhani
#
# Version: 1.0.0
#
# Stack of the nodes
#
#    NODE REF (Reference)                  NODE REG (Regular)
# +-----------------------+            +-----------------------+
# |      UW/SYNC          |            |      UW/SYNC          |
# +-----------------------+            +-----------------------+
#         |                                |
# +-----------------------------------------------------------+
# |                  UnderwaterChannel                        |
# +-----------------------------------------------------------+
######################################
# Flags to enable or disable options #
######################################
set opt(verbose) 		1
set opt(trace_files)		1
set opt(bash_parameters) 	0

#####################
# Library Loading   #
#####################
load libMiracle.so
load libMiracleWirelessCh.so
load libMiracleBasicMovement.so
load libuwip.so
load libuwstaticrouting.so
load libmphy.so
load libmmac.so
load libuwmmac_clmsgs.so
load libuwphy_clmsgs.so
load libuwmll.so
load libuwudp.so
load libuwcbr.so
load libuwtdma.so
load libuwinterference.so
load libUwmStd.so
load libUwmStdPhyBpskTracer.so
load libuwphy_clmsgs.so
load libuwstats_utilities.so
load libuwphysical.so
load libuwsync.so
#############################
# NS-Miracle initialization #
#############################
# You always need the following two lines to use the NS-Miracle simulator
set ns [new Simulator]
$ns use-Miracle

##################
# Tcl variables  #
##################
set opt(start_clock)       [clock seconds]
set opt(nn)                 2 ;# Number of Nodes
set opt(starttime)          1
set opt(stoptime)           1001

set opt(txduration)         [expr $opt(stoptime) - $opt(starttime)]
set opt(txpower)	    	140.0
set opt(maxinterval_)       200

set opt(freq) 			    26000.0
set opt(bw)              	16000.0
set opt(bitrate)	 	    3000.0
set opt(rngstream)          1
set opt(T_backoff)	 	    0
set opt(N_density)	     	5

if {$opt(bash_parameters)} {

    set opt(rngstream)  [lindex $argv 0]
    set opt(pktsize)    [lindex $argv 2]

}


set alphaMin [expr {1 - 100.0/1e6}]
set alphaMax [expr {1 + 100.0/1e6}]
set betaMin -5.0
set betaMax 5.0

set seed [clock seconds]
expr srand($seed)

set alpha [expr {$alphaMin + (rand() * ($alphaMax - $alphaMin))}]
set beta [expr {$betaMin + (rand() * ($betaMax - $betaMin))}]

puts "Alpha: $alpha"
puts "Beta: $beta"

Module/UW/SYNC/REF set alpha_ $alpha
Module/UW/SYNC/REF set beta_ $beta


set opt(tracefilename) "./test_uwsync.tr"
set opt(tracefile) [open $opt(tracefilename) w]


#########################
# Module Configuration  #
#########################


set channel [new Module/UnderwaterChannel]

MPropagation/Underwater set practicalSpreading_ 1.5
MPropagation/Underwater set debug_              0
MPropagation/Underwater set windspeed_          0
MPropagation/Underwater set shipping_           0
set propagation [new MPropagation/Underwater]


Module/UW/PHYSICAL  set BitRate_                   $opt(bitrate)
Module/UW/PHYSICAL  set AcquisitionThreshold_dB_   10.0
Module/UW/PHYSICAL  set MaxTxSPL_dB_               156
Module/UW/PHYSICAL  set MinTxSPL_dB_               10
Module/UW/PHYSICAL  set MaxTxRange_                50000
Module/UW/PHYSICAL  set CentralFreqOptimization_   0
Module/UW/PHYSICAL  set BandwidthOptimization_     0
Module/UW/PHYSICAL  set SPLOptimization_           0


set data_mask [new MSpectralMask/Rect]
$data_mask setFreq              $opt(freq)
$data_mask setBandwidth         $opt(bw)
$data_mask setPropagationSpeed  1500

set pos_x(1) 0.0
set pos_y(1) 0.0

set pos_x(2) 1000.0
set pos_y(2) 1000.0





################################
# Procedure(s) to create nodes #
################################

proc createNode { id } {

    global channel propagation data_mask ns cbr position node udp portnum ipr ipif channel_estimator
    global phy_data posdb opt rvposx rvposy rvposz mhrouting mll mac woss_utilities woss_creator db_manager
    global row pos_x pos_y sync

    set node($id) [$ns create-M_Node $opt(tracefile)]
    Module/UW/PHYSICAL  set debug_ 0

    # Check the value of id and set sync array accordingly
    if {$id == 1} {
        set sync($id) [new Module/UW/SYNC/REF]
    } elseif {$id == 2} {
        set sync($id) [new Module/UW/SYNC/REG]
    }

    puts "SYNC id [$sync($id) Id_]"

    set phy_data($id)  [new Module/UW/PHYSICAL]

    $node($id)  addModule 2 $sync($id)     1  "SYNC"
    $node($id)  addModule 1 $phy_data($id)   1  "PHY"

    $node($id) setConnection $sync($id)   $phy_data($id)   1
    $node($id) addToChannel  $channel       $phy_data($id)   1



    set position($id) [new "Position/BM"]
    $node($id) addPosition $position($id)

    #Setup positions
    $position($id) setX_ $pos_x($id)
    $position($id) setY_ $pos_y($id)
    $position($id) setZ_ -100



    set interf_data($id) [new "Module/UW/INTERFERENCE"]
    $interf_data($id) set maxinterval_ $opt(maxinterval_)
    $interf_data($id) set debug_       0



    $phy_data($id) setSpectralMask $data_mask
    $phy_data($id) setInterference $interf_data($id)
    $phy_data($id) setPropagation $propagation
    $phy_data($id) set debug_ 0
    $phy_data($id) setInterferenceModel "MEANPOWER"


}

Module/UW/SYNC/REG set alpha_ $alpha
Module/UW/SYNC/REG set beta_ $beta


#################
# Node Creation #
#################
# Create here all the nodes you want to network together
for {set id 1} {$id <= $opt(nn)} {incr id} {
    createNode $id
    puts "Node $id fully initialized"
}

# Schedule transmissions
$ns at 100 "$sync(1) transmit"
$ns at 200 "$sync(1) transmit"
$ns at 300 "$sync(1) transmit"
$ns at 400 "$sync(1) transmit"
$ns at 500 "$sync(1) transmit"
$ns at 600 "$sync(1) transmit"


# Procedure to print intermediate results
proc print_intermediate_results {time} {
    global sync
    puts "-----------------------------------------------------------------"
    puts "Intermediate results at time $time"
    puts "-----------------------------------------------------------------"
    puts "Current simulation time: $time s"
    if {[info exists sync(2)]} {
        puts "Received Time            : [$sync(2) get_timestamp]"
    }
    puts "-----------------------------------------------------------------"
}

# Schedule intermediate results printing
$ns at 100 "print_intermediate_results 100"
$ns at 200 "print_intermediate_results 200"
$ns at 300 "print_intermediate_results 300"
$ns at 400 "print_intermediate_results 400"
$ns at 500 "print_intermediate_results 500"
$ns at 600 "print_intermediate_results 600"


###################
# Final Procedure #
###################
proc finish {} {
    global ns opt outfile
    global phy_data channel propagation
    global phy
    global sync

    if {$opt(verbose)} {
        puts "-----------------------------------------------------------------"
        puts "Simulation summary"
        puts "-----------------------------------------------------------------"
        puts "Total simulation time    : [expr $opt(stoptime)-$opt(starttime)] s"
        puts "Number of nodes          : $opt(nn)"
        if {[info exists sync(2)]} {
            puts "Received Time            : [$sync(2) get_timestamp]"
        }
        puts "-----------------------------------------------------------------"
    }
}

###################
# start simulation
###################
if {$opt(verbose)} {
    puts "\nStarting Simulation\n"
}
$ns at [expr $opt(stoptime) + 50.0] "finish; $ns halt"

$ns run