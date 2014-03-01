//
//  BoundingBox.h
//  Vcore
//
//  Created by Byron Mallett on 3/19/12.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H 1

#include "Vec3.h"

namespace Vcore {

    class BoundingBox {
        
    public:
        BoundingBox();
        BoundingBox( int width, int height, int depth );
        
        //------------------------------------
        // Accessors
        //------------------------------------

        //Setters
        void setPosition( Vec3 position ){
            m_pos = position;
        }
        
        void setWidth( int width ) {
            m_btFrontLeft.x = width * -0.5;
            m_tpBackRight.x = width * 0.5;
        }
        
        void setHeight( int height ) {
            m_btFrontLeft.y = height * -0.5;
            m_tpBackRight.y = height * 0.5;
        }
        
        void setDepth ( int depth ) {
            m_btFrontLeft.z = depth * -0.5;
            m_tpBackRight.z = depth * 0.5;
        }
    
    
        //Getters
        
        Vec3 getPosition(){
            return m_pos;
        }
        
        Vec3 getPosition() const{
            return m_pos;
        }
        
        int getWidth(){
            return m_tpBackRight.x - m_btFrontLeft.x;
        }
        
        int getWidth() const{
            return getWidth();
        }
        
        int getHeight(){
            return m_tpBackRight.y- m_btFrontLeft.y;
        }
        
        int getHeight() const{
            return getHeight();
        }
        
        int getDepth(){
            return m_tpBackRight.z - m_btFrontLeft.z;
        }
        
        int getDepth() const{
            return getDepth();
        }

        
        //Return local corner positions
        Vec3 getBottomFrontLeft_l()
        {
            return m_btFrontLeft;
        }
        
        Vec3 getBottomFrontLeft_l() const
        {
            return m_btFrontLeft;
        }
        
        Vec3 getTopBackRight_l(){
            return m_tpBackRight;
        }
        
        Vec3 getTopBackRight_l() const{
            return m_tpBackRight;
        }
        
        //Return world corner positions
        Vec3 getBottomFrontLeft_w()
        {
            return m_btFrontLeft + m_pos;
        }
        
        Vec3 getBottomFrontLeft_w() const
        {
            return m_btFrontLeft + m_pos;
        }
        
        Vec3 getTopBackRight_w()
        {
            return m_tpBackRight + m_pos;
        }
        
        Vec3 getTopBackRight_w() const{
            return m_tpBackRight + m_pos;
        }


        
    private:
        Vec3 m_btFrontLeft, m_tpBackRight, m_pos;
    };

}

#endif