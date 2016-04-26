#pragma once

enum PLACEMENT_ALIGN
{
     ALIGN_LEFT,
     ALIGN_TOP,
     ALIGN_RIGHT,
     ALIGN_BOTTOM,
     ALIGN_CLIENT
};
#define MAX_PLACEMENTITEM   5


typedef
struct PLACEMENT_ITEM
{
       HWND hWnd;
       WORD width;
       WORD height;
       bool bSplit;
       char chAlign;
       char __padding__[ 2 ];
} PLACEMENT_ITEM, *PPLACEMENT_ITEM;


class CPlacer
{
private:
      PLACEMENT_ITEM m_items[ MAX_PLACEMENTITEM ];
      PPLACEMENT_ITEM m_pSplittingItem;
      HWND m_hWndParent;
      POINT m_ptSplit;
      bool m_bSplitting;
      char m_chCount;
      char m_chSplitWidth;

public:
      CPlacer( HWND hWndParent, char chSplitWidth = 2 );
      ~CPlacer();
      void setPlaces( int width, int height );
      void startSplit();
      void endSplit();
      void split();
      bool isSplitting() { return m_bSplitting; }
      bool addNew( HWND hWnd, PLACEMENT_ALIGN align, WORD width, WORD height, bool bSplitting = false );
      char getCount() { return m_chCount + 1; }
};
