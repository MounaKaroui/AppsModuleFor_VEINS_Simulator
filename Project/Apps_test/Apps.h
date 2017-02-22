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

#ifndef __PROJECT_APPS_H_
#define __PROJECT_APPS_H_

#include <omnetpp.h>
//#include "CAR_Apps.h"
/**
 * TODO - Generated class
 */
#include "veins/modules/application/ieee80211p/BaseWaveApplLayer.h"
#include "veins/modules/world/annotations/AnnotationManager.h"
#include "veins/modules/utility/Consts80211p.h"

using Veins::AnnotationManager;
class Apps : public  BaseWaveApplLayer {

public:
        double wsaInterval;

        cOutVector endToendDelay_BSM;
        cOutVector EndtoEndHW;
        long  numSentWI;
        cOutVector numSentWIVector;

        long  numSentPI;
        long  numWSA;
        long  numSentDSL;
        cOutVector numSentDSLVector;

        long numsentHWV2I;
        cOutVector numReceiveHWVector;
        cOutVector  numSentPIVector;

        cMessage* sendDataWIEvt;
        cMessage* sendDataDSLEvt;
        cMessage* sendDataPIEvt;
        cMessage* sendDataHWEvt;
        cMessage* forwardMsgHW;
        float carToTMC_dalay;
        cOutVector carToTMCDelayHWVec;


        enum WaveApplMessageKinds {
                    SERVICE_PROVIDER = LAST_BASE_APPL_MESSAGE_KIND,
                    SEND_BEACON_EVT,
                    SEND_DATA_WI_EVT,
                    SEND_DATA_DSL_EVT,
                    SEND_DATA_PI_EVT,
                    ForwardHW_EVT
                };
        virtual void forwardHW_Msg(WaveShortMessage* wsm);
        virtual void initialize(int stage);
        virtual void sendDownApps(cMessage* msg);
        int baseId;
        cGate *gate_out;
        cGate* gateIn;
        long size;
        int inApps[];
        int outApps;



    protected:
        bool sentMessage;
        virtual void finish();
        virtual void handleSelfMsg(cMessage* msg);
        virtual void onWSA(WaveServiceAdvertisment* wsa);
        virtual void onBSM(BasicSafetyMessage* wsm);
        virtual void onWSM(WaveShortMessage* wsm);
        virtual void handleMessage(cMessage* msg);
};

#endif /* APPS_H_ */
