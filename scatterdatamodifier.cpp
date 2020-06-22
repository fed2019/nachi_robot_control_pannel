/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Data Visualization module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "scatterdatamodifier.h"

using namespace QtDataVisualization;

const int numberOfItems = 3600;
const float curveDivider = 3.0f;
const int lowerNumberOfItems = 900;
const float lowerCurveDivider = 0.75f;

ScatterDataModifier::ScatterDataModifier(Q3DScatter *scatter)
    : m_graph(scatter),
      m_fontSize(20.0f),
      m_style(QAbstract3DSeries::MeshSphere),
      m_smooth(true),
      m_itemCount(lowerNumberOfItems),
      m_curveDivider(lowerCurveDivider),
      m_colorStytle(Q3DTheme::ColorStyleObjectGradient)
{
    m_graph->activeTheme()->setType(Q3DTheme::ThemeQt);
    QFont font = m_graph->activeTheme()->font();
    font.setPointSize(m_fontSize);
    m_graph->activeTheme()->setFont(font);
    m_graph->setShadowQuality(QAbstract3DGraph::ShadowQualityNone);
    m_graph->scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPresetFront);

    QScatterDataProxy *proxy = new QScatterDataProxy;
    QScatter3DSeries *series = new QScatter3DSeries(proxy);
    series->setItemLabelFormat(QStringLiteral("@xTitle: @xLabel @yTitle: @yLabel @zTitle: @zLabel"));
    series->setMeshSmooth(m_smooth);
    series->setColorStyle(m_colorStytle);
    series->setItemSize(0.2f);
    series->setBaseColor(QColor(255,255,255));
    series->setBaseGradient(QLinearGradient(-800,-800,800,800));
//    series->setSingleHighlightGradient(QLinearGradient(-800,-800,800,800));
    m_graph->addSeries(series);

    m_graph->axisX()->setTitle("X");
    m_graph->axisY()->setTitle("Y");
    m_graph->axisZ()->setTitle("Z");
    m_graph->axisX()->setTitleVisible(true);
    m_graph->axisY()->setTitleVisible(true);
    m_graph->axisZ()->setTitleVisible(true);
    m_graph->axisX()->setReversed(true);
    m_graph->axisY()->setReversed(true);
    m_graph->axisZ()->setReversed(true);
//    m_graph->axisX()->setRange(-800, 800);
//    m_graph->axisY()->setRange(-800, 800);
//    m_graph->axisZ()->setRange(-800, 800);

    connect(series,&QScatter3DSeries::selectedItemChanged,this,[=](int index){
    emit currentItemChanged(index);});
}

ScatterDataModifier::~ScatterDataModifier()
{
    delete m_graph;
}

void ScatterDataModifier::addData(float xyz[],int index)
{

    if (index==-1)
    {
        m_graph->seriesList().at(0)->dataProxy()->
                addItem(QScatterDataItem(QVector3D(xyz[0],xyz[1],xyz[2]),
                QQuaternion::fromEulerAngles(xyz[3],xyz[4],xyz[5])));
    }
    else
    {
        m_graph->seriesList().at(0)->dataProxy()->
                insertItem(index,QScatterDataItem(QVector3D(xyz[0],xyz[1],xyz[2]),
                QQuaternion::fromEulerAngles(xyz[3],xyz[4],xyz[5])));
    }

}

void ScatterDataModifier::modifyData(float xyz[],int i)
{
    int count=m_graph->seriesList().at(0)->dataProxy()->itemCount();
    int index;
    if(count==0){return;}
    else
    {
        if (i==-1)
        {index=count-1;}
        else
        {index=i;}
        m_graph->seriesList().at(0)->dataProxy()->
                setItem(index,QScatterDataItem(QVector3D(xyz[0],xyz[1],xyz[2]),
                QQuaternion::fromEulerAngles(xyz[3],xyz[4],xyz[5])));
    }
}
void ScatterDataModifier::deleteData(int i)
{
    int count=m_graph->seriesList().at(0)->dataProxy()->itemCount();
    int index;
    if(count==0){return;}
    else
    {
        if (i==-1)
        {index=count-1;}
        else
        {index=i;}
        m_graph->seriesList().at(0)->dataProxy()->
                    removeItems(index,1);
    }
}

void ScatterDataModifier::deleteAllData()
{
    int count=m_graph->seriesList().at(0)->dataProxy()->itemCount();
    m_graph->seriesList().at(0)->dataProxy()->removeItems(0, count);
}


void ScatterDataModifier::changeStyle(int style)
{
    QComboBox *comboBox = qobject_cast<QComboBox *>(sender());
    if (comboBox) {
        m_style = QAbstract3DSeries::Mesh(comboBox->itemData(style).toInt());
        if (m_graph->seriesList().size())
            m_graph->seriesList().at(0)->setMesh(m_style);
    }
}

void ScatterDataModifier::setSmoothDots(int smooth)
{
    m_smooth = bool(smooth);
    QScatter3DSeries *series = m_graph->seriesList().at(0);
    series->setMeshSmooth(m_smooth);
}

void ScatterDataModifier::changeTheme(int theme)
{
    Q3DTheme *currentTheme = m_graph->activeTheme();
    currentTheme->setType(Q3DTheme::Theme(theme));
    emit backgroundEnabledChanged(currentTheme->isBackgroundEnabled());
    emit gridEnabledChanged(currentTheme->isGridEnabled());
    emit fontChanged(currentTheme->font());
}

void ScatterDataModifier::changePresetCamera()
{
    static int preset = Q3DCamera::CameraPresetFrontLow;

    m_graph->scene()->activeCamera()->setCameraPreset((Q3DCamera::CameraPreset)preset);

    if (++preset > Q3DCamera::CameraPresetDirectlyBelow)
        preset = Q3DCamera::CameraPresetFrontLow;
}

void ScatterDataModifier::changeLabelStyle()
{
    m_graph->activeTheme()->setLabelBackgroundEnabled(!m_graph->activeTheme()->isLabelBackgroundEnabled());
}

void ScatterDataModifier::changeFont(const QFont &font)
{
    QFont newFont = font;
    newFont.setPointSizeF(m_fontSize);
    m_graph->activeTheme()->setFont(newFont);
}

void ScatterDataModifier::shadowQualityUpdatedByVisual(QAbstract3DGraph::ShadowQuality sq)
{
    int quality = int(sq);
    emit shadowQualityChanged(quality); // connected to a checkbox in main.cpp
}

void ScatterDataModifier::changeShadowQuality(int quality)
{
    QAbstract3DGraph::ShadowQuality sq = QAbstract3DGraph::ShadowQuality(quality);
    m_graph->setShadowQuality(sq);
}

void ScatterDataModifier::setBackgroundEnabled(int enabled)
{
    m_graph->activeTheme()->setBackgroundEnabled((bool)enabled);
}

void ScatterDataModifier::setGridEnabled(int enabled)
{
    m_graph->activeTheme()->setGridEnabled((bool)enabled);
}

void ScatterDataModifier::toggleItemCount()
{
    if (m_itemCount == numberOfItems)
    {
        m_itemCount = lowerNumberOfItems;
        m_curveDivider = lowerCurveDivider;
    }
    else
    {
        m_itemCount = numberOfItems;
        m_curveDivider = curveDivider;
    }
    m_graph->seriesList().at(0)->dataProxy()->resetArray(0);
}

void ScatterDataModifier::setCurrentItem(int index)
{
    m_graph->seriesList().at(0)->setSelectedItem(index);
}


