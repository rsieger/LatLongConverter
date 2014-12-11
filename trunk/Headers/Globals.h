#ifndef _GLOBALS

    #define _GLOBALS

// Application constants
    const QString _PROGRAMDOI_                       = "doi:10.1594/PANGAEA.787692";
    const QString _PROGRAMREADME_                    = "http://wiki.pangaea.de/wiki/LatLongConverter";

    const QString _VERSION_                          = "2.4";	// Version number, 2014-09-25
    const QChar   _PSEPARATOR_                       = '|';

    const int   _NOERROR_                            = 0;
    const int   _ERROR_                              = 1;
    const int   _APPBREAK_                           = 2;
    const int   _NODATAFOUND_                        = 3;
    const int   _DATAFOUND_                          = 4;
    const int   _CHOOSEABORTED_                      = 5;
    const int   _FILENOTEXISTS_                      = 6;
    const int   _WRONGTIMEFORMAT_                    = 7;

    const int   _GKLL_                               = 0;
    const int   _LLGK_                               = 1;
    const int   _UTMLL_                              = 0;
    const int   _LLUTM_                              = 1;

    const int   _ZIP_                                = 1;
    const int   _GZIP_                               = 2;

    const int   _MAX_NUM_OF_AREAS					 = 5000;	// maximum number of areas, used in find areas tool
    const int   _MAX_NUM_OF_LOCATIONS				 = 500000;	// maximum number of locations, used in find areas tool

    const int   _COMMA_                              = 1;
    const int   _DOT_                                = 2;

// EOL
    const int   _UNIX_                               = 0;    // LF
    const int   _MACOS_                              = 1;    // CR
    const int   _WIN_                                = 2;    // CR+LF

// Encoding
    const int   _SYSTEM_                             = -1;   // System
    const int   _UTF8_                               = 0;    // UTF-8
    const int   _APPLEROMAN_                         = 1;    // Apple Roman
    const int   _LATIN1_                             = 2;    // Latin-1 = ISO 8859-1

// Extension
    const int   _TXT_                                = 0;
    const int   _CSV_                                = 1;

#endif
