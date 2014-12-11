/* 2011-07-08                 */
/* Dr. Rainer Sieger          */

#include <QtGui>

#include "Application.h"

// **********************************************************************************************
// **********************************************************************************************
// **********************************************************************************************
// 2011-07-11

bool MainWindow::LineCanBeWritten( const QString &InputStr, const bool b_SkipEmptyLines, const bool b_SkipCommentLines )
{
    int i_LineCanBeWritten = FALSE;

    if ( ( InputStr.startsWith( "//") == FALSE ) && ( InputStr.startsWith( "*" ) == FALSE ) && ( InputStr.startsWith( "#" ) == FALSE ) && ( InputStr.startsWith( "!" ) == FALSE ) )
    {
        if ( ( InputStr.count( "\t" ) == InputStr.length() ) || ( InputStr.isEmpty() == TRUE ) )
        {
            if ( b_SkipEmptyLines == FALSE )
                i_LineCanBeWritten = TRUE;  // Line is empty but SkipEmptyLine is TRUE
        }
        else
        {
            i_LineCanBeWritten = TRUE; // Line is not empty
        }
    }
    else
    {
         if ( b_SkipCommentLines == FALSE )
             i_LineCanBeWritten = TRUE; // Line is a comment but SkipCommentLines is FALSE
    }

    return( i_LineCanBeWritten );
}


