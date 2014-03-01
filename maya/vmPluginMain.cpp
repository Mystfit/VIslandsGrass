//
//  vmPluginMain.cpp
//  maya
//
//  Created by Byron Mallett on 3/14/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>
#include "VmIslandNode.h"


//Plugin Initializer
//--------------------------------------
MStatus initializePlugin( MObject i_object)
{
    fprintf(stderr, "Initializing islands plugin... \n");
    
    MStatus status;
    
    MFnPlugin pluginFn( i_object, "You", "v0.1", "Any", & status);
    CHECK_MSTATUS( status);
    
    status = pluginFn.registerNode( 
                                   VmIslandNode::typeName, 
                                   VmIslandNode::id,
                                   VmIslandNode::creator,
                                   VmIslandNode::initialize,
                                   MPxNode::kLocatorNode );
    CHECK_MSTATUS( status );
    
    fprintf( stderr, "Plugin added.\n");
    
    return status;
}



//Plugin De-initializer
//--------------------------------------
MStatus uninitializePlugin( MObject i_object )
{
    fprintf( stderr, "Un-intializing islands plugin... \n" );
    
    MStatus status;
    
    MFnPlugin pluginFn( i_object );
    
    status = pluginFn.deregisterNode( VmIslandNode::id );
    CHECK_MSTATUS( status );
    
    fprintf( stderr, "Plugin removed.\n" );
    
    return status;
}

