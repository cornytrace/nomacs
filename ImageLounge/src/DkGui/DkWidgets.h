/*******************************************************************************************************
 DkWidgets.h
 Created on:	17.05.2011
 
 nomacs is a fast and small image viewer with the capability of synchronizing multiple instances
 
 Copyright (C) 2011-2013 Markus Diem <markus@nomacs.org>
 Copyright (C) 2011-2013 Stefan Fiel <stefan@nomacs.org>
 Copyright (C) 2011-2013 Florian Kleber <florian@nomacs.org>

 This file is part of nomacs.

 nomacs is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 nomacs is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

 *******************************************************************************************************/

#pragma once

#pragma warning(push, 0)	// no warnings from includes - begin
#include <QPushButton>
#include <QSlider>
#include <QFileSystemModel>
#include <QSortFilterProxyModel>
#include <QDockWidget>
#include <QPointer>
#include <QPen>
#include <QFutureWatcher>
#include <QLineEdit>
#include <QListWidget>
#include <QProgressBar>
#pragma warning(pop)		// no warnings from includes - end

#pragma warning(disable: 4251)	// TODO: remove

#include "DkMath.h"
#include "DkBaseWidgets.h"
#include "DkImageContainer.h"

// Qt defines
class QColorDialog;
class QSpinBox;
class QDoubleSpinBox;
class QToolBar;
class QBoxLayout;
class QProgressDialog;
class QMovie;
class QTreeView;
class QSlider;
class QGridLayout;
class QVBoxLayout;
class QSvgRenderer;

namespace nmc {

// nomacs defines
class DkCropToolBar;

class DkButton : public QPushButton {
	Q_OBJECT

public:
	DkButton ( QWidget * parent = 0 );
	DkButton ( const QString & text, QWidget * parent = 0 );
	DkButton ( const QIcon & icon, const QString & text, QWidget * parent = 0 );
	DkButton ( const QIcon & checkedIcon, const QIcon & uncheckedIcon, const QString & text, QWidget * parent = 0 );
	~DkButton() {};

	void adjustSize();
	void setFixedSize(QSize size);

	bool keepAspectRatio;

protected:
	QIcon checkedIcon;
	QIcon uncheckedIcon;
	bool mouseOver;
	QSize mySize;
	
	// functions
	void init();

	void paintEvent(QPaintEvent * event);
	void focusInEvent(QFocusEvent * event);
	void focusOutEvent(QFocusEvent * event);
	void enterEvent(QEvent *event);
	void leaveEvent(QEvent *event);
	QPixmap createSelectedEffect(QPixmap* pm);

};

class DkRatingLabel : public DkWidget {
	Q_OBJECT

public:
	enum {
		rating_1,
		rating_2,
		rating_3,
		rating_4,
		rating_5,
		rating_0,	// no image for that one
	};

	DkRatingLabel(int rating = 0, QWidget* parent = 0, Qt::WindowFlags flags = 0);
	~DkRatingLabel() {};

	void setRating(int rating) {
		mRating = rating;
		updateRating();
	};

	virtual void changeRating(int newRating) {
		mRating = newRating;
		updateRating();
		emit newRatingSignal(mRating);
	};

	int getRating() {
		return mRating;
	};

signals:
	void newRatingSignal(int rating = 0);

public slots:
	void rating0() {
		changeRating(0);
	};

	void rating1() {
		changeRating(1);
	};

	void rating2() {
		changeRating(2);
	};

	void rating3() {
		changeRating(3);
	};

	void rating4() {
		changeRating(4);
	};

	void rating5() {
		changeRating(5);
	};

protected:
	QVector<DkButton*> mStars;
	QBoxLayout* mLayout = 0;
	int mRating = 0;

	void updateRating() {
		
		for (int idx = 0; idx < mStars.size(); idx++) {
			mStars[idx]->setChecked(idx < mRating);
		}
	};

	virtual void init();
};

class DkRatingLabelBg : public DkRatingLabel {
	Q_OBJECT

public:
	DkRatingLabelBg(int rating = 0, QWidget* parent = 0, Qt::WindowFlags flags = 0);
	~DkRatingLabelBg();

	void changeRating(int newRating);
	QVector<QAction*> getActions() const;

protected:
	QVector<QAction*> mActions;
	QTimer* mHideTimer;
	int mTimeToDisplay = 4000;
	
	virtual void paintEvent(QPaintEvent *event);
};

class DkFileInfoLabel : public DkFadeLabel {
	Q_OBJECT

public:
	DkFileInfoLabel(QWidget* parent = 0);
	~DkFileInfoLabel() {};

	void createLayout();
	void updateInfo(const QString& filePath, const QString& attr, const QString& date, const int rating);
	void updateTitle(const QString& filePath, const QString& attr);
	void updateDate(const QString& date = QString());
	void updateRating(const int rating);
	void setEdited(bool edited);
	DkRatingLabel* getRatingLabel();

public slots:
	virtual void setVisible(bool visible, bool saveSettings = true);

protected:
	QString mFilePath;

	QBoxLayout* mLayout;
	QLabel* mTitleLabel;
	QLabel* mDateLabel;
	DkRatingLabel* mRatingLabel;

	void updateWidth();
};

class DkPlayer : public DkWidget {
	Q_OBJECT

public:
	enum {
		play_action,	// if more actions are to be assigned
	};

	DkPlayer(QWidget* parent = 0);
	~DkPlayer() {};

	void setTimeToDisplay(int ms = 1000);

	QVector<QAction*> getActions() {
		return actions;
	};

signals:
	void nextSignal();
	void previousSignal();

public slots:
	void play(bool play);
	void togglePlay();
	void startTimer();
	void autoNext();
	void next();
	void previous();
	virtual void show(int ms = 0);
	bool isPlaying() const;

protected:
	void init();
	void createLayout();

	bool playing = false;

	int timeToDisplay;
	QTimer* displayTimer;
	QTimer* hideTimer;

	QPushButton* previousButton;
	QPushButton* nextButton;
	QPushButton* playButton;
	QWidget* container;

	QVector<QAction*> actions;
};

class DkFolderScrollBar : public QSlider {
	Q_OBJECT

public: 
	DkFolderScrollBar(QWidget* parent = 0);
	~DkFolderScrollBar();

	virtual void setValue(int i);

	void registerAction(QAction* action);
	void block(bool blocked);
	void setDisplaySettings(QBitArray* displayBits);
	bool getCurrentDisplaySetting();

public slots:
	void updateDir(QVector<QSharedPointer<DkImageContainerT> > images);

	virtual void show(bool saveSettings = true);
	virtual void hide(bool saveSettings = true);
	virtual void setVisible(bool visible, bool saveSettings = true);

	void animateOpacityUp();
	void animateOpacityDown();

protected slots:
	void updateFile(int idx);

signals:
	void loadFileSignal(int idx) const;
	void visibleSignal(bool visible) const;

protected:
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);

	QColor mBgCol;
	bool mBlocked = false;
	bool mHiding = false;
	bool mShowing = false;
	bool mMouseDown = false;

	QGraphicsOpacityEffect* mOpacityEffect = 0;
	QBitArray* mDisplaySettingsBits = 0;

	void init();

};

// this class is one of the first batch processing classes -> move them to a new file in the (near) future
class DkThumbsSaver : public DkWidget {
	Q_OBJECT

public:
	DkThumbsSaver(QWidget* parent = 0);

	void processDir(QVector<QSharedPointer<DkImageContainerT> > images, bool forceSave);

signals:
	void numFilesSignal(int currentFileIdx);

public slots:
	void stopProgress();
	void thumbLoaded(bool loaded);
	void loadNext();

protected:

	QFileInfo mCurrentDir;
	QProgressDialog* mPd = 0;
	int mCLoadIdx = 0;
	bool mStop = false;
	bool mForceSave = false;
	int mNumSaved = false;
	QVector<QSharedPointer<DkImageContainerT> > mImages;
};

class DkFileSystemModel : public QFileSystemModel {
	Q_OBJECT

public:
	DkFileSystemModel(QObject* parent = 0);

protected:

};

class DkSortFileProxyModel : public QSortFilterProxyModel {
	Q_OBJECT

public:
	DkSortFileProxyModel(QObject* parent = 0);

protected:
	virtual bool lessThan(const QModelIndex& left, const QModelIndex& right) const;

};

class DllCoreExport DkExplorer : public DkDockWidget {
	Q_OBJECT

public:
	DkExplorer(const QString& title, QWidget* parent = 0, Qt::WindowFlags flags = 0);
	~DkExplorer();

	DkFileSystemModel* getModel() { return fileModel; };

public slots:
	void setCurrentImage(QSharedPointer<DkImageContainerT> img);
	void setCurrentPath(const QString& filePath);
	void fileClicked(const QModelIndex &index) const;
	void showColumn(bool show);
	void setEditable(bool editable);
	void adjustColumnWidth();
	void loadSelectedToggled(bool checked);

signals:
	void openFile(const QString& filePath) const;
	void openDir(const QString& dir) const;

protected:
	void closeEvent(QCloseEvent *event);
	void contextMenuEvent(QContextMenuEvent* event);

	void createLayout();
	void writeSettings();
	void readSettings();

	DkFileSystemModel* fileModel;
	DkSortFileProxyModel* sortModel;
	QTreeView* fileTree;
	QVector<QAction*> columnActions;
	bool mLoadSelected = false;
};

class DkOverview : public QLabel {
	Q_OBJECT

public:
	DkOverview(QWidget * parent = 0);
	~DkOverview() {};

	void setImage(const QImage& img) {
		mImg = img;

		if (isVisible())
			resizeImg();
	};

	void setTransforms(QTransform* worldMatrix, QTransform* imgMatrix){
		mWorldMatrix = worldMatrix;
		mImgMatrix = imgMatrix;
	};

	void setViewPortRect(const QRectF& viewPortRect) {
		mViewPortRect = viewPortRect;	
	};

	void setVisible(bool visible) {

		if (visible)
			resizeImg();

		QWidget::setVisible(visible);
	};

signals:
	void moveViewSignal(const QPointF& dxy) const;
	void sendTransformSignal() const;

protected:
	QImage mImg;
	QImage imgT;
	QTransform* mScaledImgMatrix;
	QTransform* mWorldMatrix;
	QTransform* mImgMatrix;
	QRectF mViewPortRect;
	QPointF mPosGrab;
	QPointF mEnterPos;

	void resizeImg();
	void paintEvent(QPaintEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void resizeEvent(QResizeEvent* event);
	QRectF getImageRect() const;
	QTransform getScaledImageMatrix();
};

class DkZoomWidget : public DkFadeLabel {
	Q_OBJECT

public:
	DkZoomWidget(QWidget* parent = 0);

	DkOverview* getOverview() const;

	bool isAutoHide() const;

signals:
	void zoomSignal(float zoomLevel);

public slots:
	virtual void setVisible(bool visible, bool autoHide = false);

	void updateZoom(float zoomLevel);
	void on_sbZoom_valueChanged(double zoomLevel);
	void on_slZoom_valueChanged(int zoomLevel);

protected:
	void createLayout();

	DkOverview* mOverview = 0;
	QSlider* mSlZoom = 0;
	QDoubleSpinBox* mSbZoom = 0;
	bool mAutoHide = false;
};

class DkTransformRect : public QWidget {
	Q_OBJECT

public:
	
	DkTransformRect(int idx = -1, DkRotatingRect* rect = 0, QWidget* parent = 0, Qt::WindowFlags f = 0);
	virtual ~DkTransformRect() {};

	void draw(QPainter *painter);
	
	QPointF getCenter() {
		return QPointF(size.width()*0.5f, size.height()*0.5f);
	};

signals:
	void ctrlMovedSignal(int, const QPointF&, Qt::KeyboardModifiers, bool);
	void updateDiagonal(int);

protected:

	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void enterEvent(QEvent *event);
	void init();
	
	DkRotatingRect* rect;
	QPointF initialPos;
	QPointF posGrab;
	int parentIdx;
	QSize size;
};


class DkEditableRect : public DkWidget {
	Q_OBJECT

public:

	enum {
		no_guide = 0,
		rule_of_thirds,
		grid,

		mode_end,
	};

	enum {
		do_nothing,
		initializing,
		rotating,
		moving,
		scaling
	};

	DkEditableRect(const QRectF& rect = QRect(), QWidget* parent = 0, Qt::WindowFlags f = 0);
	virtual ~DkEditableRect() {};

	void reset();

	void setWorldTransform(QTransform *worldTform) {
		mWorldTform = worldTform;
	};	

	void setImageTransform(QTransform *imgTform) {
		mImgTform = imgTform;
	};

	void setImageRect(QRectF* imgRect) {
		mImgRect = imgRect;
	};

	virtual void setVisible(bool visible);

signals:
	void cropImageSignal(const DkRotatingRect& cropArea, const QColor& bgCol = QColor(0,0,0,0), bool cropToMetaData = false) const;
	void angleSignal(double angle) const;
	void aRatioSignal(const QPointF& aRatio) const;

public slots:
	void updateCorner(int idx, const QPointF& point, Qt::KeyboardModifiers modifier, bool changeState = false);
	void updateDiagonal(int idx);
	void setFixedDiagonal(const DkVector& diag);
	void setAngle(double angle, bool apply = true);
	void setPanning(bool panning);
	void setPaintHint(int paintMode = rule_of_thirds);
	void setShadingHint(bool invert);
	void setShowInfo(bool showInfo);

protected:
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent* event);
	void keyPressEvent(QKeyEvent *event);
	void keyReleaseEvent(QKeyEvent *event);
	void paintEvent(QPaintEvent *event);

	QPointF clipToImage(const QPointF& pos);
	QPointF clipToImageForce(const QPointF& pos);
	void applyTransform();
	void drawGuide(QPainter* painter, const QPolygonF& p, int paintMode);
	QPointF map(const QPointF &pos);

	int mState = do_nothing;
	QTransform *mImgTform = 0;
	QTransform *mWorldTform = 0;
	QTransform mTtform;
	QTransform mRtform;
	QPointF mPosGrab;
	QPointF mClickPos;
	DkVector mOldDiag = DkVector(-1.0f, -1.0f);
	DkVector mFixedDiag;

	DkRotatingRect mRect;
	QPen mPen;
	QBrush mBrush;
	QVector<DkTransformRect*> mCtrlPoints;
	QCursor mRotatingCursor;
	QRectF* mImgRect = 0;
	bool mPanning = false;
	int mPaintMode = rule_of_thirds;
	bool mShowInfo = false;
};

class DkCropWidget : public DkEditableRect {
	Q_OBJECT

public:
	DkCropWidget(QRectF rect = QRect(), QWidget* parent = 0, Qt::WindowFlags f = 0);

	DkCropToolBar* getToolbar() const;

public slots:
	void crop(bool cropToMetadata = false);
	virtual void setVisible(bool visible);

signals:
	void cancelSignal();
	void showToolbar(QToolBar* toolbar, bool show);

protected:
	void createToolbar();

	DkCropToolBar* cropToolbar;
};

/**
 * DkAnimationLabel
 * This code is based on: http://www.developer.nokia.com/Community/Wiki/CS001434_-_Creating_a_loading_animation_with_GIF,_QMovie,_and_QLabel
 *
 * Uses animation from the path
 * to display it in a DkLabel.
 */
class DllCoreExport DkAnimationLabel : public DkLabel {

public:
	DkAnimationLabel(QString animationPath = QString(), QWidget* parent = 0);
	DkAnimationLabel(QString animationPath, QSize size, QWidget* parent);
	virtual ~DkAnimationLabel();

	virtual void showTimed(int time = 3000);
	virtual void hide();

private:
	QSharedPointer<QSvgRenderer> mSvg;

	void paintEvent(QPaintEvent* ev);
	void init(const QString& animationPath, const QSize& size);
};

// Image histogram display
class DkHistogram : public DkWidget {

	Q_OBJECT
	
public:
	DkHistogram(QWidget *parent);
	~DkHistogram();
	void drawHistogram(QImage img);
	void clearHistogram();
	void setMaxHistogramValue(int maxValue);
	void updateHistogramValues(int histValues[][256]);
	void setPainted(bool isPainted);

protected:
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);
	virtual void paintEvent(QPaintEvent* event);

private:
	int mHist[3][256];
	int mMaxValue = 20;
	bool mIsPainted = false;
	float mScaleFactor = 1;
				
};

class DkFileInfo {

public:
	DkFileInfo();
	DkFileInfo(const QFileInfo& fileInfo);

	QString getFilePath() const;
	bool exists() const;
	void setExists(bool fileExists);

	bool inUse() const;
	void setInUse(bool inUse);

protected:
	QFileInfo mFileInfo;
	bool mFileExists = false;
	bool mUsed = false;
};

class DkFolderLabel : public QLabel {
	Q_OBJECT

public:
	DkFolderLabel(const DkFileInfo& fileInfo, QWidget* parent = 0, Qt::WindowFlags f = 0);

signals:
	void loadFileSignal(const QString&) const;

protected:
	void mousePressEvent(QMouseEvent *ev);

	DkFileInfo fileInfo;
};

class DkImageLabel : public QLabel {
	Q_OBJECT

public:
	DkImageLabel(const QString& filePath, int thumbSize = 100, QWidget* parent = 0, Qt::WindowFlags f = 0);

	bool hasFile() const;
	QSharedPointer<DkThumbNailT> getThumb() const;

signals:
	void labelLoaded() const;
	void loadFileSignal(const QString&) const;

public slots:
	void thumbLoaded();
	void removeFileFromList();

protected:
	void mousePressEvent(QMouseEvent *ev);
	void enterEvent(QEvent *ev);
	void leaveEvent(QEvent *ev);
	void createLayout();

	QLabel* imageLabel;
	QLabel* highLightLabel;
	QPushButton* removeFileButton;
	QSharedPointer<DkThumbNailT> thumb;
	int mThumbSize = 64;
};

class DkRecentFilesWidget : public DkWidget {
	Q_OBJECT

public:
	DkRecentFilesWidget(QWidget* parent = 0);
	~DkRecentFilesWidget();
	void setCustomStyle(bool imgLoadedStyle = false);

signals:
	void loadFileSignal(const QString& filePath) const;

public slots:
	void updateFiles();
	//void updateFolders();
	virtual void setVisible(bool visible, bool saveSettings = true);
	virtual void hide(bool saveSettings = true);
	//void clearFileHistory();
	//void clearFolderHistory();

protected:
	void createLayout();
	void updateFileList();

	QVector<QFileInfo> recentFiles;
	//QVector<DkFileInfo> recentFolders;
	//QFutureWatcher<void> fileWatcher;
	QVector<DkImageLabel*> fileLabels;
	//QVector<DkFolderLabel*> folderLabels;

	QGridLayout* filesLayout;
	//QVBoxLayout* folderLayout;

	QWidget* filesWidget;
	//QWidget* folderWidget;

	//QLabel* folderTitle;
	//QLabel* filesTitle;
	QLabel* bgLabel;

	QLabel* clearFiles;
	//QLabel* clearFolders;

	int rFileIdx;
	int numActiveLabels;
	int mThumbSize = 100;
};

class DkDirectoryEdit : public QLineEdit {
	Q_OBJECT

public:	
	DkDirectoryEdit(QWidget* parent = 0);
	DkDirectoryEdit(const QString& content, QWidget* parent = 0);
		
	bool existsDirectory() { return existsDirectory(text());};

signals:
	bool directoryChanged(const QString& path);

public slots:
	void lineEditChanged(const QString& path);

private:
	bool existsDirectory(const QString& path);

	QString mOldPath;
	bool showFolderButton = false;
};

class DkDirectoryChooser : public QWidget {
	Q_OBJECT
public:
	DkDirectoryChooser(const QString& dirPath = "", QWidget* parent = 0);

public slots:
	void on_dirButton_clicked();

signals:
	void directoryChanged(const QString& dirPath) const;

protected:
	void createLayout(const QString& dirPath);

	DkDirectoryEdit* mDirEdit = 0;

};

class DkDelayedInfo : public QObject {
	Q_OBJECT

public:
	DkDelayedInfo(int time = 0, QObject* parent = 0) : QObject(parent) {
		timer = new QTimer();
		timer->setSingleShot(true);

		if (time)
			timer->start(time);

		connect(timer, SIGNAL(timeout()), this, SLOT(sendInfo()));
	}

	virtual ~DkDelayedInfo() {

		if (timer && timer->isActive())
			timer->stop();

		if (timer)
			delete timer;

		timer = 0;
	}

	void stop() {

		if (timer && timer->isActive())
			timer->stop();
		else
			emit infoSignal(1);
	}

	void setInfo(int time = 1000) {

		if (!timer)
			return;

		timer->start(time);
	}

signals:
	void infoSignal(int time);

	protected slots:
	virtual void sendInfo() {
		emit infoSignal(-1);
	}

protected:
	QTimer* timer;

};


class DkDelayedMessage : public DkDelayedInfo {
	Q_OBJECT

public:
	DkDelayedMessage(const QString& msg  = QString(), int time = 0, QObject* parent = 0) : DkDelayedInfo(time, parent) {
		mMsg = msg;
	}

	~DkDelayedMessage() {}

	void stop() {

		if (timer && timer->isActive())
			timer->stop();
		else
			emit infoSignal(mMsg, 1);
	}

	void setInfo(const QString& msg, int time = 1000) {

		DkDelayedInfo::setInfo(time);
		mMsg = msg;
	}

signals:
	void infoSignal(const QString& msg, int time = -1) const;

protected:
	QString mMsg;

};

class DkListWidget : public QListWidget {
	Q_OBJECT

public:
	DkListWidget(QWidget* parent);

	void startDrag(Qt::DropActions supportedActions);
	bool isEmpty() const;

	void setEmptyText(const QString& text);

signals:
	void dataDroppedSignal() const;

protected:
	void paintEvent(QPaintEvent *event);
	void dropEvent(QDropEvent *event);

	QString mEmptyText = tr("Drag Items Here");
};

class DkProgressBar : public QProgressBar {
	Q_OBJECT

public:
	DkProgressBar(QWidget* parent = 0);

public slots:
	void setVisible(bool visible) override;
	void setVisibleTimed(bool visible, int time = -1);

protected:
	void paintEvent(QPaintEvent *ev);
	void initPoints();
	void animatePoint(double& xVal);

	QTimer mTimer;
	QTimer mShowTimer;
	QVector<double> mPoints;
};

class DllCoreExport DkGenericProfileWidget : public DkNamedWidget {
	Q_OBJECT

public:
	DkGenericProfileWidget(const QString& name, QWidget* parent);
	virtual ~DkGenericProfileWidget();

	public slots:
	void saveSettings() const;
	virtual void saveSettings(const QString& name) const;
	virtual void loadSettings(const QString& name) = 0;
	void deleteCurrentSetting();
	void activate(bool active = true);
	void setDefaultModel() const;

protected:
	void paintEvent(QPaintEvent* ev) override;

	virtual void init();
	void createLayout();
	QStringList loadProfileStrings() const;
	QString loadDefaultProfileString() const;

	QPushButton* mSaveButton;
	QPushButton* mDeleteButton;
	QComboBox* mProfileList;
	bool mEmpty = true;

	QString mSettingsGroup = "ILLEGAL_GENERIC_GROUP";
};


class DllCoreExport DkTabEntryWidget : public QPushButton {
	Q_OBJECT

public:
	DkTabEntryWidget(const QIcon& icon, const QString& text, QWidget* parent);

protected:
	void paintEvent(QPaintEvent* event);

};

};
