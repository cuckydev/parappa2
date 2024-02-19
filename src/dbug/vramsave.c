#include "common.h"

#include "dbug/vramsave.h"

#include <sifdev.h>
#include <libgraph.h>
#include <malloc.h>

#if 0
sceGsStoreImage gs_simage;
/*static*/ TIM2SETINFO tim2setinfo = {
    {
        { 'T', 'I', 'M', '2' },
        4,
        0,
        1,
        { 0, 0, 0, 0, 0, 0, 0, 0},
    },
    {
        0, /* TotalSize */
        0, /* ClutSize */
        0, /* ImageSize */
        sizeof(TIM2_PICTUREHEADER), /* HeaderSize */
        0, /* ClutColors */
        0, /* PictFormat */
        1, /* MipMapTextures */
        0, /* ClutType */
        3, /* ImageType */
        0, /* ImageWidth */
        0, /* ImageHeight */
        0, /* GsTex0 */
        0, /* GsTex1 */
        0, /* GsRegs */
        0, /* GsTexClut */
    }
};
#else
extern TIM2SETINFO tim2setinfo;
#endif

static u_int BekiDat(u_int size) {
    u_int mot = 2;
    u_int i;
    
    for (i = 1; size > mot; i++) {
        mot = mot << 1;
    }

    return i;
}

#if 0
void VramSave(u_char *fname, int wsize, int hsize, int id) {
    int fd;
    u_char fname_tmp[64];
    u_long128 *dst1_pp;
    int i;
    
    sprintf(fname_tmp, "host:%s", fname);
    printf("vram save [%s]\n", fname_tmp);

    fd = sceOpen(fname_tmp, SCE_TRUNC | SCE_CREAT | SCE_WRONLY);
    if (fd < 0) {
        printf("file open error!![%s]\n", fname_tmp);
        return;
    }

    dst1_pp = malloc((wsize * hsize) * 4);

    sceGsSetDefStoreImage(
        &gs_simage,
        (id * wsize * hsize) / 64, wsize / 64,
        0,
        0, 0, wsize, hsize
    );
    FlushCache(WRITEBACK_DCACHE);
    if (sceGsExecStoreImage(&gs_simage, dst1_pp) < 0) {
        printf("vramsave Timeout error!!\n");
        free(dst1_pp);
        sceClose(fd);
        return;
    }
    sceGsSyncPath(0, 0);
                    
    tim2setinfo.t2p.ImageSize = (wsize * hsize) * 8;
    tim2setinfo.t2p.TotalSize = tim2setinfo.t2p.ImageSize + sizeof(TIM2_PICTUREHEADER);
    tim2setinfo.t2p.ImageWidth = wsize;
    tim2setinfo.t2p.ImageHeight = hsize * 2;
    tim2setinfo.t2p.GsTex0 = SCE_GS_SET_TEX0((wsize * hsize) / 32, wsize / 64, 0, BekiDat(wsize), BekiDat(hsize * 2), 0, 0, 0, 0, 0, 0, 0);

    sceLseek(fd, 0, 0);
    sceWrite(fd, &tim2setinfo, sizeof(tim2setinfo));
    sceLseek(fd, sizeof(tim2setinfo), 0);

    for (i = 0; i < hsize; i++) {
          sceWrite(fd, dst1_pp + ((i * wsize) / 4), wsize * 4);
          sceWrite(fd, dst1_pp + ((i * wsize) / 4), wsize * 4);
    }
    
    free(dst1_pp);
    sceClose(fd);
}
#else
INCLUDE_ASM(const s32, "dbug/vramsave", VramSave);
#endif

INCLUDE_RODATA(const s32, "dbug/vramsave", D_00390F30);

INCLUDE_RODATA(const s32, "dbug/vramsave", D_00390F48);

#if 0
void VramSaveBMP(u_char *fname, int wsize, int hsize, int id) {
	int fd;
	u_char fname_tmp[64];
	u_char *dst1_pp;
	u_char *tr_pp;
	u_char *img_pp;
	int i;
	int j;
	u_char *moto_pp;
	u_char *imgtr_pp;
    
    sprintf(fname_tmp, "host:%s", fname);
    printf("vram save BMP [%s]\n", fname_tmp);

    fd = sceOpen(fname_tmp, SCE_TRUNC | SCE_CREAT | SCE_WRONLY);
    if (fd < 0) {
        printf("file open error!![%s]\n", fname_tmp);
        return;
    }

    dst1_pp = malloc(wsize * hsize * 4);
    tr_pp = malloc(wsize * hsize * 3 + sizeof(BMP_HEADER));

    sceGsSetDefStoreImage(
        &gs_simage,
        (id * wsize * hsize) / 64, wsize / 64,
        0,
        0, 0, wsize, hsize
    );
    FlushCache(WRITEBACK_DCACHE);
    if (sceGsExecStoreImage(&gs_simage, (u_long128*)dst1_pp) < 0) {
        printf("vramsave Timeout error!!\n");
        free(tr_pp);
        free(dst1_pp);
        sceClose(fd);
        return;
    }
    sceGsSyncPath(0, 0);
    
    ((BMP_HEADER*)tr_pp)->tsize = (wsize * hsize * 3) + (sizeof(BMP_HEADER) - 2);
    ((BMP_HEADER*)tr_pp)->id[0] = 'B';
    ((BMP_HEADER*)tr_pp)->id[1] = 'M';
    ((BMP_HEADER*)tr_pp)->reserved[0] = 0;
    ((BMP_HEADER*)tr_pp)->reserved[1] = 0;
    ((BMP_HEADER*)tr_pp)->ofsbit = 0x36;
    ((BMP_HEADER*)tr_pp)->header = 0x28;
    ((BMP_HEADER*)tr_pp)->imageW = wsize;
    ((BMP_HEADER*)tr_pp)->imageH = hsize;
    ((BMP_HEADER*)tr_pp)->planes = 1;
    ((BMP_HEADER*)tr_pp)->bitPerPixel = 24;
    ((BMP_HEADER*)tr_pp)->compression = 0;
    ((BMP_HEADER*)tr_pp)->imageSize = wsize * hsize * 3;
    ((BMP_HEADER*)tr_pp)->XpixcelPerMeter = 0xec4;
    ((BMP_HEADER*)tr_pp)->YpixcelPerMeter = 0xec4;
    ((BMP_HEADER*)tr_pp)->numberOfColors = 0;
    ((BMP_HEADER*)tr_pp)->colorsImportant = 0;

    moto_pp = dst1_pp;
    for (i = 0; i < hsize; i++) {
        imgtr_pp = tr_pp + (hsize - i - 1) * wsize * 3 + sizeof(BMP_HEADER);
        for (j = 0; j < wsize; j++) {
            u_char r = moto_pp[0];
            u_char g = moto_pp[1];
            u_char b = moto_pp[2];
            moto_pp += 4;
            
            imgtr_pp[2] = r;
            imgtr_pp[1] = g;
            imgtr_pp[0] = b;
            imgtr_pp += 3;
        }
    }

    sceLseek(fd, 0, 0);
    sceWrite(fd, tr_pp + 2, wsize * hsize * 3 + (sizeof(BMP_HEADER) - 2));
    free(tr_pp);
    free(dst1_pp);
    sceClose(fd);
}
#else
INCLUDE_ASM(const s32, "dbug/vramsave", VramSaveBMP);
#endif

INCLUDE_ASM(const s32, "dbug/vramsave", VramSaveBMPDouble);

INCLUDE_ASM(const s32, "dbug/vramsave", VramTmpSave);

INCLUDE_ASM(const s32, "dbug/vramsave", VramTmpSaveOutBMP);

INCLUDE_RODATA(const s32, "dbug/vramsave", D_00390F80);
