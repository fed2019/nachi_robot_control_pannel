#include "subwindow.h"

const int numberOfItems = 3600;
const float curveDivider = 3.0f;
const int lowerNumberOfItems = 900;
const float lowerCurveDivider = 0.75f;

subwindow::subwindow(QWidget *parent) : QWidget(parent),
    m_fontSize(20.0f),
    m_style(QAbstract3DSeries::MeshSphere),
    m_smooth(true),
    m_itemCount(lowerNumberOfItems),
    m_curveDivider(lowerCurveDivider),
    m_colorStytle(Q3DTheme::ColorStyleObjectGradient)
{
    graph = new Q3DScatter();
    graph->activeTheme()->setType(Q3DTheme::ThemeQt);
    QFont font = graph->activeTheme()->font();
    font.setPointSize(m_fontSize);
    graph->activeTheme()->setFont(font);
    graph->setShadowQuality(QAbstract3DGraph::ShadowQualityNone);
    graph->scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPresetFront);

    QScatterDataProxy *proxy = new QScatterDataProxy;
    QScatter3DSeries *series = new QScatter3DSeries(proxy);
    series->setItemLabelFormat(QStringLiteral("@xTitle: @xLabel @yTitle: @yLabel @zTitle: @zLabel"));
    series->setMeshSmooth(m_smooth);
    series->setColorStyle(m_colorStytle);
    series->setItemSize(0.2f);
    series->setBaseColor(QColor(255,255,255));
    series->setBaseGradient(QLinearGradient(-800,-800,800,800));
    graph->addSeries(series);

    graph->axisX()->setTitle("X");
    graph->axisY()->setTitle("Y");
    graph->axisZ()->setTitle("Z");
    graph->axisX()->setTitleVisible(true);
    graph->axisY()->setTitleVisible(true);
    graph->axisZ()->setTitleVisible(true);
    graph->axisX()->setReversed(true);
    graph->axisY()->setReversed(true);
    graph->axisZ()->setReversed(true);

    connect(series,&QScatter3DSeries::selectedItemChanged,this,[=](int index){
    emit currentItemChanged(index);});
    QWidget *container = QWidget::createWindowContainer(graph,this);

    if (!graph->hasContext())
    {
        QMessageBox msgBox;
        msgBox.setText("Couldn't initialize the OpenGL context.");
        msgBox.exec();
    }

    resize(1066,600);
//    QSize screenSize = graph->screen()->size();
//    container->resize(QSize(screenSize.width() / 2, screenSize.height() / 1.5));
//    container->setMaximumSize(screenSize);
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    container->setFocusPolicy(Qt::StrongFocus);


    QHBoxLayout *hLayout = new QHBoxLayout(this);
    QVBoxLayout *vLayout = new QVBoxLayout();
    hLayout->addWidget(container, 1);
    hLayout->addLayout(vLayout);

    this->setWindowTitle(QStringLiteral("Scatter of Robot TCP position"));

    QComboBox *themeList = new QComboBox(this);
    themeList->addItem(QStringLiteral("Qt"));
    themeList->addItem(QStringLiteral("Primary Colors"));
    themeList->addItem(QStringLiteral("Digia"));
    themeList->addItem(QStringLiteral("Stone Moss"));
    themeList->addItem(QStringLiteral("Army Blue"));
    themeList->addItem(QStringLiteral("Retro"));
    themeList->addItem(QStringLiteral("Ebony"));
    themeList->addItem(QStringLiteral("Isabelle"));
    themeList->setCurrentIndex(6);

    QPushButton *labelButton = new QPushButton(this);
    labelButton->setText(QStringLiteral("Change label style"));

    QCheckBox *smoothCheckBox = new QCheckBox(this);
    smoothCheckBox->setText(QStringLiteral("Smooth dots"));
    smoothCheckBox->setChecked(true);

    QComboBox *itemStyleList = new QComboBox(this);
    itemStyleList->addItem(QStringLiteral("Sphere"), int(QAbstract3DSeries::MeshSphere));
    itemStyleList->addItem(QStringLiteral("Cube"), int(QAbstract3DSeries::MeshCube));
    itemStyleList->addItem(QStringLiteral("Minimal"), int(QAbstract3DSeries::MeshMinimal));
    itemStyleList->addItem(QStringLiteral("Point"), int(QAbstract3DSeries::MeshPoint));
    itemStyleList->setCurrentIndex(0);

    QPushButton *cameraButton = new QPushButton(this);
    cameraButton->setText(QStringLiteral("Change camera preset"));

    QPushButton *itemCountButton = new QPushButton(this);
    itemCountButton->setText(QStringLiteral("Toggle item count"));

    QCheckBox *backgroundCheckBox = new QCheckBox(this);
    backgroundCheckBox->setText(QStringLiteral("Show background"));
    backgroundCheckBox->setChecked(true);

    QCheckBox *gridCheckBox = new QCheckBox(this);
    gridCheckBox->setText(QStringLiteral("Show grid"));
    gridCheckBox->setChecked(true);

    QComboBox *shadowQuality = new QComboBox(this);
    shadowQuality->addItem(QStringLiteral("None"));
    shadowQuality->addItem(QStringLiteral("Low"));
    shadowQuality->addItem(QStringLiteral("Medium"));
    shadowQuality->addItem(QStringLiteral("High"));
    shadowQuality->addItem(QStringLiteral("Low Soft"));
    shadowQuality->addItem(QStringLiteral("Medium Soft"));
    shadowQuality->addItem(QStringLiteral("High Soft"));
    shadowQuality->setCurrentIndex(4);

    QFontComboBox *fontList = new QFontComboBox(this);
    fontList->setCurrentFont(QFont("Arial"));

    vLayout->addWidget(labelButton, 0, Qt::AlignTop);
    vLayout->addWidget(cameraButton, 0, Qt::AlignTop);
    vLayout->addWidget(itemCountButton, 0, Qt::AlignTop);
    vLayout->addWidget(backgroundCheckBox);
    vLayout->addWidget(gridCheckBox);
    vLayout->addWidget(smoothCheckBox, 0, Qt::AlignTop);
    vLayout->addWidget(new QLabel(QStringLiteral("Change dot style")));
    vLayout->addWidget(itemStyleList);
    vLayout->addWidget(new QLabel(QStringLiteral("Change theme")));
    vLayout->addWidget(themeList);
    vLayout->addWidget(new QLabel(QStringLiteral("Adjust shadow quality")));
    vLayout->addWidget(shadowQuality);
    vLayout->addWidget(new QLabel(QStringLiteral("Change font")));
    vLayout->addWidget(fontList, 1, Qt::AlignTop);


    connect(cameraButton, &QPushButton::clicked, this,
                     &subwindow::changePresetCamera);
    connect(labelButton, &QPushButton::clicked, this,
                     &subwindow::changeLabelStyle);
    connect(itemCountButton, &QPushButton::clicked, this,
                     &subwindow::toggleItemCount);

    connect(backgroundCheckBox, &QCheckBox::stateChanged, this,
                     &subwindow::setBackgroundEnabled);
    connect(gridCheckBox, &QCheckBox::stateChanged, this,
                     &subwindow::setGridEnabled);
    connect(smoothCheckBox, &QCheckBox::stateChanged, this,
                     &subwindow::setSmoothDots);

    connect(this, &subwindow::backgroundEnabledChanged,
                     backgroundCheckBox, &QCheckBox::setChecked);
    connect(this, &subwindow::gridEnabledChanged,
                     gridCheckBox, &QCheckBox::setChecked);
    connect(itemStyleList, SIGNAL(currentIndexChanged(int)), this,
                     SLOT(changeStyle(int)));

    connect(themeList, SIGNAL(currentIndexChanged(int)), this,
                     SLOT(changeTheme(int)));

    connect(shadowQuality, SIGNAL(currentIndexChanged(int)), this,
                     SLOT(changeShadowQuality(int)));

    connect(this, &subwindow::shadowQualityChanged, shadowQuality,
                     &QComboBox::setCurrentIndex);
    connect(graph, &Q3DScatter::shadowQualityChanged, this,
                     &subwindow::shadowQualityUpdatedByVisual);

    connect(fontList, &QFontComboBox::currentFontChanged, this,
                     &subwindow::changeFont);

    connect(this, &subwindow::fontChanged, fontList,
                     &QFontComboBox::setCurrentFont);
}

//关闭主窗口时要做的操作
void subwindow::closeEvent(QCloseEvent *event)
 {
    emit subWindowClosed();
 }

void subwindow::addData(float xyz[],int index)
{

    if (index==-1)
    {
        graph->seriesList().at(0)->dataProxy()->
                addItem(QScatterDataItem(QVector3D(xyz[0],xyz[1],xyz[2]),
                QQuaternion::fromEulerAngles(xyz[3],xyz[4],xyz[5])));
    }
    else
    {
        graph->seriesList().at(0)->dataProxy()->
                insertItem(index,QScatterDataItem(QVector3D(xyz[0],xyz[1],xyz[2]),
                QQuaternion::fromEulerAngles(xyz[3],xyz[4],xyz[5])));
    }

}

void subwindow::modifyData(float xyz[],int i)
{
    int count=graph->seriesList().at(0)->dataProxy()->itemCount();
    int index;
    if(count==0){return;}
    else
    {
        if (i==-1)
        {index=count-1;}
        else
        {index=i;}
        graph->seriesList().at(0)->dataProxy()->
                setItem(index,QScatterDataItem(QVector3D(xyz[0],xyz[1],xyz[2]),
                QQuaternion::fromEulerAngles(xyz[3],xyz[4],xyz[5])));
    }
}
void subwindow::deleteData(int i)
{
    int count=graph->seriesList().at(0)->dataProxy()->itemCount();
    int index;
    if(count==0){return;}
    else
    {
        if (i==-1)
        {index=count-1;}
        else
        {index=i;}
        graph->seriesList().at(0)->dataProxy()->
                    removeItems(index,1);
    }
}

void subwindow::deleteAllData()
{
    int count=graph->seriesList().at(0)->dataProxy()->itemCount();
    graph->seriesList().at(0)->dataProxy()->removeItems(0, count);
}


void subwindow::changeStyle(int style)
{
    QComboBox *comboBox = qobject_cast<QComboBox *>(sender());
    if (comboBox) {
        m_style = QAbstract3DSeries::Mesh(comboBox->itemData(style).toInt());
        if (graph->seriesList().size())
            graph->seriesList().at(0)->setMesh(m_style);
    }
}

void subwindow::setSmoothDots(int smooth)
{
    m_smooth = bool(smooth);
    QScatter3DSeries *series = graph->seriesList().at(0);
    series->setMeshSmooth(m_smooth);
}

void subwindow::changeTheme(int theme)
{
    Q3DTheme *currentTheme = graph->activeTheme();
    currentTheme->setType(Q3DTheme::Theme(theme));
    emit backgroundEnabledChanged(currentTheme->isBackgroundEnabled());
    emit gridEnabledChanged(currentTheme->isGridEnabled());
    emit fontChanged(currentTheme->font());
}

void subwindow::changePresetCamera()
{
    static int preset = Q3DCamera::CameraPresetFrontLow;

    graph->scene()->activeCamera()->setCameraPreset((Q3DCamera::CameraPreset)preset);

    if (++preset > Q3DCamera::CameraPresetDirectlyBelow)
        preset = Q3DCamera::CameraPresetFrontLow;
}

void subwindow::changeLabelStyle()
{
    graph->activeTheme()->setLabelBackgroundEnabled(!graph->activeTheme()->isLabelBackgroundEnabled());
}

void subwindow::changeFont(const QFont &font)
{
    QFont newFont = font;
    newFont.setPointSizeF(m_fontSize);
    graph->activeTheme()->setFont(newFont);
}

void subwindow::shadowQualityUpdatedByVisual(QAbstract3DGraph::ShadowQuality sq)
{
    int quality = int(sq);
    emit shadowQualityChanged(quality); // connected to a checkbox in main.cpp
}

void subwindow::changeShadowQuality(int quality)
{
    QAbstract3DGraph::ShadowQuality sq = QAbstract3DGraph::ShadowQuality(quality);
    graph->setShadowQuality(sq);
}

void subwindow::setBackgroundEnabled(int enabled)
{
    graph->activeTheme()->setBackgroundEnabled((bool)enabled);
}

void subwindow::setGridEnabled(int enabled)
{
    graph->activeTheme()->setGridEnabled((bool)enabled);
}

void subwindow::toggleItemCount()
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
    graph->seriesList().at(0)->dataProxy()->resetArray(0);
}

void subwindow::setCurrentItem(int index)
{
    graph->seriesList().at(0)->setSelectedItem(index);
}
