/*
 * QPhotoView border panel to photo meta data.
 *
 * License: GPL V2. See file COPYING for details.
 *
 * Author:  Stefan Hundhammer <Stefan.Hundhammer@gmx.de>
 */

#include <QTextStream>

#include "Photo.h"
#include "PhotoDir.h"
#include "PhotoView.h"
#include "PhotoMetaData.h"
#include "ExifBorderPanel.h"


ExifBorderPanel::ExifBorderPanel( PhotoView * parent,
				  SensitiveBorder * border )
    : TextBorderPanel( parent, border )
    , _lastPhoto( 0 )
{
    connect( this, SIGNAL( aboutToAppear()  ),
	     this, SLOT	 ( setMetaData()    ) );
}


void ExifBorderPanel::setMetaData()
{
    if ( photoView()->photoDir() )
    {
	Photo * photo = photoView()->photoDir()->current();

	if ( photo != _lastPhoto )
	    setText( formatMetaData( photo ) );

	_lastPhoto = photo;
    }
    else
    {
	setText( "" );
    }
}


QString ExifBorderPanel::formatMetaData( Photo * photo )
{
    if ( ! photo )
	return QString();

    PhotoMetaData meta = photo->metaData();

    QString text;
    QTextStream str( &text );

    if ( ! meta.isEmpty() )
    {
	if ( meta.exposureTime() < 1 )
	    str << meta.exposureTime().toString();
	else
	    str << meta.exposureTime().toDouble() << " sec";

	str << "\n";
	str << "f/" << meta.aperture().toDouble() << "\n";

	if ( meta.iso() > 0 )
	    str << "ISO " << meta.iso() << "\n";

	str << "\n";
	str << meta.focalLength() << " mm\n";

	if ( meta.focalLength35mmEquiv() != meta.focalLength() &&
	     meta.focalLength35mmEquiv() > 0 )
	{
	    str << tr( "(%1 mm equiv.)" ).arg( meta.focalLength35mmEquiv() )
		<< "\n";
	}

	str << "\n";
	str << meta.size().width() << " x " << meta.size().height();

	double megaPixel = meta.size().width() * (double) meta.size().height()
	    / ( 1000 * 1000 );

	str << "\n" << tr( "(%1 MPix)" ).arg( megaPixel, 0, 'f', 1 );

	if ( meta.origSize().isValid() )
	{
	    str << "\n\n";
	    str << tr( "Original:" ) << "\n"
		<< meta.origSize().width() << " x "
		<< meta.origSize().height();

	    megaPixel = meta.origSize().width() *
		(double) meta.origSize().height() / ( 1000 * 1000 );

	    str << "\n" << tr( "(%1 MPix)" ).arg( megaPixel, 0, 'f', 1 );
	}

	if ( meta.dateTimeTaken().isValid() )
	{
	    str << "\n\n"
		<< meta.dateTimeTaken().date().toString( Qt::ISODate )
		<< "\n"
		<< meta.dateTimeTaken().time().toString( "HH:mm:ss" );
	}
    }
    else // meta.isEmpty()
    {
	QSize size = photo->size();

	str << size.width() << " x " << size.height();

	double megaPixel = size.width() * (double) size.height()
	    / ( 1000 * 1000 );

	str << "\n" << tr( "(%1 MPix)" ).arg( megaPixel, 0, 'f', 1 );
    }

    return text;
}
