/****************************************************************************

Copyright 2006, 2008 Virginia Polytechnic Institute and State University

This file is part of the OSSIE Sound_out Device.

OSSIE Sound_out Device is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

OSSIE Sound_out Device is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with OSSIE Sound_out Device; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA


****************************************************************************/

#include <iostream>

#ifndef HAVE_LEGACY
#include <omniORB4/omniURI.h>
#endif

#include "ossie/ossieSupport.h"

#include "soundCardPlayback.h"

using namespace std;
using namespace standardInterfaces;  // For standard OSSIE interface classes


int main(int argc, char* argv[])

{
    ossieDebugLevel = 1;

#ifdef HAVE_LEGACY
    ossieSupport::ORB* orbsup = new ossieSupport::ORB();

    char *id, *profile, *label;

    if (argc != 4) {
        std::cout << argv[0] << " : <identifier> <name> <SPD Profile>" << std::endl;
        exit(-1);
    }

    id = argv[1];
    label = argv[2];
    profile = argv[3];

    DEBUG(1, Sound_out, "Identifier = " << id << "Label = " << label << " Profile = " << profile);
#else
    if ( argc < 9 ) {
        std::cout << argv[0] << " DEVICE_MGR_IOR <string> PROFILE_NAME <string> DEVICE_ID <string> DEVICE_LABEL <string> " << std::endl;
        exit(EXIT_FAILURE);
    }

    int i = 0;
    char* devmgrior = NULL;
    char* profile = NULL;
    char* id = NULL;
    char* label = NULL;
    char* ncior = NULL;

    while ( i < argc ) {
        if ( strcmp( "DEVICE_MGR_IOR", argv[i] ) == 0 ) devmgrior = argv[i+1];
        if ( strcmp( "PROFILE_NAME", argv[i] ) == 0 ) profile = argv[i+1];
        if ( strcmp( "DEVICE_ID", argv[i] ) == 0 ) id = argv[i+1];
        if ( strcmp( "DEVICE_LABEL", argv[i] ) == 0 ) label = argv[i+1];
        if ( strcmp( "NAMING_CONTEXT_IOR", argv[i] ) == 0 ) ncior = argv[i+1];
        ++i;
    }

    if ( devmgrior == NULL ) {
        std::cout << "ERROR: " << argv[0] << " missing DEVICE_MGR_IOR argument" << std::endl;
        exit(EXIT_FAILURE);
    }

    if ( profile == NULL ) {
        std::cout << "ERROR: " << argv[0] << " missing PROFILE_NAME argument" << std::endl;
        exit(EXIT_FAILURE);
    }

    if ( id == NULL ) {
        std::cout << "ERROR: " << argv[0] << " missing DEVICE_ID argument" << std::endl;
        exit(EXIT_FAILURE);
    }

    if ( label == NULL ) {
        std::cout << "ERROR: " << argv[0] << " missing DEVICE_LABEL argument" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << argv[0]
              << " DEVICE_MGR_IOR " << devmgrior
              << " PROFILE_NAME " << profile
              << " DEVICE_ID " << id
              << " DEVICE_LABEL " << label;
    if ( ncior != NULL ) std::cout << " NAMING_CONTEXT_IOR " << ncior;
    std::cout << std::endl;

    ossieSupport::ORB::orb = CORBA::ORB_init( argc, argv );

    try {
        CORBA::Object_ptr _ncobj;
        if ( ncior == NULL )
            _ncobj = ossieSupport::ORB::orb->resolve_initial_references("NameService");
        else
            _ncobj = ossieSupport::ORB::orb->string_to_object(ncior);
        ossieSupport::ORB::inc = CosNaming::NamingContext::_narrow(_ncobj);
        CORBA::release(_ncobj);
    } catch ( ... ) {
        std::cout << "ERROR: " << argv[0] << " Failed to narrow NamingContext" << std::endl;
        exit(EXIT_FAILURE);
    }

    try {
        CORBA::Object_ptr _poaobj = ossieSupport::ORB::orb->resolve_initial_references("RootPOA");
        ossieSupport::ORB::poa = PortableServer::POA::_narrow(_poaobj);
        ossieSupport::ORB::pman = ossieSupport::ORB::poa->the_POAManager();
        ossieSupport::ORB::pman->activate();
    } catch ( ... ) {
        std::cout << "ERROR: " << argv[0] << " Failed to initialize the POA" << std::endl;
        exit(EXIT_FAILURE);
    }
#endif

    SoundCardPlayback_i* soundCardPlayback_servant;
    CF::Device_var soundCardPlayback_var;

// Create the Sound Card device servant and object reference

    soundCardPlayback_servant = new SoundCardPlayback_i(id, label, profile);
    soundCardPlayback_var = soundCardPlayback_servant->_this();

    string objName = "DomainName1/";
    objName += label;

#ifdef HAVE_LEGACY
    orbsup->bind_object_to_name((CORBA::Object_ptr) soundCardPlayback_var, objName.c_str());
#else
    CosNaming::Name_var _bindingname = omni::omniURI::stringToName(objName.c_str());
    try {
        ossieSupport::ORB::inc->rebind(_bindingname, (CORBA::Object_ptr) soundCardPlayback_var);
    } catch ( ... ) {
        std::cout << "ERROR: " << argv[0] << " Failed to rebind reference to NameService" << std::endl;
        exit(EXIT_FAILURE);
    }
#endif


// Create control ports for sound in and out control

    soundOutControl_i* soundOutControl_servant;
    audioOutControl_var soundOutControl_var;

    soundOutControl_servant = new soundOutControl_i();
    soundOutControl_var = soundOutControl_servant->_this();

    soundInControl_i* soundInControl_servant;
    audioInControl_var soundInControl_var;

    soundInControl_servant = new soundInControl_i();
    soundInControl_var = soundInControl_servant->_this();

    objName = "DomainName1/";
    objName += "soundOutControl";

#ifdef HAVE_LEGACY
    orbsup->bind_object_to_name((CORBA::Object_ptr) soundOutControl_var, objName.c_str());
#else
    _bindingname = omni::omniURI::stringToName(objName.c_str());
    try {
        ossieSupport::ORB::inc->rebind(_bindingname, (CORBA::Object_ptr) soundOutControl_var);
    } catch ( ... ) {
        std::cout << "ERROR: " << argv[0] << " Failed to rebind reference to NameService" << std::endl;
        exit(EXIT_FAILURE);
    }
#endif

    objName = "DomainName1/";
    objName += "soundInControl";

#ifdef HAVE_LEGACY
    orbsup->bind_object_to_name((CORBA::Object_ptr) soundInControl_var, objName.c_str());
#else
    _bindingname = omni::omniURI::stringToName(objName.c_str());
    try {
        ossieSupport::ORB::inc->rebind(_bindingname, (CORBA::Object_ptr) soundInControl_var);
    } catch ( ... ) {
        std::cout << "ERROR: " << argv[0] << " Failed to rebind reference to NameService" << std::endl;
        exit(EXIT_FAILURE);
    }
#endif

// Create the ports for sound output data

    soundOut_i* soundOut;

    complexShort_var soundOut_var;

    soundOut = new soundOut_i(soundCardPlayback_servant);
    soundOut_var = soundOut->_this();

    objName = "DomainName1/";
    objName += "soundOut";

#ifdef HAVE_LEGACY
    orbsup->bind_object_to_name((CORBA::Object_ptr) soundOut_var, objName.c_str());
#else
    _bindingname = omni::omniURI::stringToName(objName.c_str());
    try {
        ossieSupport::ORB::inc->rebind(_bindingname, (CORBA::Object_ptr) soundOut_var);
    } catch ( ... ) {
        std::cout << "ERROR: " << argv[0] << " Failed to rebind reference to NameService" << std::endl;
        exit(EXIT_FAILURE);
    }
#endif


#if 0 // Save for when I need to input from souncard
// Create the ports for RX Data

    RX_data_i *rx_data_1;

    CF::Port_var rx_data_1_var;

    rx_data_1 = new RX_data_i(1);

    rx_data_1_var = rx_data_1->_this();

    objName = "DomainName1/";
    objName += "RX_Data_1";
    orb->bind_object_to_name((CORBA::Object_var) rx_data_1_var, objName.c_str());
#endif

// Start handling CORBA requests
#ifdef HAVE_LEGACY
    orbsup->orb->run();
#else
    ossieSupport::ORB::orb->run();
    ossieSupport::ORB::orb->destroy();
#endif
    return 0;
}
