#include "stdafx.h"

HWND        hWndApp         = NULL;
TCHAR*      szWindowClass   = _T( "PlacerApp" );
CPlacer*    pPlacer         = NULL;
MSG         msg             = { 0 };

LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
        switch ( msg ) {
               case WM_LBUTTONDOWN:
                    pPlacer->startSplit();                          // start splitting
                    break;

               case WM_LBUTTONUP:
                    pPlacer->endSplit();                            // end splitting
                    break;

               case WM_MOUSEMOVE:
                    pPlacer->split();                               // split them
                    break;

               case WM_SIZE: {
                    if ( pPlacer != NULL ) {
                       int width  = LOWORD( lParam );
                       int height = HIWORD( lParam );
                       pPlacer->setPlaces( width, height );         // set places of the child windows
                    }
                    break;
               }
               case WM_DESTROY:
                    ::PostQuitMessage( 0 );
                    break;

               default:
                    return ::DefWindowProc( hWnd, msg, wParam, lParam );
        }
        return 0;
}

ATOM registerMain( HINSTANCE hInstance )
{
     WNDCLASSEX wcex;
     wcex.cbSize         = sizeof( WNDCLASSEX );
     wcex.style          = CS_HREDRAW | CS_VREDRAW;
     wcex.lpfnWndProc    = WndProc;
     wcex.cbClsExtra     = 0;
     wcex.cbWndExtra     = 0;
     wcex.hInstance      = hInstance;
     wcex.hIcon          = NULL;
     wcex.hCursor        = ::LoadCursor( NULL, IDC_SIZEWE );
     wcex.hbrBackground  = ( HBRUSH )( COLOR_WINDOW + 1 );
     wcex.lpszMenuName   = NULL;
     wcex.lpszClassName  = szWindowClass;
     wcex.hIconSm        = NULL;
     return ::RegisterClassEx( &wcex );
}

BOOL initInstance( HINSTANCE hInstance, int nCmdShow )
{
     HWND hWnd = ::CreateWindow( szWindowClass, _T( "Placer Application" ), WS_OVERLAPPEDWINDOW, \
                                 CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL );
     if ( !hWnd )
        return FALSE;
     hWndApp = hWnd;
     ::ShowWindow( hWnd, nCmdShow );
     ::UpdateWindow( hWnd );
     return TRUE;
}

HWND createChild( HINSTANCE hInstance, LPTSTR psz )
{
     return ::CreateWindow( _T( "edit" ), // class name
                            psz,          // window text
                            WS_CHILD|WS_VISIBLE|WS_BORDER|ES_LEFT|ES_MULTILINE|ES_AUTOVSCROLL,  // styles
                            0,            // X
                            0,            // Y
                            50,           // width
                            25,           // height
                            hWndApp,      // parent
                            NULL,         // ID
                            hInstance,
                            NULL );
}

int APIENTRY _tWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow )
{
    if ( registerMain( hInstance ) &&
         initInstance( hInstance, nCmdShow )
       ) {
       pPlacer = new CPlacer( hWndApp, 4 );
       if ( pPlacer &&
            pPlacer->addNew( createChild( hInstance, _T( "Label 2" ) ), ALIGN_RIGHT , 150,  0, true ) &&
            pPlacer->addNew( createChild( hInstance, _T( "Label 1" ) ), ALIGN_LEFT  , 150,  0, true ) &&
            pPlacer->addNew( createChild( hInstance, _T( "Label 4" ) ), ALIGN_BOTTOM, 0, 30 ) &&
            pPlacer->addNew( createChild( hInstance, _T( "Label 3" ) ), ALIGN_TOP   , 0, 50 ) &&
            pPlacer->addNew( createChild( hInstance, _T( "Label 5" ) ), ALIGN_CLIENT, 0, 0  )
          ) {
          pPlacer->setPlaces( 0, 0 );
          while ( ::GetMessage( &msg, NULL, 0, 0 ) ) {
                ::TranslateMessage( &msg );
                ::DispatchMessage( &msg );
          }
          delete pPlacer;
       }
    }
    return ( int ) msg.wParam;
}
