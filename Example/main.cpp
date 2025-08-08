#include <iostream>
#include <chrono>
#include <cassert>
#include <cstring>
#include <TagUtility.h>
#include <TagStatic.h>


Topic *DB = nullptr ;
TagGroup *Tags = nullptr;
TagUtility OPC;

int main()
{  
    OPC.TopicAdd(&DB,"Dota2");
    OPC.TopicAdd(&DB,"OmiKnight");
    OPC.TopicAdd(&DB,"Zeus");
    OPC.TopicAdd(&DB,"Sven");
    OPC.TopicAdd(&DB,"TrollWarlord");
    OPC.TopicAdd(&DB,"Pudge");

    //OPC.ItemAdd(&DB->Group,)

    //size_t XX =  OPC.TopicCount(&DB);

    Topic *XY = OPC.TopicFindByName(&DB,"OmiKnight");

    OPC.ItemAdd(&XY,isType::VAR_FLOAT,"Temperature",5);
    OPC.ItemAdd(&XY,isType::VAR_INT8,"ModbusNode",4);


    OPC.ItemFindByName(&XY,"Temperature")->SetValue<float>(23.78,0);

    size_t itemcount = OPC.ItemCount(&XY);

    Topic *A = OPC.TopicFindByIndex(&DB,0);
    Topic *B = OPC.TopicFindByIndex(&DB,3);
    Topic *C = OPC.TopicFindByIndex(&DB,5);

    auto *D = OPC.ItemFindByName(&XY,"ModbusNode");
    Tag *E = OPC.ItemFindByIndex(&XY,1);
    Tag *F = OPC.ItemFindByIndex(&XY,10);

    //uint8_t G = OPC.GetValue<uint8_t>(D,0);

    //D->SetValue(0)

    printf("%s",XY->MonitorInfo());
    printf("%s",D->MonitorInfo());
    for(int n = 0 ; n < 4 ; n++)
    {
       D->SetValue<uint8_t>(n,n);
       printf("%s",D->MonitorValue(n));
    }

    //uint8_t x[255];

    //for(int n = 0 ; n < 50 ; n++)
    //  x[n] = D->SetValue<uint8_t>(n);

    
    OPC.TopicAdd(&DB,"Lion");
    OPC.TopicAdd(&DB,"UrsaWarrior");
    OPC.TopicAdd(&DB,"Rikimaru");

    size_t tmp1 = OPC.TopicCount(&DB);
    for(int n = 0 ; n < tmp1 ; n++)
    {
       Topic *t =  OPC.TopicFindByIndex(&DB,n);
       printf("%s",t->MonitorInfo());
       
       size_t tmp2 = OPC.ItemCount(&t);
       
       for(int x = 0 ; x < tmp2 ; x++)
       {
           Tag *tmp3 = OPC.ItemFindByIndex(&t,x);
           printf("%s",tmp3->MonitorInfo());

           size_t tmp4 = tmp3->GetArraySize();
           for(int y = 0 ; y < tmp4 ; y++)
            printf("%s",tmp3->MonitorValue(y));
       }
    }

    return 0 ;
}