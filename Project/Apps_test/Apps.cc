#include "Apps.h"
using Veins::AnnotationManagerAccess;
Define_Module(Apps);

void Apps::initialize(int stage) {
    BaseWaveApplLayer::initialize(stage);
    if (stage == 0) {
        // delaySignalHW=registerSignal("delayHW_TMC") ;
        baseId = gateBaseId("outApps");
        size = gateSize("outApps");


        sendDataWIEvt = new cMessage("Weather info msg", SEND_DATA_WI_EVT);
        sendDataDSLEvt = new cMessage("DynamicSpeed msg", SEND_DATA_DSL_EVT);
        sendDataPIEvt= new cMessage("Point of interest msg", SEND_DATA_PI_EVT);

        scheduleAt(simTime() + uniform(0,0.0003),sendDataWIEvt);
        scheduleAt(simTime() + uniform(0,0.0003),sendDataDSLEvt);
        scheduleAt(simTime() + uniform(0,0.0003),sendDataPIEvt);

        numSentWI=0;
        numSentDSL=0;
        numSentPI=0;
        numsentHWV2I=0;
        endToendDelay_BSM.setName("E2E_BSM");
        wsaInterval=par("wsaInterval").doubleValue();

        }}

void Apps::onBSM(BasicSafetyMessage* bsm)
{

}

void Apps::onWSM(WaveShortMessage* wsm) {
     DBG_APP<< "Received a data from on data Apps" <<  wsm->getWsmData()<<std::endl;
     bubble("Yes I am here");
     forwardHW_Msg(wsm);
}

void Apps::onWSA(WaveServiceAdvertisment* wsa)
{


}

void Apps :: sendDownApps(cMessage* msg){
    recordPacket(PassedMessage::OUTGOING,PassedMessage::LOWER_DATA,msg);
    for (int i = 0; i < size; i++) {
        gate_out = gate(baseId + i);
        send(msg, gate_out);
    }}

void Apps::handleSelfMsg(cMessage* msg){
    switch (msg->getKind()) {
          dataLengthBits=1000;
          case SEND_DATA_WI_EVT: {
                       dataPriority=1;
                       for (int i = 0; i < size; i++) {
                           gate_out = gate(baseId + i);
                           ///// WSA WI sending
                           WaveServiceAdvertisment* wsa_WI=new WaveServiceAdvertisment();
                           wsa_WI->setServiceDescription("WI service description");
                           populateWSM(wsa_WI);
                           wsa_WI->setPsid(40);
                           sendDelayed(wsa_WI,  par("wsaInterval").doubleValue(), gate_out);

                           ///// WSM WI sending
                           WaveShortMessage* wsm=new WaveShortMessage();
                           populateWSM(wsm);
                           wsm->setWsmData("WeatherInfoData");
                           wsm->setPriority(dataPriority);
                           wsm->setChannelNumber(Channels::CCH);
                           sendDelayed(wsm,  par("individualOffset").doubleValue(), gate_out);
                               }
                       numSentWI++;
                       scheduleAt(simTime() + par("dataWIInterval").doubleValue(), sendDataWIEvt);

                       break;
                            }
          case SEND_DATA_PI_EVT: {
                                            dataPriority=0;
                                            for (int i = 0; i < size; i++) {
                                                gate_out = gate(baseId + i);
                                                //// WSA PI sending
                                                WaveServiceAdvertisment* wsa_PI=new WaveServiceAdvertisment();
                                                wsa_PI->setServiceDescription("PI_service description");
                                                populateWSM(wsa_PI);
                                                wsa_PI->setPsid(41);
                                                sendDelayed(wsa_PI,  par("wsaInterval").doubleValue(), gate_out);

                                                //// WSM PI sending
                                                WaveShortMessage* wsm=new WaveShortMessage();
                                                populateWSM(wsm);
                                                wsm->setPriority(dataPriority);
                                                wsm->setWsmData("PointOfInterestData");
                                                sendDelayed(wsm, par("individualOffset").doubleValue(), gate_out);
                                                    }
                                            numSentPI++;
                                            scheduleAt(simTime() + par("dataPIInterval").doubleValue(), sendDataPIEvt);

                                            break;
                                      }
          case SEND_DATA_DSL_EVT: {
                                 dataPriority=2;
                                 for (int i = 0; i < size; i++) {
                                                  gate_out = gate(baseId + i);
                                                  //// WSM DSL sending
                                                  WaveServiceAdvertisment* wsa_DSL=new WaveServiceAdvertisment();
                                                  populateWSM(wsa_DSL);
                                                  wsa_DSL->setPsid(42);
                                                  wsa_DSL->setServiceDescription("DSL_serviceDescription");
                                                  sendDelayed(wsa_DSL, par("wsaInterval").doubleValue(), gate_out);
                                                  //// WSM DSL sending
                                                  WaveShortMessage* wsm=new WaveShortMessage();
                                                  populateWSM(wsm);
                                                  wsm->setWsmData("DynamicSpeedLimitData");
                                                  wsm->setPriority(dataPriority);
                                                  sendDelayed(wsm,par("individualOffset").doubleValue(), gate_out);
                                 }
                                              numSentDSL++;
                                              scheduleAt(simTime() + par("dataDSLInterval").doubleValue(), sendDataDSLEvt);

                                              break;
             }
           default: {
               if (msg)
               DBG_APP << "APP: Error: Got Self Message of unknown kind! Name: " << msg->getName() << endl;
               break;
           }}
}

void Apps::forwardHW_Msg(WaveShortMessage* wsm)
{
         // Forwarding WSM HW
         WaveShortMessage* wsm1;
         if((wsm->getSenderAddress()==11)&&(std::string(wsm->getWsmData()) == "HazardWarningData")){
             DBG_APP<< "copy wsm with dup function to wsm1  " <<  wsm->getWsmData()<<std::endl;
             wsm1=(WaveShortMessage*)wsm->dup();
             wsm1->setSenderAddress(12);
         }
         int k = intuniform(0, size-1);
         gate_out = gate(baseId);
         sendDelayed(wsm1, 0.05,"outApps", k);
         numsentHWV2I++;
         DBG_APP<< "Forwarding message " << wsm1->getWsmData() << " " << wsm1->getSenderAddress() << " on port out[" << k << "]\n";
         delete wsm;
         // WSA sending HW
         WaveServiceAdvertisment* wsa_V2I=new WaveServiceAdvertisment();
         populateWSM(wsa_V2I);
         wsa_V2I->setPsid(43);
         wsa_V2I->setServiceDescription("WSA V2I service description");
         sendDelayed(wsa_V2I,par("wsaInterval").doubleValue(),"outApps", k);

}

void Apps:: finish()
{
   recordScalar("numSentDSL",numSentDSL);
   recordScalar("numSentWI",numSentWI);
   recordScalar("numSentPI",numSentPI);
   recordScalar("numSentHWV2I",numsentHWV2I);
}

void Apps::handleMessage(cMessage* msg){
    if (msg->isSelfMessage()){
          handleSelfMsg(msg);
      } else if(msg->getArrivalGateId()==upperLayerIn) {
          recordPacket(PassedMessage::INCOMING,PassedMessage::UPPER_DATA,msg);
          handleUpperMsg(msg);
      } else if(msg->getArrivalGateId()==upperControlIn) {
          recordPacket(PassedMessage::INCOMING,PassedMessage::UPPER_CONTROL,msg);
          handleUpperControl(msg);
      } else if(msg->getArrivalGateId()==lowerControlIn){
          recordPacket(PassedMessage::INCOMING,PassedMessage::LOWER_CONTROL,msg);
          handleLowerControl(msg);
      } else if(msg->getArrivalGateId()==lowerLayerIn) {
          recordPacket(PassedMessage::INCOMING,PassedMessage::LOWER_DATA,msg);
          handleLowerMsg(msg);
      }
      else if(msg->getArrivalGateId()==-1) {
          /* Classes extending this class may not use all the gates, f.e.
           * BaseApplLayer has no upper gates. In this case all upper gate-
           * handles are initialized to -1. When getArrivalGateId() equals -1,
           * it would be wrong to forward the message to one of these gates,
           * as they actually don't exist, so raise an error instead.
           */
      } else {
          /* msg->getArrivalGateId() should be valid, but it isn't recognized
           * here. This could signal the case that this class is extended
           * with extra gates, but handleMessage() isn't overridden to
           * check for the new gate(s).
           */
            recordPacket(PassedMessage::INCOMING,PassedMessage::LOWER_DATA,msg);
            handleLowerMsg(msg);
}}
