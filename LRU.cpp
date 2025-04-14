#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <ctime>
#include<stdio.h>
#include<stdlib.h>

using namespace std;
/*Records */
typedef struct _Record
{
   int userId;
   char data[100];
   time_t ts;
}Record;

#define RECORD_SIZE   0xffff         /*Considering 100000 records in a day is created */
unordered_map<int, vector<Record>> hashList;

void insertRecord(const Record& record)
{
   int hashKey = (record.ts>>9 ) & RECORD_SIZE;
   hashList[hashKey].push_back(record);
   printf("Insert userId=%d , ts=%d , hashKey=%d\n", record.userId, record.ts, hashKey);
}

/* Delete the records older than ttl */
bool deleteOldRecords(int ttl, time_t crntTs)
{
    int crnt_hash_idx, hash_idx_diff, hash_idx_delete;
    crnt_hash_idx = (crntTs >>9) & RECORD_SIZE;
    /* Get the Hash Idx where the records before ttl time is present */
    hash_idx_delete= ((crntTs - ttl)>>9)&(RECORD_SIZE);
    printf("hash_idx_delete =%d, crnt_hash_idx =%d \n", hash_idx_delete, crnt_hash_idx);
    while(hash_idx_delete != crnt_hash_idx)
    {
       auto& bucket = hashList[hash_idx_delete];
       if(bucket.empty())
          break;
       auto it = bucket.begin();
       //for(auto it = bucket.begin(); it != bucket.end(); ++it)
       while (!bucket.empty())
       {
          if( (crntTs - it->ts) >= ttl)
          {
             printf("Delete the record- userId=%d,ts=%d \n", it->userId, it->ts);
             it->userId = -1;
             bucket.erase(it);
          }
          else
          {
             break;
          }
          it = bucket.begin();
       }
       hash_idx_delete--;
       if(hash_idx_delete == 0)
         hash_idx_delete = RECORD_SIZE ;
    }
}
Record recDB[65536];
int main()
{
    int duration;
    int i, userId=1, ttl;
    time_t ts ;

    printf("Enter the ttl\n");
    scanf("%d", &ttl);

    time_t crntTs = time(nullptr);·
    /*Insert the record 100 times after every 200ms */
    for(i=0; i< 100; i++)
    {
       recDB[userId-1].userId= userId;
       crntTs = crntTs + 200;
       recDB[userId-1].ts = crntTs ;
       insertRecord(recDB[userId-1]);
       userId++;
    }
    deleteOldRecords(ttl, crntTs);
}
