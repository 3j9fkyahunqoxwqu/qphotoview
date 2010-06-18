/*
 * QPhotoView border panel.
 *
 * License: GPL V2. See file COPYING for details.
 *
 * Author:  Stefan Hundhammer <Stefan.Hundhammer@gmx.de>
 */

#ifndef BorderPanel_h
#define BorderPanel_h

#include <QGraphicsItem>
#include <QObject>
#include <QFlags>
#include <QBrush>
#include <QPen>
#include <QTimer>

class PhotoView;
class SensitiveBorder;
class GraphicsItemPosAnimation;

class QWidget;
class QPainter;
class QGraphicsSceneHoverEvent;
class QGraphicsSceneMouseEvent;
class QStyleOptionGraphicsItem;

/**
 * Border panel that moves in and out of the PhotoView from one of the borders
 * as a reaction to the mouse pointer hovering over a SensitiveBorder.
 *
 * Put QGraphicsItem and a layout (e.g., a QGraphicsAnchorLayout) into this
 * panel.
 */
class BorderPanel: public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES( QGraphicsItem )

public:

    enum BorderFlag
    {
	NoBorder     = 0x0,
	LeftBorder   = 0x1,
	RightBorder  = 0x2,
	TopBorder    = 0x4,
	BottomBorder = 0x8,
        CenterInView = 0x10
    };

    Q_DECLARE_FLAGS( BorderFlags, BorderFlag );

    /**
     * Constructor. Create a BorderPanel as child of the specified parent.
     *
     * If 'border' is specified, its dimensions are used for alignments (rather
     * than the PhotoView viewport dimensions), and its borderEntered() and
     * borderLeft() signals are automatically connected to this BorderPanel's
     * appearAnimated() and disappearAnimated() slots, respectively.
     */
    BorderPanel( PhotoView * parent, SensitiveBorder * border = 0 );

    /**
     * Destructor.
     */
    virtual ~BorderPanel();

    /**
     * Set the border flags for this panel. This is a value of enum BorderFlag
     * OR'ed together.
     *
     * A panel that is attached to the right border would use
     * BorderPanel::RightBorder. A panel that is attached to the bottom left
     * corner would use BorderPanel::LeftBorder | BorderPanel::BottomBorder.
     */
    void setBorderFlags( BorderFlags flags ) { m_borderFlags = flags; }

    /**
     * Return the border flags.
     */
    BorderFlags borderFlags() const { return m_borderFlags; }

    /**
     * Set the alignment in the "other" dimension -- the dimension that does
     * not have a BorderFlag specified. This is relative to the SensitiveBorder
     * if one was specified or to the PhotoView if not.
     *
     * The default is Qt::Center.
     */
    void setAlignment( Qt::Alignment align ) { m_alignment = align; }

    /**
     * Return the alignment in the "other" dimension.
     */
    Qt::Alignment alignment() const { return m_alignment; }

    /**
     * Set the border margin. This is the distance between the outline of the
     * panel and the edge of the border it is attached to.
     */
    void setBorderMargin( qreal margin ) { m_borderMargin = margin; }

    /**
     * Return the edge margin.
     */
    qreal borderMargin() const { return m_borderMargin; }

    /**
     * Set the inner margin. This is the distance betwen the outline of the
     * panel and its content.
     */
    void setMargin( qreal margin ) { m_margin = margin; }

    /**
     * Return the margin.
     */
    qreal margin() const { return m_margin; }

    /**
     * Set this panel "sticky", i.e. it stays on screen even when the mouse is
     * moved outside.
     *
     * This is the same that happens when the user clicks on the panel.
     */
    void setSticky( bool sticky = true );

    /**
     * Return 'true' if this panel is sticky.
     */
    bool isSticky() const { return m_sticky; }

    /**
     * Return 'true' if this panel is currently active, i.e. if it is visible
     * on the screen.
     */
    bool isActive() const { return m_active; }

    /**
     * Return the animation that is used to make this panel appear.
     * This will create the animation if it is not created yet.
     * Use this function to override the default easing curve, duration etc. of
     * the animation.
     */
    GraphicsItemPosAnimation * appearAnimation();

    /**
     * Return the animation that is used to make this panel disappear.
     * This will create the animation if it is not created yet.
     * Use this function to override the default easing curve, duration etc. of
     * the animation.
     */
    GraphicsItemPosAnimation * disappearAnimation();

    /**
     * Return the size.
     */
    virtual QSizeF size() const;

    /**
     * Set the size. This is only used if there are no child items.
     */
    virtual void setSize( const QSizeF & size ) { m_size = size; }

    /**
     * Set the size. Overloaded for convenience.
     * This value is only used if there are no child items.
     */
    void setSize( qreal width, qreal height )
        { setSize( QSizeF( width, height ) ); }

    /**
     * Set the brush used for painting the panel.
     * By default, this is a dark transparent color.
     */
    void setBrush( const QBrush & brush ) { m_brush = brush; }

    /**
     * Return the current brush.
     */
    QBrush brush() const { return m_brush; }

    /**
     * Set the pen used for painting the panel.
     * By default, this is Qt::NoPen.
     */
    void setPen( const QPen & pen ) { m_pen = pen; }

    /**
     * Return the current pen.
     */
    QPen pen() const { return m_pen; }

    /**
     * Return the parent PhotoView.
     */
    PhotoView * photoView() const { return m_photoView; }

    /**
     * Return the associated SensitiveBorder.
     */
    SensitiveBorder * sensitiveBorder() const { return m_sensitiveBorder; }

    /**
     * Implemented from QGraphicsItem: Paint this item.
     */
    virtual void paint( QPainter * painter,
			const QStyleOptionGraphicsItem * option,
			QWidget * widget = 0 );
    /**
     * Implemented from QGraphicsItem: Return the bounding rectangle.
     */
    virtual QRectF boundingRect() const;

public slots:

    /**
     * Appear from the outside with animation.
     */
    void appearAnimated();

    /**
     * Appear from the outside without animation.
     */
    void appearNow();

    /**
     * Disappear to the outside with animation.
     */
    void disappearAnimated();

    /**
     * show() as a slot.
     */
    void show() { QGraphicsItem::show(); }

    /**
     * hide() as a slot.
     */
    void hide() { QGraphicsItem::hide(); }


private slots:

    /**
     * Disappear with animation unless sticky.
     */
    void maybeDisappear();


protected:

    /**
     * Return the position when active, i.e. inside the PhotoView.
     */
    QPointF activePos();

    /**
     * Return the position when inactive, i.e. outside the PhotoView.
     */
    QPointF inactivePos();

    /**
     * Return the position in the secondary dimension: The one (if any) not
     * attached to a border.
     */
    QPointF secondaryPos( const QPointF( primaryPos ) );


    // Event handlers, all reimiplemented from QGraphicsItem

    virtual void hoverEnterEvent       ( QGraphicsSceneHoverEvent * event );
    virtual void hoverLeaveEvent       ( QGraphicsSceneHoverEvent * event );
    virtual void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event );

#if 0
    virtual void mousePressEvent       ( QGraphicsSceneMouseEvent * event );
    virtual void mouseReleaseEvent     ( QGraphicsSceneMouseEvent * event );
#endif

private:

    PhotoView *			m_photoView;
    SensitiveBorder *		m_sensitiveBorder;
    GraphicsItemPosAnimation *	m_appearAnimation;
    GraphicsItemPosAnimation *	m_disappearAnimation;

    BorderFlags		        m_borderFlags;
    Qt::Alignment               m_alignment;
    qreal		        m_borderMargin;
    qreal		        m_margin;
    QSizeF                      m_size;
    bool                        m_sticky;
    bool                        m_active;
    QBrush                      m_brush;
    QPen                        m_pen;
    QTimer		        m_leaveTimer;
};

Q_DECLARE_OPERATORS_FOR_FLAGS( BorderPanel::BorderFlags );


#endif // BorderPanel_h