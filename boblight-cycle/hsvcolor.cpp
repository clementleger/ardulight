/*
 * boblight
 * Copyright (C) Clemvangelis  2011
 * 
 * boblight is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * boblight is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <cmath>
#include <iostream> 

#include "hsvcolor.h"

using namespace std;

HSVColor::HSVColor(float hue, float saturation, float value) :
	m_hue (hue), m_saturation (saturation), m_value (value)
{}

HSVColor::HSVColor() :
	m_hue (0), m_saturation (1), m_value (1)
{}


void HSVColor::ToRGB(int *color)
{
    float f,p,q,t, hueRound;
    int hueIndex; 
    float r, g, b;

    hueIndex = int(floor(m_hue / 60)) % 6;
    
    f = (m_hue / 60) - hueIndex;
    p = m_value * (1.0 - m_saturation);
    q = m_value * (1.0 - f * m_saturation);
    t = m_value * (1.0 - (1.0 - f) * m_saturation);

    switch(hueIndex)
    {
        case 0:
            r = m_value; g = t; b = p;
        break;
        case 1:
            r = q; g = m_value; b = p;
        break;
        case 2:
            r = p; g = m_value; b = t;
        break;
        case 3:
            r = p; g = q; b = m_value;
        break;
        case 4:
            r = t; g = p; b = m_value;
        break;
        case 5:
            r = m_value; g = p; b = q;
        break;
    }
    color[0] = 255.0 * r;
    color[1] = 255.0 * g;
    color[2] = 255.0 * b;
}


void HSVColor::NextHue(){
	m_hue = int(m_hue+1) % 360;
}

