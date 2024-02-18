#ifndef VRAMSAVE_H
#define VRAMSAVE_H

#include <eetypes.h>
#include <libgraph.h>

typedef struct { // 0x10
	/* 0x0 */ char FileId[4];
	/* 0x4 */ u_char FormatVersion;
	/* 0x5 */ u_char FormatId;
	/* 0x6 */ u_short Pictures;
	/* 0x8 */ char Reserved[8];
} TIM2_FILEHEADER;

typedef struct { // 0x30
	/* 0x00 */ u_int TotalSize;
	/* 0x04 */ u_int ClutSize;
	/* 0x08 */ u_int ImageSize;
	/* 0x0c */ u_short HeaderSize;
	/* 0x0e */ u_short ClutColors;
	/* 0x10 */ u_char PictFormat;
	/* 0x11 */ u_char MipMapTextures;
	/* 0x12 */ u_char ClutType;
	/* 0x13 */ u_char ImageType;
	/* 0x14 */ u_short ImageWidth;
	/* 0x16 */ u_short ImageHeight;
	/* 0x18 */ u_long GsTex0;
	/* 0x20 */ u_long GsTex1;
	/* 0x28 */ u_int GsRegs;
	/* 0x2c */ u_int GsTexClut;
} TIM2_PICTUREHEADER;

typedef struct { // 0x40
	/* 0x00 */ TIM2_FILEHEADER t2h;
	/* 0x10 */ TIM2_PICTUREHEADER t2p;
} TIM2SETINFO;

typedef struct { // 0x38
    /* 0x00 */ u_char pad[2];
    /* 0x02 */ u_char id[2];
    /* 0x04 */ u_int tsize;
    /* 0x08 */ u_short reserved[2];
    /* 0x0c */ u_int ofsbit;
    /* 0x10 */ u_int header;
    /* 0x14 */ u_int imageW;
    /* 0x18 */ u_int imageH;
    /* 0x1c */ u_short planes;
    /* 0x1e */ u_short bitPerPixel;
    /* 0x20 */ u_int compression;
    /* 0x24 */ u_int imageSize;
    /* 0x28 */ u_int XpixcelPerMeter;
    /* 0x2c */ u_int YpixcelPerMeter;
    /* 0x30 */ u_int numberOfColors;
    /* 0x34 */ u_int colorsImportant;
    /* 0x38 */ u_char imageData[0];
} BMP_HEADER;

extern sceGsStoreImage gs_simage;

#endif // VRAMSAVE_H