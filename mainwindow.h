#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "widgets/aboutwidget.h"
#include "graphicsview.h"
#include "globalvariables.h"
#include "globalsettings.h"
#include "thumbnails/thumbnails.h"
#include "graphicsitem.h"
#include "settings/settings.h"
#include "widgets/dropdownmenu.h"
#include "widgets/filehandling.h"
#include "widgets/exifwidget.h"
#include "slideshow/slideshowsettings.h"
#include "slideshow/slideshowbar.h"
#include "widgets/startupwidget.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QDateTime>
#include <QtDebug>
#include <QMessageBox>
#include <QtSql>
#include <QGraphicsPixmapItem>
#include <QPainter>
#include <QLabel>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QImageReader>
#include <QTimer>
#include <QShortcut>
#include <QSystemTrayIcon>
#include <QMenu>

// A custom label (the "x" top right corner) for closing Photo
class QuickInfoLabel : public QLabel {
	Q_OBJECT

public:
	QuickInfoLabel(QWidget *parent = 0, QString objName = "");
	QMenu *c;
	QAction *dohideFilepath;
	QAction *dohide;
	QString version;

	QMap<QString,QVariant> globSet;

protected:
	void mouseReleaseEvent(QMouseEvent *e);
	void contextMenuEvent(QContextMenuEvent *ev);
signals:
	void clicked();
};



class MainWindow : public QMainWindow {
	Q_OBJECT

private:
	void setupTrayIcon();
	
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	// The current screenshot
	QPixmap screenshot;
	QLabel *bglabel;

	// Global variables and settings
	GlobalVariables *globVar;
	GlobalSettings *globSet;

	// The graphicsscenes and -views
	QGraphicsScene sceneBig;
	GraphicsView *viewBig;
	Thumbnails *viewThumbs;

	// The big main graphic item
	GraphicsItem *graphItem;

	// The timer is started at startup to wait until window is completely set up
	QTimer *startUpTimer;

	// The settings dialog
	Settings *set;

	// All "normal" key shortcuts
	QMap<QString,QList<QVariant> > allKeySHdo;
	QMap<QString,QShortcut* > allKeySH;
	// All system key shortcuts. These function as normal shrtcuts, unless a widget like the settings or open dialogs are open, then they are applied to these widgets. Therefore some shortcuts have always to be set up (just not always connected to a normal shortcut)
	QMap<QString,QList<QVariant> > systemKeySHdo;
	QMap<QString,QShortcut* > systemKeySH;

	// The main dropdown menu top right corner
	DropDownMenu *menu;

	// The tray icon
	QSystemTrayIcon *trayIcon;
	// The tray icon menu
	QMenu *trayIconMenu;

	// The quickinfo labels
	QuickInfoLabel *quickInfoCounterTOP;
	QLabel *quickInfoSepTOP;
	QuickInfoLabel *quickInfoFilenameTOP;
	QuickInfoLabel *closeWindowX;
	QuickInfoLabel *quickInfoCounterBOT;
	QLabel *quickInfoSepBOT;
	QuickInfoLabel *quickInfoFilenameBOT;

	// The filehandling widget
	FileHandling *filehandling;

	// Showing some about information
	About *about;

	// The two slideshow widgets
	SlideShow *slideshow;
	SlideShowBar *slideshowbar;

	// Showing some exif information
	Exif *exif;

	// The timer that ensures only one instance of Photo is running a time
	QTimer *globalRunningProgTimer;

	// This widget is shown after an update/fresh install
	StartUpWidget *startup;


	// Adjust all the geometries (QRects and stuff)
	void adjustGeometries();

	// Draw the big main image
	void drawImage();

	// Used to catch an Alt+F4
	bool eventFilter(QObject *o, QEvent *e);

	// Show the startup widget (after update/fresh install)
	void showStartupUpdateInstallMsg();

	// Load a new image from the open file dialog
	void loadNewImgFromOpen(QString path);

	// Open a new file
	void openFile();

	// This function slips the current big image vertically/horizontally
	void rotateFlip(bool rotateNotFlipped, QString direction);

	// Set the background of Photo
	void setBackground();

	// Update the quickinfo labels top bar
	void updateQuickInfo();

	// Zoom the current image; if a string is set, the boolean is ignored
	void zoom(bool zoomin, QString ignoreBoolean = "");

public slots:
	// This is called, whenever the settings have changed, and updates all sub-widgets
	// This function is needed together with the updateSettings() function to avoid a crash on startup
	void applySettings(QMap<QString,bool> applySet = QMap<QString,bool>(), bool justApplyAllOfThem = false);

	// If a widget (like open or settings) is opened, all other functions are suspended
	void blockFunc(bool);

	// Some images store a orientation in their exif data
	void getOrientationFromExif(int degree, bool flipHor);

	// Timeout of the global timer (see above)
	void globalRunningProgTimerTimeout();

	// A click on the main image graphicsview
	void gotViewBigClick(QPoint pos);

	// This is called by the menus set to the quickinfo labels
	void hideQuickInfo();

	// Load a new image in current dir from the thumbnail view
	void loadNewImgFromThumbs(QString path);

	// A menuitem has been clicked
	void menuClicked(QString txt, int close);

	// The mouse has been moved over the main image view (called from viewBig)
	void mouseMoved(int x, int y);

	// Move in the current directory (1=right, 0=left)
	void moveInDirectory(int direction);

	// After a file has been manipulated (renamed, deleted, moved), the current dir is reloaded
	void reloadDir(QString t);

	// After a timeout, the switch for catching the Alt+F4 is reset
	void resetAltForF4();

	// Restore default settings
	void restoreDefaultSettings();

	// Setup the shortcuts
	void setupShortcuts();

	// Called by shortcuts to execute something
	void shortcutDO(QString todo = "", bool mouseSH = false);

	// After the user closed the startup widget
	void startupInstallUpdateMsgClosed();

	// On startup, this timer calls drawImage() as soon as the mainwindow finished setup
	void startuptimer();

	// Start and Stop a slideshow
	void startSlideShow();
	void stopSlideShow();

	// A system shortcut has been called (needed for widgets like about or settings)
	void systemShortcutDO(QString todo);

	// Click on a tray icon menu item
	void trayAcDo(QSystemTrayIcon::ActivationReason rsn = QSystemTrayIcon::Trigger);

	// Update the main image scene rect (called from graphicsitem.cpp)
	void updateSceneBigRect();

	// The settings have been updated, so the map is passed to suib-widgets
	void updateSettings(QMap<QString,QVariant>);

protected:
	void closeEvent(QCloseEvent *);
	void resizeEvent(QResizeEvent *);
	void keyReleaseEvent(QKeyEvent *);

};

#endif // MAINWINDOW_H