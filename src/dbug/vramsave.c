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

#if 1
void VramSave(u_char *fname, int wsize, int hsize, int id) {
    int fd;
    u_char fname_tmp[64];
    u_long128 *dst1_pp;
    int i;
    
    sprintf(fname_tmp, "s_host:%s", fname);
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

INCLUDE_ASM(const s32, "dbug/vramsave", VramSaveBMP);

INCLUDE_ASM(const s32, "dbug/vramsave", VramSaveBMPDouble);

INCLUDE_ASM(const s32, "dbug/vramsave", VramTmpSave);

INCLUDE_ASM(const s32, "dbug/vramsave", VramTmpSaveOutBMP);

INCLUDE_RODATA(const s32, "dbug/vramsave", D_00390F80);
