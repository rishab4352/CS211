#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

//Author:Rishab Das//
//cachism.c//
int mr;
int mw;
int miss;
int hit;
struct Lline** cache;
unsigned long int count;

//Structure//
typedef struct Lline{
unsigned long int tag;
int valid;
unsigned long int time;
}Lline;
//function//
void simread(unsigned long int tagi,unsigned long int seti,int assoc);
Lline** makeCache(int numSet,int assoc);
void writesim(unsigned long int tagi,unsigned long int seti,int assoc);
void empty(int numSet, int assoc);
void w2prefetch(unsigned long int tagi,unsigned long int seti,int assoc);
void w1prefetch(unsigned long int tagi,unsigned long int seti,int assoc,unsigned long int tagin,unsigned long int setin);
void r1prefetch(unsigned long int tagi,unsigned long int seti,int assoc,unsigned long int tagin,unsigned long int setin);

void r2prefetch(unsigned long int tagi,unsigned long int seti,int assoc);
void writesiml(unsigned long int tagi,unsigned long int seti,int assoc);
void rlprefetch(unsigned long int tagi,unsigned long int seti,int assoc,unsigned long int tagin,unsigned long int setin);
void wlprefetch(unsigned long int tagi,unsigned long int seti,int assoc,unsigned long int tagin,unsigned long int setin);
void simlread(unsigned long int tagi,unsigned long int seti,int assoc);
int main(int argc, char** argv){
  int sizeCache=atoi(argv[1]);
  int sizeBlock=atoi(argv[4]);
  int n;//set assoc;
  int numSet;
  int assoc;
  int b;
  int s;
  //int t;
  char work;
  unsigned long int address;
  unsigned long int newaddress;
  unsigned long int maskSet;
  //unsigned long int tagmask;
  unsigned long int tagi;
  unsigned long int seti;
  unsigned long int tagin;
  unsigned long int setin;
  //unsigned long int blockmask;
    if(argv[3][0]=='f'){
        FILE* fl;
        fl=fopen(argv[5],"r");
        if(fl==NULL){
        return 0;
        }
        if(argv[2][0]=='d'){//direct map
            assoc=1;
            numSet=sizeCache/sizeBlock;
        }else if(argv[2][5]!=':'){//fullassoc
            numSet=1;
            assoc=sizeCache/sizeBlock;
        }else{
            sscanf(argv[2],"assoc:%d",&n);
            assoc=n;
            numSet=sizeCache/sizeBlock/n;
            }
            //find how many bits//
            b=log(sizeBlock)/log(2);
            s=log(numSet)/log(2);
            //blockmask=sizeBlock-1;
            maskSet=((1<<s)-1);
            cache=makeCache(numSet,assoc);
            while(fscanf(fl, "%*x: %c %lx", &work, &address)==2){
                seti=(address>>b)&maskSet;
                tagi=address>>(b+s);
                if(work=='R'){
                    simread(tagi,seti,assoc);
                }else if(work=='W'){
                    writesim(tagi,seti,assoc);
                }
            }
            fclose(fl);
            fl=fopen(argv[5],"r");
            if(fl==NULL){
            return 0;
            }
            printf("Prefetch 0\n");
            printf("Memory reads: %d\nMemory writes: %d\nCache hits: %d\nCache misses: %d\n",mr,mw,hit,miss);
            empty(numSet,assoc);
            while(fscanf(fl, "%*x: %c %lx", &work, &address)==2){
                seti=(address>>b)&maskSet;
                tagi=address>>(b+s);
                newaddress=address+sizeBlock;
                setin=(newaddress>>b)&maskSet;
                tagin=newaddress>>(b+s);
                if(work=='R'){
                    r1prefetch(tagi,seti,assoc,tagin,setin);

                }else if(work=='W'){
                    w1prefetch(tagi,seti,assoc,tagin,setin);
                }
            }
            printf("Prefetch 1\n");
            printf("Memory reads: %d\nMemory writes: %d\nCache hits: %d\nCache misses: %d\n",mr,mw,hit,miss);

            }else if(argv[3][0]=='l'){

                FILE* fl;
                fl=fopen(argv[5],"r");
                if(fl==NULL){
                    return 0;
                }
            if(argv[2][0]=='d'){
                assoc=1;
                numSet=sizeCache/sizeBlock;
            }else if(argv[2][5]!=':'){
                numSet=1;
                assoc=sizeCache/sizeBlock;
            }else{
                sscanf(argv[2],"assoc:%d",&n);
                assoc=n;
                numSet=sizeCache/sizeBlock/n;
            }
            b=log(sizeBlock)/log(2);
            s=log(numSet)/log(2);

            maskSet=((1<<s)-1);
            cache=makeCache(numSet,assoc);

            while(fscanf(fl, "%*x: %c %lx", &work, &address)==2){
                seti=(address>>b)&maskSet;
                tagi=address>>(b+s);
                if(work=='R'){
                simlread(tagi,seti,assoc);
                }else if(work=='W'){
                writesiml(tagi,seti,assoc);
                }
            }
            fclose(fl);
            fl=fopen(argv[5],"r");
            if(fl==NULL){
                printf("cannot find tracefile with that name\n");
                return 0;
            }
            printf("Prefetch 0\n");
            printf("Memory reads: %d\nMemory writes: %d\nCache hits: %d\nCache misses: %d\n",mr,mw,hit,miss);
            empty(numSet,assoc);
            while(fscanf(fl, "%*x: %c %lx", &work, &address)==2){
                seti=(address>>b)&maskSet;
                tagi=address>>(b+s);
                newaddress=address+sizeBlock;
                setin=(newaddress>>b)&maskSet;
                tagin=newaddress>>(b+s);
                if(work=='R'){
                    rlprefetch(tagi,seti,assoc,tagin,setin);
                }else if(work=='W'){
                wlprefetch(tagi,seti,assoc,tagin,setin);
                }
            }
            printf("Prefetch 1\n");
            printf("Memory reads: %d\nMemory writes: %d\nCache hits: %d\nCache misses: %d\n",mr,mw,hit,miss);
        }
        return 0;
    }   
    //cache function//
    Lline** makeCache(int numSet,int assoc){
        int i,j;
        cache=(Lline**)malloc(numSet*sizeof(Lline*));
        for(i=0;i<numSet;i++){
            cache[i]=(Lline*)malloc((assoc)*sizeof(Lline));
        }
        for(i=0;i<numSet;i++){
            for(j=0;j<assoc;j++){
                cache[i][j].valid=0;
            }
        }
            return cache;
    }
    void simread(unsigned long int tagi,unsigned long int seti,int assoc){
    int i,j,min;
    for(i=0;i<assoc;i++){
        if(cache[seti][i].valid==0){
            miss++;
            mr++;
            count++;
            cache[seti][i].valid=1;
            cache[seti][i].tag=tagi;
            cache[seti][i].time=count;
            return; 
        }else{
            if(cache[seti][i].tag==tagi){
                hit++;
                return;
            }	
            if(i==(assoc-1)){
                miss++;
                mr++;
                min=0;
                for(j=0;j<assoc;j++){
                    if(cache[seti][j].time<=cache[seti][min].time){
                        min=j;
                    }	
                }
                cache[seti][min].valid=1;
                cache[seti][min].tag=tagi;
                count++;
                cache[seti][min].time=count;
                return;
            }
        }
    }
    return;
        }
        //write function
        void writesim(unsigned long int tagi,unsigned long int seti,int assoc){
        int i,j,min;
        for(i=0;i<assoc;i++){
        if(cache[seti][i].valid==0){
        miss++;
        mr++;
        mw++;
        count++;
        cache[seti][i].valid=1;
        cache[seti][i].tag=tagi;
        cache[seti][i].time=count;

        return;
        }else{

        if(cache[seti][i].tag==tagi){
        hit++;
        mw++;
        return;
        }

        if(i==(assoc-1)){
        miss++;
        mr++;
        mw++;
        min=0;
        for(j=0;j<assoc;j++){

        if(cache[seti][j].time<=cache[seti][min].time){
        min=j;
        }	
        }

        cache[seti][min].valid=1;
        cache[seti][min].tag=tagi;
        count++;
        cache[seti][min].time=count;
        return;
        }
        }
        }
        return;
        }
        void empty(int numSet, int assoc){
        int i,j;
        for(i=0;i<numSet;i++){
        for(j=0;j<assoc;j++){
        cache[i][j].tag=0;
        cache[i][j].valid=0;
        cache[i][j].time=0;
        }
        }
        miss=0;
        hit=0;
        mr=0;
        mw=0;
        count=0;
        }
        void w2prefetch(unsigned long int tagi,unsigned long int seti,int assoc){

        int i,j,min;
        for(i=0;i<assoc;i++){
        if(cache[seti][i].valid==0){

        mr++;
        count++;
        cache[seti][i].valid=1;
        cache[seti][i].tag=tagi;
        cache[seti][i].time=count;
        return;
        }else{
        if(cache[seti][i].tag==tagi){
        return;
        }

        if(i==(assoc-1)){

        mr++;
        min=0;
        for(j=0;j<assoc;j++){

        if(cache[seti][j].time<=cache[seti][min].time){
        min=j;
        }	
        }

        cache[seti][min].valid=1;
        cache[seti][min].tag=tagi;
        count++;
        cache[seti][min].time=count;
        return;
        }
        }

        }

        return;
        }
        void r2prefetch(unsigned long int tagi,unsigned long int seti,int assoc){

        int i,j,min;
        for(i=0;i<assoc;i++){
        if(cache[seti][i].valid==0){
        mr++;
        count++;
        cache[seti][i].valid=1;
        cache[seti][i].tag=tagi;
        cache[seti][i].time=count;

        return;
        }else{

        if(cache[seti][i].tag==tagi){

        return;
        }

        if(i==(assoc-1)){

        mr++;

        min=0;
        for(j=0;j<assoc;j++){

        if(cache[seti][j].time<=cache[seti][min].time){
        min=j;
        }	
        }
        cache[seti][min].valid=1;
        cache[seti][min].tag=tagi;
        count++;
        cache[seti][min].time=count;
        return;
        }
        }
        }

        }
        void w1prefetch(unsigned long int tagi,unsigned long int seti,int assoc,unsigned long int tagin,unsigned long int setin){

        int i,j,min;
        for(i=0;i<assoc;i++){
        if(cache[seti][i].valid==0){
        count++;
        miss++;
        mr++;
        mw++;
        cache[seti][i].valid=1;
        cache[seti][i].tag=tagi;
        cache[seti][i].time=count;
        w2prefetch(tagin,setin,assoc);
        return;
        }else{

        if(cache[seti][i].tag==tagi){
        hit++;
        mw++;
        return;
        }

        if(i==(assoc-1)){
        miss++;
        mr++;
        mw++;
        min=0;
        for(j=0;j<assoc;j++){

        if(cache[seti][j].time<=cache[seti][min].time){
        min=j;
        }	
        }

        cache[seti][min].valid=1;
        cache[seti][min].tag=tagi;
        count++;
        cache[seti][min].time=count;
        w2prefetch(tagin, setin,assoc);
        return;
        }
        }
        }

        return;
        }
        void r1prefetch(unsigned long int tagi,unsigned long int seti,int assoc,unsigned long int tagin,unsigned long int setin){
        int i,j,min;
        for(i=0;i<assoc;i++){
        if(cache[seti][i].valid==0){
        miss++;
        mr++;
        count++;
        cache[seti][i].valid=1;
        cache[seti][i].tag=tagi;
        cache[seti][i].time=count;

        r2prefetch(tagin,setin,assoc);
        return;
        }else{

        if(cache[seti][i].tag==tagi){
        hit++;
        return;
        }

        if(i==(assoc-1)){
        miss++;
        mr++;

        min=0;
        for(j=0;j<assoc;j++){

        if(cache[seti][j].time<=cache[seti][min].time){
        min=j;
        }	
        }

        cache[seti][min].valid=1;
        cache[seti][min].tag=tagi;
        count++;
        cache[seti][min].time=count;
        r2prefetch(tagin,setin,assoc);
        return;
        }

        }
        }
        return;
        }
        void simlread(unsigned long int tagi,unsigned long int seti,int assoc){
        int i,j,min;
        for(i=0;i<assoc;i++){
        if(cache[seti][i].valid==0){
        miss++;
        mr++;
        count++;
        cache[seti][i].valid=1;
        cache[seti][i].tag=tagi;
        cache[seti][i].time=count;

        return;
        }else{

        if(cache[seti][i].tag==tagi){
        hit++;
        count++;
        cache[seti][i].time=count;
        return;
        }

        if(i==(assoc-1)){
        miss++;
        mr++;

        min=0;
        for(j=0;j<assoc;j++){

        if(cache[seti][j].time<=cache[seti][min].time){
        min=j;
        }	
        }

        cache[seti][min].valid=1;
        cache[seti][min].tag=tagi;
        count++;
        cache[seti][min].time=count;

        return;
        }
        }
        }
        return;
        }
        //write function//
        void writesiml(unsigned long int tagi,unsigned long int seti,int assoc){
        int i,j,min;
        for(i=0;i<assoc;i++){
        if(cache[seti][i].valid==0){
        miss++;
        mr++;
        mw++;
        count++;
        cache[seti][i].valid=1;
        cache[seti][i].tag=tagi;
        cache[seti][i].time=count;

        return;
        }else{
        if(cache[seti][i].tag==tagi){
        hit++;
        mw++;
        count++;
        cache[seti][i].time=count;
        return;
        }

        if(i==(assoc-1)){
        miss++;
        mr++;
        mw++;
        min=0;
        for(j=0;j<assoc;j++){

        if(cache[seti][j].time<=cache[seti][min].time){
        min=j;
        }	
        }
        cache[seti][min].valid=1;
        cache[seti][min].tag=tagi;
        count++;
        cache[seti][min].time=count;
        return;
        }
        }
        }

        return;
        }
       
        void wlprefetch(unsigned long int tagi,unsigned long int seti,int assoc,unsigned long int tagin,unsigned long int setin){

        int i,j,min;
        for(i=0;i<assoc;i++){
        if(cache[seti][i].valid==0){
        miss++;
        mr++;
        mw++;
        count++;
        cache[seti][i].valid=1;
        cache[seti][i].tag=tagi;
        cache[seti][i].time=count;
        w2prefetch(tagin,setin,assoc);
        return;
        }else{
        if(cache[seti][i].tag==tagi){
        hit++;
        mw++;
        count++;
        cache[seti][i].time=count;
        return;
        }

        if(i==(assoc-1)){
        miss++;
        mr++;
        mw++;
        min=0;
        for(j=0;j<assoc;j++){
        if(cache[seti][j].time<=cache[seti][min].time){
        min=j;
        }	
        }
        cache[seti][min].valid=1;
        cache[seti][min].tag=tagi;
        count++;
        cache[seti][min].time=count;
        w2prefetch(tagin, setin,assoc);
        return;
        }
        }
        }     
        return;
        }
        void rlprefetch(unsigned long int tagi,unsigned long int seti,int assoc,unsigned long int tagin,unsigned long int setin){
        int i,j,min;
        for(i=0;i<assoc;i++){
        if(cache[seti][i].valid==0){
        miss++;
        mr++;
        count++;
        cache[seti][i].valid=1;
        cache[seti][i].tag=tagi;
        cache[seti][i].time=count;

        r2prefetch(tagin,setin,assoc);
        return;
        }else{
        if(cache[seti][i].tag==tagi){
        hit++;
        count++;
        cache[seti][i].time=count;
        return;
        }

        if(i==(assoc-1)){
        miss++;
        mr++;
        min=0;
        for(j=0;j<assoc;j++){
        if(cache[seti][j].time<=cache[seti][min].time){
        min=j;
        }
        }
        cache[seti][min].valid=1;
        cache[seti][min].tag=tagi;
        count++;
        cache[seti][min].time=count;
        r2prefetch(tagin,setin,assoc);
        return;
            }
    }
    }
return; 
}