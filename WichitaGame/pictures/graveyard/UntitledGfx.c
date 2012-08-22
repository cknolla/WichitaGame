#ifndef _UntitledGfx_c
#define _UntitledGfx_c


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

signed short testsetMap1MapData[17][17] =
  {{ 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1},
  { 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1},
  { 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1},
  { 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1},
  { 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1},
  { 1, 1, 1, 2, 2, 2, 2, 2, 3, 2, 2, 2, 2, 2, 1, 1, 1},
  { 1, 1, 1, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 2, 1, 1, 1},
  { 1, 1, 1, 2, 2, 3, 2, 2, 2, 2, 2, 3, 2, 2, 1, 1, 1},
  { 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1},
  { 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1},
  { 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1},
  { 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1},
  { 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1},
  { 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1},
  { 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1},
  { 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1},
  { 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 1}};

unsigned short testsetMap1BoundMapData[17][17] =
  {{15,15,15,15,15,15,15,15,0,15,15,15,15,15,15,15,15},
  {15,15,15,15,15,15,0,0,0,0,0,15,15,15,15,15,15},
  {15,15,15,15,15,0,0,0,0,0,0,0,15,15,15,15,15},
  {15,15,15,15,0,0,0,0,0,0,0,0,0,15,15,15,15},
  {15,15,15,0,0,0,0,0,0,0,0,0,0,0,15,15,15},
  {15,15,15,0,0,0,0,0,15,0,0,0,0,0,15,15,15},
  {15,15,15,0,0,0,0,0,15,0,0,0,0,0,15,15,15},
  {15,15,15,0,0,15,0,0,0,0,0,15,0,0,15,15,15},
  {15,15,15,0,0,0,0,0,0,0,0,0,0,0,15,15,15},
  {15,15,15,0,0,0,0,0,0,0,0,0,0,0,15,15,15},
  {15,15,15,0,0,0,0,0,0,0,0,0,0,0,15,15,15},
  {15,15,15,15,0,0,0,0,0,0,0,0,0,15,15,15,15},
  {15,15,15,15,15,0,0,0,0,0,0,0,15,15,15,15,15},
  {15,15,15,15,15,15,0,0,0,0,0,15,15,15,15,15,15},
  {15,15,15,15,15,15,15,0,0,0,15,15,15,15,15,15,15},
  {15,15,15,15,15,15,15,15,0,15,15,15,15,15,15,15,15},
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

 

/* bitmap data */
const struct GFX_BITMAP_DATA GfxtestsetBitmap = 
  { "testset",  160, 32,  32, 32,  
    5, 1,  128, 0, 0,
    &testsetSequences[0], &testsetSeqLength[0],
    &testsetSeqPosition[0], &testsetSeqCounter[0]
  };


#endif