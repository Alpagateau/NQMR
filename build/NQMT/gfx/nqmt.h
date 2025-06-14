
//{{BLOCK(nqmt)

//======================================================================
//
//	nqmt, 256x256@8, 
//	+ palette 256 entries, not compressed
//	+ 524 tiles (t|f reduced) not compressed
//	+ regular map (flat), not compressed, 32x32 
//	Total size: 512 + 33536 + 2048 = 36096
//
//	Time-stamp: 2025-06-10, 21:19:32
//	Exported by Cearn's GBA Image Transmogrifier, BlocksDS v1.10.0-blocks
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_NQMT_H
#define GRIT_NQMT_H

#define nqmtTilesLen 33536
extern const unsigned int nqmtTiles[8384];

#define nqmtMapLen 2048
extern const unsigned short nqmtMap[1024];

#define nqmtPalLen 512
extern const unsigned short nqmtPal[256];

#endif // GRIT_NQMT_H

//}}BLOCK(nqmt)
