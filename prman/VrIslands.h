
#ifndef VRISLANDS_H 
#define VRISLANDS_H 1

#include <rx.h>

extern "C" 
{
    // Init function..
    RtPointer ConvertParameters( RtString i_parameters );

    // Render function..
    RtVoid Subdivide( RtPointer i_data, RtFloat i_detail );

    // Cleanup function..
    RtVoid Free( RtPointer i_data );
}

RtVoid Primitive_Subdivide( RtPointer i_data, RtFloat i_detail );
RtVoid Primitive_Free( RtPointer i_data );

#endif

