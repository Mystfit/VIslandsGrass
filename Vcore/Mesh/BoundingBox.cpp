//
//  BoundingBox.cpp
//  Vcore
//
//  Created by Byron Mallett on 3/19/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "BoundingBox.h"

using namespace Vcore;

BoundingBox::BoundingBox()
{
    
}

BoundingBox::BoundingBox( int width, int height, int depth )
{
    setWidth( width );
    setHeight( height );
    setDepth( depth );
}