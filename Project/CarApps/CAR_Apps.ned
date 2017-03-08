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

package project.CarApps;

//
// TODO auto-generated module
//
import org.car2x.veins.modules.application.ieee80211p.BaseWaveApplLayer;
import org.car2x.veins.modules.application.traci.TraCIDemo11p;

simple CAR_Apps extends BaseWaveApplLayer
{
    parameters :
    volatile double dataHWInterval = default(0.095s) @unit(s);  
    volatile double individualOffset=default(0.005s) @unit(s);
    
    @class(CAR_Apps);
    @display("i=block/app2");
  


}
