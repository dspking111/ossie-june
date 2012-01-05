/****************************************************************************

 Copyright 2007 Virginia Polytechnic Institute and State University

 This file is part of the OSSIE Channel.

 OSSIE Channel is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 OSSIE Channel is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with OSSIE Channel; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

 ****************************************************************************/

#ifndef CHANNEL_IMPL_H
#define CHANNEL_IMPL_H

#include <stdlib.h>
#include "ossie/cf.h"
#include "ossie/PortTypes.h"
#include "ossie/Resource_impl.h"
#include "ossie/debug.h"

#include "standardinterfaces/complexShort.h"
#include "standardinterfaces/complexShort_u.h"
#include "standardinterfaces/complexShort_p.h"

/** \brief
 *
 *
 */
class Channel_i: public virtual Resource_impl
{
public:
/// Initializing constructor
    Channel_i(const char *uuid, omni_condition *sem);

/// Destructor
    ~Channel_i(void);

/// Static function for omni thread
    static void Run(void * data);

///
    void start() throw (CF::Resource::StartError, CORBA::SystemException);

///
    void stop() throw (CF::Resource::StopError, CORBA::SystemException);

///
    CORBA::Object_ptr getPort(const char* portName)
    throw (CF::PortSupplier::UnknownPort, CORBA::SystemException);

///
    void releaseObject() throw (CF::LifeCycle::ReleaseError,
                                CORBA::SystemException);

///
    void initialize() throw (CF::LifeCycle::InitializeError,
                             CORBA::SystemException);

    void query(CF::Properties &configProperties) throw (CF::UnknownProperties,
            CORBA::SystemException);

/// Configures properties read from .prf.xml
    void configure(const CF::Properties&) throw (CORBA::SystemException,
            CF::PropertySet::InvalidConfiguration,
            CF::PropertySet::PartialConfiguration);

private:
/// Disallow default constructor
    Channel_i();

/// Disallow copy constructor
    Channel_i(Channel_i&);

/// Main signal processing method
    void ProcessData();

    omni_condition *component_running; ///< for component shutdown
    omni_thread *processing_thread; ///< for component writer function


// list components provides and uses ports
    standardInterfaces_i::complexShort_p *dataIn;
    standardInterfaces_i::complexShort_u *dataOut;

    bool fading;
    bool envelope;
    short int Ns; //Number of samples per symbol

    double noise_sigma,K_factor,MaxDopplerRate;

    float randn();
    float randf();

};
#endif
