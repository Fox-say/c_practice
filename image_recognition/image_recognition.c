/*
 * main.c
 *
 *  Created on: 2016-04-23
 *  Author: Isaac
 *  Bitmap file include four parts,which are bmp file header,
    bitmap information,color palette and bitmap data.

    1 represent white,while 0 represent black in the single bit bitmap.

    A 240*184 bitmap file has 44160 pixels,which amount to 5520(30*128) bytes.But in windows,
    the byte num of pixel in a line must be integral multiple of 4. So we can see the
    bytes of the bitmap is 5888(32*128).The redundant bit if full of 0.
 */

#include <stdio.h>
#include <stdlib.h>
#pragma pack(2)

#define SEEK_SET 0  //�ļ���ͷ
#define SEEK_CUR 1  //��ǰλ��
#define SEEK_END 2  //�ļ���β
/*���Ը��������ͳ��ȼ�����*/
//printf("%d",sizeof(unsigned char));   ���= 1
//printf("%d",sizeof(unsigned short));  ���= 2
//printf("%d",sizeof(unsigned long));   ���= 4
typedef unsigned char BYTE;
/*����WORDΪ�����ֽڵ�����*/
typedef unsigned short WORD;
/*����DWORDΪ�ĸ��ֽڵ�����*/
typedef unsigned long DWORD;

typedef struct{/*λͼ�ļ�ͷ��14 byte��0000-000d*/
    WORD bType;         /*2 byte��0000-0001 �ļ���ʶ��������ΪBM */
    DWORD bSize;        /*4 byte��0002-0005 �ļ��Ĵ�С */
    WORD bReserved1; /*2 byte��0006-0009 ����ֵ,��������Ϊ0  */
    WORD bReserved2; /*2 byte�� ����ֵ,��������Ϊ0 */
    DWORD bOffset;     /*4 byte��000a-000d ʵ��λͼ���ݵ�ƫ���ֽ�������ǰ�������ֳ���֮��*/
}bmp_file_header;

typedef struct {/*λͼ��Ϣͷ��40 byte��000e-0035*/
    DWORD bInfoSize;          /*4 byte��000e-0011 ��Ϣͷ�Ĵ�С��Ϊ28H */
    DWORD bWidth;             /*4 byte��0012-0015 ͼ���ȣ�������Ϊ��λ */
    DWORD bHeight;            /*4 byte��0016-0019 ͼ��߶ȣ�����˵��ͼ��洢���µ��� */
    WORD   bPlanes;             /*2 byte��001a-001b ͼ������ƽ�棬BMP�洢RGB���ݣ�Ϊ1 */
    WORD   bBitCount;          /*2 byte��001c-001d ͼ������λ����������1��2��4��8��16��24��32 */
    DWORD bCompression;   /*4 byte��001e-0021 ѹ�����ͣ�0-��ѹ����1-RLE8��2-RLE4 */
    DWORD bmpImageSize;  /*4 byte��0022-0025 λͼ�Ĵ�С,���ֽ�Ϊ��λ */
    DWORD bXPelsPerMeter; /*4 byte��0026-0029 ˮƽ�ֱ��ʣ�����/�� */
    DWORD bYPelsPerMeter; /*4 byte��002a-002d ��ֱ�ֱ��ʣ�����/�� */
    DWORD bClrUsed;           /*4 byte��002e-0031 ʹ�õ�ɫ������0��ʾĬ��ֵ��2^����λ���� */
    DWORD bClrImportant;    /*4 byte��0032-0035 ��Ҫ����ɫ����0��ʾ������ɫ������Ҫ�� */
}bmp_inf;

typedef struct{/*��ɫ�壬��С��bBITCount����,0036-x*/
    /*2ɫͼ��Ϊ8�ֽڣ�
    16ɫͼ��Ϊ64�ֽڣ�
    256ɫͼ��Ϊ1024�ֽڡ�
    ÿ4�ֽڱ�ʾһ����ɫ������B����ɫ����G����ɫ����R����ɫ����alpha��32λλͼ��͸����ֵ��һ�㲻��Ҫ����
    ������4�ֽڱ�ʾ��ɫ��0����ɫ��
    ��������ʾ��ɫ��1����ɫ���������ơ�
    1��4��8λͼ���ʹ�õ�ɫ������*/
    WORD rgbBlue; /* ��ɫǿ�� */
    WORD rgbGreen; /* ��ɫǿ�� */
    WORD rgbRed; /* ��ɫǿ�� */
    WORD rgbReversed; /* ����ֵ */
}rgb_quad;

typedef struct{/*��������*/
    unsigned int pixel_x; //���ص�x����
    unsigned int pixel_y; //���ص�y����
}pixel_coordinate;

pixel_coordinate coordinate[500] = {};
unsigned char COOR_num = 0;

void Output_Image_Info(bmp_file_header *,bmp_inf *);
unsigned int Get_Real_Width(bmp_inf *);
void Get_Pixel_Coordinate(long, unsigned int, WORD);
void Get_Image_Center(unsigned int *,unsigned int *);


int main(void){
    unsigned int center_x=0,center_y=0;
    long pixel_num = 0;
    unsigned int width = 0;
    FILE *fp;
    bmp_file_header fileHeader,*p_fileHeader;
    bmp_inf infoHeader,*p_infoHeader;
    WORD pixel_data = 0;
    p_fileHeader = &fileHeader;
    p_infoHeader = &infoHeader;
    if((fp = fopen("1.bmp", "rb")) == NULL){
        printf("Cann't open the file!\n");
        exit(0);
    }
    fseek( fp, 0, 0 );//�����ļ�ָ��λ��
    fread( &fileHeader, sizeof(fileHeader), 1, fp );
    fread( &infoHeader, sizeof(infoHeader), 1, fp );
    Output_Image_Info( p_fileHeader, p_infoHeader );//��ȡ�����ͼ����Ϣ
    width = Get_Real_Width( p_infoHeader );

    fseek(fp, fileHeader.bOffset, SEEK_SET);//�����ļ�ָ��λ��Ϊͼ����������ʼλ��
    pixel_data = fgetc(fp);
    while (!feof(fp)){
        pixel_num++;
        if(pixel_data != 0){
            Get_Pixel_Coordinate(pixel_num,width,pixel_data);
        }
        pixel_data = fgetc(fp);
    }
    Get_Image_Center(&center_x,&center_y);
    printf("��ɫ��������Ϊ��");
    printf("( %d,%d )\n",center_x,center_y);
    fclose(fp);
    return 0;
}

void Output_Image_Info(bmp_file_header *fileHeader, bmp_inf *infoHeader){
    unsigned int offset = 0, width = 0, height = 0, bytesPerPixel = 0, bitCount = 0;
    long bmp_size = 0, bmpImageSize = 0;
    //���㲢���λͼ���ݵ�ƫ������ͼ��Ĵ�С����Ⱥ͸߶ȣ�ÿ�����ص���ռ���ֽ�
    bmp_size = fileHeader->bSize;
    offset = fileHeader->bOffset;
    bmpImageSize = infoHeader->bmpImageSize;
    width = infoHeader->bWidth;
    height = infoHeader->bHeight;
    bitCount = infoHeader->bBitCount;
    bytesPerPixel = infoHeader->bBitCount/8;
    printf("�ļ���С��");
    printf("%ld\n", bmp_size);
    printf("ƫ�Ƶ�ַ��");
    printf("%d\n", offset);
    printf("ͼ���С��");
    printf("%ld\n", bmpImageSize);
    printf("ͼ���ȣ�");
    printf("%d\n", width);
    printf("ͼ��߶ȣ�");
    printf("%d\n", height);
    printf("ͼ��λ����");
    printf("%d\n", bitCount);
    printf("ÿ���ص�ռ���ֽڣ�");
    printf("%d\n", bytesPerPixel);
}

unsigned int Get_Real_Width(bmp_inf *infoHeader){
    unsigned int real_width = 0;
    real_width = infoHeader -> bWidth;
    while((real_width % 32)!= 0)
        real_width++;
    real_width/=8;
    return real_width;
}

void Get_Pixel_Coordinate(long num, unsigned int width,WORD data){
    WORD temp = 0x01;
    unsigned int x_data = 0;
    unsigned int y_data = 0;
    x_data = num % width;
    if(x_data == 32 )
        y_data = num / width;
    else
        y_data = num / width + 1;
    x_data--;
    x_data *=8;
    while(temp){
        if(data & temp){
            x_data += 1;
            coordinate[COOR_num].pixel_x = x_data;
            coordinate[COOR_num++].pixel_y = y_data;
        }
        temp <<= 1;
    }
}

void Get_Image_Center(unsigned int *center_x,unsigned int *center_y){
    unsigned int x_max=0,x_min=0,y_max=0,y_min=0;
    unsigned int temp_num = 0;
    if(COOR_num == 0)
        exit(0);
    x_max = coordinate[0].pixel_x;
    x_min = coordinate[0].pixel_x;
    y_max = coordinate[0].pixel_y;
    y_min = coordinate[0].pixel_y;
    temp_num++;
    while(temp_num < COOR_num){
        if(coordinate[temp_num].pixel_x > x_max)
            x_max = coordinate[temp_num].pixel_x;
        if(coordinate[temp_num].pixel_x < x_min)
            x_min = coordinate[temp_num].pixel_x;
        if(coordinate[temp_num].pixel_y > y_max)
            y_max = coordinate[temp_num].pixel_y;
        if(coordinate[temp_num].pixel_y < y_min)
            y_min = coordinate[temp_num].pixel_y;
        temp_num++;
    }
    *center_x = (x_max + x_min)/2;
    *center_y = (y_max + y_min)/2;
}

