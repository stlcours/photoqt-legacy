#ifndef CUSTOMCHECKBOX_H
#define CUSTOMCHECKBOX_H

#include <QCheckBox>
#include <QGraphicsOpacityEffect>

// A custom styled checkbox
class CustomCheckBox : public QCheckBox {

	Q_OBJECT

public:
	explicit CustomCheckBox(const QString &text = "", QWidget *parent = 0);
	~CustomCheckBox();

	// The tick indicator can be resized
	void setIndicatorSize(int indicatorsize);
	void setIndicatorSize(int indicatorWidth, int indicatorHeight);

	// And the font color can be changed
	void setFontColor(QString col);

	// A custom indicator image can be set
	void setIndicatorImage(QString imgChkd, QString imgUnchkd);

	// The font size can be adjusted
	void setFontSize(int size);

public slots:
	void setEnabled(bool e);
	void setDisabled(bool b) { setEnabled(!b); }


private:
	// The variables below correspond to the functions above, but they shouldn't be accessed/changed directly
	int indicSizeW;
	int indicSizeH;
	QString fontCol;
	QString imgChkd;
	QString imgUnchkd;
	int fontsize;

};

#endif // CUSTOMCHECKBOX_H
