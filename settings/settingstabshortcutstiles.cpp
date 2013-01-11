#include "settingstabshortcutstiles.h"

ShortcutsTiles::ShortcutsTiles(QString exeCmd, QString exeDesc, QString category, QString availORexist, QWidget *parent) : QWidget(parent) {

	// Store the type of this tile
	type = availORexist;

	// The command and the category
	exe = exeCmd;
	cat = category;

	// Bool for mouse shortcut
	isMouseShortcut = false;

	// A fixed size
	this->setFixedSize(90,90);

	// The standard default stylesheet
	css = "font-weight: bold; color: black; border-radius: 5px; padding: 1px; font-size: 8pt;";

	// Some special stylesheet for hovered and normal
	cssBackgroundNorm = "background: rgba(255,255,255,100);";
	cssBackgroundHov = "background: rgba(255,255,255,200);";

	// Main Layout
	QVBoxLayout *lay = new QVBoxLayout;

	// If the tile is used to display an existing user set shortcut
	if(type == "exist") {

		quit = new CustomCheckBox(tr("quit"));
		quit->setIndicatorSize(8);
		quit->setFontSize(7);
		quit->setIndicatorImage(":/img/radiobutton_checked.png",":/img/radiobutton_unchecked.png");
		quit->setFontColor("black");
		if(category != "extern")
			quit->hide();
		mouse = new QLabel("M");
		mouse->setStyleSheet("background: none;color: white; font-weight: bold; font-size: 12pt;");
		mouse->hide();
		del = new QPushButton(QIcon(":/img/delsh.png"),"");
		del->setCursor(Qt::PointingHandCursor);
		del->setStyleSheet("background: none;");
		QHBoxLayout *top = new QHBoxLayout;
		top->addWidget(quit);
		top->addStretch();
		top->addWidget(mouse);
		top->addWidget(del);
		lay->addLayout(top);
		connect(del, SIGNAL(clicked()), this, SLOT(removeMe()));
	}

	// The back label is the same no matter what type
	back = new Label(exeDesc);
	back->setTextFormat(Qt::PlainText);
	back->setAlignment(Qt::AlignCenter);
	back->setStyleSheet("background: transparent");
	back->setWordWrap(true);
	if(category == "extern" && type == "exist") {
		back->setCursor(Qt::PointingHandCursor);
		connect(back, SIGNAL(clicked()), this, SLOT(backClicked()));
	}
	lay->addWidget(back);

	if(type == "avail") {

		// The add button is only shown for type "avail"
		add = new QPushButton(QIcon(":/img/addsh.png"),"");
		add->setStyleSheet("background: transparent");
		add->setCursor(Qt::PointingHandCursor);
		connect(add, SIGNAL(clicked()), this, SLOT(addButClkd()));
		lay->addWidget(add);

	} else if(type == "exist") {

		// The shortcut button is only shown for type "exist"
		shortcut = new Label("<" + tr("key") + ">");
		shortcut->setTextFormat(Qt::PlainText);
		shortcut->setAlignment(Qt::AlignCenter);
		shortcut->setStyleSheet("background: transparent; font-size: 7pt;");
		shortcut->setCursor(Qt::PointingHandCursor);
		connect(shortcut, SIGNAL(clicked()), this, SLOT(shButClkd()));
		QHBoxLayout *shLay = new QHBoxLayout;
		shLay->addStretch();
		shLay->addWidget(shortcut);
		shLay->addStretch();
		lay->addLayout(shLay);

	}

	lay->setMargin(0);
	this->setLayout(lay);


	// Set the right style
	if(type == "avail") {

		back->setStyleSheet(css + cssBackgroundNorm);
		add->setStyleSheet(css + cssBackgroundNorm);

	} else if(type == "exist")

		this->setStyleSheet(css + cssBackgroundNorm);

}

// Click on back
void ShortcutsTiles::backClicked() {

	emit changeCmd(back->text().remove("<center>").remove("</center>"),id);

}

// The add button has been clicked
void ShortcutsTiles::addButClkd() {

	if(type == "avail")
		emit addShortcut(exe,cat);

}

// Remove this tile
void ShortcutsTiles::removeMe() {

	emit removeItem(id,cat);

}

// Starting to hover
void ShortcutsTiles::enterEvent(QEvent *) {

	if(type == "avail") {

		back->setStyleSheet(css + cssBackgroundHov);
		add->setStyleSheet(css + cssBackgroundHov);

	} else if(type == "exist")

		this->setStyleSheet(css + cssBackgroundHov);

}

// Ending to hover
void ShortcutsTiles::leaveEvent(QEvent *) {

	if(type == "avail") {

		back->setStyleSheet(css + cssBackgroundNorm);
		add->setStyleSheet(css + cssBackgroundNorm);

	} else if(type == "exist")

		this->setStyleSheet(css + cssBackgroundNorm);

}

// A double click on the tile
void ShortcutsTiles::mouseDoubleClickEvent(QMouseEvent *) {

	if(type == "avail")
		emit addShortcut(exe,cat);

}

// The shortcut button has been clicked
void ShortcutsTiles::shButClkd() {

	emit renewShortcut(cat,id,exe);

}

// Turn shortcut into mouse shortcut
void ShortcutsTiles::makeMouseShortcut(bool m) {

	isMouseShortcut = m;
	mouse->setShown(m);

}

void ShortcutsTiles::paintEvent(QPaintEvent *) {

	QStyleOption o;
	o.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);

}

ShortcutsTiles::~ShortcutsTiles() { }



Label::Label(QString txt, QWidget *parent) : QLabel(txt, parent) {
	this->setWordWrap(true);
}

Label::~Label() { }

void Label::mouseReleaseEvent(QMouseEvent *) {
	emit clicked();
}