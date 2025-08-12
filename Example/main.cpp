#include <cstdio>
#include <cstdint>
#include <cstring>
#include "TagStatic.h"  // Ensure header is patched as discussed earlier

TopicGroup BMS; 

int main()
{
   auto *ROOM_DATA_CENTER = BMS.Add("ROOM_DATA_CENTER");
   ROOM_DATA_CENTER->Items()->Add(isType::VAR_FLOAT,"Temperature",2);
   ROOM_DATA_CENTER->Items()->Add(isType::VAR_FLOAT,"Humidity",2);
   ROOM_DATA_CENTER->Items()->Add(isType::VAR_FLOAT,"Temperature2");
   ROOM_DATA_CENTER->Items()->Add(isType::VAR_FLOAT,"Humidity2");

   auto *ROOM_AHU1 = BMS.Add("ROOM_AHU1");
   ROOM_AHU1->Items()->Add(isType::VAR_FLOAT,"Temperature_Supply");
   ROOM_AHU1->Items()->Add(isType::VAR_FLOAT,"Temperature_Return");

   auto *Chiller = BMS.Add("Chiller1");
   Chiller->Items()->Add(isType::VAR_INT8,"Evap_Inlet_Temperature");
   Chiller->Items()->Add(isType::VAR_INT8,"Evap_Outet_Temperature");
   
   /* simple set value tag  ROOM_AHU1/Temperature */
   BMS.Find(1)->Items()->FindByIndex(0)->Set<float>(25.3,0);
   /* simple set value tag  ROOM_AHU1/Humidity_Supply */
   BMS.Find(1)->Items()->FindByIndex(1)->Set<float>(55.4);

   /* Print Tag Infomation and data */
   auto x = BMS.Count();
   for(int n = 0 ; n < x ; n++)
   {
      printf("%s",BMS.Find(n)->MonitorInfo());
      auto item = BMS.Find(n)->Items()->Count();
      for(int o = 0 ; o < item ; o++)
      {
         printf("%s",BMS.Find(n)->Items()->FindByIndex(o)->MonitorInfo());

         auto array = BMS.Find(n)->Items()->FindByIndex(o)->GetArraySize();
         for(int p = 0 ; p < array ;p++)
           printf("%s",BMS.Find(n)->Items()->FindByIndex(o)->MonitorValue(p));
      }
   }
   

   return 0 ;
}