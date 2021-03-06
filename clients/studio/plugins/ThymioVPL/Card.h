#ifndef VPL_CARD_H
#define VPL_CARD_H

#include <QObject>
#include <QGraphicsObject>
#include <QList>

class QMimeData;
class QSlider;

namespace Aseba { namespace ThymioVPL
{
	/** \addtogroup studio */
	/*@{*/
	
	class GeometryShapeButton;
	
	/**
		An "event" or "action" card.
		
		These cards have a type (event or action) and a name (prox, etc.)
		and may provide several values (set/get by setValue()/getValue()).
		These values are set by the user through buttons (typically
		GeometryShapeButton), sliders, or specific widgets.
		
		In addition, event cards can have an associated state filter,
		which is an array of 4 tri-bool value (yes/no/don't care) "serialized"
		in a single integer (2 bits per value). The state filter
		can be set/get using setStateFilter()/getStateFilter().
	*/
	class Card : public QGraphicsObject
	{
		Q_OBJECT
		
	public:
		class ThymioBody : public QGraphicsItem
		{
		public:
			ThymioBody(QGraphicsItem *parent = 0, int yShift = 0) : QGraphicsItem(parent), bodyColor(Qt::white), yShift(yShift), up(true) { }
			
			void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
			QRectF boundingRect() const { return QRectF(-128, -128+yShift, 256, 256); }
			void setUp(bool u) { up = u; }
			
			static void drawBody(QPainter * painter, int xShift, int yShift, bool up, const QColor& bodyColor);
			
			QColor bodyColor;
			
		private:
			const int yShift;
			bool up;
		};
		
		static Card* createCard(const QString& name, bool advancedMode=false);
		
		Card(bool eventButton = true, bool advanced=false, QGraphicsItem *parent=0);
		virtual ~Card();
		
		virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
		QRectF boundingRect() const { return QRectF(0, 0, 256, 256); }

		void setBackgroundColor(const QColor& color) { backgroundColor = color; update(); }
		void setParentID(int id) { parentID = id; }
		int getParentID() const { return parentID; }
		QString getType() const { return data(0).toString(); }
		QString getName() const { return data(1).toString(); }
		
		virtual unsigned valuesCount() const = 0;
		virtual int getValue(unsigned i) const = 0;
		virtual void setValue(unsigned i, int value) = 0;
		virtual bool isAnyValueSet() const;
		
		unsigned stateFilterCount() const;
		int getStateFilter() const;
		int getStateFilter(unsigned i) const;
		void setStateFilter(int val);
		
		virtual bool isAnyAdvancedFeature() const;
		virtual void setAdvanced(bool advanced);
		
		void setScaleFactor(qreal factor);
		
		void render(QPainter& painter);
		virtual QPixmap image(qreal factor=1);
		QMimeData* mimeData() const;

	signals:
		void contentChanged();

	protected:
		void addAdvancedModeButtons();
		virtual void mouseMoveEvent( QGraphicsSceneMouseEvent *event );
		
	protected:
		QList<GeometryShapeButton*> stateButtons;
		
		QColor backgroundColor;
		int parentID;
		qreal trans;
	};
	
	class CardWithNoValues: public Card
	{
	public:
		CardWithNoValues(bool eventButton, bool advanced, QGraphicsItem *parent);
		
		virtual unsigned valuesCount() const { return 0; }
		virtual int getValue(unsigned i) const { return -1; }
		virtual void setValue(unsigned i, int value) {}
	};
	
	class CardWithBody: public Card
	{
	public:
		CardWithBody(bool eventButton, bool up, bool advanced, QGraphicsItem *parent);
		
		virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
	
	protected:
		bool up;
		QColor bodyColor;
	};
	
	class CardWithButtons: public CardWithBody
	{
	public:
		CardWithButtons(bool eventButton, bool up, bool advanced, QGraphicsItem *parent);
		
		virtual unsigned valuesCount() const;
		virtual int getValue(unsigned i) const;
		virtual void setValue(unsigned i, int value);
		
	protected:
		QList<GeometryShapeButton*> buttons;
	};
	
	class CardWithButtonsAndRange: public CardWithButtons
	{
	public:
		CardWithButtonsAndRange(bool eventButton, bool up, int lowerBound, int upperBound, int defaultLow, int defaultHigh, bool advanced, QGraphicsItem *parent);
		
		virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
		
		virtual unsigned valuesCount() const;
		virtual int getValue(unsigned i) const;
		virtual void setValue(unsigned i, int value);
		virtual bool isAnyValueSet() const;
		
		virtual bool isAnyAdvancedFeature() const;
		virtual void setAdvanced(bool advanced);
		
	public:
		const int lowerBound;
		const int upperBound;
		const int range;
		const int defaultLow;
		const int defaultHigh;
	
	protected:
		virtual void mousePressEvent( QGraphicsSceneMouseEvent * event);
		virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
		
		QRectF rangeRect() const;
		float pixelToVal(float pixel) const;
		float valToPixel(float val) const;
		
	protected:
		int low; // low activation threshold (at right)
		int high; // high activation threshold (at left)
		bool lastPressedIn; // whether last mouse press event was in
		bool showRangeControl; // whether we are in advanced mode
	};
	
	/*@}*/
} } // namespace ThymioVPL / namespace Aseba

#endif
