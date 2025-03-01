/******************
* "Zdini"         *
* *****************
* @ Auctor: Zuc   *
* @ Version: 3.0  *
******************/

#if defined _Zdini_included
#endinput
#endif
#define _Zdini_included
#pragma library dutils
#include <dutils>

native gpci(playerid, const serial[ ], maxlen);
#define COL_EASY           "{FFF1AF}"
#define COL_WHITE          "{FFFFFF}"
#define COL_BLACK          "{0E0101}"
#define COL_GREY           "{C3C3C3}"
#define COL_GREEN          "{6EF83C}"
#define COL_RED            "{F81414}"
#define COL_YELLOW         "{F3FF02}"
#define COL_ORANGE         "{FFAF00}"
#define COL_LIME           "{B7FF00}"
#define COL_CYAN           "{00FFEE}"
#define COL_LIGHTBLUE      "{00C0FF}"
#define COL_BLUE           "{0049FF}"
#define COL_MAGENTA        "{F300FF}"
#define COL_VIOLET         "{B700FF}"
#define COL_PINK           "{FF00EA}"
#define COL_MARONE         "{A90202}"
#define COL_CMD            "{B8FF02}"
#define COL_PARAM          "{3FCD02}"
#define COL_SERVER         "{AFE7FF}"
#define COL_VALUE          "{A3E4FF}"
#define COL_RULE           "{F9E8B7}"
#define COL_RULE2          "{FBDF89}"
#define COL_RWHITE         "{FFFFFF}"
#define COL_LGREEN         "{C9FFAB}"
#define COL_LRED           "{FFA1A1}"
#define COL_LRED2          "{C77D87}"

stock udb_hash(buf[]){
  new length=strlen(buf);
  new s1=1; new s2=0;new n;
  for(n=0; n<length; n++){
    s1=(s1+buf[n]) %65521;
    s2=(s2+s1) %65521;}
  return (s2<<16)+s1;}

stock dini_Exists(filename[]){
	if(fexist(filename)) return true;
	return false;}

stock dini_Remove(filename[]){
	if(!fexist(filename)) return false;
	fremove(filename);
	return true;}

stock dini_Create(filename[]){
	new File:fhnd;
	if(fexist(filename)) return false;
	fhnd=fopen(filename,io_write);
	fclose(fhnd);
	return true;}
	
stock dini_PRIVATE_ExtractKey(line[]){
	new tmp[255]; tmp[0]=0;
	if(strfind(line,"=",true)==-1) return tmp;
	set(tmp,strlower(ret_memcpy(line,0,strfind(line,"=",true))));
	return tmp;}

stock dini_PRIVATE_ExtractValue(line[]){
    new tmp[255];
    tmp[0]=0;
    if(strfind(line,"=",true)==-1){
        return tmp;}
    set(tmp,ret_memcpy(line,strfind(line,"=",true)+1,strlen(line)));
    return tmp;}

stock dini_Set(filename[],key[],value[]){
	new File:fohnd, File:fwhnd;
	new bool:wasset=false;
	new tmpres[255];
	if(key[0]==0) return false;
	format(tmpres,sizeof(tmpres),"%s.part",filename);
	fohnd=fopen(filename,io_read);
	if(!fohnd) return false;
	fremove(tmpres);
	fwhnd=fopen(tmpres,io_write);
	while(fread(fohnd,tmpres)){
		StripNewLine(tmpres);
		if((!wasset)&&(equal(dini_PRIVATE_ExtractKey(tmpres),key,true))){
			format(tmpres,sizeof(tmpres),"%s=%s",key,value);
			wasset=true;}
		fwrite(fwhnd,tmpres);
		fwrite(fwhnd,"\r\n");}
	if(!wasset){
		format(tmpres,sizeof(tmpres),"%s=%s",key,value);
		fwrite(fwhnd,tmpres);
		fwrite(fwhnd,"\r\n");}
	fclose(fohnd);
	fclose(fwhnd);
	format(tmpres,sizeof(tmpres),"%s.part",filename);
	if(fcopytextfile(tmpres,filename)){
		return fremove(tmpres);}
	return false;}

stock dini_IntSet(filename[],key[],value){
   new valuestring[255];
   format(valuestring,sizeof(valuestring),"%d",value);
   return dini_Set(filename,key,valuestring);}

stock dini_Int(filename[],key[]){
   return strval(dini_Get(filename,key));}

stock dini_FloatSet(filename[],key[],Float:value) {
   new valuestring[255];
   format(valuestring,sizeof(valuestring),"%f",value);
   return dini_Set(filename,key,valuestring);}

stock dini_BoolSet(filename[],key[],value) {
   new valuestring[255];
   format(valuestring,sizeof(valuestring),"%d",value);
   return dini_Set(filename,key,valuestring);}

stock dini_Unset(filename[],key[]) {
	new File:fohnd, File:fwhnd;
	new tmpres[255];
	format(tmpres,sizeof(tmpres),"%s.part",filename);
	fohnd=fopen(filename,io_read);
	if(!fohnd) return false;
	fremove(tmpres);
	fwhnd=fopen(tmpres,io_write);
	while (fread(fohnd,tmpres)) {
		StripNewLine(tmpres);
		if (equal(dini_PRIVATE_ExtractKey(tmpres),key,true)) {
			/* We've got what needs to be removed! */
		}else{
			format(tmpres,sizeof(tmpres),"%s",tmpres);
			fwrite(fwhnd,tmpres);
			fwrite(fwhnd,"\r\n");}}
	fclose(fohnd);
	fclose(fwhnd);
	format(tmpres,sizeof(tmpres),"%s.part",filename);
	if(fcopytextfile(tmpres,filename)){
		return fremove(tmpres);}
	return false;}

stock dini_Get(filename[],key[]){
	new File:fohnd;
	new tmpres[MAX_STRING];
	new tmpres2[MAX_STRING];
	tmpres[0]=0;
	fohnd=fopen(filename,io_read);
	if(!fohnd) return tmpres;
	while(fread(fohnd,tmpres)){
		StripNewLine(tmpres);
		if(equal(dini_PRIVATE_ExtractKey(tmpres),key,true)){
			/* We've got what we need */
			tmpres2[0]=0;
			strcat(tmpres2,dini_PRIVATE_ExtractValue(tmpres));
			fclose(fohnd);
			return tmpres2;}}
	fclose(fohnd);
	return tmpres;}

stock dini_Isset(filename[],key[]){
	new File:fohnd;
	new tmpres[255];
	fohnd=fopen(filename,io_read);
	if(!fohnd) return false;
	while(fread(fohnd,tmpres)) {
		StripNewLine(tmpres);
		if(equal(dini_PRIVATE_ExtractKey(tmpres),key,true)) {
			/* We've got what we need */
			fclose(fohnd);
			return true;}}
	fclose(fohnd);
	return false;}
