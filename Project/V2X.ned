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

import org.car2x.veins.base.connectionManager.ConnectionManager;
import org.car2x.veins.base.modules.BaseWorldUtility;
import org.car2x.veins.base.modules.IBaseApplLayer;
import org.car2x.veins.modules.mobility.traci.TraCIScenarioManagerLaunchd;
import org.car2x.veins.modules.world.annotations.AnnotationManager;
import org.car2x.veins.nodes.Scenario;
import project.Apps_test.Apps;


network V2X
{
    parameters:
        double playgroundSizeX @unit(m); // x size of the area the nodes are in (in meters)
        double playgroundSizeY @unit(m); // y size of the area the nodes are in (in meters)
        double playgroundSizeZ @unit(m); // z size of the area the nodes are in (in meters)
        int numRsu = default(0);
        @display("bgb=646,415");
    submodules:
        TMC: Apps {
            @display("p=252,146;i=device/server2;is=l");
        }
        rsu[numRsu]: RSU {
            @display("i=device/antennatower;p=252,330");
        }
        world: BaseWorldUtility {
            parameters:
                playgroundSizeX = playgroundSizeX;
                playgroundSizeY = playgroundSizeY;
                playgroundSizeZ = playgroundSizeZ;
                @display("p=553,223;i=misc/globe");
        }
        manager: TraCIScenarioManagerLaunchd {
            @display("p=552,303");
        }
        connectionManager: ConnectionManager {
            @display("p=542,125");
        }
        annotations: AnnotationManager {
            @display("p=47,273");
        }
    connections allowunconnected:
        //        TMC.lowerLayerOut --> rsu[0].in;
        //        rsu[0].out --> TMC.lowerLayerIn;
        for i=0..TMC.size-1 {
            TMC.outApps[i] --> rsu[i].in;
            TMC.inApps[i] <-- rsu[i].out;
        }

}
