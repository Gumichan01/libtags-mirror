</$objtype/mkfile
LIB=/$objtype/lib/libtags.a

OFILES=\
	8859.$O\
	flac.$O\
	id3genres.$O\
	id3v1.$O\
	id3v2.$O\
	m4a.$O\
	opus.$O\
	tags.$O\
	utf16.$O\
	vorbis.$O\
	wav.$O\

HFILES=\
	/sys/include/tags.h\
	tagspriv.h\

/sys/include/%.h: %.h
	cp $stem.h /sys/include/$stem.h

</sys/src/cmd/mksyslib
