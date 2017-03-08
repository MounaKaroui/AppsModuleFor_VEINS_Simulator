//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package project.Apps_test;

//
// TODO auto-generated module
//
import org.car2x.veins.modules.application.ieee80211p.BaseWaveApplLayer;
simple Apps  extends BaseWaveApplLayer
{
     parameters:
     volatile double dataWIInterval = default(0.095s) @unit(s);  
     volatile double dataDSLInterval = default(0.095s) @unit(s);
     volatile double dataPIInterval = default(0.095s) @unit(s);
     volatile double individualOffset=default(0.005s) @unit(s);
   
     @class(Apps);
     int size;
     
     gates:
     output outApps[size];
     input inApps[size];
}


