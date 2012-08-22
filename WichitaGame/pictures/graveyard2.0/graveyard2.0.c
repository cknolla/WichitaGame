#ifndef _testmapGfx_c
#define _testmapGfx_c


#ifndef _GFX_BITMAP_DATA
#define _GFX_BITMAP_DATA
/* structure with bitmap data */
typedef struct GFX_BITMAP_DATA
{
  char * BitmapIdentifier;
  int BitmapWidth;
  int BitmapHeight;
  int TileWidth;
  int TileHeight;
  int TileCountX;
  int TileCountY;
  int TransparentX;
  int TransparentY;
  int Sequences;
  int ** SequenceData;
  int * SequenceLength;
  int * SequencePosition;
  int * SequenceCounter;

} GFX_BITMAP_DATA;
#endif


#ifndef _GFX_EMPTY
#define _GFX_EMPTY
static int Empty[] = {0};
#endif


#define GfxtestsetMap1MapWidth (17)
#define GfxtestsetMap1MapHeight (17)

signed short testsetMap1BottomLayer[17][17] =
  {{ 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
  { 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2},
  { 2, 2, 2, 0, 0, 0, 0, 0,-1, 0, 0, 0, 0, 0, 2, 2, 2},
  { 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2},
  { 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2},
  { 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2},
  { 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2},
  { 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2},
  { 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2},
  { 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2},
  { 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2},
  { 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2},
  { 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2},
  { 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2},
  { 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2},
  { 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2},
  { 2, 2, 2, 2, 2, 2, 2, 2, 0, 2, 2, 2, 2, 2, 2, 2, 2}};

signed short testsetMap1MiddleLayer[17][17] =
  {{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
  {-1,-1,-1,-1,-1,-1,-1,-1, 5,-1,-1,-1,-1,-1,-1,-1,-1},
  {-1,-1,-1,-1, 5,-1, 5,-1,-1,-1, 5,-1, 5,-1,-1,-1,-1},
  {-1,-1,-1,-1,-1,-1,-1,-1, 6,-1,-1,-1,-1,-1,-1,-1,-1},
  {-1,-1,-1,-1,-1,-1,-1,-1, 6,-1,-1,-1,-1,-1,-1,-1,-1},
  {-1,-1,-1, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,-1,-1,-1},
  {-1,-1,-1, 6,-1, 4,-1,-1, 4,-1,-1, 4,-1, 6,-1,-1,-1},
  {-1,-1,-1, 6,-1,-1,-1,-1,-1,-1,-1,-1,-1, 6,-1,-1,-1},
  {-1,-1,-1, 6,-1,-1,-1,-1,-1,-1,-1,-1,-1, 6,-1,-1,-1},
  {-1,-1,-1, 6,-1, 4,-1,-1, 4,-1,-1, 4,-1, 6,-1,-1,-1},
  {-1,-1,-1, 6,-1,-1,-1,-1,-1,-1,-1,-1,-1, 6,-1,-1,-1},
  {-1,-1,-1, 6, 6, 6,-1, 5,-1, 5,-1, 6, 6, 6,-1,-1,-1},
  {-1,-1,-1,-1,-1, 6,-1,-1,-1,-1,-1, 6,-1,-1,-1,-1,-1},
  {-1,-1,-1,-1,-1, 6, 6, 6, 6, 6, 6, 6,-1,-1,-1,-1,-1},
  {-1,-1,-1,-1,-1,-1,-1,-1, 6,-1,-1,-1,-1,-1,-1,-1,-1},
  {-1,-1,-1,-1,-1,-1,-1,-1, 6,-1,-1,-1,-1,-1,-1,-1,-1},
  {-1,-1,-1,-1,-1,-1,-1,-1, 6,-1,-1,-1,-1,-1,-1,-1,-1}};

signed short testsetMap1TopLayer[17][17] =
  {{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
  {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
  {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
  {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
  {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
  {-1,-1,-1,-1,-1, 5,-1,-1, 5,-1,-1, 5,-1,-1,-1,-1,-1},
  {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
  {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
  {-1,-1,-1,-1,-1, 5,-1,-1, 5,-1,-1, 5,-1,-1,-1,-1,-1},
  {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
  {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
  {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
  {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
  {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
  {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
  {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
  {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}};

unsigned short testsetMap1BoundMapData[17][17] =
  {{15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15},
  {15,15,0,0,0,0,0,0,15,0,0,0,0,0,0,15,15},
  {15,15,15,0,15,0,15,0,0,0,15,0,15,0,15,15,15},
  {15,15,15,0,0,0,0,0,0,0,0,0,0,0,15,15,15},
  {15,15,15,0,0,0,0,0,0,0,0,0,0,0,15,15,15},
  {15,15,15,0,0,0,0,0,0,0,0,0,0,0,15,15,15},
  {15,15,15,0,0,15,0,0,15,0,0,15,0,0,15,15,15},
  {15,15,15,0,0,0,0,0,0,0,0,0,0,0,15,15,15},
  {15,15,15,0,0,0,0,0,0,0,0,0,0,0,15,15,15},
  {15,15,15,0,0,15,0,0,15,0,0,15,0,0,15,15,15},
  {15,15,15,0,0,0,0,0,0,0,0,0,0,0,15,15,15},
  {15,15,15,0,0,0,0,15,0,15,0,0,0,0,15,15,15},
  {15,15,15,15,0,0,0,0,0,0,0,0,0,15,15,15,15},
  {15,15,15,15,15,0,0,0,0,0,0,0,15,15,15,15,15},
  {15,15,15,15,15,15,0,0,0,0,0,15,15,15,15,15,15},
  {15,15,15,15,15,15,15,0,0,0,15,15,15,15,15,15,15},
  {15,15,15,15,15,15,15,15,0,15,15,15,15,15,15,15,15}};


/* tile sequences */

int testsetSeqLength[] =
{
  -1
};

int testsetSeqPosition[0 + 1];
int testsetSeqCounter[0 + 1];

int * testsetSequences[] = 
{
  Empty
};



/* tile sequences */

int Tiles1SeqLength[] =
{
  -1
};

int Tiles1SeqPosition[0 + 1];
int Tiles1SeqCounter[0 + 1];

int * Tiles1Sequences[] = 
{
  Empty
};

 

/* bitmap data */
const struct GFX_BITMAP_DATA GfxtestsetBitmap = 
  { "testset",  32, 256,  32, 32,  
    1, 8,  -1, -1, 0,
    &testsetSequences[0], &testsetSeqLength[0],
    &testsetSeqPosition[0], &testsetSeqCounter[0]
  };

const struct GFX_BITMAP_DATA GfxTiles1Bitmap = 
  { "Tiles1",  32, 256,  32, 32,  
    1, 8,  -1, -1, 0,
    &Tiles1Sequences[0], &Tiles1SeqLength[0],
    &Tiles1SeqPosition[0], &Tiles1SeqCounter[0]
  };


#endif