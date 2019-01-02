#include<iostream>
#include<string>
#include<sstream>
using namespace std;
#include "./gdal/gdal_priv.h"
#pragma comment(lib, "gdal_i.lib")


int main()
{
    char* mulPath = "Mul_large.tif";
    char* panPath = "Pan_large.tif";
    char* outPath = "Out_large2.tif";
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

	outPic = GetGDALDriverManager()->GetDriverByName("GTiff")->Create(outPath,imgXlen,imgYlen,bandNum,GDT_Byte,NULL);
  	//�����ȡ�Ľ��
	cout << "IMG  X Length:" << imgXlen << endl;
	cout << "IMG  Y Length:" << imgYlen << endl;
	cout << "Band Number:" << bandNum << endl;
	int tx = 256,ty = imgYlen;
	//�����ڴ�ռ�
    buffR = (float *)CPLMalloc(tx * ty * sizeof(float));
    buffG = (float *)CPLMalloc(tx * ty * sizeof(float));
    buffB = (float *)CPLMalloc(tx * ty * sizeof(float));
    buffP = (float *)CPLMalloc(tx * ty * sizeof(float));
    buffH = (float *)CPLMalloc(tx * ty * sizeof(float));
    buffS = (float *)CPLMalloc(tx * ty * sizeof(float));
    /*
    //�ֿ鴦��ת��
	for(int i = 0;i < imgXlen;i+=256){
        for(int j = 0 ; j < imgYlen;j+=256){
            if(i + ty > imgXlen || j + tx > imgYlen)
                break;
            printf("ͼ���x:%d  y:%d\n",i,j);
            //���뻺��
            mulPic->GetRasterBand(1)->RasterIO(GF_Read,i,j,tx,ty,buffR,tx,ty,GDT_Float32,0,0);
            mulPic->GetRasterBand(2)->RasterIO(GF_Read,i,j,tx,ty,buffG,tx,ty,GDT_Float32,0,0);
            mulPic->GetRasterBand(3)->RasterIO(GF_Read,i,j,tx,ty,buffB,tx,ty,GDT_Float32,0,0);
            panPic->GetRasterBand(1)->RasterIO(GF_Read,i,j,tx,ty,buffP,tx,ty,GDT_Float32,0,0);
            //ת������
            for(int k = 0;k < tx * ty ;k++){
                buffH[k] = -sqrt(2.0f)/6.0f * buffR[k] - sqrt(2.0f)/6.0f*buffG[k] + sqrt(2.0f)/3.0f*buffB[k];
                buffS[k] = 1.0f/sqrt(2.0f)*buffR[k] - 1/sqrt(2.0f)*buffG[k];

                buffR[k] = buffP[k] - 1.0f/sqrt(2.0f) * buffH[k] + 1.0f/sqrt(2.0f) * buffS[k];
                buffG[k] = buffP[k] - 1.0f/sqrt(2.0f) * buffH[k] - 1.0f/sqrt(2.0f) * buffS[k];
                buffB[k] = buffP[k] + sqrt(2.0f) * buffH[k];
            }
            //д����ͼ��
            outPic->GetRasterBand(1)->RasterIO(GF_Write,i,j,tx,ty,buffR,tx,ty,GDT_Float32,0,0);
            outPic->GetRasterBand(2)->RasterIO(GF_Write,i,j,tx,ty,buffG,tx,ty,GDT_Float32,0,0);
            outPic->GetRasterBand(3)->RasterIO(GF_Write,i,j,tx,ty,buffB,tx,ty,GDT_Float32,0,0);
        }
	}*/
    //���д���  256��һ��
    for(int i = 0 ; i < imgXlen;i+=256){
        printf("ͼ���x:%d  y:%d\n",i,0);
        if(i + tx > imgXlen)
            break;


        //���뻺��
        mulPic->GetRasterBand(1)->RasterIO(GF_Read,i,0,tx,ty,buffR,tx,ty,GDT_Float32,0,0);
        mulPic->GetRasterBand(2)->RasterIO(GF_Read,i,0,tx,ty,buffG,tx,ty,GDT_Float32,0,0);
        mulPic->GetRasterBand(3)->RasterIO(GF_Read,i,0,tx,ty,buffB,tx,ty,GDT_Float32,0,0);
        panPic->GetRasterBand(1)->RasterIO(GF_Read,i,0,tx,ty,buffP,tx,ty,GDT_Float32,0,0);
        //ת������
        for(int k = 0;k < tx * ty ;k++){
            buffH[k] = -sqrt(2.0f)/6.0f * buffR[k] - sqrt(2.0f)/6.0f*buffG[k] + sqrt(2.0f)/3.0f*buffB[k];
            buffS[k] = 1.0f/sqrt(2.0f)*buffR[k] - 1/sqrt(2.0f)*buffG[k];

            buffR[k] = buffP[k] - 1.0f/sqrt(2.0f) * buffH[k] + 1.0f/sqrt(2.0f) * buffS[k];
            buffG[k] = buffP[k] - 1.0f/sqrt(2.0f) * buffH[k] - 1.0f/sqrt(2.0f) * buffS[k];
            buffB[k] = buffP[k] + sqrt(2.0f) * buffH[k];
        }
        //д����ͼ��
        outPic->GetRasterBand(1)->RasterIO(GF_Write,i,0,tx,ty,buffR,tx,ty,GDT_Float32,0,0);
        outPic->GetRasterBand(2)->RasterIO(GF_Write,i,0,tx,ty,buffG,tx,ty,GDT_Float32,0,0);
        outPic->GetRasterBand(3)->RasterIO(GF_Write,i,0,tx,ty,buffB,tx,ty,GDT_Float32,0,0);
    }
    //�ͷŻ���
    CPLFree(buffR);
    CPLFree(buffG);
    CPLFree(buffB);
    CPLFree(buffP);
    CPLFree(buffH);
    CPLFree(buffS);
    //�ر����ݼ�
    GDALClose(panPic);
    GDALClose(mulPic);
    GDALClose(outPic);

	return 0;
}
