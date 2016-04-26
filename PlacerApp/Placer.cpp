#include "stdafx.h"
#include "Placer.h"

CPlacer::CPlacer( HWND hWndParent, char chSplitWidth )
{
     m_hWndParent = hWndParent;
     m_chCount = -1;
     m_bSplitting = false;
     m_pSplittingItem = NULL;
     m_chSplitWidth = chSplitWidth;
     ::ZeroMemory( &m_items, sizeof( m_items ) );
}

CPlacer::~CPlacer()
{
}

bool CPlacer::addNew( HWND hWnd, PLACEMENT_ALIGN chAlign, WORD width, WORD height, bool bSplitting )
{
     PPLACEMENT_ITEM pli;
     if ( m_chCount + 1 < MAX_PLACEMENTITEM &&
          ::IsWindow( hWnd ) ) {
        m_chCount++;
        pli = &m_items[ m_chCount ];
        pli->chAlign = ( char ) chAlign;
        pli->width = width;
        pli->height = height;
        pli->hWnd = hWnd;
        pli->bSplit = bSplitting;
        return true;
     }
     return false;
}

void CPlacer::startSplit()
{
     RECT rcItem;
     RECT rcParent;
     PPLACEMENT_ITEM pli;
     int i;
     if ( !m_bSplitting ) {
        ::GetCursorPos( ( LPPOINT ) &m_ptSplit );
        ::ScreenToClient( m_hWndParent, ( LPPOINT ) &m_ptSplit );
        ::GetClientRect( m_hWndParent, ( LPRECT ) &rcParent );
        ::ClientToScreen( m_hWndParent, ( LPPOINT ) &rcParent );
        ::ClientToScreen( m_hWndParent, ( LPPOINT ) &rcParent.right );
        for ( i = 0; i <= m_chCount; i++ ) {
            pli = &m_items[ i ];
            ::GetClientRect( pli->hWnd, ( LPRECT ) &rcItem );
            ::ClientToScreen( pli->hWnd, ( LPPOINT ) &rcItem );
            ::ClientToScreen( pli->hWnd, ( LPPOINT ) &rcItem.right );
            rcItem.left -= rcParent.left;
            rcItem.right -= rcParent.left;
            if ( pli->chAlign != ALIGN_CLIENT )
               if ( ( m_ptSplit.x >= rcItem.left - m_chSplitWidth && m_ptSplit.x <= rcItem.left ) ||
                    ( m_ptSplit.x >= rcItem.right && m_ptSplit.x <= rcItem.right + m_chSplitWidth )
                  ) {
                  m_pSplittingItem = pli;
                  break;
               }
        }
        if ( m_pSplittingItem ) {
           ::SetCapture( m_hWndParent );
           m_bSplitting = true;
        }
     }
}

void CPlacer::endSplit()
{
     if ( m_bSplitting ) {
        ::ReleaseCapture();
        m_bSplitting = false;
        m_pSplittingItem = NULL;
     }
}

void CPlacer::split()
{
     int iWidth, iSplit;
     POINT pt;
     if ( m_bSplitting ) {
        ::GetCursorPos( ( LPPOINT ) &pt );
        ::ScreenToClient( m_hWndParent, ( LPPOINT ) &pt );
        iWidth = ( int ) m_pSplittingItem->width;
        iSplit = ( pt.x - m_ptSplit.x );
        if ( m_pSplittingItem->chAlign != ALIGN_RIGHT )
           iWidth += iSplit;
        else
           iWidth -= iSplit;
        if ( iWidth > 10 ) {
           m_pSplittingItem->width = ( WORD )iWidth;
           m_ptSplit.x += iSplit;
           setPlaces( 0, 0 );
        }
     }
}

void CPlacer::setPlaces( int width, int height )
{
     int i, left, right, top, bottom, clientHeight, x, y, x1, y1;
     PPLACEMENT_ITEM pli;
     RECT rc;
     if ( height == 0 || width == 0 ) {
        ::GetClientRect( m_hWndParent, ( LPRECT ) &rc );
        width = rc.right;
        height = rc.bottom;
     }
     left = 0;
     right = 0;
     top = 0;
     bottom = 0;
     clientHeight = height;
     for ( i = 0; i <= m_chCount; i++ ) {
         pli = &m_items[ i ];
         switch ( pli->chAlign ) {
                case ALIGN_LEFT: {
                     left = pli->width;
                     break;
                }
                case ALIGN_TOP: {
                     clientHeight -= pli->height;
                     top += pli->height;
                     break;
                }
                case ALIGN_RIGHT: {
                     right = width - pli->width;
                     break;
                }
                case ALIGN_BOTTOM: {
                     clientHeight -= pli->height;
                     bottom = clientHeight + top;
                     break;
                }
         }
     }
     if ( !right )
        right = width;
     if ( !bottom )
        bottom = height;
     for ( i = 0; i <= m_chCount; i++ ) {
         x = 0;
         y = 0;
         x1 = 0;
         y1 = 0;
         pli = &m_items[ i ];
         switch ( pli->chAlign ) {
                case ALIGN_LEFT: {
                     y = top;
                     x1 = pli->width;
                     y1 = bottom - y;
                     if ( pli->bSplit )
                        x1 -= m_chSplitWidth;
                     break;
                }
                case ALIGN_TOP: {
                     x1 = width;
                     y1 = top;
                     break;
                }
                case ALIGN_RIGHT: {
                     x = right;
                     y = top;
                     x1 = width - right;
                     y1 = bottom - y;
                     if ( pli->bSplit ) {
                        x += m_chSplitWidth;
                        x1 -= m_chSplitWidth;
                     }
                     break;
                }
                case ALIGN_BOTTOM: {
                     y = bottom;
                     x1 = width;
                     y1 = height - y;
                     break;
                }
                case ALIGN_CLIENT: {
                     x = left;
                     y = top;
                     x1 = right - left;
                     y1 = clientHeight;
                     break;
                }
         }
         ::MoveWindow( pli->hWnd, x, y, x1, y1, TRUE );
     }
}
