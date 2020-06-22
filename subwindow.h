#ifndef SUBWINDOW_H
#define SUBWINDOW_H

#include <QWidget>
#include <QApplication>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QComboBox>
#include <QFontComboBox>
#include <QLabel>
#include <QMessageBox>
#include <QScreen>
#include <QFontDatabase>
#include <QtDataVisualization>
#include <QFont>
#include <qmath.h>
#include <qrandom.h>
#include <QComboBox>
#include <QColor>
#include <QSlider>
#include "scatterdatamodifier.h"

class subwindow : public QWidget
{
    Q_OBJECT
private:
    Q3DScatter *graph;
    int m_fontSize;
    QAbstract3DSeries::Mesh m_style;
    bool m_smooth;
    int m_itemCount;
    float m_curveDivider;
    Q3DTheme::ColorStyle m_colorStytle;
public:
    explicit subwindow(QWidget *parent = nullptr);
    void addData(float *,int);
    void deleteData(int);
    void deleteAllData();
    void modifyData(float *,int);
    void changeStyle();
    void changePresetCamera();
    void changeLabelStyle();
    void changeFont(const QFont &font);
    void changeFontSize(int fontsize);
    void setBackgroundEnabled(int enabled);
    void setGridEnabled(int enabled);
    void setSmoothDots(int smooth);
    void toggleItemCount();
    void start();
signals:
    void backgroundEnabledChanged(bool enabled);
    void gridEnabledChanged(bool enabled);
    void shadowQualityChanged(int quality);
    void fontChanged(QFont font);
    void currentItemChanged(int);
    void subWindowClosed();
protected:
    void closeEvent(QCloseEvent *);

public slots:
    void changeStyle(int style);
    void changeTheme(int theme);
    void changeShadowQuality(int quality);
    void shadowQualityUpdatedByVisual(QAbstract3DGraph::ShadowQuality shadowQuality);
    void setCurrentItem(int);
};

#endif // SUBWINDOW_H
