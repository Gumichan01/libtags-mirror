#include "tagspriv.h"

/* insane. */
static char *variants[] =
{
	"M.K.",
	"M!K!",
	"M&K!",
	"N.T.",
	"NSMS",
	"FLT4",
	"M\0\0\0",
	"8\0\0\0",
	"FEST",
	"FLT8",
	"CD81",
	"OCTA",
	"OKTA",
	"10CH",
	"16CN",
	"32CN",
};

int
tagmod(Tagctx *ctx)
{
	uchar d[20+4], o[20*UTFmax+1];
	int i;

	if(ctx->read(ctx, d, 20) != 20)
		return -1;
	if(ctx->seek(ctx, 1080, 0) != 1080)
		return -1;
	if(ctx->read(ctx, d+20, 4) != 4)
		return -1;
	for(i = 0; i < nelem(variants); i++){
		if(memcmp(d+20, variants[i], 4) == 0)
			break;
	}
	if(i >= nelem(variants))
		return -1;
	if(iso88591toutf8(o, sizeof(o), d, 20) > 0)
		txtcb(ctx, Ttitle, "", o);

	return 0;
}
