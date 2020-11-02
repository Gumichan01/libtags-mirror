#include "tagspriv.h"

#define le16u(d) (u16int)((d)[0] | (d)[1]<<8)

int
tagwav(Tagctx *ctx)
{
	uchar *d;
	int i;
	u32int csz;
	uvlong sz;

	d = (uchar*)ctx->buf;

	sz = 1;
	for(i = 0; i < 8 && sz > 0; i++){
		if(ctx->read(ctx, d, 4+4+(i?0:4)) != 4+4+(i?0:4))
			return -1;
		if(i == 0){
			if(memcmp(d, "RIFF", 4) != 0 || memcmp(d+8, "WAVE", 4) != 0)
				return -1;
			if((sz = leuint(d+4)) < 4)
				return -1;
			sz -= 4;
			continue;
		}else if(memcmp(d, "INFO", 4) == 0){
			ctx->seek(ctx, -4, 1);
			continue;
		}

		sz -= 4+4;
		csz = leuint(d+4);
		if(csz > sz)
			return -1;
		sz -= csz;

		if(i == 1){
			if(memcmp(d, "fmt ", 4) != 0 || csz < 16)
				return -1;
			if(ctx->read(ctx, d, 16) != 16)
				return -1;
			csz -= 16;
			ctx->channels = le16u(d+2);
			ctx->samplerate = leuint(d+4);
			ctx->duration = sz*1000 / leuint(d+8);
		}else if(memcmp(d, "LIST", 4) == 0){
			sz = csz - 4;
			continue;
		}else if(memcmp(d, "IART", 4) == 0){
			if(ctx->read(ctx, d, csz) != csz)
				return -1;
			d[csz] = 0;
			txtcb(ctx, Tartist, "", d);
			csz = 1;
		}else if(memcmp(d, "IPRD", 4) == 0){
			if(ctx->read(ctx, d, csz) != csz)
				return -1;
			d[csz] = 0;
			txtcb(ctx, Talbum, "", d);
			csz = 1;
		}else if(memcmp(d, "INAM", 4) == 0){
			if(ctx->read(ctx, d, csz) != csz)
				return -1;
			d[csz] = 0;
			txtcb(ctx, Ttitle, "", d);
			csz = 1;
		}else if(memcmp(d, "data", 4) == 0){
			break;
		}

		if(ctx->seek(ctx, csz, 1) < 0)
			return -1;
	}

	return i > 0 ? 0 : -1;
}
