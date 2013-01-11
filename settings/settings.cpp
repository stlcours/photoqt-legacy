#include "settings.h"

Settings::Settings(QWidget *parent, QMap<QString, QVariant> glob) : QWidget(parent) {

	// the settings and shortcuts
	globSet = glob;
	sh = new Shortcuts;
	sh->loadSH();

	// Setting the background
	this->setObjectName("topsetting");
	this->setStyleSheet("QWidget#topsetting { background:rgba(0,0,0,210); }");

	// The animation, and a boolean
	ani = new QPropertyAnimation(this,"geometry");
	isShown = false;

	// The tab widget
	tabs = new QTabWidget;

	// All the tabs
	tabOther = new SettingsTabOther(this,globSet);
	tabLookFeel = new SettingsTabLookAndFeel(this,globSet);
	tabThumb = new SettingsTabThumbnail(this,globSet);
	tabExif = new SettingsTabExif(this,globSet);
	tabShortcuts = new SettingsTabShortcuts(this);
	tabShortcuts->allKeyShortcuts = sh->allKeyShortcuts;
	tabShortcuts->allMouseShortcuts = sh->allMouseShortcuts;
	tabShortcuts->loadUserSetShortcuts();

	// Add the tabs
	tabs->addTab(tabLookFeel, QIcon(), tr("Look and Feel"));
	tabs->addTab(tabThumb, QIcon(), tr("Thumbnails"));
	tabs->addTab(tabExif, QIcon(), tr("Exif"));
	tabs->addTab(tabOther, QIcon(), tr("Other Settings"));
	tabs->addTab(tabShortcuts, QIcon(), tr("Shortcuts"));
	connect(tabs, SIGNAL(currentChanged(int)), this, SLOT(tabChanged()));

	// The cursor changes over the tabbar
	QList<QTabBar *> tabBar = tabs->findChildren<QTabBar *>();
	tabBar.at(0)->setCursor(Qt::PointingHandCursor);

	QVBoxLayout *layout = new QVBoxLayout;
	this->setLayout(layout);
	layout->addWidget(tabs);

	// the tabwidget is majorly styled
	QString css = "QTabWidget::pane {";
		css += "border-bottom: 4px double black;";
		css += "padding: 10px;";
		css += "padding-top: 20px;";
	css += "}";
	css += "QTabBar::tab {";
		css += "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #b4b4b4, stop: 1 #080808);";
		css += "border: 2px solid transparent;";
		css += "font-weight: bold;";
		css += "border-bottom-left-radius: 10px;";
		css += "border-bottom-right-radius: 10px;";
		css += "min-width: 25ex;";
		css += "padding: 5px;";
	css += "}";
	css += "QTabBar::tab:selected, QTabBar::tab:hover {";
		css += "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #ffffff, stop: 1 #a8a8a8);";
	css += "}";
	css += "QTabBar::tab:selected {";
		css += "border-bottom-left-radius: 10px;";
		css += "border-bottom-right-radius: 10px;";
	css += "}";
	css += "QTabWidget::tab-bar {";
		css += "alignment: center;";
	css += "}";
	css += "QTabWidget::pane {";
		css += "position: absolute;";
		css += "top: -0.5em;";
	css += "}";
	tabs->setStyleSheet(css);

	// Some buttons
	CustomPushButton *setDefault = new CustomPushButton(tr("Restore Default Settings"));
	CustomPushButton *saveExit = new CustomPushButton(tr("Save Changes and Exit"));
	CustomPushButton *cancel = new CustomPushButton(tr("Exit and Discard Changes"));
	QHBoxLayout *butLay = new QHBoxLayout;
	butLay->addWidget(setDefault);
	butLay->addStretch();
	butLay->addWidget(saveExit);
	butLay->addWidget(cancel);
	butLay->addSpacing(30);
	layout->addLayout(butLay);

	// Ask for confirmation of restoring default settings
	restoreDefaultConfirm = new CustomConfirm(tr("Restore Default Settings?"),tr("Do you really want to get rid of your custom settings and set the default ones? This only resets all settings. A default set of shortcuts can be set in the shortcuts tab.") + "<br><br>" + tr("This step cannot be reversed!"),tr("Yep, I want new stuff"),tr("Um, no, not really"),QSize(400,200),this);
	restoreDefaultConfirm->showBorder("white",2);

	connect(saveExit, SIGNAL(clicked()), this, SLOT(animate()));
	connect(saveExit, SIGNAL(clicked()), this, SLOT(saveSettings()));
	connect(cancel, SIGNAL(clicked()), this, SLOT(loadSettings()));
	connect(cancel, SIGNAL(clicked()), this, SLOT(animate()));

	connect(setDefault, SIGNAL(clicked()), restoreDefaultConfirm, SLOT(animate()));
	connect(restoreDefaultConfirm, SIGNAL(confirmed()), this, SLOT(restoreDefaultSettings()));

	connect(ani, SIGNAL(finished()), this, SLOT(aniFinished()));

	connect(tabShortcuts, SIGNAL(setDefaultShortcuts()), this, SLOT(restoreDefaultShortcuts()));

	// Load the settings
	loadSettings();

}

// the animation function
void Settings::animate() {

	// Open widget
	if(ani->state() == QPropertyAnimation::Stopped && !isShown) {

		ani->setDuration(600);
		isShown = true;

		ani->setStartValue(aniStart);
		ani->setEndValue(rectShown);
		ani->setEasingCurve(QEasingCurve::InBack);
		ani->start();

		emit blockFunc(true);

		this->raise();

	// Close widget
	} else if(ani->state() == QPropertyAnimation::Stopped && isShown) {

		ani->setDuration(300);
		isShown = false;

		ani->setStartValue(rectShown);
		ani->setEndValue(aniStart);
		ani->setEasingCurve(QEasingCurve::OutBack);
		ani->start();

		emit blockFunc(false);

	}

}

// Load all settings
void Settings::loadSettings() {

	tabLookFeel->loadSettings();
	tabThumb->loadSettings();
	tabExif->loadSettings();
	tabOther->loadSettings();

}

// Save all settings
void Settings::saveSettings() {

	tabShortcuts->saveShortcuts();
	sh->allKeyShortcuts = tabShortcuts->allKeyShortcutsNEW;
	sh->allMouseShortcuts = tabShortcuts->allMouseShortcutsNEW;
	sh->saveSH();

	tabLookFeel->saveSettings();
	tabThumb->saveSettings();
	tabExif->saveSettings();
	tabOther->saveSettings();

	QList<QMap<QString,QVariant> > allnewsets;
	allnewsets << tabLookFeel->updatedSet;
	allnewsets << tabThumb->updatedSet;
	allnewsets << tabExif->updatedSet;
	allnewsets << tabOther->updatedSet;

	for(int j = 0; j < allnewsets.length(); ++j) {

		QMapIterator<QString, QVariant> i(allnewsets.at(j));
		while (i.hasNext()) {
			i.next();
			globSet[i.key()] = i.value();
		}

	}

	emit updateSettings(globSet);


}

// When the animation has finished
void Settings::aniFinished() {

	tabs->setCurrentIndex(0);

	tabLookFeel->scrollbar->setScrollbarShown();
	tabThumb->scrollbar->setScrollbarShown();
	tabShortcuts->scrollbar->setScrollbarShown();

	if(!isShown)
		this->setGeometry(rectHidden);

}

void Settings::restoreDefaultSettings() {

	emit restoreDefault();

}

void Settings::restoreDefaultShortcuts() {

	sh->setDefault();
	tabShortcuts->allKeyShortcuts = sh->allKeyShortcuts;
	tabShortcuts->allMouseShortcuts = sh->allMouseShortcuts;
	tabShortcuts->loadUserSetShortcuts();

}

// On Tab Change, scroll to top
void Settings::tabChanged() {

	if(tabs->currentIndex() == 1)
		tabThumb->setDatabaseInfo();

	tabLookFeel->scrollbar->setValue(0);
	tabThumb->scrollbar->setValue(0);
	tabOther->scrollbar->setValue(0);
	tabExif->scrollbar->setValue(0);
	tabShortcuts->scrollbar->setValue(0);

}

// Go to next tab
void Settings::nextTab() {

	int current = tabs->currentIndex();
	if(current == 4)
		current = -1;
	++current;
	tabs->setCurrentIndex(current);

}

// Go to prev tab
void Settings::prevTab() {

	int current = tabs->currentIndex();
	if(current == 0)
		current = 5;
	--current;
	tabs->setCurrentIndex(current);

}

void Settings::paintEvent(QPaintEvent *) {
	QStyleOption o;
	o.initFrom(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &o, &p, this);
}

Settings::~Settings() { }