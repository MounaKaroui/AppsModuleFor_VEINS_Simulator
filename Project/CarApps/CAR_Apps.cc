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
#include "CAR_Apps.h"
using Veins::TraCIMobilityAccess;
using Veins::AnnotationManagerAccess;

Define_Module(CAR_Apps);
void CAR_Apps::initialize(int stage)
{
   BaseWaveApplLayer::initialize(stage);
   if (stage == 0) {
           mobility = TraCIMobilityAccess().get(getParentModule());
           traci = mobility->getCommandInterface();
           traciVehicle = mobility->getVehicleCommandInterface();
           annotations = AnnotationManagerAccess().getIfExists();
           ASSERT(annotations);
           //schedule for first sending
           HWEvt = new cMessage("Hazard warning", SEND_DATA_HW_EVT);
           scheduleAt(simTime() + uniform(0,0.0003), HWEvt);

           sendWSA=par("sendWSA").boolValue();


           if(sendWSA){
               scheduleAt(simTime()+wsaInterval,sendWSAEvt);
           }
           carTocar_E2E_HW.setName("E2E_carTocar_HW");
           startTime=simTime().dbl();

           totalBitsDSL=0;
           totalBitsPI=0;
           totalBitsWI=0;
           totalBitsHW_V2I=0;
           totalBitsHW_V2V=0;

           numSentHWV2V=0;
           numReceiveDSL=0;
           numReceivePI=0;
           numReceiveWI=0;
           numReceiveHW_V2I=0;
           numReceiveHW_V2V=0;
           //// end to end delay WSM
           endToendDelay_bsm.setName("E2E_delayBSM");
           endToEndDelayHWVec.setName("E2E_HWDelay");
           endToEndDelayWIVec.setName("E2E_WIDelay");
           endToEndDelayDSLVec.setName("E2E_DSLDelay");
           endToEndDelayPIVec.setName("E2E_PIDelay");
           //// end to end delay WSA
           E2E_wsaWI.setName("E2E_delay _WSA_WI");
           E2E_wsaPI.setName("E2E_delay_WSA_PI");
           E2E_wsaDSL.setName("E2E_delay_WSA_DSL");
           E2E_wsaHW_V2I.setName("E2E_delay_WSAHW_V2I");
           E2E_wsaHW_V2V.setName("E2E_delay_WSAHW_V2V");
           //// average throughput
           avgThroughput_HW_V2I=0;
           avgThroughput_HW_V2V=0;
           avgThroughput_DSL=0;
           avgThroughput_PI=0;
           avgThroughput_WI=0;
   }
}

void CAR_Apps::finish(){

    if(HWEvt->isScheduled()){
        cancelAndDelete(HWEvt);
    }
    else{
     delete HWEvt;
    }
    recordScalar("numReceiveDSL",numReceiveDSL);
    recordScalar("numReceiveWI",numReceiveWI);
    recordScalar("numReceivePI",numReceivePI);
    recordScalar("numSentHW",numSentHWV2V);
    recordScalar("numReceiveHW_V2I", numReceiveHW_V2I);
    recordScalar("numReceiveHW_V2V", numReceiveHW_V2V);

    double t=0;
    t=(simTime()-startTime).dbl();

    avgThroughput_HW_V2I=totalBitsHW_V2I/t;
    avgThroughput_HW_V2V=totalBitsHW_V2V/t;
    avgThroughput_DSL=totalBitsDSL/t;
    avgThroughput_PI=totalBitsPI/t;
    avgThroughput_WI=totalBitsWI/t;

    recordScalar("Average throughput_HW_V2V", avgThroughput_HW_V2V);
    recordScalar("Average throughput_HW_V2I", avgThroughput_HW_V2I);
    recordScalar("Average throughput_DSL", avgThroughput_DSL);
    recordScalar("Average throughput_WI", avgThroughput_WI);
    recordScalar("Average throughput_PI", avgThroughput_PI);
}

void CAR_Apps::onBSM(BasicSafetyMessage* bsm){
    if(std::string(bsm->getName()) == "BSM"){
    endToendDelay_bsm.record((simTime()-bsm->getTimestamp()).dbl());
    }}


void CAR_Apps::onWSA(WaveServiceAdvertisment*  wsa){

//    if (currentSubscribedServiceId == -1) {
//           mac->changeServiceChannel(wsa->getTargetChannel());
//           currentSubscribedServiceId = wsa->getPsid();
//           if  (currentOfferedServiceId != wsa->getPsid())
//           {
//               stopService();
//               startService((Channels::ChannelNumber) wsa->getTargetChannel(), wsa->getPsid(), "Mirrored Traffic Service");
//           }
//       }

    switch(wsa->getPsid()){
    case 40 :
        E2E_wsaWI.record((simTime()-wsa->getTimestamp()).dbl());
        break;
    case 41 :
        E2E_wsaPI.record((simTime()-wsa->getTimestamp()).dbl());
        break;
    case 42 :
        E2E_wsaDSL.record((simTime()-wsa->getTimestamp()).dbl());
         break;
    case 43 :
        E2E_wsaHW_V2I.record((simTime()-wsa->getTimestamp()).dbl());
        break;
    case 44 :
        E2E_wsaHW_V2V.record((simTime()-wsa->getTimestamp()).dbl());
        break;
    }

}

void CAR_Apps::sendWSM(WaveShortMessage* wsm) {
    DBG_APP << "Sending msg wsm" << wsm->getSenderAddress() <<  " " << wsm->getWsmData() <<std::endl;
    sendDelayedDown(wsm,par("individualOffset").doubleValue());
}

void  CAR_Apps::improveHW(WaveShortMessage* wsm){
    uint64_t currenTime = simTime().raw();
    uint64_t switchingTime = SWITCHING_INTERVAL_11P.raw();
 if((currenTime / switchingTime) % 2 == 0){
     sendWSM(wsm);
     scheduleAt(simTime() + par("dataHWInterval").doubleValue(), HWEvt);
 }
 else{
     scheduleAt(simTime() + par("dataHWInterval").doubleValue()+0.05, HWEvt);
 }
}

void CAR_Apps::handleSelfMsg(cMessage* msg) {
    switch (msg->getKind()) {
        case SEND_DATA_HW_EVT:{
            dataPriority=3;
            WaveServiceAdvertisment* wsa_HWV2V=new WaveServiceAdvertisment();
            populateWSM(wsa_HWV2V);
            wsa_HWV2V->setPsid(44);
            sendDelayedDown(wsa_HWV2V,par("wsaInterval").doubleValue());

            //sendDown(wsa_HWV2V);
            WaveShortMessage* wsm=new WaveShortMessage();

            populateWSM(wsm);
            wsm->setPriority(dataPriority);
            wsm->setWsmData("HazardWarningData");
            wsm->setSenderAddress(11);
            wsm->setChannelNumber(Channels::CCH);
            sendWSM(wsm);
            DBG_APP<< "sending HW messages  from CAR"<<endl;
            scheduleAt(simTime() + par("dataHWInterval").doubleValue(), HWEvt);
            numSentHWV2V++;
            break;
        }
        default: {
            if (msg)
              DBG_APP << "APP: Error: Got Self Message of unknown kind! Name: " << msg->getName() << endl;
            break;
        }}
       BaseWaveApplLayer::handleSelfMsg(msg);

}

void CAR_Apps::onWSM(WaveShortMessage* wsm) {

        findHost()->getDisplayString().updateWith("r=16,green");
        if((std::string(wsm->getWsmData()) == "HazardWarningData")&& wsm->getSenderAddress()==11){
        carTocar_E2E_HW.record((simTime()-wsm->getTimestamp()).dbl());
                       numReceiveHW_V2V++;
                       totalBitsHW_V2V+=wsm->getBitLength();
                           }
         if((std::string(wsm->getWsmData()) == "HazardWarningData")&& wsm->getSenderAddress()==12) {
                        endToEndDelayHWVec.record((simTime()-wsm->getTimestamp()).dbl());
                        numReceiveHW_V2I++;
                        totalBitsHW_V2I+=wsm->getBitLength();
                        }
          if((std::string(wsm->getWsmData()) == "WeatherInfoData")){
                         endToEndDelayWIVec.record((simTime()-wsm->getTimestamp()).dbl());
                         numReceiveWI++;
                         totalBitsWI+=wsm->getBitLength();
                        }
          if((std::string(wsm->getWsmData()) == "PointOfInterestData")){
                          endToEndDelayPIVec.record((simTime()-wsm->getTimestamp()).dbl());
                          numReceivePI++;
                          totalBitsPI+=wsm->getBitLength();
                          }
          if((std::string(wsm->getWsmData()) == "DynamicSpeedLimitData")){
                          endToEndDelayDSLVec.record((simTime()-wsm->getTimestamp()).dbl());
                          numReceiveDSL++;
                          totalBitsDSL+=wsm->getBitLength();

                          }}




