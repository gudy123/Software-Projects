#include<iostream>
#include<string>
#include<sstream>
using namespace std;
#include "./gdal/gdal_priv.h"
#pragma comment(lib, "gdal_i.lib")



int main()
{
    char* mulPath = "American_MUL.bmp";
    char* panPath = "American_PAN.bmp";
    char* outPath = "American.tif";
    //ͼ��Ŀ�Ⱥ͸߶�
	int imgXlen, imgYlen;
	//ͼ�񲨶���
	int bandNum;
    //����ͼ��
	GDALDataset* mulPic,*panPic;
	//���ͼ��
	GDALDataset* outPic;
	//ͼ���ڴ�洢
	float *buffR,*buffG,*buffB,*buffP,*buffH,*buffS;
	//ע������
	GDALAllRegister();

	//��ͼ��
	mulPic = (GDALDataset*)GDALOpenShared(mulPath, GA_ReadOnly);
	panPic = (GDALDataset*)GDALOpenShared(panPath, GA_ReadOnly);
	//��ȡͼ���ȣ��߶ȣ�������
	imgXlen = mulPic->GetRasterXSize();
	imgYlen = mulPic->GetRasterYSize();
	bandNum = mulPic->GetRasterCount();
    //�������ͼ��
	outPic = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(outPath,imgXlen,imgYlen,bandNum,GDT_Byte,NULL);
  	//�����ȡ�Ľ��
	cout << "IMG  X Length:" << imgXlen << endl;
	cout << "IMG  Y Length:" << imgYlen << endl;
	cout << "Band Number:" << bandNum << endl;
	//���ٻ���ռ�
    buffR = (float *)CPLMalloc(imgXlen * imgYlen * sizeof(float));
    buffG = (float *)CPLMalloc(imgXlen * imgYlen * sizeof(float));
    buffB = (float *)CPLMalloc(imgXlen * imgYlen * sizeof(float));
    buffP = (float *)CPLMalloc(imgXlen * imgYlen * sizeof(float));
    buffH = (float *)CPLMalloc(imgXlen * imgYlen * sizeof(float));
    buffS = (float *)CPLMalloc(imgXlen * imgYlen * sizeof(float));
    //��ȡ�����ͼ���R G B��ȫɫͼ��P
    mulPic->GetRasterBand(1)->RasterIO(GF_Read,0,0,imgXlen,imgYlen,buffR,imgXlen,imgYlen,GDT_Float32,0,0);
    mulPic->GetRasterBand(2)->RasterIO(GF_Read,0,0,imgXlen,imgYlen,buffG,imgXlen,imgYlen,GDT_Float32,0,0);
    mulPic->GetRasterBand(3)->RasterIO(GF_Read,0,0,imgXlen,imgYlen,buffB,imgXlen,imgYlen,GDT_Float32,0,0);
    panPic->GetRasterBand(1)->RasterIO(GF_Read,0,0,imgXlen,imgYlen,buffP,imgXlen,imgYlen,GDT_Float32,0,0);
    //��ÿ�����ؽ��д���
    //�ȼ����H S��Ȼ��ת��ΪR G B��ת��ʱP�������е�I��I����������Ҳû�б�Ҫ����
    for(int i = 0 ;i < imgXlen * imgYlen ;i++){
        buffH[i] = -sqrt(2.0f)/6.0f * buffR[i] - sqrt(2.0f)/6.0f*buffG[i] + sqrt(2.0f)/3.0f*buffB[i];
        buffS[i] = 1.0f/sqrt(2.0f)*buffR[i] - 1/sqrt(2.0f)*buffG[i];

        buffR[i] = buffP[i] - 1.0f/sqrt(2.0f) * buffH[i] + 1.0f/sqrt(2.0f) * buffS[i];
        buffG[i] = buffP[i] - 1.0f/sqrt(2.0f) * buffH[i] - 1.0f/sqrt(2.0f) * buffS[i];
        buffB[i] = buffP[i] + sqrt(2.0f) * buffH[i];
    }
    //д��ͼ��
    outPic->GetRasterBand(1)->RasterIO(GF_Write,0,0,imgXlen,imgYlen,buffR,imgXlen,imgYlen,GDT_Float32,0,0);
    outPic->GetRasterBand(2)->RasterIO(GF_Write,0,0,imgXlen,imgYlen,buffG,imgXlen,imgYlen,GDT_Float32,0,0);
    outPic->GetRasterBand(3)->RasterIO(GF_Write,0,0,imgXlen,imgYlen,buffB,imgXlen,imgYlen,GDT_Float32,0,0);
    //�ͷŻ���
    CPLFree(buffR);
    CPLFree(buffG);
    CPLFree(buffB);
    CPLFree(buffP);
    CPLFree(buffH);
    CPLFree(buffS);
    //�ر�ͼ��
    GDALClose(panPic);
    GDALClose(mulPic);
    GDALClose(outPic);

	return 0;
}
