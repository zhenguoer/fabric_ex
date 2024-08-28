/* Include the controller definition */
#include "footbot_diffusion.h"
/* Function definitions for XML parsing */
#include <argos3/core/utility/configuration/argos_configuration.h>
/* 2D vector definition */
#include <argos3/core/utility/math/vector2.h>
#include <argos3/plugins/robots/foot-bot/simulator/footbot_entity.h>
#include <argos3/core/utility/networking/tcp_socket.h>
#include <sstream>
#include <iostream>
#include <argos3/core/control_interface/ci_sensor.h>
#include <argos3/plugins/robots/generic/control_interface/ci_positioning_sensor.h>
#include <array>
#include <memory>
#include <stdexcept>
#include <stdio.h>
#include <argos3/core/simulator/entity/entity.h>
#include <argos3/plugins/simulator/entities/cylinder_entity.h>
#include <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_proximity_sensor.h>
#include <string>
#include <random>
#include <argos3/core/simulator/simulator.h>
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/simulator/entity/controllable_entity.h>
#include <argos3/core/utility/logging/argos_log.h>

#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/entity/floor_entity.h>
#include <thread>
#include <chrono>

CFootBotDiffusion::CFootBotDiffusion() : m_pcWheels(NULL),
                                         m_pcProximity(NULL),
                                         m_cAlpha(10.0f),
                                         m_fDelta(0.5f),
                                         output_sped6(0),
                                         initvelocity(0.0f),
                                         flag_ave(0),
                                         m_fWheelVelocity(0.0f),
                                         m_cGoStraightAngleRange(-ToRadians(m_cAlpha),
                                                                 ToRadians(m_cAlpha)) {}

/****************************************/
/****************************************/

void CFootBotDiffusion::Init(TConfigurationNode &t_node)
{

   m_pcWheels = GetActuator<CCI_DifferentialSteeringActuator>("differential_steering");
   m_pcProximity = GetSensor<CCI_FootBotProximitySensor>("footbot_proximity");
   m_pcPosSensor = GetSensor<CCI_PositioningSensor>("positioning");

   GetNodeAttributeOrDefault(t_node, "alpha", m_cAlpha, m_cAlpha);
   m_cGoStraightAngleRange.Set(-ToRadians(m_cAlpha), ToRadians(m_cAlpha));
   GetNodeAttributeOrDefault(t_node, "delta", m_fDelta, m_fDelta);
   GetNodeAttributeOrDefault(t_node, "velocity", m_fWheelVelocity, m_fWheelVelocity);
   //m_cSocket.Connect("localhost", 1234);
   output_sped6=0;
   initvelocity=0;
   flag_ave=0;
}

/****************************************/
/****************************************/

//执行bash代码的函数
std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}





void CFootBotDiffusion::ControlStep()
{
   //接近传感器的读数
   const CCI_FootBotProximitySensor::TReadings &tProxReads = m_pcProximity->GetReadings();
   const CVector3 &cPos = m_pcPosSensor->GetReading().Position;
 //  m_pcWheels->SetLinearVelocity(m_fWheelVelocity, m_fWheelVelocity);
   const CQuaternion &cQuaternion = m_pcPosSensor->GetReading().Orientation;
   CQuaternion chaQuaternion;
   //目标位置
   if (cPos.GetX() >= -1.0f && cPos.GetX() <= 0.1f && cPos.GetY() >= -1.0f && cPos.GetY() <= 0.1f && output_sped6!=2){
   chaQuaternion.BetweenTwoVectors(CVector3(-0.5,-0.5,0.0),cPos); 
CSpace& space = CSimulator::GetInstance().GetSpace();
CEntity* entity = &(space.GetEntity(GetId()));
CFootBotEntity* footBotEntity = dynamic_cast<CFootBotEntity*>(entity);
CEmbodiedEntity& embodiedEntity = footBotEntity->GetEmbodiedEntity();
embodiedEntity.MoveTo(cPos,chaQuaternion,false);
   }


  /*
    EPuck_Environment_Classification& cController =  dynamic_cast<EPuck_Environment_Classification&>(cEpuck.GetControllableEntity().GetController());
    EPuck_Environment_Classification::Opinion& opinion = cController.GetOpinion();
    EPuck_Environment_Classification::CollectedData& collectedData = cController.GetColData();


*/
   //CEmbodiedEntity* embodiedEntity = GetEmbodiedEntity();
//CEmbodiedEntity* embodiedEntity = dynamic_cast<CEmbodiedEntity*>(GetEntity("robot0"));
//CFootBotEntity* footBot = dynamic_cast<CFootBotEntity*>(GetRobot());
   /* Sum them together */
  // CVector2 cAccumulator;
  // for (size_t i = 0; i < tProxReads.size(); ++i)
   {
   //   cAccumulator += CVector2(tProxReads[i].Value, tProxReads[i].Angle);
/*
      if (tProxReads[i].Value > 0)
      {
         if (cPos.GetX() >= 0.25f && cPos.GetX() <= 0.75f && cPos.GetY() >= 0.25f && cPos.GetY() <= 0.75f) {
         std::string message = "|,Landmark 1," + std::to_string(cPos.GetX()) + "," + std::to_string(cPos.GetY()) + "," + GetId() + ",|";
         CByteArray byteArray(reinterpret_cast<const UInt8 *>(message.c_str()), message.size());
        // m_cSocket.SendMsg(byteArray);
         }
         else if (cPos.GetX() <= -0.25f && cPos.GetX() >= -0.75f && cPos.GetY() >= 0.25f && cPos.GetY() <= 0.75f) {
         std::string message = "|,Landmark 2," + std::to_string(cPos.GetX()) + "," + std::to_string(cPos.GetY()) + "," + GetId()+ ",|";
         CByteArray byteArray(reinterpret_cast<const UInt8 *>(message.c_str()), message.size());
         //m_cSocket.SendMsg(byteArray);
         }
      }*/
   }
   //cAccumulator /= tProxReads.size();
   /* If the angle of the vector is small enough and the closest obstacle
    * is far enough, continue going straight, otherwise curve a little
    */
  // CRadians cAngle = cAccumulator.Angle();



//初始数据
   int initNumber;
   int num;
//速度为0时，调用计算可靠性的函数，确定初始时派出的robot个数
   if (m_fWheelVelocity==0&& output_sped6==0){
      std::string output = exec("bash ../network/interface_ex.sh caculateReliability -peer robot0");
      initNumber = std::stoi(output);
      //std::cout << "Payload value: " << initNumber << std::endl;
   
//将getid获取的robotid转化为整型和initnumber比较，小于initnumber-1的robot就出发，随机生成速度值，getid获取到的robotid是0-8
//借助helper.h函数，将robot0变为整数0
   //std::cout << GetId()<< std::endl;
   std::string numStr = GetId().substr(5);
   num = std::stoi(numStr);
   //std::cout << numStr<< std::endl;
   if (num<initNumber){
      std::random_device rd;
      std::uniform_real_distribution<double> floatDistribution(1.0, 15.0);
      double randomFloat = floatDistribution(rd);
      m_fWheelVelocity=randomFloat;
      //上传速度
      std::string posx = std::to_string(cPos.GetX());
      std::string posy = std::to_string(cPos.GetY());
      std::string message="bash ../network/interface_ex.sh createAsset -peer "+ GetId() + " -posx " + posx + " -posy " + posy + " -speed " + std::to_string(randomFloat);
      std::string output_kong = exec(message.c_str());
      //std::cout << randomFloat << std::endl;
   }else
   {
      m_fWheelVelocity=0.0f;
   }
   }

 
   //获取平均速度
   if (output_sped6==0 && m_fWheelVelocity!=0 && std::stoi(GetId().substr(5))<initNumber && flag_ave==0 ){
      std::string output_sped6_string=(exec("bash ../network/interface_ex.sh queryRobotspeed -peer robot6"));
      if (output_sped6_string!="0"){
         output_sped6=1;
      }
      
   }else if(output_sped6==1 && std::stoi(GetId().substr(5))<initNumber && m_fWheelVelocity!=0 && flag_ave==0)
   {
      std::string output_ave = exec("bash ../network/interface_ex.sh queryAveRobotspeed -peer robot0");
      initvelocity= std::stof(output_ave);
      m_fWheelVelocity=initvelocity;
      flag_ave=1;
   }


   m_pcWheels->SetLinearVelocity(m_fWheelVelocity, m_fWheelVelocity);
   std::cout << GetId() << " Velocity is " << m_fWheelVelocity << std::endl;



const CCI_FootBotProximitySensor::TReadings &tProxReads1 = m_pcProximity->GetReadings();
CVector2 cAccumulator;
  for (size_t i = 0; i < tProxReads1.size(); ++i)
   {
      cAccumulator += CVector2(tProxReads1[i].Value, tProxReads1[i].Angle);
/*
      if (tProxReads[i].Value > 0)
      {
         if (cPos.GetX() >= 0.25f && cPos.GetX() <= 0.75f && cPos.GetY() >= 0.25f && cPos.GetY() <= 0.75f) {
         std::string message = "|,Landmark 1," + std::to_string(cPos.GetX()) + "," + std::to_string(cPos.GetY()) + "," + GetId() + ",|";
         CByteArray byteArray(reinterpret_cast<const UInt8 *>(message.c_str()), message.size());
        // m_cSocket.SendMsg(byteArray);
         }
         else if (cPos.GetX() <= -0.25f && cPos.GetX() >= -0.75f && cPos.GetY() >= 0.25f && cPos.GetY() <= 0.75f) {
         std::string message = "|,Landmark 2," + std::to_string(cPos.GetX()) + "," + std::to_string(cPos.GetY()) + "," + GetId()+ ",|";
         CByteArray byteArray(reinterpret_cast<const UInt8 *>(message.c_str()), message.size());
         //m_cSocket.SendMsg(byteArray);
         }
      }*/
   }
   cAccumulator /= tProxReads1.size();
   /* If the angle of the vector is small enough and the closest obstacle
    * is far enough, continue going straight, otherwise curve a little
    */
   CRadians cAngle = cAccumulator.Angle();
if (m_cGoStraightAngleRange.WithinMinBoundIncludedMaxBoundIncluded(cAngle) &&
       cAccumulator.Length() < m_fDelta && output_sped6!=2)
   {
      /* Go straight */
      m_pcWheels->SetLinearVelocity(m_fWheelVelocity, m_fWheelVelocity);
   }
   else if (output_sped6!=2 &&  (cAccumulator.Length() >= m_fDelta || !m_cGoStraightAngleRange.WithinMinBoundIncludedMaxBoundIncluded(cAngle) ))
   {
      /* Turn, depending on the sign of the angle */
     if (cAngle.GetValue() > 0.0f)
      {
         m_pcWheels->SetLinearVelocity(m_fWheelVelocity, 0.0f);
      }
      else
      {
         m_pcWheels->SetLinearVelocity(0.0f, m_fWheelVelocity);
      }
   }





//是否到达目标位置：
std::cout << GetId() << output_sped6  << std::endl;
if (cPos.GetX() >= -1.0f && cPos.GetX() <= 0.1f && cPos.GetY() >= -1.0f && cPos.GetY() <= 0.1f && output_sped6!=2 ) {
   m_fWheelVelocity=0;
   m_pcWheels->SetLinearVelocity(m_fWheelVelocity, m_fWheelVelocity);
   std::string posx = std::to_string(cPos.GetX());
   std::string posy = std::to_string(cPos.GetY());
   std::string message="bash ../network/interface_ex.sh createAsset -peer "+ GetId() + " -posx " + posx + " -posy " + posy + " -speed " + std::to_string(m_fWheelVelocity);
   std::string output_kong2 = exec(message.c_str());   
   output_sped6=2;
}else if(output_sped6!=2 && (cPos.GetX() < -1.0f || cPos.GetX() > 0.1f || cPos.GetY() < -1.0f || cPos.GetY() > 0.1f))
{
   const CCI_FootBotProximitySensor::TReadings &tProxReads2 = m_pcProximity->GetReadings();
   const CVector3 &cPos1 = m_pcPosSensor->GetReading().Position;
   const CQuaternion &cQuaternion1 = m_pcPosSensor->GetReading().Orientation;
   CQuaternion chaQuaternion1;
   //目标位置
   chaQuaternion1.BetweenTwoVectors(CVector3(-0.5,-0.5,0.0),cPos1);
   CSpace& space = CSimulator::GetInstance().GetSpace();
CEntity* entity = &(space.GetEntity(GetId()));
CFootBotEntity* footBotEntity = dynamic_cast<CFootBotEntity*>(entity);
CEmbodiedEntity& embodiedEntity = footBotEntity->GetEmbodiedEntity();
   
   embodiedEntity.MoveTo(cPos1,chaQuaternion1,false);
  // m_pcWheels->SetLinearVelocity(m_fWheelVelocity, m_fWheelVelocity);
   //m_pcWheels->SetLinearVelocity(m_fWheelVelocity, 0);
 //  embodiedEntity.MoveTo(cPos1,chaQuaternion1,false);
}


if (m_cGoStraightAngleRange.WithinMinBoundIncludedMaxBoundIncluded(cAngle) &&
       cAccumulator.Length() < m_fDelta && output_sped6!=2 && std::stoi(GetId().substr(5))<initNumber)
   {
      /* Go straight */
      m_pcWheels->SetLinearVelocity(m_fWheelVelocity, m_fWheelVelocity);
   }
/*   else
   {
      /* Turn, depending on the sign of the angle */
  /*   if (cAngle.GetValue() > 0.0f)
      {
         m_pcWheels->SetLinearVelocity(m_fWheelVelocity, 0.0f);
      }
      else
      {
         m_pcWheels->SetLinearVelocity(0.0f, m_fWheelVelocity);
      }
   }*/ 

float a=1;
if (output_sped6==2 )
{
   std::string output_check=exec("bash ../network/interface_ex.sh queryAveRobotspeed -peer robot0");
   a= std::stof(output_check);
   //std::cout << GetId() << "a=" << a  << std::endl;
}

//第二阶段的任务，要确定所有的robot都到达了第一个landmark的位置，通过查询速度来确定，在output_sped=2的情况下，此时自身robot已满足速度为0
//查询别的所有的robot是不是速度都为0，如果都为0的话，通过计算平均速度，如果平均速度为0，说明全部为0，确定到达了第一个landmark位置
if (output_sped6==2 && a==0 && std::stoi(GetId().substr(5))<initNumber && output_sped6!=3 )
{
   const CCI_FootBotProximitySensor::TReadings &tProxReads3 = m_pcProximity->GetReadings();
   const CVector3 &cPos2 = m_pcPosSensor->GetReading().Position;
   const CQuaternion &cQuaternion2 = m_pcPosSensor->GetReading().Orientation;
   CQuaternion chaQuaternion2;
   //目标位置
   chaQuaternion2.BetweenTwoVectors(CVector3(1.5,-0.5,0.0),cPos2);
   CSpace& space = CSimulator::GetInstance().GetSpace();
   CEntity* entity = &(space.GetEntity(GetId()));
   CFootBotEntity* footBotEntity = dynamic_cast<CFootBotEntity*>(entity);
   CEmbodiedEntity& embodiedEntity = footBotEntity->GetEmbodiedEntity();
   embodiedEntity.MoveTo(cPos2,chaQuaternion2,false);
   m_fWheelVelocity=initvelocity;
  // m_pcWheels->SetLinearVelocity(m_fWheelVelocity, m_fWheelVelocity);
   embodiedEntity.MoveTo(cPos2,chaQuaternion2,false);
   CVector2 cAccumulator1;
  for (size_t i = 0; i < tProxReads3.size(); ++i)
   {
      cAccumulator1 += CVector2(tProxReads3[i].Value, tProxReads3[i].Angle);
   }
   cAccumulator1 /= tProxReads3.size();
   /* If the angle of the vector is small enough and the closest obstacle
    * is far enough, continue going straight, otherwise curve a little
    */
   CRadians cAngle1 = cAccumulator1.Angle();
if (m_cGoStraightAngleRange.WithinMinBoundIncludedMaxBoundIncluded(cAngle1) &&
       cAccumulator1.Length() < m_fDelta && output_sped6!=3 && output_sped6==2)
   {
      /* Go straight */
      embodiedEntity.MoveTo(cPos2,chaQuaternion2,false);
      m_pcWheels->SetLinearVelocity(m_fWheelVelocity, m_fWheelVelocity);
   }
   else if (output_sped6!=3 &&  (cAccumulator1.Length() >= m_fDelta || !m_cGoStraightAngleRange.WithinMinBoundIncludedMaxBoundIncluded(cAngle1) ) && output_sped6==2 )
   {
      /* Turn, depending on the sign of the angle */
   if (cAngle1.GetValue() > 0.0f)
      {
         m_pcWheels->SetLinearVelocity(m_fWheelVelocity, 0.0f);
      }
     else
      {
         m_pcWheels->SetLinearVelocity(0.0f, m_fWheelVelocity);
      }
   } 
   //是否到达目标位置
   if (cPos2.GetX() >= 0.7f && cPos2.GetX() <= 2.1f && cPos2.GetY() >= -1.1f && cPos2.GetY() <=0.5f && output_sped6!=3 && output_sped6==2 ) {
   m_fWheelVelocity=0;
   m_pcWheels->SetLinearVelocity(m_fWheelVelocity, m_fWheelVelocity);
   output_sped6=3;
}else if(output_sped6!=3 && (cPos2.GetX() < 0.7f || cPos2.GetX() > 2.1f || cPos2.GetY() < -1.1f || cPos2.GetY() > 0.5f) && output_sped6==2 )
{
   const CCI_FootBotProximitySensor::TReadings &tProxReads4 = m_pcProximity->GetReadings();
   const CVector3 &cPos3 = m_pcPosSensor->GetReading().Position;
   const CQuaternion &cQuaternion3 = m_pcPosSensor->GetReading().Orientation;
   CQuaternion chaQuaternion3;
   //目标位置
   chaQuaternion3.BetweenTwoVectors(CVector3(-0.5,0.0,0.0),cPos3);
   CSpace& space = CSimulator::GetInstance().GetSpace();
CEntity* entity = &(space.GetEntity(GetId()));
CFootBotEntity* footBotEntity = dynamic_cast<CFootBotEntity*>(entity);
CEmbodiedEntity& embodiedEntity = footBotEntity->GetEmbodiedEntity();
   
   embodiedEntity.MoveTo(cPos3,chaQuaternion3,false);
  // m_pcWheels->SetLinearVelocity(m_fWheelVelocity, m_fWheelVelocity);
   //m_pcWheels->SetLinearVelocity(m_fWheelVelocity, 0);
 //  embodiedEntity.MoveTo(cPos1,chaQuaternion1,false);
 CVector2 cAccumulator2;
  for (size_t i = 0; i < tProxReads4.size(); ++i)
   {
      cAccumulator2 += CVector2(tProxReads4[i].Value, tProxReads3[i].Angle);
   }
   cAccumulator2 /= tProxReads4.size();
   /* If the angle of the vector is small enough and the closest obstacle
    * is far enough, continue going straight, otherwise curve a little
    */
   CRadians cAngle2 = cAccumulator2.Angle();
if (m_cGoStraightAngleRange.WithinMinBoundIncludedMaxBoundIncluded(cAngle2) &&
       cAccumulator2.Length() < m_fDelta && output_sped6!=3 && std::stoi(GetId().substr(5))<initNumber && output_sped6==2 ) 
   {
      /* Go straight */
      embodiedEntity.MoveTo(cPos3,chaQuaternion3,false);
      m_pcWheels->SetLinearVelocity(m_fWheelVelocity, m_fWheelVelocity);
   }


}
}





   //if (desiredAngle.GetValue() > 0.0f || desiredAngle.GetValue() < 0.0f)
   //   {
   //      Real fAngularVelocity = desiredAngle.GetValue() * m_fWheelVelocity;
   //      m_pcWheels->SetLinearVelocity(m_fWheelVelocity, -fAngularVelocity); 
   //   }else
   //   {
    //     m_pcWheels->SetLinearVelocity(m_fWheelVelocity, m_fWheelVelocity);
   //   }
   //std::string output_sped0=exec("bash ../network/interface_ex.sh queryRobotspeed -peer robot0");
   //std::string output_sped1=exec("bash ../network/interface_ex.sh queryRobotspeed -peer robot1");
   //std::string output_sped2=exec("bash ../network/interface_ex.sh queryRobotspeed -peer robot2");
   //std::string output_sped3=exec("bash ../network/interface_ex.sh queryRobotspeed -peer robot3");
   //std::string output_sped4=exec("bash ../network/interface_ex.sh queryRobotspeed -peer robot4");
   //std::string output_sped5=exec("bash ../network/interface_ex.sh queryRobotspeed -peer robot5");
   //std::string output_sped6=exec("bash ../network/interface_ex.sh queryRobotspeed -peer robot6");
   /*  if (m_cGoStraightAngleRange.WithinMinBoundIncludedMaxBoundIncluded(cAngle) &&
       cAccumulator.Length() < m_fDelta)
   {
      /* Go straight */
  /*    m_pcWheels->SetLinearVelocity(m_fWheelVelocity, m_fWheelVelocity);
   }
   else
   {
      /* Turn, depending on the sign of the angle */
  /*   if (cAngle.GetValue() > 0.0f)
      {
         m_pcWheels->SetLinearVelocity(m_fWheelVelocity, 0.0f);
      }
      else
      {
         m_pcWheels->SetLinearVelocity(0.0f, m_fWheelVelocity);
      }
   }*/ 
}

REGISTER_CONTROLLER(CFootBotDiffusion, "footbot_diffusion_controller")
