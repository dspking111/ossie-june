/****************************************************************************

Copyright 2005, 2006, 2007 Virginia Polytechnic Institute and State University

This file is part of the OSSIE USRP Device.

OSSIE USRP Device is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

OSSIE USRP Device is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with OSSIE USRP Device; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA


****************************************************************************/

#include <iostream>
#include <omniORB4/CORBA.h>
#include "ossie/debug.h"
#include "USRP.h"

USRP_TX_Control_i::USRP_TX_Control_i(USRP_i *_usrp, const char* _name, const char* _domain) : standardInterfaces_i::TX_Control_p(_name, _domain), usrp(_usrp)
{
    DEBUG(3, USRP, "TX Control port constructor called")
}

void USRP_TX_Control_i::set_number_of_channels(CORBA::ULong nchan)
{
    DEBUG(3, USRP, "Setting number of channels to " << nchan);

    omni_mutex_lock l(usrp->tx_control_access);

    usrp->usrp_tx->set_nchannels(nchan);
}

void USRP_TX_Control_i::get_number_of_channels(CORBA::ULong &num)
{
    omni_mutex_lock l(usrp->tx_control_access);

    num = usrp->usrp_tx->nchannels();
}

void USRP_TX_Control_i::get_gain_range(CORBA::ULong channel, CORBA::Float &gmin, CORBA::Float &gmax, CORBA::Float &gstep)
{
    if (channel == 0) {
        if (usrp->tx_db0_control) {
            gmin = usrp->tx_db0_control->gain_min();
            gmax = usrp->tx_db0_control->gain_max();
            gstep = usrp->tx_db0_control->gain_db_per_step();
        }
    } else if (channel == 1) {
        if (usrp->tx_db1_control) {
            gmin = usrp->tx_db1_control->gain_min();
            gmax = usrp->tx_db1_control->gain_max();
            gstep = usrp->tx_db1_control->gain_db_per_step();
        }
    } else {
        return; ///\todo throw bad channel exception
    }
}

void USRP_TX_Control_i::set_gain(CORBA::ULong channel, CORBA::Float gain)
{
    omni_mutex_lock l(usrp->tx_control_access);

    if (channel == 0) {
        if (usrp->tx_db0_control) {
            usrp->tx_db0_control->set_gain(gain);
        }
    } else if (channel == 1) {
        if (usrp->tx_db1_control)
            usrp->tx_db1_control->set_gain(gain);
    } else {
        return; ///\todo throw bad channel exception
    }
}

void USRP_TX_Control_i::get_gain(CORBA::ULong channel, CORBA::Float &gain)
{
    omni_mutex_lock l(usrp->tx_control_access);

    gain = usrp->usrp_tx->pga(channel);
}

void USRP_TX_Control_i::get_frequency_range(CORBA::ULong channel, CORBA::Float &fmin, CORBA::Float &fmax, CORBA::Float &fstep)
{
    if (channel == 0) {
        if (usrp->tx_db0_control) {
            fmin = usrp->tx_db0_control->freq_min();
            fmax = usrp->tx_db0_control->freq_max();
            fstep = 0;
        }
    } else if (channel == 1) {
        if (usrp->tx_db1_control) {
            fmin = usrp->tx_db1_control->freq_min();
            fmax = usrp->tx_db1_control->freq_max();
            fstep = 0;
        }
    } else {
        return; ///\todo throw bad channel exception
    }
}

void USRP_TX_Control_i::set_frequency(CORBA::ULong channel, CORBA::Float f)
{
    DEBUG(3, USRP, "In TX Control set frequency channel: " << channel << ", frequency: " << f);

    db_base_sptr db;
    usrp_tune_result result;

    omni_mutex_lock l(usrp->tx_control_access);

    if (channel == 0) {
        db = usrp->tx_db0_control;
    } else if (channel == 1) {
        db = usrp->tx_db1_control;
    } else {
        DEBUG(3, USRP, "USRP TX set_frequency (invalid channel)");
        return;
    }

    if (!usrp->usrp_tx->tune(channel, db, f, &result)) {
        DEBUG(3, USRP, "USRP TX set_frequency tune failed");
        return; // throw exception
    }

    DEBUG(3, USRP, "USRP TX tune_result:" <<
          " baseband_freq="      << result.baseband_freq <<
          " dxc_freq="           << result.dxc_freq      <<
          " residual_freq="      << result.residual_freq <<
          " inverted="           << result.inverted);
}

void USRP_TX_Control_i::get_frequency(CORBA::ULong channel, CORBA::Float &f)
{
    omni_mutex_lock l(usrp->tx_control_access);

    f = usrp->usrp_tx->tx_freq(channel);
}

void USRP_TX_Control_i::start(CORBA::ULong channel)
{
    omni_mutex_lock l(usrp->tx_control_access);

    if (channel == 0) {
        if (usrp->tx_db0_control)
            usrp->tx_db0_control->set_enable(true);
    } else if (channel == 1) {
        if (usrp->tx_db1_control)
            usrp->tx_db1_control->set_enable(true);
    } else {
        return; /// \todo throw bad channel exception
    }

    if (!usrp->tx_active) {
        usrp->usrp_tx->start();
        usrp->tx_thread = new omni_thread(USRP_i::do_tx_data_process, ((void *)usrp));
        usrp->tx_thread->start();
    }
}

void USRP_TX_Control_i::stop(CORBA::ULong channel)
{
    omni_mutex_lock l(usrp->tx_control_access);

    if (channel == 0) {
        if (usrp->tx_db0_control)
            usrp->tx_db0_control->set_enable(false);
    } else if (channel == 1) {
        if (usrp->tx_db1_control)
            usrp->tx_db1_control->set_enable(false);
    }

    usrp->tx_active = false;

// Previously never called
//usrp->usrp_tx->stop();
}

void USRP_TX_Control_i::set_values(const CF::Properties &values)
{
    DEBUG(3, USRP, "USRP TX setting " << values.length() << " values, value[0].id " << values[0].id)

    for (unsigned int i =0; i < values.length(); ++i) {
        if (strcmp(values[i].id, "SET_MUX") == 0 ) {
            CORBA::ULong mux;
            values[i].value >>= mux;
            DEBUG(3, USRP, "Setting transmit mux to  " << mux)
            omni_mutex_lock l(usrp->tx_control_access);
            usrp->usrp_tx->set_mux(mux);
        } else if (strcmp(values[i].id, "SET_AUTO_TR_1") == 0) {
            CORBA::ULong atx;
            values[i].value >>= atx;
            DEBUG(3, USRP, "Set Auto TX/RX for side 1 to " << atx)
            omni_mutex_lock l(usrp->tx_control_access);
            if (usrp->tx_db0_control && usrp->rx_db0_control) {
                if (atx) {
                    usrp->tx_db0_control->set_auto_tr(true);
                    usrp->rx_db0_control->set_auto_tr(true);
                } else {
                    usrp->tx_db0_control->set_auto_tr(false);
                    usrp->rx_db0_control->set_auto_tr(false);
                }
            }
        }
    }
}

void USRP_TX_Control_i::set_interpolation_rate(CORBA::ULong channel, CORBA::ULong I)
{
// USRP interpolation rate applies to all channels, ignore channel
    omni_mutex_lock l(usrp->tx_control_access);

    usrp->usrp_tx->set_interp_rate(I);
}

void USRP_TX_Control_i::get_interpolation_range(CORBA::ULong channel, CORBA::ULong &imin, CORBA::ULong &imax, CORBA::ULong &istep)
{
    imin = 4;
    imax = 512;
    istep = 4;
}

USRP_RX_Control_i::USRP_RX_Control_i(USRP_i *_usrp, const char* _name, const char* _domain) : standardInterfaces_i::RX_Control_p(_name, _domain), usrp(_usrp)
{
    DEBUG(3, USRP, "RX Control port constructor called")
}

void USRP_RX_Control_i::set_number_of_channels(CORBA::ULong nchan)
{
    DEBUG(3, USRP, "Setting number of channels to " << nchan)

    omni_mutex_lock l(usrp->rx_control_access);

    usrp->usrp_rx->set_nchannels(nchan);
}

void USRP_RX_Control_i::get_number_of_channels(CORBA::ULong &num)
{
    omni_mutex_lock l(usrp->rx_control_access);

    num = usrp->usrp_rx->nchannels();
}

void USRP_RX_Control_i::get_gain_range(CORBA::ULong channel, CORBA::Float &gmin, CORBA::Float &gmax, CORBA::Float &gstep)
{
    if (channel == 0) {
        if (usrp->rx_db0_control) {
            gmin = usrp->rx_db0_control->gain_min();
            gmax = usrp->rx_db0_control->gain_max();;
            gstep = usrp->rx_db0_control->gain_db_per_step();
        }
    } else if (channel == 1) {
        if (usrp->rx_db1_control) {
            gmin = usrp->rx_db1_control->gain_min();
            gmax = usrp->rx_db1_control->gain_max();;
            gstep = usrp->rx_db1_control->gain_db_per_step();
        }
    } else {
        return; ///\todo throw bad channel exception
    }
}

void USRP_RX_Control_i::set_gain(CORBA::ULong channel, CORBA::Float gain)
{
    DEBUG(1, USRP, "set gain to "<<gain);

    omni_mutex_lock l(usrp->rx_control_access);

    if (channel == 0) {
        if (usrp->rx_db0_control)
            usrp->rx_db0_control->set_gain(gain);
    } else if (channel == 1) {
        if (usrp->rx_db1_control)
            usrp->rx_db1_control->set_gain(gain);
    } else {
        DEBUG(1, USRP, "Attempt to set gain on non-existent db.");
        return; ///\todo throw bad channel exception
    }
}

void USRP_RX_Control_i::get_gain(CORBA::ULong channel, CORBA::Float &gain)
{
    omni_mutex_lock l(usrp->rx_control_access);

    int temp_gain, temp_aux;
    temp_gain = (int) usrp->usrp_rx->pga(channel);
    temp_aux = usrp->usrp_rx->read_aux_adc(channel, 0);

// for now adding temp_gain and temp_aux.....PHELPS with fix it
    gain = temp_gain + temp_aux;
}

void USRP_RX_Control_i::get_frequency_range(CORBA::ULong channel, CORBA::Float &fmin, CORBA::Float &fmax, CORBA::Float &fstep)
{
    if (channel == 0) {
        if (usrp->rx_db0_control) {
            fmin = usrp->rx_db0_control->freq_min();
            fmax = usrp->rx_db0_control->freq_max();
            fstep = 0;
        }
    } else if (channel == 1) {
        if (usrp->rx_db1_control) {
            fmin = usrp->rx_db1_control->freq_min();
            fmax = usrp->rx_db1_control->freq_max();
            fstep = 0;
        }
    } else {
        return; ///\todo throw bad channel exception
    }
}

void USRP_RX_Control_i::set_frequency(CORBA::ULong channel, CORBA::Float f)
{
    DEBUG(3, USRP, "In RX Control set frequency channel: " << channel << ", frequency: " << f);

    db_base_sptr db;
    usrp_tune_result result;

    omni_mutex_lock l(usrp->rx_control_access);

    if (channel == 0) {
        if (usrp->rx_db0_control)
            db = usrp->rx_db0_control;
    } else if (channel == 1) {
        if (usrp->rx_db1_control)
            db = usrp->rx_db1_control;
    } else {
        std::cerr << "Bad channel specified" << std::endl;
        return; ///\todo throw bad channel exception
    }

    if (!usrp->usrp_rx->tune(channel, db, f, &result)) {
        DEBUG(3, USRP, "USRP RX set_frequency failed");
        return; // throw exception
    }

    DEBUG(3, USRP, "USRP RX tune_result:" <<
          " baseband_freq="      << result.baseband_freq <<
          " dxc_freq="           << result.dxc_freq      <<
          " residual_freq="      << result.residual_freq <<
          " inverted="           << result.inverted);
}

void USRP_RX_Control_i::get_frequency(CORBA::ULong channel, CORBA::Float &f)
{
    omni_mutex_lock l(usrp->rx_control_access);

    f = usrp->usrp_rx->rx_freq(channel);
}

void USRP_RX_Control_i::start(CORBA::ULong channel)
{
    omni_mutex_lock l(usrp->rx_control_access);

    usrp->rx_packet_count = usrp->set_rx_packet_count;
    usrp->rx_active = true;

// Set up RX thread
    usrp->rx_thread = new omni_thread(USRP_i::do_rx_data_process, ((void *)usrp));
    usrp->rx_thread->start();
}

void USRP_RX_Control_i::stop(CORBA::ULong channel)
{
    omni_mutex_lock l(usrp->rx_control_access);

    usrp->rx_active = false;
}

void USRP_RX_Control_i::set_values(const CF::Properties &values)
{
    DEBUG(3, USRP, "USRP RX setting " << values.length() << " values, value[0].id " << values[0].id)

    for (unsigned int i =0; i < values.length(); ++i) {
        if (strcmp(values[i].id, "SET_NUM_RX_PACKETS") == 0) {
            CORBA::ULong num_packets;
            values[i].value >>= num_packets;
            DEBUG(3, USRP, "Number of packets to RX = " << num_packets)
            usrp->set_rx_packet_count = num_packets;
        } else if (strcmp(values[i].id, "SET_MUX") == 0 ) {
            CORBA::ULong mux;
            values[i].value >>= mux;
            DEBUG(3, USRP, "Setting receive mux to  " << mux)
            omni_mutex_lock l(usrp->rx_control_access);
            usrp->usrp_rx->set_mux(mux);
        } else if (strcmp(values[i].id, "SET_RX_ANT_1") == 0 ) {
            CORBA::ULong ant;
            values[i].value >>= ant;
            DEBUG(3, USRP, "Setting receive antenna to  " << ant)
            omni_mutex_lock l(usrp->rx_control_access);
            if (usrp->rx_db0_control)
                usrp->rx_db0_control->select_rx_antenna(ant);
        }
    }
}

void USRP_RX_Control_i::set_decimation_rate(CORBA::ULong channel, CORBA::ULong D)
{
    omni_mutex_lock l(usrp->rx_control_access);

    usrp->usrp_rx->set_decim_rate(D);
}

void USRP_RX_Control_i::get_decimation_range(CORBA::ULong channel, CORBA::ULong &dmin, CORBA::ULong &dmax, CORBA::ULong &dstep)
{
    dmin = 4;
    dmax = 256;
    dstep = 2;
}

void USRP_RX_Control_i::set_data_packet_size(CORBA::ULong channel, CORBA::ULong N)
{
    omni_mutex_lock l(usrp->rx_control_access);

    usrp->rx_packet_size = N;
}
