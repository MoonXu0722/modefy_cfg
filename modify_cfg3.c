
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	char modify[500][512];
	char tmp[512] = {0};
	char data[512][3][512] = {0};
	int index = 0;
	char str[128] = {0};
	char row[500][512] = {0};
	FILE *fp = NULL;
	char *p = NULL;
	char aa[1024];
	char ip[20] = {0};
	char ip2[20] = {0};
	char ip3[20] = {0};
	char ip_arr[16][20] = {0};
	char dir[10] = {0};
	int suffix[16] = {0};
	int first = 0, second = 0;
	int i = 0;
	int m = 0;
	int k = 0;
	char ftpbuf[16][128] = {0};
	char tmp1[128] = {0}, tmp2[128] = {0};
	if(argc > 1)
	{
		strcpy(ip, argv[1]);
		printf("ip = %s\n", ip);
		p = strtok(ip, ".");
		if(!p) return -1;
		printf("p = %s\n", p);
		strcat(ip2, p);
		strcat(ip2, ".");
		p = strtok(NULL, ".");
		if(!p) return -1;
		printf("p = %s\n", p);
		strcat(ip2, p);
		strcat(ip2, ".");
		p = strtok(NULL, ".");
		if(!p) return -1;
		printf("p = %s\n", p);
		strcat(ip2, p);
		strcat(ip2, ".");
		p = strtok(NULL, ".");
		if(!p) return -1;
		printf("p = %s\n", p);
		first = atoi(p);
		if(argc > 2)
		{
			memset(ip, 0, sizeof(ip));
			strcpy(ip, argv[2]);
		p = strtok(ip, ".");
		if(!p) return -1;
		printf("p = %s\n", p);
		strcat(ip3, p);
		strcat(ip3, ".");
		p = strtok(NULL, ".");
		if(!p) return -1;
		printf("p = %s\n", p);
		strcat(ip3, p);
		strcat(ip3, ".");
		p = strtok(NULL, ".");
		if(!p) return -1;
		printf("p = %s\n", p);
		strcat(ip3, p);
		strcat(ip3, ".");
		p = strtok(NULL, ".");
		if(!p) return -1;
		printf("p = %s\n", p);
		second = atoi(p);
		}
		while(i < 16)
		{
			sprintf(dir, "ch%02d.cfg", i);
			if(argc > 2 && i > 7)
			{
				sprintf(ftpbuf[i], "ftpput %s%d ",ip3, second);
				sprintf(ip_arr[i], "%s%d", ip3, second);
				second ++;
			}
			else
			{
				sprintf(ftpbuf[i], "ftpput %s%d ",ip2, first);
				sprintf(ip_arr[i], "%s%d", ip2, first);
				first ++;
			}
			printf("%s\n", ip_arr[i]);
			i ++;			
		}
	}
	if(access("copy", 0) == -1)
	{
		system("mkdir copy");
		for(i = 0; i < 16; i++)
		{
			memset(str, 0, 128);
			sprintf(str, "cp ch%02d.cfg copy", i);
			system(str);
		}
	}
	fp = fopen("modify_cfg.txt", "r");
	if(!fp) 
	{
		printf("modify_cfg.txt not exist\n");
		return -1;
	}
	while(fgets(modify[index], 512, fp))
	{
		p = strtok(modify[index] , " ");
		if(!p) continue;
		strcpy(data[index][0] , p);
		p = strtok(NULL, " ");
		if(p)
			strcpy(data[index][1] , p);
		p = strtok(NULL, " ");
		if(p)
			strcpy(data[index][2] , p);
	//	printf("%s\n",data[index][2]);
		index++;
	}
	fclose(fp);
	for(i = 0; i < 16; i++)
	{
		memset(str, 0, 128);
		sprintf(str, "ch%02d.cfg", i);
		fp = fopen(str, "r");
		int j = 0;
		while(fgets(row[j], 512, fp))
		{
//			printf("rr:%s\n",row[j]);
			memset(tmp, 0, sizeof(tmp));
			strcpy(tmp, row[j]);
			p = strtok(tmp, " ");
			if(!p) continue;
			for(m = 0; m < index; m++)
			{
				if (strcmp(p, data[m][0]) == 0)
				{
					memset(row[j], 0, sizeof(row[j]));
					if(argc > 1 && (!strcmp(data[m][0], "channel_ip") || !strcmp(data[m][0], "channel_ip2")))
					{
						sprintf(row[j], "%s = %s\n", data[m][0], ip_arr[i]);
					}
					else
						sprintf(row[j], "%s %s %s", data[m][0], data[m][1], data[m][2]);
				}
			}
			j++;
		}
		fclose(fp);
		fp = fopen(str, "w");
		for(k = 0; strcmp(row[k], "") != 0; k++)
		{
			fputs(row[k], fp);
			//fwrite(row[k], 1, strlen(row[k] + 1), fp);
			//fwrite("\n", 1, 2, fp);
//			printf("k = %d\n",k);
		}
		fclose(fp);
	}
	if(argc == 4)
	{
		for(i = 0; i < 16; i++)
		{
			memset(tmp1, 0, sizeof(tmp1));
			sprintf(tmp1, "%s%s", ftpbuf[i], "dvb_ts_player");
			//printf("buf1:%s\n",tmp1);
			system(tmp1);
			memset(tmp1, 0, sizeof(tmp1));
			sprintf(tmp1, "%s%s", ftpbuf[i], "wdt_rst");
			//printf("buf2:%s\n",tmp1);
			system(tmp1);
			memset(tmp1, 0, sizeof(tmp1));
			sprintf(tmp1, "%s%s", ftpbuf[i], "config.ini");
			//printf("buf3:%s\n",tmp1);
			system(tmp1);
		}
	}
	printf("successful...\n");
}
