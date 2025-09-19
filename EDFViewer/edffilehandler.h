/*
The MIT License (MIT)

Copyright (c) 2016 kostasrotas

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef EDFFILEHANDLER_H
#define EDFFILEHANDLER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

// change __declspec(dllexport) to __declspec(dllimport) for host applications
// and include edffilehandler.h to headers
// add edffilehandler.a to libs
// add edffilehandler.dll to executable path

class EDFfilehandler{
public:

    FILE *f;
    char pathname[256];
    //EDF Data
    char patientid[81];
    char recordid[81];
    char startdaterec[9];
    char starttimerec[9];
    int bytesinheader;
    long numofdatarecords;
    float recduration;//duration of record in secs
    int numofsignals;
    // end of EDF Data
    int recsize;//record data size in (signed short int)
    int *edfdatarecoffset;
    int *samplesperrecord;
    long *numofsignaldata;
    char annotsignal;
    char * annotbuffer;
    char addannotationtime;
    char fileisreadonly;
private:
    int addedsignals;
    signed short *readbuffer;
    long readbuffersize;
    long readbufferstart;
public:

    EDFfilehandler(long readbuffersize);
    ~EDFfilehandler();
    int openfile(char *pathname);
    int opennewfile(char *pathname,char *patientid,char* recordid,char* startdaterec,char *starttimerec,float recduration,int  numofsignals);
                // must be followed by <numofsignals> addsignaldata
    int closefile();
    int getsignaldata(int signum,char *name,char *transducertype=NULL,char *phdimension=NULL,float *phmin=NULL,float *phmax=NULL,int *digmin=NULL,int *digmax=NULL,char *prefiltering=NULL,int *samplesperrecord=NULL);
    int addsignaldata(const char *name,const char *transducertype,const char *phdimension,float phmin,float phmax,int digmin,int digmax,const char *prefiltering,int samplesperrecord);
    int addannotsignaldata(int samplesperrecord);

    signed short int getdata(int signalnum,long pos);
    int putdata(int signalnum,long pos,signed short int value);
    int appenddata(int signalnum,signed short int value);
    float totaltime(int signum=0);
    void getdescriptionstr(char *);
    void addemptyrecord();

    //
    int readannotbuffer(int recnum);
    int writeannotbuffer(int recnum);
    int numofgraphsingals();//numofsignals - annotations signal
    int getannotation(int recnum,int annotnum,char*annotstr,char *timestr=NULL,int signum=-1);
    int addannotation(float time,char *annotstr,int signum=-1);
    int delannotation(int recnum,int annotnum);
    int delannotation(int recnum,char* annotstr);



private:
    void writefilestr(const char * str, int size);
    void readfilestr(char *str,int size);
};

#endif // EDFFILEHANDLER_H
