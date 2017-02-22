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

package project;
import org.car2x.veins.base.modules.BaseMobility;
import org.car2x.veins.modules.nic.INic80211p;


//
// TODO documentation
//

module RSU
{
    parameters:

        @display("bgb=169,226");
        string type = default("Nic80211p");
    gates:
        input in;
        output out;

    submodules:

        nic: <type> like INic80211p {
            @display("p=80,140");
        }
        mobility: BaseMobility {
            @display("p=29,48");
        }
    connections allowunconnected:

        nic.upperLayerIn <-- in;
        nic.upperLayerOut --> out;
}



