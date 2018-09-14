#include<stdio.h>
#include<string.h>
#include<dirent.h>
#define LEN_MAX 1020
int i,cnt,lineFlag;
int flagDo;	//�����Ƿ���ȷ
int flagNum=0;	//�Ƿ�ͳ���ַ�
int flagWord=0;	//�Ƿ�ͳ�Ƶ���
int flagLine=0;	//�Ƿ�ͳ������
int flagRec=0;	//�Ƿ�ݹ�
int flagMore=0;	//�Ƿ���ʾ������Ϣ

int cntNum=0;	//�ַ���
int cntWord=0;	//������
int cntLine=0;	//����
int cntMore[3];	//��¼���С������С�ע����
char strTemp[LEN_MAX];	//��ʱ����ַ���
int isWord(char c) {
	//return (c<='Z'&&c>='A')||(c<='z'&&c>='a');	//�Ƿ�Ϊ�ַ�
	return c!=' '&&c!='\t'&&c!='\n';
}
void statistics(char *dirPath, char *fileName) {
	int l;
	char *tempName;
	struct _finddata_t fileinfo;
	int handle=_findfirst(fileName,&fileinfo);;
	do {
		if(handle==-1) {
			printf("�ļ������ڣ�\n");
			break;
		}
		DIR *dir = NULL;
		struct dirent *entry;
		char *tdirPath=dirPath;
		strcpy(strTemp, tdirPath);
		strcat(strTemp, fileinfo.name);
		if((dir = opendir(strTemp))==NULL) {
			//���ļ�
			FILE *fp;
			fp=fopen(strTemp,"r");
			printf("-----�ļ�����%s-----\n",strTemp);
			char line[LEN_MAX];
			cntNum=0;
			cntWord=0;
			cntLine=0;
			cntMore[0]=cntMore[1]=cntMore[2]=0;
			while(!feof(fp)&&!ferror(fp)) {	//���ж�ȡ
				strcpy(line,"\n");
				fgets(line,sizeof(line),fp);
				//printf("-%s=\n",line);
				if(flagNum) {
					for(i=0; i<strlen(line); i++) {	//���ֶ�ȡ
						//printf("-%c=",line[i]);
						cntNum++;
					}
				}
				if(flagWord) {
					int wordFlag=0;
					for(i=0; i<strlen(line); i++) {
						if(isWord(line[i])) {
							if(!wordFlag) {	//��δ����� 
								wordFlag=1;
							}
						}else{
							if(wordFlag){	//��ǰ�����ַ� 
								wordFlag=0;
								cntWord++;
							}
						}
					}
					cntWord+=wordFlag;	//�������һ�����ַ������ 
				}
				if(flagLine) {
					cntLine++;	//ÿ�ζ�ȡһ��ʱ��һ 
				}
				if(flagMore) {
					cnt=0;
					for(i=0; i<strlen(line); i++) {
						if(isWord(line[i]))
							cnt++;	 
					}
					if(cnt<2)	//���޿ɼ��ַ���ֻ��һ�� 
						cntMore[0]++;
					else {
						lineFlag=1;
						for(i=1; i<strlen(line); i++) {
							if(line[i]==line[i-1]&&line[i]=='/') {
								lineFlag=2;	//������//��Ϊע���� ����Ϊ������ 
							}
						}
						cntMore[lineFlag]++;
					}
				}
			}
			fclose(fp);
			if(flagNum)
				printf("�ַ�����%d\n",cntNum-1);
			if(flagWord)
				printf("��������%d\n",cntWord);
			if(flagLine)
				printf("������%d\n",cntLine);
			if(flagMore) {
				printf("��������%d\n",cntMore[0]);
				printf("����������%d\n",cntMore[1]);
				printf("ע��������%d\n",cntMore[2]);
			}
		} else {
			//���ļ���
			if(flagRec) {	//�Ƿ�ݹ�
				strcpy(strTemp,dirPath);
				strcat(strTemp,fileinfo.name);
				strcat(strTemp,"/");	//ָ����һ��
				statistics(strTemp,fileinfo.name);	//�ݹ鴦����һ��
			} else {
				printf("��ӵݹ�����Դ����ļ���\n");
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
		statistics("./",fileName);	//ͳ�Ƶ�ǰĿ¼��ָ�����ļ���
	} else {
		printf("��������\n");
	}
	return 0;
}
