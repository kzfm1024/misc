//imageLoadSave.h

struct Pixel
{
	unsigned char b, g, r;
};

struct PixelA
{
	unsigned char b, g, r, a;
};

struct Palette
{
	unsigned char  r, g, b, a;
};

struct FILEHEADER
{
	unsigned short Type;       //file type
	unsigned long  Size;       //file size
	unsigned short Reserved1;  //予約
	unsigned short Reserved2;  //予約
	unsigned long  OffBits;    //ピクセルデータへのオフセット
};
	
struct INFOHEADER
{
	unsigned long  Size;          //BMPINFOHEADER size
	long           Width;         //bitmap の幅
	long           Height;        //bitmap の高さ
	unsigned short Planes;        //カラープレーン数
	unsigned short BitCount;      //色の数
	unsigned long  Compression;   //圧縮形式
	unsigned long  SizeImage;     //image size
	long           XPelsPerMeter; //水平解像度
	long           YPelsPerMeter; //垂直解像度
	unsigned long  ClrUsed;       //パレット数
	unsigned long  ClrImportant;  //カラーインデックス数
};

struct Bitmap
{
	FILEHEADER bf;
	INFOHEADER bi;
	struct Pixel* pixel;
	struct PixelA* pixelA;
};

static int getByte(FILE *f)
{
	return fgetc(f) & 0xFF;
}

static int get2Byte(FILE *f)
{
	return  fgetc(f) | fgetc(f) << 8; 
}

static int get4Byte(FILE *f)
{
	return fgetc(f) | fgetc(f) << 8 | fgetc(f) << 16 | fgetc(f) << 24;
}
//----------------------------------------------
void loadBitmap(Bitmap* bm, const char *filename)
{
	FILE *fp = fopen( filename, "rb");

	if(!fp){
		fprintf(stderr, "ファイルを開くことができません %s\n", filename);
	}

	
	if(get2Byte(fp) != 0x4D42)//BMか(B=42, M=4D)
	{
		fprintf(stderr, "%s はBMPファイルではない\n", filename);
		//exit(0);
	}

	bm->bf.Size = get4Byte(fp);
	bm->bf.Reserved1 = get2Byte(fp);
	bm->bf.Reserved2 = get2Byte(fp);
	bm->bf.OffBits = get4Byte(fp);
	bm->bi.Size = get4Byte(fp);
	bm->bi.Width = get4Byte(fp);
	bm->bi.Height = get4Byte(fp);
	bm->bi.Planes = get2Byte(fp);
	bm->bi.BitCount = get2Byte(fp);
	bm->bi.Compression = get4Byte(fp);
	bm->bi.SizeImage = get4Byte(fp);
	bm->bi.XPelsPerMeter = get4Byte(fp);
	bm->bi.YPelsPerMeter = get4Byte(fp);
	bm->bi.ClrUsed = get4Byte(fp);
	bm->bi.ClrImportant = get4Byte(fp);

	if(bm->bi.BitCount < 16)
	{
		bm->pixel = (Pixel*)malloc(3 * bm->bi.Width * bm->bi.Height);
		int paletteSize = 1 << bm->bi.BitCount;
		Palette* plt = (Palette*)malloc(sizeof(struct Palette) * paletteSize);
		int i, x, y;
		for(i = 0; i < paletteSize; i++){
			plt[i].b = fgetc(fp);
			plt[i].g = fgetc(fp);
			plt[i].r = fgetc(fp);
			plt[i].a = fgetc(fp);
		}
		if(bm->bi.BitCount == 1)//1bit mono
		{
			int c, d, e, k;
			int n = 0;
			for(y = 0; y < bm->bi.Height; y++)
				for(x = 0; x < bm->bi.Width / 8; x++)
				{
					c = fgetc(fp);
					for(k = 0; k < 8; k++)
					{	
						d = c & 0x80;//最上位ビット
						if(d == 0) e = 0;
						else e = 255;
						bm->pixel[n].b = (unsigned char)e;
						bm->pixel[n].g = (unsigned char)e;
						bm->pixel[n].r = (unsigned char)e;
						n++;
						c = c << 1;//1ビット左シフト
					}
				}
		}
		else if(bm->bi.BitCount == 8)//8bit mono or color
		{
			int c;
			
			for(y = 0; y < bm->bi.Height; y++)
				for(x = 0; x < bm->bi.Width; x++)
				{
					c = getByte(fp) ;
					Palette p = plt[c];
					bm->pixel[x + y * bm->bi.Width].b = (unsigned char)p.b;
					bm->pixel[x + y * bm->bi.Width].g = (unsigned char)p.g;
					bm->pixel[x + y * bm->bi.Width].r = (unsigned char)p.r;
				}
		}
		free(plt);
	}
	else if(bm->bi.BitCount == 24)//24bit color
	{
		bm->pixel = (Pixel*)malloc(3 * bm->bi.Width * bm->bi.Height);
			
		fread(bm->pixel, 1, 3*bm->bi.Width * bm->bi.Height, fp);
	}

	else//32bit color
	{
		bm->pixelA = (PixelA*)malloc(4 * bm->bi.Width * bm->bi.Height);

		fread(bm->pixelA, 1, 4 * bm->bi.Width * bm->bi.Height, fp);
	}
	fclose(fp);
}
//-----------------------------------------------
//24bit(RGB各256諧調)
void saveBitmap24(Bitmap* bm, const char *filename, 
		int imageWidth, int imageHeight, GLubyte image[][3])
{
	int i, j;

	FILE *fp = fopen( filename, "wb");
  if(fp == NULL)
  {
		printf("not file opened\n");
		return;
  }

//出力用のBITMAPFILEHEADERの作成 
  bm->bf.Type = 0x4D42;
	bm->bf.Size = 54 + imageWidth * imageHeight * 3;
  bm->bf.Reserved1 = 0;
  bm->bf.Reserved2 = 0;
	bm->bf.OffBits = 54;
  fwrite(&bm->bf.Type,sizeof(unsigned short),1,fp);
  fwrite(&bm->bf.Size,sizeof(unsigned long),1,fp);
  fwrite(&bm->bf.Reserved1,sizeof(unsigned short),1,fp);
  fwrite(&bm->bf.Reserved2,sizeof(unsigned short),1,fp);
  fwrite(&bm->bf.OffBits,sizeof(unsigned long),1,fp);

//出力用BITMAPINFOHEADERの作成
  bm->bi.Size = 40;
  bm->bi.Width = imageWidth;
  bm->bi.Height = imageHeight;
  bm->bi.Planes = 1;
	bm->bi.BitCount = 24;
  bm->bi.Compression = 0;//無圧縮
  bm->bi.SizeImage = 0;
  bm->bi.XPelsPerMeter = 3780;
  bm->bi.YPelsPerMeter = 3780;
	bm->bi.ClrUsed = 0;
	bm->bi.ClrImportant = 0;
	fwrite(&bm->bi, sizeof(INFOHEADER), 1, fp);
	//image dataの書き込み
	for (j = 0;j < imageHeight; j++)
	{
		for(i = 0; i < imageWidth; i++)
		{
			fputc(image[j * imageWidth + i][2], fp);//b
			fputc(image[j * imageWidth + i][1], fp);//g
			fputc(image[j * imageWidth + i][0], fp);//r
		}
	}
	fclose(fp);
}

//-----------------------------------------------
//32bit(RGB各256諧調)の保存
void saveBitmap32(Bitmap* bm, const char *filename, 
								int imageWidth, int imageHeight,
  							GLubyte image[][4])
{
	int i, j;

	FILE *fp = fopen( filename, "wb");
  if(fp == NULL)
  {
		printf("not file opened\n");
		return;
  }
//出力用のBITMAPFILEHEADERの作成 
  bm->bf.Type = 0x4D42;
	bm->bf.Size = 54 + imageWidth * imageHeight * 4;
//printf("bfSize = %d \n", (int)bm->bf.Size);
	bm->bf.OffBits = 54;
	bm->bi.BitCount = 32;

  bm->bf.Reserved1 = 0;
  bm->bf.Reserved2 = 0;

  fwrite(&bm->bf.Type,sizeof(unsigned short),1,fp);
  fwrite(&bm->bf.Size,sizeof(unsigned long),1,fp);
  fwrite(&bm->bf.Reserved1,sizeof(unsigned short),1,fp);
  fwrite(&bm->bf.Reserved2,sizeof(unsigned short),1,fp);
  fwrite(&bm->bf.OffBits,sizeof(unsigned long),1,fp);

//出力用BITMAPINFOHEADERの作成
  bm->bi.Size = 40;
  bm->bi.Width = imageWidth;
  bm->bi.Height = imageHeight;
  bm->bi.Planes = 1;
	bm->bi.BitCount = 32;
  bm->bi.Compression = 0;//無圧縮
  bm->bi.SizeImage = 0;
  bm->bi.XPelsPerMeter = 3780;
  bm->bi.YPelsPerMeter = 3780;
	bm->bi.ClrUsed = 0;
	bm->bi.ClrImportant = 0;
	fwrite(&bm->bi, sizeof(INFOHEADER), 1, fp);

	//image dataの書き込み
	for (j = 0;j < imageHeight; j++)
	{
		for(i = 0; i < imageWidth; i++)
		{
			fputc(image[j * imageWidth + i][2], fp);//b
			fputc(image[j * imageWidth + i][1], fp);//g
			fputc(image[j * imageWidth + i][0], fp);//r
			fputc(image[j * imageWidth + i][3], fp);//a
		}
	}
	fclose(fp);
}