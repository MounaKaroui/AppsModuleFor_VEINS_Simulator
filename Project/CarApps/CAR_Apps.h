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
#ifndef __PROJECT_CAR_APPS_H_
#define __PROJECT_CAR_APPS_H_
#include <omnetpp.h>
#include "veins/modules/application/traci/TraCIDemo11p.h"
#include "veins/modules/mobility/traci/TraCIMobility.h"
#include "veins/modules/mobility/traci/TraCICommandInterface.h"
#include "veins/modules/application/ieee80211p/BaseWaveApplLayer.h"
#include"Apps.h"
using Veins::TraCIMobility;
using Veins::TraCICommandInterface;
using Veins::AnnotationManager;
/**
 * TODO - Generated class
 */
class CAR_Apps : public BaseWaveApplLayer
{
public:

    cOutVector endToendDelay_bsm;
    cOutVector endToendDelay_wsa;
    cOutVector endToEndDelayHWVec;
    cOutVector endToEndDelayWIVec;
    cOutVector endToEndDelayDSLVec;
    cOutVector endToEndDelayPIVec;
    cOutVector carTocar_E2E_HW;

    cOutVector  E2E_wsaWI;
    cOutVector  E2E_wsaPI;
    cOutVector  E2E_wsaDSL;
    cOutVector  E2E_wsaHW_V2I;
    cOutVector  E2E_wsaHW_V2V;

    bool      sendData;
    double    startTime;
    double    totalBitsPI;
    double    totalBitsDSL;
    double    totalBitsWI;
    double    totalBitsHW_V2V;
    double    totalBitsHW_V2I;
    double    totalBitsBSM;

    double    avgThroughput_HW_V2I;
    double    avgThroughput_HW_V2V;
    double    avgThroughput_DSL;
    double    avgThroughput_WI;
    double    avgThroughput_PI;
    double    avgThroughtput_BSM;

    float delayHWV2I;
    long numSentHWV2V=0;
    cOutVector numSentHWVector;
    long numReceiveWI=0;
    cOutVector numReceiveWIVector;
    long numReceiveDSL=0;
    cOutVector numReceiveDSLVector;

    long numReceivePI=0;
    cOutVector numReceivePIVector;

    long numReceiveHW_V2I=0;
    long numReceiveHW_V2V=0;
    cOutVector numReceiveHWVector;


    enum WaveApplMessageKinds {
                SERVICE_PROVIDER = LAST_BASE_APPL_MESSAGE_KIND,
                SEND_BEACON_EVT,
                SEND_DATA_HW_EVT,
                        };
    cMessage*HWEvt;
    bool getSentMessageHW();
    void SendHW_CCH(WaveShortMessage* wsm);
    void improveHW(WaveShortMessage* wsm);
  protected:
    int currentSubscribedServiceId;
    TraCIMobility* mobility;
    TraCICommandInterface* traci;
    TraCICommandInterface::Vehicle* traciVehicle;
    AnnotationManager* annotations;
    virtual void initialize(int stage);
    virtual void finish();
    virtual void onWSA(WaveServiceAdvertisment*  wsa);
    virtual void onBSM(BasicSafetyMessage* bsm);
    virtual void sendWSM(WaveShortMessage* wsm);
    virtual void handleSelfMsg(cMessage* msg);
    virtual void onWSM(WaveShortMessage* msg);
};
#endif
