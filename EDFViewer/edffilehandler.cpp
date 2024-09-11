
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

#include "edffilehandler.h"

//#include<qdebug.h>

static const char ttstr[]="[Total time]";
EDFfilehandler::EDFfilehandler(long readbuffersize){
    this->readbuffersize=readbuffersize;
    readbuffer=(signed short*)malloc(readbuffersize* sizeof (signed short));
    f=NULL;
    edfdatarecoffset=NULL;
    samplesperrecord=NULL;
    numofsignaldata=NULL;
    annotbuffer=NULL;
    readbufferstart=0;
    addannotationtime=1;
}

EDFfilehandler::~EDFfilehandler(){
  closefile();
  if (edfdatarecoffset)
      free(edfdatarecoffset);
  if (samplesperrecord)
      free(samplesperrecord);
  if (numofsignaldata)
      free(numofsignaldata);
  if (readbuffer)
      free(readbuffer);
  if (annotbuffer!=NULL)
      free(annotbuffer);
}

void EDFfilehandler::readfilestr(char *str,int size){
     char str1[256];
     fgets (str1,size+1,f);
     while (1){
       if (strlen(str1)==0)
         break;
       if (str1[strlen(str1)-1]!=' ')
         break;
       str1[strlen(str1)-1]=0;
       }
     strcpy(str,str1);
     return;
     }

int EDFfilehandler::openfile(char *pathname){
    char str[256];
    int i;
    strcpy(this->pathname,pathname);
    f=fopen(pathname,"rb+");
    if (f==NULL)
        return 0;//failed to open file
    readfilestr(str,8);//version data format
    readfilestr(str,80);// patient name
    strcpy(patientid,str);
    readfilestr(str,80);//recording id
    strcpy(recordid,str);
    readfilestr(str,8);
    strcpy(startdaterec,str);
    readfilestr(str,8);
    strcpy(starttimerec,str);
    readfilestr(str,8);//bytes in header
    bytesinheader=atol(str);
    readfilestr(str,44);// reserved
    readfilestr(str,8);// num of data records
    numofdatarecords=atol(str);
    //qDebug()<<"numofdatarecords"<<numofdatarecords;
    readfilestr(str,8);// duration of a data record, in seconds
    if (atof("0,1")==0.1){// use decimal comma instead of decimal point
        for (i=0;i<strlen(str);i++)
         if (str[i]=='.')
             str[i]=',';
    }
    else if (atof("0.1")==0.1){// use decimal comma instead of decimal point
        for (i=0;i<strlen(str);i++)
         if (str[i]==',')
             str[i]='.';
    }
    recduration=atof(str);
    //qDebug()<<"recduration"<<recduration;
    readfilestr(str,4);//number of sigs (ns) in data record
    numofsignals=atol(str);
    if (edfdatarecoffset)
        free(edfdatarecoffset);
    edfdatarecoffset=(int *)malloc(numofsignals*sizeof(int));
    if (samplesperrecord)
        free(samplesperrecord);
    samplesperrecord=(int *)malloc(numofsignals*sizeof(int));
    if (numofsignaldata)
        free(numofsignaldata);
    numofsignaldata=(long *)malloc(numofsignals*sizeof(long));
    recsize=0;
    annotsignal=-1;
    for (i=0;i<numofsignals;i++){
        fseek(f,256+216*numofsignals+8*i,SEEK_SET);
        readfilestr(str,8);
        samplesperrecord[i]=atol(str);
        edfdatarecoffset[i]=recsize;
        recsize+=samplesperrecord[i];
        numofsignaldata[i]=samplesperrecord[i]*numofdatarecords;
    }
    readbufferstart=bytesinheader;
    fseek(f,bytesinheader,SEEK_SET);
    memset((void*)readbuffer,0,readbuffersize*sizeof(signed short));
    fread(readbuffer,sizeof(signed short),readbuffersize,f);
    //check for annotation signal
    if (getsignaldata(numofsignals-1,str)){
        if (strcmp(str,"EDF Annotations")==0){//annotsingal exists
            annotsignal=numofsignals-1;
            if (annotbuffer!=NULL)
                free(annotbuffer);
            annotbuffer=(char*)malloc(samplesperrecord[annotsignal]*sizeof(signed short));
        }
    }
    int acount=1;
    if (addannotationtime)
        while (getannotation(0,acount++,str))
                if (strncmp(str,ttstr,strlen(ttstr))==0){
                    float tt=atof(str+strlen(ttstr));
                    //qDebug()<<"Total time"<<tt;
                    for (int i=0;i<numofsignals-1;i++)
                        numofsignaldata[i]=tt/recduration*samplesperrecord[i];
                }//if
    /*
    for (i=0;i<numofsignals;i++){
        for (long j=samplesperrecord[i]*(numofdatarecords-1);j<samplesperrecord[i]*(numofdatarecords);j++)
            if (getdata(i,j)==-32768){
                numofsignaldata[i]=j+1;
                break;
                }
        }
    */
    return 1;
}


void EDFfilehandler::writefilestr(const char * str, int size){
     char str1[10000];
     strcpy(str1,str);
     if (strlen(str1)>size){
       str1[size]=0;
       }
     else{
       while (strlen(str1)<size)
             strcat(str1," ");
       }
     for (int i=0;i<size;i++)
         fputc(str1[i],f);
  }


int EDFfilehandler::opennewfile(char *pathname, char *patientid, char *recordid, char *startdaterec, char *starttimerec, float recduration, int numofsignals){
    char str[256];
    f=fopen(pathname,"wb+");
    if (f==NULL)
        return 0;
    //setvbuf(f,NULL,_IOFBF,32000);
    strcpy(this->pathname,pathname);
    writefilestr("0",8);//version data format 0
    if (patientid)
        strcpy(str,patientid);
    else
        str[0]=0;
    strcpy(this->patientid,str);
    writefilestr(str,80);// patient name
    if (recordid)
        strcpy(str,recordid);
    else
        str[0]=0;
    strcpy(this->recordid,str);
    writefilestr(str,80);//recording id
    if (startdaterec)
        strcpy(str,startdaterec);
    else
        str[0]=0;
    if (str[0]==0){
        time_t timer;
        timer=time(NULL);
        struct tm* tmp=localtime(&timer);
        sprintf(str,"%02d.%02d.%02d",tmp->tm_mday,tmp->tm_mon+1,tmp->tm_year%100);
        }
    strcpy(this->startdaterec,str);
    writefilestr(str,8);
    if (starttimerec)
        strcpy(str,starttimerec);
    else
        str[0]=0;
    if (str[0]==0){
        time_t timer;
        timer=time(NULL);
        struct tm* tmp=localtime(&timer);
        sprintf(str,"%02d.%02d.%02d",tmp->tm_hour,tmp->tm_min,tmp->tm_sec);
        }
    strcpy(this->starttimerec,str);
    writefilestr(str,8);
    this->recduration=recduration;
    this->numofsignals=numofsignals;
    bytesinheader=256*(1+numofsignals);
    sprintf(str,"%d",bytesinheader);//bytes in header
    writefilestr(str,8);
    writefilestr("",44);// reserved
    numofdatarecords=0;
    writefilestr("0",8);// num of data records
    sprintf(str,"%f",recduration);
    writefilestr(str,8);//duration of a data record, in seconds
    sprintf(str,"%d",numofsignals);
    writefilestr(str,4);//number of signals (ns) in data record
    writefilestr("",256*numofsignals);//to be completed with addsignaldata(...)
    if (edfdatarecoffset)
        free(edfdatarecoffset);
    edfdatarecoffset=(int *)malloc(numofsignals*sizeof(int));
    if (samplesperrecord)
        free(samplesperrecord);
    samplesperrecord=(int *)malloc(numofsignals*sizeof(int));
    if (numofsignaldata)
        free(numofsignaldata);
    numofsignaldata=(long *)malloc(numofsignals*sizeof(long));
    recsize=0;
    addedsignals=0;
    memset((void*)readbuffer,0,readbuffersize*sizeof(signed short));
    readbufferstart=bytesinheader;// dont need to read , file is emtpty
    annotsignal=-1;
    return 1;
}

int EDFfilehandler::closefile(){
if (f){
    char str[256];
    if (addannotationtime && numofdatarecords>0){
        char found=0;
        int acount=1;
        while (getannotation(0,acount++,str))
            if (strncmp(str,ttstr,strlen(ttstr))==0){
                found=1;
                break;
            }
        if (!found){
            sprintf(str,"%s%+.3f",ttstr,totaltime());
            addannotation(0,str);
        }
    }
    fseek(f,8,SEEK_SET);
    writefilestr(patientid,80);// patient name
    writefilestr(recordid,80);//recording id
    writefilestr(startdaterec,8);
    writefilestr(starttimerec,8);
    fseek(f,236,SEEK_SET);
    sprintf(str,"%ld",numofdatarecords);
    writefilestr(str,8);// num of data records
    fclose(f);
    f=NULL;
    //qDebug()<<"edfhcalled file closed";
    }
//.... buffers?
return 0;
}

int EDFfilehandler::getsignaldata(int signum,char *name, char *transducertype, char *phdimension, float *phmin, float *phmax, int *digmin, int *digmax, char *prefiltering, int *samplesperrecordp){
    char str[256];
    if (signum>=numofsignals)
        return 0;
    fseek(f,256+0+16*signum,SEEK_SET);
    readfilestr(str,16);
    if (name)
        strcpy(name,str);
    fseek(f,256+16*numofsignals+80*signum,SEEK_SET);
    readfilestr(str,80);
    if (transducertype)
        strcpy(transducertype,str);
    fseek(f,256+96*numofsignals+8*signum,SEEK_SET);
    readfilestr(str,8);
    if (phdimension)
        strcpy(phdimension,str);
    fseek(f,256+104*numofsignals+8*signum,SEEK_SET);
    readfilestr(str,8);
    if (phmin)
        *phmin=atof(str);
    fseek(f,256+112*numofsignals+8*signum,SEEK_SET);
    readfilestr(str,8);
    if (phmax)
        *phmax=atof(str);
    fseek(f,256+120*numofsignals+8*signum,SEEK_SET);
    readfilestr(str,8);
    if (digmin)
        *digmin=atol(str);
    fseek(f,256+128*numofsignals+8*signum,SEEK_SET);
    readfilestr(str,8);
    if (digmax)
        *digmax=atol(str);
    fseek(f,256+136*numofsignals+80*signum,SEEK_SET);
    readfilestr(str,80);
    if (prefiltering)
        strcpy(prefiltering,str);
    fseek(f,256+216*numofsignals+8*signum,SEEK_SET);
    readfilestr(str,8);
    if (samplesperrecordp)
        *samplesperrecordp=atol(str);
    return 1;
}

int EDFfilehandler::addsignaldata(const char *name,const char *transducertype,const char *phdimension, float phmin, float phmax, int digmin, int digmax,const char *prefiltering, int ssamplesperrecord){
char str[256];
fseek(f,256+0+16*addedsignals,SEEK_SET);
if (name)
    strcpy(str,name);
else
  str[0]=0;
writefilestr(str,16);

fseek(f,256+16*numofsignals+80*addedsignals,SEEK_SET);
if (transducertype)
    strcpy(str,transducertype);
else
    str[0]=0;
writefilestr(str,80);

fseek(f,256+96*numofsignals+8*addedsignals,SEEK_SET);
if (phdimension)
    strcpy(str,phdimension);
else
    str[0]=0;
writefilestr(str,8);

fseek(f,256+104*numofsignals+8*addedsignals,SEEK_SET);
sprintf(str,"%f",phmin);
writefilestr(str,8);

fseek(f,256+112*numofsignals+8*addedsignals,SEEK_SET);
sprintf(str,"%f",phmax);
writefilestr(str,8);

fseek(f,256+120*numofsignals+8*addedsignals,SEEK_SET);
sprintf(str,"%d",digmin);
writefilestr(str,8);
fseek(f,256+128*numofsignals+8*addedsignals,SEEK_SET);
sprintf(str,"%d",digmax);
writefilestr(str,8);

fseek(f,256+136*numofsignals+80*addedsignals,SEEK_SET);
if (prefiltering)
    strcpy(str,prefiltering);
else
  str[0]=0;
writefilestr(str,80);


fseek(f,256+216*numofsignals+8*addedsignals,SEEK_SET);
sprintf(str,"%d",ssamplesperrecord);
writefilestr(str,8);

edfdatarecoffset[addedsignals]=recsize;
samplesperrecord[addedsignals]=ssamplesperrecord;
numofsignaldata[addedsignals]=0;
recsize+=ssamplesperrecord;
addedsignals++;
return 1;
}

int EDFfilehandler::addannotsignaldata(int numofsamplesperrecord){
    if (addedsignals!=numofsignals-1)
        return 0;// not accepted , only the last can be annotation signal
    annotsignal=numofsignals-1;
    if (annotbuffer!=NULL)
        free(annotbuffer);
    annotbuffer=(char*)malloc(numofsamplesperrecord*sizeof(signed short));
    return addsignaldata("EDF Annotations","","",0,1,0,1,"",numofsamplesperrecord);
}

signed short EDFfilehandler::getdata(int signalnum, long pos){
    signed short v;
    long recnum=pos/samplesperrecord[signalnum];
    long filepos=bytesinheader+sizeof(signed short int)*(recnum*recsize+edfdatarecoffset[signalnum]+pos%samplesperrecord[signalnum]);
    if (filepos>=readbufferstart &&filepos<readbufferstart+sizeof(signed short)*readbuffersize)
        return readbuffer[(filepos-readbufferstart)/sizeof(signed short)];
    fseek(f,filepos,SEEK_SET);
    readbufferstart=filepos;
    memset((void*)readbuffer,0,readbuffersize*sizeof(signed short));
    fread(readbuffer,sizeof(signed short),readbuffersize,f);
    return readbuffer[filepos-readbufferstart];
}

int EDFfilehandler::putdata(int signalnum, long pos, signed short int value)
{
    int r=1;
    long recnum=pos/samplesperrecord[signalnum];
    while (recnum>=numofdatarecords){
        addemptyrecord();
        r=2;
    }
    long filepos=bytesinheader+sizeof(signed short int )*(recnum*recsize+edfdatarecoffset[signalnum]+pos%samplesperrecord[signalnum]);
    if (filepos>=readbufferstart && filepos<readbufferstart+sizeof(signed short)*readbuffersize)
        readbuffer[(filepos-readbufferstart)/sizeof(signed short)]=value;
    fseek(f,filepos,SEEK_SET);
    if (fwrite(&value,sizeof(signed short int),1,f)==0)
        return 0;
    if (pos>=numofsignaldata[signalnum])
        numofsignaldata[signalnum]=pos+1;
    return r;
}

int EDFfilehandler::appenddata(int signalnum, signed short value){
    return putdata(signalnum,numofsignaldata[signalnum],value);
}

float EDFfilehandler::totaltime(int signum){
    return (recduration*numofsignaldata[signum])/samplesperrecord[signum];
}

void EDFfilehandler::getdescriptionstr(char *str){
    char str1[256];
    str[0]=0;
    strcat(str,"Filename :");
    strcat(str,pathname);
    strcat(str,"\nPatient:");
    strcat(str,patientid);
    strcat(str,"\nRecord id:");
    strcat(str,recordid);
    strcat(str,"\nDate:");
    strcat(str,startdaterec);
    strcat(str,"\nTime:");
    strcat(str,starttimerec);
    sprintf(str1,"\nNum of data recs:%d\nRecording duration: %f\nNum of Signals:%d\n",numofdatarecords,recduration,numofsignals);
    strcat(str,str1);
    for (int i=0;i<numofsignals;i++){
        char s1[256],s2[256],s3[256];
        int ii;
        getsignaldata(i,s1,s3,s2,NULL,NULL,NULL,NULL,NULL,&ii);
        sprintf(str1,"\nSignal %d.\nSig name:%s\nTrd t:%s\nDim:%s\nSamples[disk]:%d\n",1+i,s1,s3,s2,ii);
        strcat(str,str1);
        sprintf(str1,"offset :%d\nsamples[mem]:%d\n",edfdatarecoffset[i],samplesperrecord[i]);
        strcat(str,str1);
    }
}

void EDFfilehandler::addemptyrecord(){
signed short v=0;
int i;
fseek(f,bytesinheader+sizeof(short signed)*recsize*numofdatarecords,SEEK_SET);
for (i=0;i<recsize;i++)
    fwrite(&v,sizeof(signed short),1,f);
numofdatarecords++;

if (annotsignal!=-1){
    char str[256];
    sprintf(str,"%+.2f%c%c",recduration*(numofdatarecords-1),20,20);
    //strcpy(str,"+0");
    //str[0]=0;
    for (i=0;i<samplesperrecord[annotsignal]*2;i++)//clear annotbuffer
        if (i<strlen(str))
            annotbuffer[i]=str[i];
        else
            annotbuffer[i]=0;
    writeannotbuffer(numofdatarecords-1);


    //for (i=0;i<samplesperrecord[annotsignal];i++){
        //v=annotbuffer[2*i+1]*256+annotbuffer[2*i];
     //   putdata(annotsignal,(numofdatarecords-1)*samplesperrecord[annotsignal]+i,v);
     //   }

    }//if
//addannotation(totaltime(),"");
//fseek(f,readbufferstart,SEEK_SET);
//fread(readbuffer,sizeof(signed short),readbuffersize,f);
}

int EDFfilehandler::readannotbuffer(int recnum){
/*fseek(f,bytesinheader+sizeof(short signed)*(recsize*recnum+edfdatarecoffset[annotsignal]),SEEK_SET);
fread(annotbuffer,samplesperrecord[annotsignal],sizeof(signed short),f);*/
    short int v;
    for (int i=0;i<samplesperrecord[annotsignal];i++){
        v=getdata(annotsignal,recnum*samplesperrecord[annotsignal]+i);
        annotbuffer[2*i+1]=v/256;
        annotbuffer[2*i]=v%256;

        }
return 1;
}

int EDFfilehandler::writeannotbuffer(int recnum){
//fseek(f,bytesinheader+sizeof(short signed)*(recsize*recnum+edfdatarecoffset[annotsignal]),SEEK_SET);
//fwrite(annotbuffer,samplesperrecord[annotsignal],sizeof(signed short),f);
    short int v;
    for (int i=0;i<samplesperrecord[annotsignal];i++){
        v=annotbuffer[2*i+1]*256+annotbuffer[2*i];
        putdata(annotsignal,recnum*samplesperrecord[annotsignal]+i,v);
        }
return 1;
}

int EDFfilehandler::numofgraphsingals(){
    if (annotsignal==-1)
        return numofsignals;
    else
        return numofsignals-1;
}

int EDFfilehandler::getannotation(int recnum,int annotnum,char*annotstr,char *timestr,int signum){
    int countannot=0;
    char signumname[256];
    if (signum!=-1)
        getsignaldata(signum,signumname);
    if (annotsignal==-1)
        return 0;
    //qDebug() << "getannotation"<<recnum<<" "<<annotnum;
    char lastzero=1,intext=0;
    int spos,i;
    if (timestr)
        timestr[0]=0;
    annotstr[0]=0;
    readannotbuffer(recnum);
    for (i=0;i<numofsignaldata[annotsignal];i++){
            if (annotbuffer[i]==0){
                if (lastzero){
                    if (timestr)
                        timestr[0]=0;
                    annotstr[0]=0;
                    return 0;
                    }
                intext=0;
                lastzero=1;
                if (signum==-1){
                    if (countannot==annotnum)
                        return 1;
                    if (timestr)
                        timestr[0]=0;
                    annotstr[0]=0;
                    countannot++;
                    continue;
                }
                else{
                    char * c1,*c2,found=0;
                    c1=strchr(annotstr,'{');
                    c2=strchr(annotstr,'}');
                    if (c1!=NULL && c2!=NULL && c2>c1)
                        if (strncmp(&(annotstr[1]),signumname,c2-c1-1)==0)
                            found=1;
                    if (found){
                        if (countannot==annotnum){
                            strcpy(annotstr,c2+1);
                            return 1;
                        }
                        countannot++;
                    }
                    if (timestr)
                        timestr[0]=0;
                    annotstr[0]=0;
                    continue;
                }

                return 1;
            }//if
            else
                lastzero=0;
            if (annotbuffer[i]==20)
                intext=1;
            else if (intext){
                spos=strlen(annotstr);
                annotstr[spos]=annotbuffer[i];
                annotstr[spos+1]=0;
            }
            else if (timestr){
                spos=strlen(timestr);
                if (annotbuffer[i]==21)
                    annotbuffer[i]=',';
                timestr[spos]=annotbuffer[i];
                timestr[spos+1]=0;
            }

        }//for
    return 1;
}

int EDFfilehandler::addannotation(float time, char *annotstr, int signum){
static char annotstr1[10000];
if (annotsignal==-1)
    return 0;
if (signum!=-1){
    char signame[256];
    getsignaldata(signum,signame);
    sprintf(annotstr1,"{%s}%s",signame,annotstr);
}
else
    strcpy(annotstr1,annotstr);
long recnum=time/recduration;
readannotbuffer(recnum);
int pos=0;
char c;
char lastzero=1;
do{
    c=annotbuffer[pos];
    if (++pos==samplesperrecord[annotsignal]*2)
        return 0;
    if (c==0){
        if (lastzero){//here to write;
            char str[1000];
            int i=0;
            char v=20;
            sprintf(&(annotbuffer[pos-1]),"%+.3f%c%s%c",time,20,annotstr1,20);
            //if (startpos-startstartpos+strlen(str)+strlen(annotstr1)+3>samplesperrecord[numofsignals-1])
            //    return 0;
            break;
        }
        lastzero=1;
    }
    else
        lastzero=0;
    }while(1);
writeannotbuffer(recnum);
return 1;
}

int EDFfilehandler::delannotation(int recnum, int annotnum){
int startpos=0,endpos;
readannotbuffer(recnum);
while (annotnum){
    if (annotbuffer[++startpos]==0)
        --annotnum;
}
endpos=startpos+1;
while (annotbuffer[endpos]!=0)
    endpos++;
for (int i=startpos;i<samplesperrecord[annotsignal]*2;i++)
    if (i+endpos-startpos<samplesperrecord[annotsignal]*2)
        annotbuffer[i]=annotbuffer[i+endpos-startpos];
    else
        annotbuffer[i]=0;
writeannotbuffer(recnum);
return 1;
}

int EDFfilehandler::delannotation(int recnum, char *annotstr){
int count=1;
int r=0;
char str[1024];
while (getannotation(recnum,count,str)!=0){
    if (strcmp(str,annotstr)==0)
        delannotation(recnum,count);
    count++;
    r++;
    }
return r;
}


