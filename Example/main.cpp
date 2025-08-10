#include <cstdio>
#include <cstdint>
#include <cstring>
#include "TagStatic.h"  // Ensure header is patched as discussed earlier

TopicGroup BMS; 

int main()
{
   auto *ROOM_DATA_CENTER = BMS.Add("ROOM_DATA_CENTER");
   ROOM_DATA_CENTER->Items()->Add(isType::VAR_FLOAT,"Temperature",3);
   ROOM_DATA_CENTER->Items()->Add(isType::VAR_FLOAT,"Humidity",5);
   ROOM_DATA_CENTER->Items()->Add(isType::VAR_FLOAT,"Temperature2");
   ROOM_DATA_CENTER->Items()->Add(isType::VAR_FLOAT,"Humidity2");

   auto *ROOM_AHU1 = BMS.Add("ROOM_AHU1");
   ROOM_AHU1->Items()->Add(isType::VAR_FLOAT,"Temperature_Supply");
   ROOM_AHU1->Items()->Add(isType::VAR_FLOAT,"Temperature_Return");
   ROOM_AHU1->Items()->Add(isType::VAR_FLOAT,"Humidity_Supply");
   ROOM_AHU1->Items()->Add(isType::VAR_INT8,"Operation");

   auto *Chiller = BMS.Add("Chiller1");
   Chiller->Items()->Add(isType::VAR_INT8,"Evap_Inlet_Temperature");
   Chiller->Items()->Add(isType::VAR_INT8,"Evap_Output_Temperature");
   Chiller->Items()->Add(isType::VAR_FLOAT,"Evap_FlowSwitch");
   Chiller->Items()->Add(isType::VAR_FLOAT,"Cond_FlowSwitch");
   
   auto x = BMS.Count();
   for(int n = 0 ; n < x ; n++)
   {
      printf("%s",BMS.Find(n)->MonitorInfo());
      auto item = BMS.Find(n)->Items()->Count();
      for(int o = 0 ; o < item ; o++)
      {
         printf("%s",BMS.Find(n)->Items()->Find(o)->MonitorInfo());

         auto array = BMS.Find(n)->Items()->Find(o)->GetArraySize();
         for(int p = 0 ; p < array ;p++)
           printf("%s",BMS.Find(n)->Items()->Find(o)->MonitorValue(p));
      }
   }
   
   return 0 ;
}