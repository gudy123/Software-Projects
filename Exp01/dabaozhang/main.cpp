#include <iostream>
#include <cstdio>
using namespace std;
#include"./gdal/gdal_priv.h"
#pragma comment(lib,"gdal_i.lib")


int main()
{
	//����ͼ��
	GDALDataset* poSrcDS;
	//���ͼ��w
	GDALDataset* poDstDS;
	//ͼ��Ŀ�Ⱥ͸߶�
	int imgXlen, imgYlen,StartX,StartY,tmpXlen,tmpYlen;
	//����ͼ��·��
	char* srcPath = "input.jpg";
	//���ͼ��·��
	char* dstPath = "output.tif";
	//ͼ���ڴ�洢
	GByte* buffTmp,* buffTmp1;
	//ͼ�񲨶���
	int i,j, bandNum;
	//ע������
	GDALAllRegister();
	//��ͼ��
	poSrcDS = (GDALDataset*)GDALOpenShared(srcPath, GA_ReadOnly);
	 //��ȡͼ���ȣ��߶ȺͲ�������
	imgXlen = poSrcDS->GetRasterXSize();
	imgYlen = poSrcDS->GetRasterYSize();
	bandNum = poSrcDS->GetRasterCount();
	//�����ȡ�Ľ��
	/*cout << "Image X Length:" << imgXlen << endl;
	cout << "Image Y Length:" << imgYlen << endl;
	cout << "Band number:" << bandNum << endl;
	//����ͼ��Ŀ�Ⱥ͸߶ȷ����ڴ�

*/
buffTmp = (GByte*)CPLMalloc(imgXlen*imgYlen * sizeof(GByte));
	//�������ͼ��
	poDstDS = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(
		dstPath, imgXlen, imgYlen, bandNum, GDT_Byte, NULL);
		//һ�������ε����룬Ȼ��һ�������ε����
	for (i = 0; i < bandNum; i++)
		{
			poSrcDS->GetRasterBand(i + 1)->RasterIO(GF_Read,
				0, 0, imgXlen, imgYlen, buffTmp, imgXlen, imgYlen, GDT_Byte, 0, 0);
			poDstDS->GetRasterBand(i + 1)->RasterIO(GF_Write,
				0, 0, imgXlen, imgYlen, buffTmp, imgXlen, imgYlen, GDT_Byte, 0, 0);
			cout << "... ... band " << i << "processing ... ... \n" ;
//			printf("... ... band %d processing ... ... \n", i);
		}
	//��ʼλ������
	StartX = 100;
	StartY = 100;
	//�����Ⱥ͸߶�
	tmpXlen = 200;
	tmpYlen = 150;
	//�����ڴ�
	buffTmp1 = (GByte*)CPLMalloc(tmpXlen*tmpYlen*sizeof(GByte));
	//��ȡ��ɫͨ��������buffTmp��
	poSrcDS->GetRasterBand(1)->RasterIO(GF_Read,
                                     StartX,StartY,tmpXlen,tmpYlen,buffTmp1,tmpXlen,tmpYlen,GDT_Byte,0,0);
    //����������������Ϊ255
    for(j=0;j < tmpYlen; j ++)
    {
        for(i=0;i<tmpXlen;i++)
        {
            buffTmp1[j*tmpXlen+i] = (GByte)255;
        }
    }
//����д��poDstDS
poDstDS->GetRasterBand(1)->RasterIO(GF_Write,
                                    StartX,StartY,tmpXlen,tmpYlen,buffTmp1,tmpXlen,tmpYlen,GDT_Byte,0,0);



	//����ڴ�
	CPLFree(buffTmp);
    CPLFree(buffTmp1);

	//�ر�dataset
	GDALClose(poDstDS);
	GDALClose(poSrcDS);
//	system("PAUSE");
    return 0;
}
