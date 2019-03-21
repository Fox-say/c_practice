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

#define SEEK_SET 0  //文件开头
#define SEEK_CUR 1  //当前位置
#define SEEK_END 2  //文件结尾
/*测试各数据类型长度及其结果*/
//printf("%d",sizeof(unsigned char));   结果= 1
//printf("%d",sizeof(unsigned short));  结果= 2
//printf("%d",sizeof(unsigned long));   结果= 4
typedef unsigned char BYTE;
/*定义WORD为两个字节的类型*/
typedef unsigned short WORD;
/*定义DWORD为四个字节的类型*/
typedef unsigned long DWORD;

typedef struct{/*位图文件头，14 byte，0000-000d*/
    WORD bType;         /*2 byte，0000-0001 文件标识符，必须为BM */
    DWORD bSize;        /*4 byte，0002-0005 文件的大小 */
    WORD bReserved1; /*2 byte，0006-0009 保留值,必须设置为0  */
    WORD bReserved2; /*2 byte， 保留值,必须设置为0 */
    DWORD bOffset;     /*4 byte，000a-000d 实际位图数据的偏移字节数，即前三个部分长度之和*/
}bmp_file_header;

typedef struct {/*位图信息头，40 byte，000e-0035*/
    DWORD bInfoSize;          /*4 byte，000e-0011 信息头的大小，为28H */
    DWORD bWidth;             /*4 byte，0012-0015 图像宽度，以像素为单位 */
    DWORD bHeight;            /*4 byte，0016-0019 图像高度，正数说明图像存储由下到上 */
    WORD   bPlanes;             /*2 byte，001a-001b 图像数据平面，BMP存储RGB数据，为1 */
    WORD   bBitCount;          /*2 byte，001c-001d 图像像素位数，可以是1，2，4，8，16，24，32 */
    DWORD bCompression;   /*4 byte，001e-0021 压缩类型，0-不压缩，1-RLE8，2-RLE4 */
    DWORD bmpImageSize;  /*4 byte，0022-0025 位图的大小,以字节为单位 */
    DWORD bXPelsPerMeter; /*4 byte，0026-0029 水平分辨率，像素/米 */
    DWORD bYPelsPerMeter; /*4 byte，002a-002d 垂直分辨率，像素/米 */
    DWORD bClrUsed;           /*4 byte，002e-0031 使用的色彩数，0表示默认值（2^像素位数） */
    DWORD bClrImportant;    /*4 byte，0032-0035 重要的颜色数，0表示所有颜色都是重要的 */
}bmp_inf;

typedef struct{/*调色板，大小由bBITCount决定,0036-x*/
    /*2色图像为8字节；
    16色图像为64字节；
    256色图像为1024字节。
    每4字节表示一种颜色，并以B（蓝色）、G（绿色）、R（红色）、alpha（32位位图的透明度值，一般不需要）。
    即首先4字节表示颜色号0的颜色，
    接下来表示颜色号1的颜色，依此类推。
    1、4、8位图像才使用调色板数据*/
    WORD rgbBlue; /* 蓝色强度 */
    WORD rgbGreen; /* 绿色强度 */
    WORD rgbRed; /* 红色强度 */
    WORD rgbReversed; /* 保留值 */
}rgb_quad;

typedef struct{/*像素坐标*/
    unsigned int pixel_x; //像素点x坐标
    unsigned int pixel_y; //像素点y坐标
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
    fseek( fp, 0, 0 );//设置文件指针位置
    fread( &fileHeader, sizeof(fileHeader), 1, fp );
    fread( &infoHeader, sizeof(infoHeader), 1, fp );
    Output_Image_Info( p_fileHeader, p_infoHeader );//获取并输出图像信息
    width = Get_Real_Width( p_infoHeader );

    fseek(fp, fileHeader.bOffset, SEEK_SET);//设置文件指针位置为图像数据区起始位置
    pixel_data = fgetc(fp);
    while (!feof(fp)){
        pixel_num++;
        if(pixel_data != 0){
            Get_Pixel_Coordinate(pixel_num,width,pixel_data);
        }
        pixel_data = fgetc(fp);
    }
    Get_Image_Center(&center_x,&center_y);
    printf("白色区域坐标为：");
    printf("( %d,%d )\n",center_x,center_y);
    fclose(fp);
    return 0;
}

void Output_Image_Info(bmp_file_header *fileHeader, bmp_inf *infoHeader){
    unsigned int offset = 0, width = 0, height = 0, bytesPerPixel = 0, bitCount = 0;
    long bmp_size = 0, bmpImageSize = 0;
    //计算并输出位图数据的偏移量，图像的大小，宽度和高度，每个像素点所占的字节
    bmp_size = fileHeader->bSize;
    offset = fileHeader->bOffset;
    bmpImageSize = infoHeader->bmpImageSize;
    width = infoHeader->bWidth;
    height = infoHeader->bHeight;
    bitCount = infoHeader->bBitCount;
    bytesPerPixel = infoHeader->bBitCount/8;
    printf("文件大小：");
    printf("%ld\n", bmp_size);
    printf("偏移地址：");
    printf("%d\n", offset);
    printf("图像大小：");
    printf("%ld\n", bmpImageSize);
    printf("图像宽度：");
    printf("%d\n", width);
    printf("图像高度：");
    printf("%d\n", height);
    printf("图像位数：");
    printf("%d\n", bitCount);
    printf("每像素点占用字节：");
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

