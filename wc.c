#include<stdio.h>
#include<string.h>
#include<dirent.h>
#define LEN_MAX 1020
int i,cnt,lineFlag;
int flagDo;	//参数是否正确
int flagNum=0;	//是否统计字符
int flagWord=0;	//是否统计单词
int flagLine=0;	//是否统计行数
int flagRec=0;	//是否递归
int flagMore=0;	//是否显示更多信息

int cntNum=0;	//字符数
int cntWord=0;	//单词数
int cntLine=0;	//行数
int cntMore[3];	//记录空行、代码行、注释行
char strTemp[LEN_MAX];	//临时存放字符串
int isWord(char c) {
	//return (c<='Z'&&c>='A')||(c<='z'&&c>='a');	//是否为字符
	return c!=' '&&c!='\t'&&c!='\n';
}
void statistics(char *dirPath, char *fileName) {
	int l;
	char *tempName;
	struct _finddata_t fileinfo;
	int handle=_findfirst(fileName,&fileinfo);;
	do {
		if(handle==-1) {
			printf("文件不存在！\n");
			break;
		}
		DIR *dir = NULL;
		struct dirent *entry;
		char *tdirPath=dirPath;
		strcpy(strTemp, tdirPath);
		strcat(strTemp, fileinfo.name);
		if((dir = opendir(strTemp))==NULL) {
			//是文件
			FILE *fp;
			fp=fopen(strTemp,"r");
			printf("-----文件名：%s-----\n",strTemp);
			char line[LEN_MAX];
			cntNum=0;
			cntWord=0;
			cntLine=0;
			cntMore[0]=cntMore[1]=cntMore[2]=0;
			while(!feof(fp)&&!ferror(fp)) {	//逐行读取
				strcpy(line,"\n");
				fgets(line,sizeof(line),fp);
				//printf("-%s=\n",line);
				if(flagNum) {
					for(i=0; i<strlen(line); i++) {	//逐字读取
						//printf("-%c=",line[i]);
						cntNum++;
					}
				}
				if(flagWord) {
					int wordFlag=0;
					for(i=0; i<strlen(line); i++) {
						if(isWord(line[i])) {
							if(!wordFlag) {	//若未被标记 
								wordFlag=1;
							}
						}else{
							if(wordFlag){	//若前面是字符 
								wordFlag=0;
								cntWord++;
							}
						}
					}
					cntWord+=wordFlag;	//处理最后一个是字符的情况 
				}
				if(flagLine) {
					cntLine++;	//每次读取一行时加一 
				}
				if(flagMore) {
					cnt=0;
					for(i=0; i<strlen(line); i++) {
						if(isWord(line[i]))
							cnt++;	 
					}
					if(cnt<2)	//若无可见字符或只有一个 
						cntMore[0]++;
					else {
						lineFlag=1;
						for(i=1; i<strlen(line); i++) {
							if(line[i]==line[i-1]&&line[i]=='/') {
								lineFlag=2;	//若存在//则为注释行 否则为代码行 
							}
						}
						cntMore[lineFlag]++;
					}
				}
			}
			fclose(fp);
			if(flagNum)
				printf("字符数：%d\n",cntNum-1);
			if(flagWord)
				printf("单词数：%d\n",cntWord);
			if(flagLine)
				printf("行数：%d\n",cntLine);
			if(flagMore) {
				printf("空行数：%d\n",cntMore[0]);
				printf("代码行数：%d\n",cntMore[1]);
				printf("注释行数：%d\n",cntMore[2]);
			}
		} else {
			//是文件夹
			if(flagRec) {	//是否递归
				strcpy(strTemp,dirPath);
				strcat(strTemp,fileinfo.name);
				strcat(strTemp,"/");	//指定下一层
				statistics(strTemp,fileinfo.name);	//递归处理下一层
			} else {
				printf("请加递归参数以处理文件夹\n");
			}
		}
	} while(!_findnext(handle,&fileinfo));
	_findclose(handle);
}
int main (int argc,char *argv[]) {
	int flagDo=argc-1;
	int i;
	char *fileName;
	for(i=1; i<argc-1; i++) {
		if(!strcmp(argv[i],"-c")) {
			flagNum=1;
		} else if(!strcmp(argv[i],"-w")) {
			flagWord=1;
		} else if(!strcmp(argv[i],"-l")) {
			flagLine=1;
		} else if(!strcmp(argv[i],"-s")) {
			flagRec=1;
		} else if(!strcmp(argv[i],"-a")) {
			flagMore=1;
		} else {
			flagDo=0;
		}
	}
	if(flagDo) {
		fileName=argv[argc-1];
		statistics("./",fileName);	//统计当前目录下指定的文件名
	} else {
		printf("参数有误！\n");
	}
	return 0;
}
