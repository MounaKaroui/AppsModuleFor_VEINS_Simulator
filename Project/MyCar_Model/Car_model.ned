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

package project.MyCar_Model;
import org.car2x.veins.base.modules.*;
import org.car2x.veins.modules.nic.INic80211p;
import org.car2x.veins.modules.nic.Nic80211p;
//
// TODO auto-generated module
//

//
// TODO documentation
//
module Car_model
{
    parameters:
        string applType; //type of the application layer
        string nicType = default("Nic80211p"); // type of network interface card
        string veinsmobilityType; //type of the mobility module
    gates:
        input veinsradioIn; // gate for sendDirect
    submodules:
        appl: <applType> like org.car2x.veins.base.modules.IBaseApplLayer {
            parameters:
                @display("p=60,50");
        }

        nic: <nicType> like org.car2x.veins.modules.nic.INic80211p {
            parameters:
                @display("p=60,166");
        }

        veinsmobility: <veinsmobilityType> like org.car2x.veins.base.modules.IMobility {
            parameters:
                @display("p=192,166;i=block/cogwheel");
        }

    connections allowunconnected:
        nic.upperLayerOut --> appl.lowerLayerIn;
        nic.upperLayerIn <-- appl.lowerLayerOut;

        // nic_sch.upperLayerOut --> appl.lowerLayerIn;
        // nic_sch.upperLayerIn <-- appl.lowerLayerOut;
        veinsradioIn --> nic.radioIn;
}


